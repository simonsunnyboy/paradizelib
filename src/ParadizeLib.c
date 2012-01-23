/*
 * ParadizeLib.c
 * main implementation the library
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

#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "ParadizeLib.h"
#include "ParadizeLib_Joystick.h"

/* public data - can be read via Get-Functions() */
static SDL_Surface *screen; /* SDL_Surface for the screen */

static struct
{
	Uint16 mouse_x;
	Uint16 mouse_y;
	Uint8 mouse_button;
	Uint8 mouse_button_pressed;
	Uint8 joystick_state;
} InputDevicesStatus;

/* private data - for use by the lib only */
/**
 * holds value for Framerate control
 */
static Uint32 ParadizeLib_FrameRateNext;
static Uint8 InputState;

/* private functions */
#if(HAS_JOYSTICK_AXIS == 1)
void ParadizeLib_ProcessJoystick ( Uint8, Sint16 );
#endif

void ParadizeLib_ProcessJoystickButtons ( Uint8, Uint8 );

/**
 * Function: ParadizeLib_Init()
 *
 * Purpose: Inits the ParadizeLib and SDL and prepares the screen
 */
Uint16 ParadizeLib_Init ( char *WindowName )
{
	Uint32 VideoFlags = SDL_DOUBLEBUF | SDL_SWSURFACE;
#if(ENABLE_FULLSCREEN == 1)
	VideoFlags = ( VideoFlags | SDL_FULLSCREEN );
#endif

	/* Initialize SDL's video system and check for errors. */
	if ( SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) != 0 )
	{
		fprintf ( stderr, "ParadizeLib: Unable to initialize SDL: %s\n",
		          SDL_GetError() );
		return ( 1 );
	}

	/* Make sure SDL_Quit gets called when the program exits! */
	atexit ( ParadizeLib_Exit );
	/* Set videomode as configured */
	screen = SDL_SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, 16, VideoFlags );

	if ( screen == NULL )
	{
		fprintf ( stderr, "ParadizeLib: Unable to set video mode: %s\n",
		          SDL_GetError() );
		return ( 1 );
	}

	SDL_WM_SetCaption ( WindowName, NULL ); /* set window caption */
	SDL_ShowCursor ( SDL_DISABLE ); /* disable mouse pointer */
	ParadizeLib_FrameRateNext = SDL_GetTicks() + TICKS_PER_VBL;
	ParadizeLib_InitializeJoysticks();
	return ( 0 );
}

/**
 * Function: ParadizeLib_Exit()
 *
 * Purpose: - handles clean application exit, call via atexit()
 */
void ParadizeLib_Exit()
{
	ParadizeLib_StopJoystick();
	SDL_Quit();
}

/**
 * Function: ParadizeLib_Vsync()
 *
 * Purpose: - flips the screen and waits for the next "virtual" vertical blank
 *          - tries to maintain a fixed framerate (50fps by default)
 */
void ParadizeLib_Vsync()
{
	Uint32 now;
	SDL_Flip ( screen ); /* show other screen */
	/* framerate control: tries to hold 50fps per default */
	now = SDL_GetTicks();

	if ( ParadizeLib_FrameRateNext < now )
	{
		SDL_Delay ( 0 ); /* do not really wait but allow other software to get CPU time */
	}
	else
	{
		SDL_Delay ( ParadizeLib_FrameRateNext - now ); /* wait until next VBL */
	}

	ParadizeLib_FrameRateNext += TICKS_PER_VBL; /* for framerate control */
}

/**
 * Function: ParadizeLib_GetScreen()
 *
 * Purpose: returns pointer to the SDL_Surface for the actual screen
 */
SDL_Surface* ParadizeLib_GetScreen()
{
	return ( screen );
}

/**
 * Function: ParadizeLib_HandleInput()
 *
 * Purpose: read SDL Event Queue and signal any pending input events
 */
