/*
 * testprogram.c
 * Simple testprogram for Paradize Lib
 *
 *  Created on: 28.03.2009
 *      Author: marndt
 *
 * Copyright (c) 2009/2012   The ParadizeLib Team (see authors.txt)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>

#include "ParadizeLib.h"

int main()
{
	Uint16 vbls,i;
	Uint32 color;
	Uint8 blue,js,mask;
	SDL_Surface *screen;
	SDL_Rect src;

	if ( ParadizeLib_Init ( "ParadizeLib Testprogram" ) == 0 )
		fprintf ( stderr,"SDL ok!" );

	screen = ParadizeLib_GetScreen();
	MAINLOOP()
	{
		ParadizeLib_Vsync();

		switch ( ParadizeLib_HandleInput ( C_OFF ) )
		{
		case C_QUIT:
			return ( 0 );
			break;
		case C_SPACE:
			vbls=0;
			blue = 0;
			break;
		case C_PAUSE:
			blue = 255;
			break;
		default:
			break;
		}

		/* use SDLs builtin function for drawing rectangles... */
		src.x=0;
		src.y=0;
		src.w =320;
		src.h =240;
		color = SDL_MapRGB ( screen->format, ( vbls % 255 ),0,blue );
		SDL_FillRect ( screen,&src,color );

		/* test joystick position */
		if ( ParadizeLib_Stick() & C_JOYSTICK_UP )
		{
			src.x=152;
			src.y=0;
			src.w =16;
			src.h =16;
			color = SDL_MapRGB ( screen->format,0,0,200 );
			SDL_FillRect ( screen,&src,color );
		}

		if ( ParadizeLib_Stick() & C_JOYSTICK_DOWN )
		{
			src.x=152;
			src.y=184;
			src.w =16;
			src.h =16;
			color = SDL_MapRGB ( screen->format,0,0,200 );
			SDL_FillRect ( screen,&src,color );
		}

		if ( ParadizeLib_Stick() & C_JOYSTICK_LEFT )
		{
			src.x=0;
			src.y=92;
			src.w =16;
			src.h =16;
			color = SDL_MapRGB ( screen->format,0,0,200 );
			SDL_FillRect ( screen,&src,color );
		}

		if ( ParadizeLib_Stick() & C_JOYSTICK_RIGHT )
		{
			src.x=304;
			src.y=92;
			src.w =16;
			src.h =16;
			color = SDL_MapRGB ( screen->format,0,0,200 );
			SDL_FillRect ( screen,&src,color );
		}

		if ( ParadizeLib_Stick() & C_JOYSTICK_ANYBUTTON )
		{
			src.x=152;
			src.y=92;
			src.w =16;
			src.h =16;
			color = SDL_MapRGB ( screen->format,0,200,0 );
			SDL_FillRect ( screen,&src,color );
		}

		/* draw squares for each joystick button pressed: */
		js = ParadizeLib_Stick();
		mask = C_JOYSTICK_BUTTON1;

		for ( i=1; i < 5; i++ )
		{
			if ( js & mask )
			{
				src.x = i * ( 320 / 6 );
				src.y = 200;
				src.w = 16;
				src.h = 16;
				color = SDL_MapRGB ( screen->format,255,255-i* ( 256/6 ),0 );
				SDL_FillRect ( screen,&src,color );
			}

			mask = ( mask >> 1 );
		}

		/* emulated mouse pointer*/
		src.x = ParadizeLib_MouseX();
		src.y = ParadizeLib_MouseY();
		src.w = 16;
		src.h = 16;

		if ( ParadizeLib_MouseButton() == 1 )
		{
			color = SDL_MapRGB ( screen->format,0,255,0 );
		}
		else
		{
			color = SDL_MapRGB ( screen->format,255,255,255 );
		}

		SDL_FillRect ( screen,&src,color );
		vbls = vbls + 16;
	}
	return ( 0 );
}
