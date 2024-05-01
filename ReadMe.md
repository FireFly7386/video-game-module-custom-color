# Flipper Video Game Module (Powered by Raspberry Pi)
A fork of the flipperzero vgm firmware, which allows you to change the color of the video output.

## Flashing
Download the latest release *.uf2 (Coming up).
Hold the boot button on you vgm while plugging it in.
Drop the .uf2 in the RPI-RP2 Folder.
You can change the color either by using the vgm cli with the command: color <orange|red|green|blue|magenta|yellow> or with the Flipper VGM Color App from the Flipper App Store (App isnt available yet).

## Building

Requirements: 

- arm-none-eabi-gcc
- CMake
- Protobuf

## Getting Source Code

	git clone --recursive https://github.com/FireFly7386/video-game-module-custom-color

Make sure that all git sub-modules was recursively cloned.

## Compiling

	# In project folder
	( cd build && cmake .. && make )

Compiled firmware can be found in `app` folder.

## Flashing

- Press and hold boot button, plug VGM into your computer USB
- Copy `vgm-fw-*.uf2` from `build/app` folder to newly appeared drive
