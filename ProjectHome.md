**ParadizeLib** is a small input and video abstraction framework for various SDL based targets. The idea is to integrate the differences in various SDL targets into a common API.

Supported targets are:
  * Generic SDL such as PC, Mac (untested)
  * GP2X Wiz
  * GP2X Caanoo (untested)

A game or other multimedia application only links the ParadizeLib for the current binary target beside libSDL and does not need to keep its own abstraction layer.

The library provides abstraction for a digital joystick with up to 4 buttons, 3 other keys and a common mouse pointer interface.

To build ParadizeLib you will need CMake (http://www.cmake.org) and a suitable liBSDL for your target.