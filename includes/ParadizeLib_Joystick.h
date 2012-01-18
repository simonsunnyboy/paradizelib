/*
 * ParadizeLib_Joystick.h
 * internal header file for joystick handling
 *
 *  Created on: 29.03.2009
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

#include "config.h"

#ifndef PARADIZELIB_JOYSTICK_H_
#define PARADIZELIB_JOYSTICK_H_

/* constant for joystick submodule */
#define D_JOYSTICK_THRESHOLD    16383

#define JOYSTICK_XAXIS          0
#define JOYSTICK_YAXIS          1

#if(defined __GP2X_WIZ)
/* defines for GP2X Wiz target only */
#define JOYSTICK_BUTTON1    GP2X_BUTTON_B
#define JOYSTICK_BUTTON2    GP2X_BUTTON_A
#define JOYSTICK_BUTTON3    GP2X_BUTTON_X
#define JOYSTICK_BUTTON4    GP2X_BUTTON_Y
#elif(defined __GP2X_CAANOO)
/* defines for GP2X Caanoo target only */
#define JOYSTICK_BUTTON1    CAANOO_BUTTON_B
#define JOYSTICK_BUTTON2    CAANOO_BUTTON_A
#define JOYSTICK_BUTTON3    CAANOO_BUTTON_X
#define JOYSTICK_BUTTON4    CAANOO_BUTTON_Y
#else
#define JOYSTICK_BUTTON1    0
#define JOYSTICK_BUTTON2    1
#define JOYSTICK_BUTTON3    2
#define JOYSTICK_BUTTON4    3
#endif

Uint8 ParadizeLib_InitializeJoysticks ( void );
void ParadizeLib_StopJoystick ( void );
void ParadizeLib_SelectJoystick ( Uint8 );
Uint8 ParadizeLib_GetJoystickCount ( void );
Uint8 ParadizeLib_AreJoysticksAvailable ( void );

#endif /* PARADIZELIB_JOYSTICK_H_ */
