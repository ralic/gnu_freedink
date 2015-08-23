#include "IOGfxSurfaceSW.h"

#include "SDL.h"
#include "SDL2_rotozoom.h"

#include "log.h"

IOGfxSurfaceSW::IOGfxSurfaceSW(SDL_Surface* s) {
	this->surf = s;
}

IOGfxSurfaceSW::~IOGfxSurfaceSW() {
	SDL_FreeSurface(surf);
}

void IOGfxSurfaceSW::fill(int num, SDL_Color* palette) {
	/* Warning: palette indexes 0 and 255 are hard-coded
	   to black and white (cf. gfx_palette.c). */
	if (surf->format->format == SDL_PIXELFORMAT_INDEX8)
		SDL_FillRect(surf, NULL, num);
	else
		SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, palette[num].r, palette[num].g, palette[num].b));
}

void IOGfxSurfaceSW::vlineRGB(Sint16 x, Sint16 y1, Sint16 y2, Uint8 r, Uint8 g, Uint8 b) {
	SDL_Rect dst = { x, y1, 1, y2-y1 };
	SDL_FillRect(surf, &dst, SDL_MapRGB(surf->format, r, g, b));
}
void IOGfxSurfaceSW::hlineRGB(Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b) {
	SDL_Rect dst = { x1, y, x2-x1, 1 };
	SDL_FillRect(surf, &dst, SDL_MapRGB(surf->format, r, g, b));
}


void IOGfxSurfaceSW::drawBox(rect box, int color) {
	SDL_Rect dst;
	dst.x = box.left; dst.y = box.top;
	dst.w = box.right - box.left;
	dst.h = box.bottom - box.top;
	SDL_FillRect(surf, &dst, color);
}

int IOGfxSurfaceSW::blit(IOGfxSurface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect) {
	if (src == NULL)
		return SDL_SetError("SDL_UpperBlit: passed a NULL surface");
	SDL_Surface* src_sdl = dynamic_cast<IOGfxSurfaceSW*>(src)->surf;
	return SDL_BlitSurface(src_sdl, srcrect, surf, dstrect);
}

/**
 * Blit and resize so that 'src' fits in 'dst_rect'
 */
int gfx_blit_stretch(SDL_Surface *src_surf, const SDL_Rect *src_rect_opt,
		SDL_Surface *dst_surf, SDL_Rect *dst_rect) {
	int retval = -1;

	SDL_Rect src_rect;
	if (src_rect_opt == NULL) {
		src_rect.x = 0;
		src_rect.y = 0;
		src_rect.w = src_surf->w;
		src_rect.h = src_surf->h;
	} else {
		src_rect = *src_rect_opt;
	}

	if (src_rect.w == 0 || src_rect.h == 0)
		return 0;  // OK, ignore drawing, and don't mess sx/sy below

	double sx = 1.0 * dst_rect->w / src_rect.w;
	double sy = 1.0 * dst_rect->h / src_rect.h;
	/* In principle, double's are precise up to 15 decimal digits */
	if (fabs(sx-1) > 1e-10 || fabs(sy-1) > 1e-10) {
		SDL_Surface *scaled = zoomSurface(src_surf, sx, sy, SMOOTHING_OFF);

		/* Keep the same transparency / alpha parameters (SDL_gfx bug,
	 report submitted to the author: SDL_gfx adds transparency to
	 non-transparent surfaces) */
		Uint8 r, g, b, a;
		Uint32 colorkey;
		int colorkey_enabled = (SDL_GetColorKey(src_surf, &colorkey) != -1);
		SDL_GetRGBA(colorkey, src_surf->format, &r, &g, &b, &a);

		SDL_SetColorKey(scaled, colorkey_enabled,
				SDL_MapRGBA(scaled->format, r, g, b, a));
		/* Don't mess with alpha transparency, though: */
		/* int alpha_flag = src->flags & SDL_SRCALPHA; */
		/* int alpha = src->format->alpha; */
		/* SDL_SetAlpha(scaled, alpha_flag, alpha); */

		src_rect.x = (int) round(src_rect.x * sx);
		src_rect.y = (int) round(src_rect.y * sy);
		src_rect.w = (int) round(src_rect.w * sx);
		src_rect.h = (int) round(src_rect.h * sy);
		retval = SDL_BlitSurface(scaled, &src_rect, dst_surf, dst_rect);
		SDL_FreeSurface(scaled);
	} else {
		/* No scaling */
		retval = SDL_BlitSurface(src_surf, &src_rect, dst_surf, dst_rect);
	}
	return retval;
}

int IOGfxSurfaceSW::blitStretch(IOGfxSurface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect) {
	if (src == NULL)
		return SDL_SetError("SDL_UpperBlit: passed a NULL surface");
	SDL_Surface* src_sdl = dynamic_cast<IOGfxSurfaceSW*>(src)->surf;
	return gfx_blit_stretch(src_sdl, srcrect, surf, dstrect);
}

/**
 * Temporary disable src's transparency and blit it to dst
 */
int gfx_blit_nocolorkey(SDL_Surface *src, const SDL_Rect *src_rect,
		SDL_Surface *dst, SDL_Rect *dst_rect) {
	int retval = -1;
	if (src == NULL) {
		log_error("attempting to blit a NULL surface");
		return retval;
	}

	Uint32 colorkey;
	SDL_BlendMode blendmode;
	Uint32 rle_flags = src->flags & SDL_RLEACCEL;
	int has_colorkey = (SDL_GetColorKey(src, &colorkey) != -1);
	SDL_GetSurfaceBlendMode(src, &blendmode);

	if (has_colorkey)
		SDL_SetColorKey(src, SDL_FALSE, 0);
	SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_NONE);
	retval = SDL_BlitSurface(src, src_rect, dst, dst_rect);

	SDL_SetSurfaceBlendMode(src, blendmode);
	if (has_colorkey)
		SDL_SetColorKey(src, SDL_TRUE, colorkey);
	if (rle_flags)
		SDL_SetSurfaceRLE(src, 1);

	return retval;
}

int IOGfxSurfaceSW::blitNoColorKey(IOGfxSurface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect) {
	if (src == NULL)
		return SDL_SetError("SDL_UpperBlit: passed a NULL surface");
	SDL_Surface* src_sdl = dynamic_cast<IOGfxSurfaceSW*>(src)->surf;
	return gfx_blit_nocolorkey(src_sdl, srcrect, surf, dstrect);
}

unsigned int IOGfxSurfaceSW::getMemUsage() {
	// TODO: take RLE and metadata into account
	return surf->h * surf->pitch;
}