Uint8 ParadizeLib_HandleInput ( Uint8 HandleAllEvents )
{
	InputState = C_CONTINUE; /* reset InputState */
	SDL_Event CurrentEvent;
	SDL_keysym keysym;

	/* main event polling and filtering loop */
	while ( SDL_PollEvent ( &CurrentEvent ) == C_ON )
	{
		switch ( CurrentEvent.type )
		{
		case SDL_MOUSEMOTION:
			/* collect mouse coordinates */
			InputDevicesStatus.mouse_x = CurrentEvent.motion.x;
			InputDevicesStatus.mouse_y = CurrentEvent.motion.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			/* collect mouse coordinates + mouse button press */
			InputDevicesStatus.mouse_x = CurrentEvent.button.x;
			InputDevicesStatus.mouse_y = CurrentEvent.button.y;
			InputDevicesStatus.mouse_button = CurrentEvent.button.button;
			InputDevicesStatus.mouse_button_pressed = C_ON;
			break;
		case SDL_MOUSEBUTTONUP:
			/* mouse button has been released? */
			InputDevicesStatus.mouse_button_pressed = C_OFF;
			break;
		case SDL_KEYDOWN:
			keysym = CurrentEvent.key.keysym;

			/* if(HandleAllEvents == C_OFF) */
			if ( keysym.sym == SDLK_SPACE )
				InputState = C_SPACE;

			if ( keysym.sym == SDLK_ESCAPE )
				InputState = C_QUIT;

			if ( keysym.sym == SDLK_PAUSE )
				InputState = C_PAUSE;

			/* handle cursor key input and treat as joystick input: */
			if ( keysym.sym == SDLK_UP )
				InputDevicesStatus.joystick_state |= C_JOYSTICK_UP;

			if ( keysym.sym == SDLK_DOWN )
				InputDevicesStatus.joystick_state |= C_JOYSTICK_DOWN;

			if ( keysym.sym == SDLK_LEFT )
				InputDevicesStatus.joystick_state |= C_JOYSTICK_LEFT;

			if ( keysym.sym == SDLK_RIGHT )
				InputDevicesStatus.joystick_state |= C_JOYSTICK_RIGHT;

			break;
		case SDL_KEYUP:
			keysym = CurrentEvent.key.keysym;

			/* handle cursor key input and treat as joystick input: */
			if ( keysym.sym == SDLK_UP )
				InputDevicesStatus.joystick_state &= ~C_JOYSTICK_UP;

			if ( keysym.sym == SDLK_DOWN )
				InputDevicesStatus.joystick_state &= ~C_JOYSTICK_DOWN;

			if ( keysym.sym == SDLK_LEFT )
				InputDevicesStatus.joystick_state &= ~C_JOYSTICK_LEFT;

			if ( keysym.sym == SDLK_RIGHT )
				InputDevicesStatus.joystick_state &= ~C_JOYSTICK_RIGHT;

			break;
#if(HAS_JOYSTICK_AXIS == 1)
		case SDL_JOYAXISMOTION:
			ParadizeLib_ProcessJoystick ( CurrentEvent.jaxis.axis,CurrentEvent.jaxis.value );
			break;
#endif
		case SDL_JOYBUTTONUP:
		case SDL_JOYBUTTONDOWN:
			ParadizeLib_ProcessJoystickButtons ( CurrentEvent.jbutton.button,
			                                     CurrentEvent.jbutton.state );
			break;
		case SDL_JOYHATMOTION:

			switch ( CurrentEvent.jhat.value )
			{
			case SDL_HAT_UP:
				ParadizeLib_ProcessJoystick ( 0, 0 );
				ParadizeLib_ProcessJoystick ( 1, -32000 );
				break;
			case SDL_HAT_RIGHT:
				ParadizeLib_ProcessJoystick ( 0, 32000 );
				ParadizeLib_ProcessJoystick ( 1, 0 );
				break;
				break;
			case SDL_HAT_DOWN:
				ParadizeLib_ProcessJoystick ( 0, 0 );
				ParadizeLib_ProcessJoystick ( 1, 32000 );
				break;
			case SDL_HAT_LEFT:
				ParadizeLib_ProcessJoystick ( 0, -32000 );
				ParadizeLib_ProcessJoystick ( 1, 0 );
				break;
			case SDL_HAT_RIGHTUP:
				ParadizeLib_ProcessJoystick ( 0, 32000 );
				ParadizeLib_ProcessJoystick ( 1, -32000 );
				break;
			case SDL_HAT_RIGHTDOWN:
				ParadizeLib_ProcessJoystick ( 0, 32000 );
				ParadizeLib_ProcessJoystick ( 1, 32000 );
				break;
			case SDL_HAT_LEFTUP:
				ParadizeLib_ProcessJoystick ( 0, -32000 );
				ParadizeLib_ProcessJoystick ( 1, -32000 );
				break;
			case SDL_HAT_LEFTDOWN:
				ParadizeLib_ProcessJoystick ( 0, -32000 );
				ParadizeLib_ProcessJoystick ( 1, 32000 );
				break;
			default:
				ParadizeLib_ProcessJoystick ( 0, 0 );
				ParadizeLib_ProcessJoystick ( 1, 0 );
				break;
			}

			break;
		case SDL_QUIT:
			/* Window closer */
			InputState = C_QUIT;
			break;
		default:
			/* do nothing */
			break;
		}

		if ( InputState != C_CONTINUE )
			break; /* stop event collection if there was an quit or terminate event */
	}

	if ( InputState != C_CONTINUE )
	{
		while ( SDL_PollEvent ( &CurrentEvent ) == C_ON )
		{
			/* eat remaining events from the queue */
		}
	}

	return ( InputState );
}

