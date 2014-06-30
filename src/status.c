/**
 * Game status bar

 * Copyright (C) 1997, 1998, 1999, 2002, 2003  Seth A. Robinson
 * Copyright (C) 2005, 2007, 2008, 2009, 2010, 2012, 2014  Sylvain Beucler

 * This file is part of GNU FreeDink

 * GNU FreeDink is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.

 * GNU FreeDink is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "status.h"
#define SEQ_LEVEL_NUMS 442

#include "SDL.h"
#include <string.h> /* strlen */
#include "gfx.h" /* gfx_blit_nocolorkey */
#include "gfx_sprites.h" /* GFX_k */
#include "game_engine.h" /* fexp, fraise, fstrength, fdefense, fmagic,
			    plevel, fgold, plife... */
#include "dinkvar.h"
#include "sfx.h"
#include "talk.h"
#include "screen.h" /* spr */

int draw_num(int mseq, char nums[50], int mx, int my)
{
  int length = 0;
/*   HRESULT             ddrval; */
  int rnum = 0;
  int i;

  for (i=0; i < strlen(nums); i++)
    {
      if (nums[i] == '0') rnum = 10;
      else if (nums[i] == '1') rnum = 1;
      else if (nums[i] == '2') rnum = 2;
      else if (nums[i] == '3') rnum = 3;
      else if (nums[i] == '4') rnum = 4;
      else if (nums[i] == '5') rnum = 5;
      else if (nums[i] == '6') rnum = 6;
      else if (nums[i] == '7') rnum = 7;
      else if (nums[i] == '8') rnum = 8;
      else if (nums[i] == '9') rnum = 9;
      else if (nums[i] == '/') rnum = 11;
/*     again: */
      if ((rnum != 11) && (!(mseq == SEQ_LEVEL_NUMS)))
	{
/* 	  ddrval = lpDDSTwo->BltFast(mx+length, my, k[seq[mseq].frame[rnum]].k, */
/* 				     &k[seq[mseq].frame[rnum]].box, DDBLTFAST_NOCOLORKEY); */
	  // GFX
	  {
	    SDL_Rect dst = {mx+length, my};
	    gfx_blit_nocolorkey(GFX_k[seq[mseq].frame[rnum]].k, NULL, GFX_lpDDSTwo, &dst);
	  }
	}
      else
	{
/* 	  ddrval = lpDDSTwo->BltFast(mx+length, my, k[seq[mseq].frame[rnum]].k, */
/* 				     &k[seq[mseq].frame[rnum]].box, DDBLTFAST_SRCCOLORKEY); */
	  /* Draw experience level number _with_ transparency */
	  // GFX
	  {
	    SDL_Rect dst = {mx+length, my};
	    SDL_BlitSurface(GFX_k[seq[mseq].frame[rnum]].k, NULL, GFX_lpDDSTwo, &dst);
	  }
	}


/*       if (ddrval != DD_OK) */
/* 	{ */
/* 	  if (ddrval == DDERR_WASSTILLDRAWING) goto again; */
/* 	  //dderror(ddrval); */
/* 	} */
/*       else */
/* 	{ */
	  length += k[seq[mseq].frame[rnum]].box.right;
/* 	} */
    }
  return(length);
}


void draw_exp()
{
        char buffer[30];
        char nums[30];
        char final[30];

        //Msg("Drawing exp.. which is %d and %d",fexp, *pexp);
        strcpy(final, "");
	sprintf(buffer, "%d", fexp);
        strcpy(nums, buffer);
        if (strlen(nums) < 5)
	  {
	    int i;
	    for (i = 1; i < (6 - strlen(nums)); i++)
	      strcat(final, "0");
	  }
	strcat(final, nums);
	strcat(final,"/");

		sprintf(buffer, "%d", fraise);
                strcpy(nums, buffer);
                if (strlen(nums) < 5)
		  {
		    int i;
		    for (i = 1; i < (6 - strlen(nums)); i++)
		      strcat(final, "0");
		  }
		strcat(final, nums);
		draw_num(181, final, 404, 459);

}


void draw_strength()
{
        char final[30];
        char buffer[30];
        char nums[30];
        //Msg("Drawing exp.. which is %d and %d",fexp, *pexp);
        strcpy(final, "");

	sprintf(buffer, "%d", fstrength);
        strcpy(nums, buffer);
        if (strlen(nums) < 3)
	  {
	    int i;
	    for (i = 1; i < (4 - strlen(nums)); i++)
	      strcat(final, "0");
	  }
	strcat(final, nums);
	//Msg("Drawing %s..",final);
	draw_num(182, final, 81, 415);
}


