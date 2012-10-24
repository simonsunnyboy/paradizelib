cmake_minimum_required(VERSION 2.8)
#Name your project here
project(ParadizeLib)

OPTION(ENABLE_GP2X_WIZ "compile for GP2X Wiz" OFF)
OPTION(ENABLE_GP2X_CAANOO "compile for GP2X Caanoo" OFF)

#Sends the -std=c99 flag to the gcc compiler
#add_definitions(-std=c99)

include_directories(includes)
include_directories(.)

if(ENABLE_GP2X_WIZ)
    add_definitions(-D__GP2X_WIZ)
elseif(ENABLE_GP2X_CAANOO)    
    add_definitions(-D__GP2X_CAANOO)
endif()

add_library(paradizelib SHARED
	config.h
	includes/ParadizeLib.h
	includes/ParadizeLib_Joystick.h
	src/ParadizeLib.c
	src/ParadizeLib_Joystick.c
)

Find_Package ( SDL REQUIRED )
#Find_Package ( SDL_image REQUIRED ) # if using SDL_image

link_libraries (
	paradizelib
   ${SDL_LIBRARY}
#   ${SDLIMAGE_LIBRARY} # if using SDL_image, obviously
   SDLmain # Sadly not included in SDL_LIBRARY variable
)

add_executable(testprogram src/testprogram.c)