#if(HAS_JOYSTICK_AXIS == 1)
/**
 * Function: ParadizeLib_ProcessJoystick()  NON PUBLIC
 *
 * Purpose: reads axis values from given joystick event and updates the digital joystick state accordingly
 */
void ParadizeLib_ProcessJoystick ( Uint8 CurrentAxis, Sint16 JoystickPosition )
{
	/* process x axis of joystick */
	if ( CurrentAxis == JOYSTICK_XAXIS )
	{
		InputDevicesStatus.joystick_state &= ~ ( C_JOYSTICK_LEFT | C_JOYSTICK_RIGHT );

		if ( JoystickPosition < -1* ( D_JOYSTICK_THRESHOLD ) )
			InputDevicesStatus.joystick_state |= C_JOYSTICK_LEFT;

		if ( JoystickPosition > ( D_JOYSTICK_THRESHOLD ) )
			InputDevicesStatus.joystick_state |= C_JOYSTICK_RIGHT;
	}

	/* process x axis of joystick */
	if ( CurrentAxis == JOYSTICK_YAXIS )
	{
		InputDevicesStatus.joystick_state &= ~ ( C_JOYSTICK_UP | C_JOYSTICK_DOWN );

		if ( JoystickPosition < -1* ( D_JOYSTICK_THRESHOLD ) )
			InputDevicesStatus.joystick_state |= C_JOYSTICK_UP;

		if ( JoystickPosition > ( D_JOYSTICK_THRESHOLD ) )
			InputDevicesStatus.joystick_state |= C_JOYSTICK_DOWN;
	}
}
#endif

/**
 * Function: ParadizeLib_ProcessJoystickButtons()  NON PUBLIC
 *
 * Purpose: reads the joysticks buttons updates the digital joystick state accordingly
 */