void draw_defense()
{
        char final[30];
        char buffer[30];
        char nums[30];
        //Msg("Drawing exp.. which is %d and %d",fexp, *pexp);
        strcpy(final, "");
	sprintf(buffer, "%d", fdefense);
        strcpy(nums, buffer);
        if (strlen(nums) < 3)
	  {
	    int i;
	    for (i = 1; i < (4 - strlen(nums)); i++)
	      strcat(final, "0");
	  }
	strcat(final, nums);
	draw_num(183, final, 81, 437);
}


void draw_magic()
{
        char final[30];
        char buffer[30];
        char nums[30];
        //Msg("Drawing exp.. which is %d and %d",fexp, *pexp);
        strcpy(final, "");
	sprintf(buffer, "%d", fmagic);
        strcpy(nums, buffer);
        if (strlen(nums) < 3)
	  {
	    int i;
	    for (i = 1; i < (4 - strlen(nums)); i++)
	      strcat(final, "0");
	  }
	strcat(final, nums);
	draw_num(184, final, 81, 459);
}


void draw_level()
{
        char final[30];
        char buffer[30];
        //*plevel = 15;
        //Msg("Drawing level.. which is %d ",*plevel);
	sprintf(buffer, "%d", *plevel);
        strcpy(final, buffer);

        if (strlen(final) == 1)

                draw_num(SEQ_LEVEL_NUMS, final, 528, 456); else
                draw_num(SEQ_LEVEL_NUMS, final, 523, 456);

}


void draw_gold()
{
        char final[30];
        char buffer[30];
        char nums[30];
        //Msg("Drawing exp.. which is %d and %d",fexp, *pexp);
        strcpy(final, "");
	sprintf(buffer, "%d", fgold);
        strcpy(nums, buffer);
        if (strlen(nums) < 5)
	  {
	    int i;
	    for (i = 1; i < (6 - strlen(nums)); i++)
	      strcat(final, "0");
	  }
	strcat(final, nums);
	draw_num(185, final, 298, 457);
}


void draw_bar(int life, int seqman)
{
  int cur = 0;
  int curx = 284;
  int cury = 412;
  int rnum = 3;
  int curx_start = curx;

  rect box;
  while(1)
    {
      cur++;
      if (cur > life)
	{
	  cur--;
	  int rem = (cur) - (cur / 10) * 10;
	  if (rem != 0)
	    {
	      rect_copy(&box, &k[seq[seqman].frame[rnum]].box);
	      //Msg("Drawing part bar . cur is %d", rem);
	      box.right = (box.right * ((rem) * 10)/100);
	      //woah, there is part of a bar remaining.  Lets do it.
/* 	    again: */
/* 	      ddrval = lpDDSTwo->BltFast(curx, cury, k[seq[seqman].frame[rnum]].k, */
/* 					 &box, DDBLTFAST_NOCOLORKEY); */
/* 	      if (ddrval == DDERR_WASSTILLDRAWING) */
/* 		goto again; */
	      // GFX
	      {
		SDL_Rect src, dst;
		src.x = 0; src.y = 0;
		src.w = GFX_k[seq[seqman].frame[rnum]].k->w * (rem * 10) / 100;
		src.h = GFX_k[seq[seqman].frame[rnum]].k->h;
		dst.x = curx; dst.y = cury;
		gfx_blit_nocolorkey(GFX_k[seq[seqman].frame[rnum]].k, &src, GFX_lpDDSTwo, &dst);
	      }
	    }
	  //are we done?
	  return;
	}

      rnum = 2;
      if (cur < 11) rnum = 1;
      if (cur == *plifemax) rnum = 3;

      if ((cur / 10) * 10 == cur)
	{
/* 	again2: */
/* 	  ddrval = lpDDSTwo->BltFast( curx, cury, k[seq[seqman].frame[rnum]].k, */
/* 				      &k[seq[seqman].frame[rnum]].box  , DDBLTFAST_NOCOLORKEY); */
/* 	  if (ddrval == DDERR_WASSTILLDRAWING) goto again2; */
	  // GFX
	  {
	    SDL_Rect dst;
	    dst.x = curx;
	    dst.y = cury;
	    gfx_blit_nocolorkey(GFX_k[seq[seqman].frame[rnum]].k, NULL, GFX_lpDDSTwo, &dst);
	  }

	  //if (ddrval != DD_OK) dderror(ddrval);
	  curx += k[seq[seqman].frame[rnum]].box.right;
	  if (cur == 110)
	    {cury += k[seq[seqman].frame[rnum]].box.bottom+5;
	      curx = curx_start;

	    }

	  if (cur == 220) return;
	}
    }
}


