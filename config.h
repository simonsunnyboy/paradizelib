/*
 * config.h
 * Project configuration for ParadizeLib
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

#ifndef CONFIG_H_
#define CONFIG_H_

	#define SCREEN_WIDTH 	320
	#define SCREEN_HEIGHT	240

	/* Ticks to wait before VBL: 1000ms / 20 = 50fps */
	#define TICKS_PER_VBL	 20

	/* functionality switches */
	#define ENABLE_FULLSCREEN		0
	#define ENABLE_CONSOLE_DEBUG    1

	/* target specific defines and switches */
	#if(defined __GP2X_WIZ)

		#define GP2X_BUTTON_UP              (0)
		#define GP2X_BUTTON_DOWN            (4)
		#define GP2X_BUTTON_LEFT            (2)
		#define GP2X_BUTTON_RIGHT           (6)
		#define GP2X_BUTTON_UPLEFT          (1)
		#define GP2X_BUTTON_UPRIGHT         (7)
		#define GP2X_BUTTON_DOWNLEFT        (3)
		#define GP2X_BUTTON_DOWNRIGHT       (5)
		#define GP2X_BUTTON_CLICK           (18)
		#define GP2X_BUTTON_A               (12)
		#define GP2X_BUTTON_B               (13)
		#define GP2X_BUTTON_X               (14)
		#define GP2X_BUTTON_Y               (15)
		#define GP2X_BUTTON_L               (10)
		#define GP2X_BUTTON_R               (11)
		#define GP2X_BUTTON_MENU            (8)
		#define GP2X_BUTTON_SELECT          (9)
		#define GP2X_BUTTON_VOLUP           (16)
		#define GP2X_BUTTON_VOLDOWN         (17)

		#define HAS_JOYSTICK_AXIS			0			/* do we need to handle joystick movement as axis or buttons? */
	#elif(defined __GP2X_CAANOO)
	
		#define CAANOO_A_BUTTON 0
		#define CAANOO_X_BUTTON 1
		#define CAANOO_B_BUTTON 2
		#define CAANOO_Y_BUTTON 3
		#define CAANOO_L_BUTTON 4
		#define CAANOO_R_BUTTON 5
		#define CAANOO_HOME_BUTTON 6
		#define CAANOO_I_BUTTON 8
		#define CAANOO_II_BUTTON 9
		#define HAS_JOYSTICK_AXIS			1			/* do we need to handle joystick movement as axis or buttons? */
	#else
		#define HAS_JOYSTICK_AXIS			1
	#endif

#endif /* CONFIG_H_ */