void ParadizeLib_ProcessJoystickButtons ( Uint8 Button, Uint8 ButtonState )
{
	if ( Button == JOYSTICK_BUTTON1 )
	{
		if ( ButtonState == SDL_PRESSED )
			InputDevicesStatus.joystick_state |= C_JOYSTICK_BUTTON1;
		else
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_BUTTON1;
	}

	if ( Button == JOYSTICK_BUTTON2 )
	{
		if ( ButtonState == SDL_PRESSED )
			InputDevicesStatus.joystick_state |= C_JOYSTICK_BUTTON2;
		else
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_BUTTON2;
	}

	if ( Button == JOYSTICK_BUTTON3 )
	{
		if ( ButtonState == SDL_PRESSED )
			InputDevicesStatus.joystick_state |= C_JOYSTICK_BUTTON3;
		else
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_BUTTON3;
	}

	if ( Button == JOYSTICK_BUTTON4 )
	{
		if ( ButtonState == SDL_PRESSED )
			InputDevicesStatus.joystick_state |= C_JOYSTICK_BUTTON4;
		else
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_BUTTON4;
	}

#if(defined __GP2X_WIZ)

	/* GP2X Wiz handles joystick directions as additional buttons */
	if ( Button == GP2X_BUTTON_UP )
	{
		if ( ButtonState == SDL_PRESSED )
			InputDevicesStatus.joystick_state |= C_JOYSTICK_UP;
		else
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_UP;
	}

	if ( Button == GP2X_BUTTON_DOWN )
	{
		if ( ButtonState == SDL_PRESSED )
			InputDevicesStatus.joystick_state |= C_JOYSTICK_DOWN;
		else
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_DOWN;
	}

	if ( Button == GP2X_BUTTON_LEFT )
	{
		if ( ButtonState == SDL_PRESSED )
			InputDevicesStatus.joystick_state |= C_JOYSTICK_LEFT;
		else
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_LEFT;
	}

	if ( Button == GP2X_BUTTON_RIGHT )
	{
		if ( ButtonState == SDL_PRESSED )
			InputDevicesStatus.joystick_state |= C_JOYSTICK_RIGHT;
		else
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_RIGHT;
	}

	if ( Button == GP2X_BUTTON_UPRIGHT )
	{
		if ( ButtonState == SDL_PRESSED )
		{
			InputDevicesStatus.joystick_state |= C_JOYSTICK_RIGHT;
			InputDevicesStatus.joystick_state |= C_JOYSTICK_UP;
		}
		else
		{
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_RIGHT;
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_UP;
		}
	}

	if ( Button == GP2X_BUTTON_UPLEFT )
	{
		if ( ButtonState == SDL_PRESSED )
		{
			InputDevicesStatus.joystick_state |= C_JOYSTICK_LEFT;
			InputDevicesStatus.joystick_state |= C_JOYSTICK_UP;
		}
		else
		{
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_LEFT;
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_UP;
		}
	}

	if ( Button == GP2X_BUTTON_DOWNLEFT )
	{
		if ( ButtonState == SDL_PRESSED )
		{
			InputDevicesStatus.joystick_state |= C_JOYSTICK_LEFT;
			InputDevicesStatus.joystick_state |= C_JOYSTICK_DOWN;
		}
		else
		{
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_LEFT;
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_DOWN;
		}
	}

	if ( Button == GP2X_BUTTON_DOWNRIGHT )
	{
		if ( ButtonState == SDL_PRESSED )
		{
			InputDevicesStatus.joystick_state |= C_JOYSTICK_RIGHT;
			InputDevicesStatus.joystick_state |= C_JOYSTICK_DOWN;
		}
		else
		{
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_RIGHT;
			InputDevicesStatus.joystick_state &= ~C_JOYSTICK_DOWN;
		}
	}

	/* handle extra Wiz buttons for basic key funtionalities ESC and SPACE */
	if ( ( Button == GP2X_BUTTON_MENU ) && ( ButtonState == SDL_RELEASED ) )
	{
		InputState = C_QUIT;
	}

	if ( ( Button == GP2X_BUTTON_SELECT ) && ( ButtonState == SDL_RELEASED ) )
	{
		InputState = C_SPACE;
	}

	if ( ( Button == GP2X_BUTTON_L ) && ( ButtonState == SDL_RELEASED ) )
	{
		InputState = C_PAUSE;
	}

#elif(defined __GP2X_CAANOO)

	/* handle extra Caanoo buttons for basic key funtionalities ESC and SPACE */
	if ( ( Button == CAANOO_HOME_BUTTON ) && ( ButtonState == SDL_RELEASED ) )
	{
		InputState = C_QUIT;
	}

	if ( ( Button == CAANOO_I_BUTTON ) && ( ButtonState == SDL_RELEASED ) )
	{
		InputState = C_SPACE;
	}

	if ( ( Button == CAANOO_II_BUTTON ) && ( ButtonState == SDL_RELEASED ) )
	{
		InputState = C_PAUSE;
	}

#endif
}

/**
 * Function: ParadizeLib_MouseX()
 *
 * Purpose: returns the x coordinate of the mouse pointer
 */
Uint16 ParadizeLib_MouseX()
{
	return ( InputDevicesStatus.mouse_x );
}

/**
 * Function: ParadizeLib_MouseY()
 *
 * Purpose: returns the y coordinate of the mouse pointer
 */
Uint16 ParadizeLib_MouseY()
{
	return ( InputDevicesStatus.mouse_y );
}

/**
 * Function: ParadizeLib_MouseButton()
 *
 * Purpose: returns the last mouse button pressed
 */
Uint8 ParadizeLib_MouseButton()
{
	if ( InputDevicesStatus.mouse_button_pressed == C_ON )
	{
		return ( InputDevicesStatus.mouse_button );
	}
	else
	{
		return ( 0 );
	}
}

/**
 * Function: ParadizeLib_Joystick()
 *
 * Purpose: returns the state of the current active joystick (bitcoded ATARI compatible)
 */
Uint8 ParadizeLib_Stick()
{
	return ( InputDevicesStatus.joystick_state );
}