void draw_health( void )
{
        flifemax = *plifemax;
        draw_bar(flifemax, 190);
        flife = *plife;
        draw_bar(flife, 451);
}

void draw_icons()
{
  if (*pcur_weapon >= 1 && *pcur_weapon <= NB_ITEMS && play.item[*pcur_weapon - 1].active)
    {
      //disarm old weapon
      check_seq_status(play.item[*pcur_weapon - 1].seq);
      SDL_Rect dst = {557, 413};
      SDL_BlitSurface(GFX_k[seq[play.item[*pcur_weapon - 1].seq].frame[play.item[*pcur_weapon - 1].frame]].k, NULL,
		      GFX_lpDDSTwo, &dst);
    }

  if (*pcur_magic >= 1 && *pcur_magic <= NB_MITEMS && play.mitem[*pcur_magic - 1].active)
    {
      //disarm old weapon
      check_seq_status(play.mitem[*pcur_magic - 1].seq);
      SDL_Rect dst = {153, 413};
      SDL_BlitSurface(GFX_k[seq[play.mitem[*pcur_magic - 1].seq].frame[play.mitem[*pcur_magic - 1].frame]].k, NULL,
		      GFX_lpDDSTwo, &dst);
    }
}


/** draw_virtical, draw_hor, draw_virt2, draw_hor2: used to draw the
    magic jauge (in that order) (dinkvar.cpp:draw_mlevel() only) **/

void draw_virtical(int percent, int mx, int my, int mseq, int mframe)
{
  int cut;
  if (percent > 25) percent = 25;
  percent = (percent * 4);
  rect myrect;
  rect_copy(&myrect, &k[seq[mseq].frame[mframe]].box);
  int full = myrect.bottom;
  cut = (full * percent) / 100;
  myrect.bottom = cut;

  my += (full - cut);

/*   ddrval = lpDDSTwo->BltFast(mx, my, k[seq[mseq].frame[mframe]].k, */
/* 			     &myrect, DDBLTFAST_NOCOLORKEY); */
  // GFX
  {
    /* TODO: test me! */
    SDL_Rect src, dst;
    src.x = src.y = 0;
    src.w = GFX_k[seq[mseq].frame[mframe]].k->w;
    src.h = GFX_k[seq[mseq].frame[mframe]].k->h * percent / 100;
    dst.x = mx;
    dst.y = my;
    gfx_blit_nocolorkey(GFX_k[seq[mseq].frame[mframe]].k, &src, GFX_lpDDSTwo, &dst);
  }
}

void draw_virt2(int percent, int mx, int my, int mseq, int mframe)
{
  int cut;
  if (percent > 25) percent = 25;
  percent = (percent * 4);
  rect myrect;
  rect_copy(&myrect, &k[seq[mseq].frame[mframe]].box);
  int full = myrect.bottom;
  cut = (full * percent) / 100;
  myrect.bottom = cut;

/*  again: */
/*   ddrval = lpDDSTwo->BltFast( mx, my, k[seq[mseq].frame[mframe]].k, */
/* 			      &myrect, DDBLTFAST_NOCOLORKEY); */
/*   if (ddrval == DDERR_WASSTILLDRAWING) goto again; */
  // GFX
  {
    SDL_Rect src, dst;
    src.x = src.y = 0;
    src.w = GFX_k[seq[mseq].frame[mframe]].k->w;
    src.h = GFX_k[seq[mseq].frame[mframe]].k->h * percent / 100;
    dst.x = mx; dst.y = my;
    gfx_blit_nocolorkey(GFX_k[seq[mseq].frame[mframe]].k, &src, GFX_lpDDSTwo, &dst);
  }
}

