/*
 * ParadizeLib.h
 * public header for the Paradize Lib
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

#ifndef PARADIZELIB_H_
#define PARADIZELIB_H_

#include <SDL/SDL.h>

/* Constants ParadizeLib */

#define C_CONTINUE          0
#define C_QUIT              1
#define C_SPACE             2
#define C_PAUSE             3

#define C_OFF               0
#define C_ON                1

#define C_JOYSTICK_UP       1
#define C_JOYSTICK_DOWN     2
#define C_JOYSTICK_LEFT     4
#define C_JOYSTICK_RIGHT    8
#define C_JOYSTICK_BUTTON1  128
#define C_JOYSTICK_BUTTON2  64
#define C_JOYSTICK_BUTTON3  32
#define C_JOYSTICK_BUTTON4  16
#define C_JOYSTICK_ANYBUTTON (C_JOYSTICK_BUTTON1 |C_JOYSTICK_BUTTON2 |C_JOYSTICK_BUTTON3 |C_JOYSTICK_BUTTON4 )

#define MAINLOOP()      while(1)

/* Function prototypes ParadizeLib */
Uint16 ParadizeLib_Init ( char *WindowName );
void ParadizeLib_Exit ( void );
void ParadizeLib_Vsync ( void );
SDL_Surface* ParadizeLib_GetScreen ( void );
Uint8 ParadizeLib_HandleInput ( Uint8 );
Uint16 ParadizeLib_MouseX ( void );
Uint16 ParadizeLib_MouseY ( void );
Uint8 ParadizeLib_MouseButton ( void );
Uint8 ParadizeLib_Stick ( void );

#endif /* PARADIZELIB_H_ */
