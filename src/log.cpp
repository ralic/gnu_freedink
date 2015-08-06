/**
 * Different ways to give information to the user

 * Copyright (C) 1997, 1998, 1999, 2002, 2003  Seth A. Robinson
 * Copyright (C) 2007, 2008, 2009, 2014  Sylvain Beucler

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h> /* strerror */
#include <errno.h>
#include "log.h"
#include "paths.h"

char last_debug[200];
int debug_mode = 0;
static SDL_LogOutputFunction sdl_logger;
static FILE* out = NULL;

void log_output(void *userdata,
		int category, SDL_LogPriority priority,
		const char *message) {
  sdl_logger(userdata, category, priority, message);
  
  if (debug_mode)
    {
      // display message on screen in debug mode
      strncpy(last_debug, message, sizeof(last_debug) - 1);
      
      // also write to DEBUG.TXT
      if (out != NULL)
	{
	  fputs(message, out);
	  fputc('\n', out);
	}
    }
}

void log_init() {
  SDL_LogGetOutputFunction(&sdl_logger, NULL);
  SDL_LogSetOutputFunction(log_output, NULL);
}

void log_debug_on()
{
  debug_mode = 1;
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

  out = paths_dmodfile_fopen("DEBUG.TXT", "a");
  if (out == NULL) {
    log_error("Cannot open DEBUG.TXT: %s", strerror(errno));
    return;
  }
}

void log_debug_off()
{
  strcpy(last_debug, "");
  if (out != NULL)
    fclose(out);
  out = NULL;
  debug_mode = 0;
  SDL_LogResetPriorities();
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_ERROR);
  /* Avoid startup error about X11 missing symbols */
  SDL_LogSetPriority(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL);
  /* If you need to debug early: */
  /* SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG); */
}