void draw_hor(int percent, int mx, int my, int mseq, int mframe)
{
  int cut;
  if (percent > 25) percent = 25;
  percent = (percent * 4);
  rect myrect;
  rect_copy(&myrect, &k[seq[mseq].frame[mframe]].box);
  int full = myrect.right;
  cut = (full * percent) / 100;
  full = cut;
  myrect.right = full;
/*  again: */
/*   ddrval = lpDDSTwo->BltFast( mx, my, k[seq[mseq].frame[mframe]].k, */
/* 			      &myrect, DDBLTFAST_NOCOLORKEY); */
/*   if (ddrval == DDERR_WASSTILLDRAWING) goto again; */
  // GFX
  {
    /* TODO: test me! */
    SDL_Rect src, dst;
    src.x = src.y = 0;
    src.w = GFX_k[seq[mseq].frame[mframe]].k->w * percent / 100;
    src.h = GFX_k[seq[mseq].frame[mframe]].k->h;
    dst.x = mx; dst.y = my;
    gfx_blit_nocolorkey(GFX_k[seq[mseq].frame[mframe]].k, &src, GFX_lpDDSTwo, &dst);
  }
}

void draw_hor2(int percent, int mx, int my, int mseq, int mframe)
{
  int cut;
  if (percent > 25) percent = 25;
  percent = (percent * 4);
  rect myrect;
  rect_copy(&myrect, &k[seq[mseq].frame[mframe]].box);
  int full = myrect.right;
  cut = (full * percent) / 100;

  myrect.right = cut;
  mx += (full - cut);

/*  again: */
/*   ddrval = lpDDSTwo->BltFast( mx, my, k[seq[mseq].frame[mframe]].k, */
/* 			      &myrect, DDBLTFAST_NOCOLORKEY); */
/*   if (ddrval == DDERR_WASSTILLDRAWING) goto again; */
  // GFX
  {
    SDL_Rect src, dst;
    src.x = src.y = 0;
    src.w = GFX_k[seq[mseq].frame[mframe]].k->w * percent / 100;
    src.h = GFX_k[seq[mseq].frame[mframe]].k->h;
    dst.x = mx;
    dst.y = my;
    gfx_blit_nocolorkey(GFX_k[seq[mseq].frame[mframe]].k, &src, GFX_lpDDSTwo, &dst);
  }
}

void draw_mlevel(int percent)
{
  //if (*pmagic_level < 1) return;

  int mseq = 180;
  int bary = 6;
  int barx = 7;

  if (percent > 0) draw_virtical(percent, 149, 411, mseq, bary);
  percent -= 25;
  if (percent > 0) draw_hor(percent, 149, 409, mseq, barx);
  percent -= 25;
  if (percent > 0) draw_virt2(percent, 215, 411, mseq, bary);
  percent -= 25;
  if (percent > 0) draw_hor2(percent, 149, 466, mseq, barx);
}


/* Draw the status bar and the magic jauge */
void draw_status_all(void)
{
/*   RECT rcRect; */
/*   rcRect.left = 0; */
/*   rcRect.top = 0; */
/*   rcRect.right = 640; */
/*   rcRect.bottom = 80; */
/*  again: */
/*   ddrval = lpDDSTwo->BltFast(0, 400, k[seq[180].frame[3]].k, */
/* 			     &rcRect, DDBLTFAST_NOCOLORKEY); */
/*   if (ddrval == DDERR_WASSTILLDRAWING) goto again; */
  // GFX
  {
    SDL_Rect src = {0, 0, 640, 80}, dst = {0, 400};
    gfx_blit_nocolorkey(GFX_k[seq[180].frame[3]].k, &src, GFX_lpDDSTwo, &dst);
  }

/*   rcRect.left = 0; */
/*   rcRect.top = 0; */
/*   rcRect.right = 20; */
/*   rcRect.bottom = 400; */
/*  again2: */
/*   ddrval = lpDDSTwo->BltFast(0, 0, k[seq[180].frame[1]].k, */
/* 			     &rcRect, DDBLTFAST_NOCOLORKEY); */
/*   if (ddrval == DDERR_WASSTILLDRAWING) goto again2; */
/*  again3: */
/*   ddrval = lpDDSTwo->BltFast(620, 0, k[seq[180].frame[2]].k, */
/* 			     &rcRect, DDBLTFAST_NOCOLORKEY); */
/*   if (ddrval == DDERR_WASSTILLDRAWING) goto again3; */
  // GFX
  {
    SDL_Rect src = {0, 0, 20, 400}, dst1 = {0, 0}, dst2 = {620, 0};
    gfx_blit_nocolorkey(GFX_k[seq[180].frame[1]].k, &src, GFX_lpDDSTwo, &dst1);
    gfx_blit_nocolorkey(GFX_k[seq[180].frame[2]].k, &src, GFX_lpDDSTwo, &dst2);
  }

  fraise = next_raise();
  if (*pexper < fraise)
    fexp = *pexper;
  else
    fexp = fraise - 1;
  fstrength = *pstrength;
  fmagic = *pmagic;
  fgold = *pgold;
  fdefense = *pdefense;
  last_magic_draw = 0;
  draw_exp();
  draw_health();
  draw_strength();
  draw_defense();
  draw_magic();
  draw_gold();
  draw_level();
  draw_icons();
  if (*pmagic_cost > 0 && *pmagic_level > 0)
    draw_mlevel(*pmagic_level * 100 / *pmagic_cost);
}

