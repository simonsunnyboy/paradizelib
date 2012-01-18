/*
 * ParadizeLib_Joystick.c
 * abstracts joystick handling
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

#include <SDL/SDL.h>
#include <stdio.h>

#include "ParadizeLib.h"
#include "ParadizeLib_Joystick.h"
#include "config.h"

/**
 * Number of SDL joysticks found on the system
 */
int JoystickCount;
/**
 * Number of selected active SDL joystick
 */
Uint8 ActiveJoystick;

SDL_Joystick *CurrentJoystick;

/**
 * Function: ParadizeLib_InitializeJoysticks()
 *
 * Purpose: initializes joysticks
 * Limitation: only one joystick can be active at a time
 */
Uint8 ParadizeLib_InitializeJoysticks()
{
	Uint8 i;
	JoystickCount = SDL_NumJoysticks();
#if(ENABLE_CONSOLE_DEBUG == C_ON)
	fprintf ( stderr,"ParadizeLib DEBUG: %i joysticks found\n", JoystickCount );

	if ( JoystickCount > 0 )
	{
		for ( i = 0; i< JoystickCount; i++ )
		{
			CurrentJoystick = SDL_JoystickOpen ( i );

			if ( CurrentJoystick == NULL )
			{
				fprintf ( stderr,"ParadizeLib: Unable to open joystick %i.\n", i+1 );
			}
			else
			{
				fprintf ( stderr,"ParadizeLib DEBUG: Joystick %i: %s\n", i+1, SDL_JoystickName ( i ) );
				SDL_JoystickClose ( CurrentJoystick );
				CurrentJoystick = NULL;
			}
		}
	}

#endif
	ParadizeLib_SelectJoystick ( 1 );  /* try to use first available joystick */
	return ( JoystickCount );
}

/**
 * Function: ParadizeLib_SelectJoystick()
 *
 * Purpose: select one of the connected joysticks to create input
 */
void ParadizeLib_SelectJoystick ( Uint8 JoystickNumber )
{
	ParadizeLib_StopJoystick();  /* close old joystick if open */

	if (  ( JoystickCount   > 0 )
	        && ( JoystickNumber <= JoystickCount )
	   )
	{
		ActiveJoystick = JoystickNumber;
		CurrentJoystick = SDL_JoystickOpen ( JoystickNumber-1 );

		if ( CurrentJoystick == NULL )
		{
			ActiveJoystick = 0;
#if(ENABLE_CONSOLE_DEBUG == C_ON)
			fprintf ( stderr,"ParadizeLib: Unable to open joystick %i.\n", JoystickNumber );
#endif
		}
	}

#if(ENABLE_CONSOLE_DEBUG == C_ON)
	fprintf ( stderr,"ParadizeLib: Joystick %i has %i hats.\n", JoystickNumber, SDL_JoystickNumHats ( CurrentJoystick ) );
	fprintf ( stderr,"ParadizeLib: Joystick %i has %i trackballs.\n", JoystickNumber, SDL_JoystickNumBalls ( CurrentJoystick ) );
#endif
}

/**
 * Function: ParadizeLib_StopJoystick()
 *
 * Purpose: closes devices for current joystick (if necessary)
 */
void ParadizeLib_StopJoystick()
{
	if ( CurrentJoystick != NULL )
	{
		SDL_JoystickClose ( CurrentJoystick );
		ActiveJoystick = 0;
		CurrentJoystick = NULL;
	}
}

/**
 * Function: ParadizeLib_GetJoystickCount()
 *
 * Purpose: returns number of available joysticks
 */
Uint8 ParadizeLib_GetJoystickCount()
{
	return ( JoystickCount );
}

/**
 * Function: ParadizeLib_AreJoysticksAvailable()
 *
 * Purpose: returns if any joysticks are available
 */
Uint8 ParadizeLib_AreJoysticksAvailable()
{
	return ( ( JoystickCount > 0 ) );
}

