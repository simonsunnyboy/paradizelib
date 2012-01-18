#
# ParadizeLib Makefile
# (c) 2009 by Simon Sunnyboy / Paradize
#

CC = gcc
CFLAGS = -Wall
SDL_FLAGS = `sdl-config --cflags`
SDL_LIB = `sdl-config --libs`
DATE = `date +%Y%m%d`


PDZLIB_INCLUDE = -Iincludes/ -I.
PDZLIB_OBJECTS = objs/ParadizeLib.o objs/ParadizeLib_Joystick.o

all: lib src/testprogram.c config.h
	$(CC) $(CFLAGS) $(PDZLIB_INCLUDE) $(SDL_FLAGS) -o testprogram src/testprogram.c $(PDZLIB_OBJECTS) $(SDL_LIB)

lib: $(PDZLIB_OBJECTS)

objs/ParadizeLib.o: src/ParadizeLib.c includes/ParadizeLib.h config.h
	  $(CC) $(CFLAGS) $(PDZLIB_INCLUDE) $(SDL_FLAGS) -o objs/ParadizeLib.o -c src/ParadizeLib.c
	  
objs/ParadizeLib_Joystick.o: src/ParadizeLib_Joystick.c includes/ParadizeLib_Joystick.h config.h
	  $(CC) $(CFLAGS) $(PDZLIB_INCLUDE) $(SDL_FLAGS) -o objs/ParadizeLib_Joystick.o -c src/ParadizeLib_Joystick.c

	
clean:
	rm objs/*.o
	rm testprogram

backup:
	tar cvzf ParadizeLib_$(DATE).tar.gz src/ includes/ objs/ Makefile
	