void update_status_all(void)
{
        /*bool*/int drawexp = /*false*/0;
        int next = next_raise();
    int script;
        if (next != fraise)
        {
                fraise += next / 40;

                if (fraise > next) fraise = next;
                //make noise here
                drawexp = /*true*/1;
                SoundPlayEffect( 13,15050, 0,0 ,0);


        }

        if (*pexper != fexp
	    && ((talk.active == 0 && show_inventory == 0 && spr[1].freeze == 0)
		|| fexp + 10 < fraise))

        {
                //update screen experience
                fexp += 10;
                //make noise here

                if (fexp > *pexper) fexp = *pexper;
                drawexp = /*true*/1;
                SoundPlayEffect( 13,29050, 0,0 ,0);

                if (fexp >= fraise)
                {

                        *pexper -= next;
                        fexp = 0;

                        script = load_script("lraise", 1, /*false*/0);
                        if (locate(script, "raise")) run_script(script);
                }
        }



        if (drawexp)
        {


                draw_exp();
        }


        if ( (flifemax != *plifemax) || (flife != *plife) )
        {
                if (flifemax < *plifemax) flifemax++;
                if (flifemax > *plifemax) flifemax--;
                if (flife > *plife) flife--;
                if (flife < *plife) flife++;
                if (flife > *plife) flife--;
                if (flife < *plife) flife++;
                draw_bar(flifemax, 190);
                draw_bar(flife, 451);
        }

        if ( fstrength != *pstrength)
        {
                if (fstrength < *pstrength) fstrength++;
                if (fstrength > *pstrength) fstrength--;
                SoundPlayEffect( 22,22050, 0,0 ,0);

                draw_strength();
        }

        if ( fdefense != *pdefense)
        {
                if (fdefense < *pdefense) fdefense++;
                if (fdefense > *pdefense) fdefense--;
                SoundPlayEffect( 22,22050, 0,0 ,0);
                draw_defense();
        }
        if ( fmagic != *pmagic)
        {
                if (fmagic < *pmagic) fmagic++;
                if (fmagic > *pmagic) fmagic--;
                SoundPlayEffect( 22,22050, 0,0 ,0);
                draw_magic();
        }

        if (fgold != *pgold)
        {
                if (fgold < *pgold)
                {
                        fgold += 20;
                        if (fgold > *pgold) fgold = *pgold;
                }

                if (fgold > *pgold)
                {
                        fgold -= 20;
                        if (fgold < *pgold) fgold = *pgold;
                }
                SoundPlayEffect( 14,22050, 0,0 ,0);
                draw_gold();
        }

        if (*pmagic_level < *pmagic_cost)
        {
                if (show_inventory == 0)
                        *pmagic_level += *pmagic;
                if (*pmagic_level > *pmagic_cost) *pmagic_level = *pmagic_cost;
        }
        if (*pmagic_cost > 0) if (*pmagic_level > 0)
        {
                int mnum = *pmagic_level * 100 / *pmagic_cost;
                if (mnum != last_magic_draw)
                {

                        draw_mlevel(mnum);

                        //draw_status_all();
                        last_magic_draw = mnum;


                }
        }


        spr[1].strength = fstrength;
        spr[1].defense = fdefense;


        if (flife < 1)
        {
                script = load_script("dinfo", 1000, /*false*/0);
                if (locate(script, "die")) run_script(script);
        }

}