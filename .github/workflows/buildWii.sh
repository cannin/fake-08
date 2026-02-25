#!/bin/bash
source /etc/profile.d/devkit-env.sh

# Install Wii SDL2 portlibs
dkp-pacman -Syu --noconfirm wii-sdl2 wii-sdl2_image wii-sdl2_mixer wii-sdl2_ttf wii-sdl2_gfx wii-pkg-config

cd /build_dir

# Build Wii
make wii
