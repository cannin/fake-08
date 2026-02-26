source /etc/profile.d/devkit-env.sh

cd /build_dir

dkp-pacman -Syyu --noconfirm wii-sdl2 wii-sdl2_gfx wii-sdl2_mixer wii-sdl2_ttf wii-sdl2_image

#Build dol
make wii
