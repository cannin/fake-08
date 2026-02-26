#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "../../SDL2Common/source/sdl2basehost.h"
#include "../../../source/hostVmShared.h"
#include "../../../source/nibblehelpers.h"
#include "../../../source/filehelpers.h"
#include "../../../source/logger.h"

// sdl
#include <SDL2/SDL.h>

#define WINDOW_SIZE_X 640
#define WINDOW_SIZE_Y 480

#define WINDOW_FLAGS SDL_WINDOW_FULLSCREEN
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED
#define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888

string _wiiSettingsDir = "fake08";
string _wiiSettingsPrefix = "fake08/";
string _wiiCartDir = "sd:/p8carts";
string _wiiCustomBiosLua = "cartpath = \"sd:/p8carts/\"\n"
        "pausebtn = \"+\"";

Host::Host(int windowWidth, int windowHeight) 
{
    fatInitDefault();

    struct stat st = {0};

    // Try SD first, fallback to USB
    if (chdir("sd:/") != 0) {
        chdir("usb:/");
    }

    if (stat(_wiiSettingsDir.c_str(), &st) == -1) {
        mkdir(_wiiSettingsDir.c_str(), 0777);
    }
    
    string cartdatadir = _wiiSettingsPrefix + "cdata";
    if (stat(cartdatadir.c_str(), &st) == -1) {
        mkdir(cartdatadir.c_str(), 0777);
    }
	
    setPlatformParams(
        WINDOW_SIZE_X,
        WINDOW_SIZE_Y,
        WINDOW_FLAGS,
        RENDERER_FLAGS,
        PIXEL_FORMAT,
        _wiiSettingsPrefix,
        _wiiCustomBiosLua,
        _wiiCartDir
    );
}

InputState_t Host::scanInput(){
    currKDown = 0;
    uint8_t kUp = 0;
    stretchKeyPressed = false;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_JOYBUTTONDOWN:
                switch (event.jbutton.button) {
                    case 0:  currKDown |= P8_KEY_X; break; // A
                    case 1:  currKDown |= P8_KEY_O; break; // B
                    case 4:  stretchKeyPressed = true; break; // -
                    case 5:  currKDown |= P8_KEY_PAUSE; break; // +
                    case 6:  quit = 1; break; // Home
                    case 7:  currKDown |= P8_KEY_UP; break;
                    case 8:  currKDown |= P8_KEY_DOWN; break;
                    case 9:  currKDown |= P8_KEY_LEFT; break;
                    case 10: currKDown |= P8_KEY_RIGHT; break;
                }
                break;
            case SDL_JOYBUTTONUP:
                switch (event.jbutton.button) {
                    case 0:  kUp |= P8_KEY_X; break;
                    case 1:  kUp |= P8_KEY_O; break;
                    case 5:  kUp |= P8_KEY_PAUSE; break;
                    case 7:  kUp |= P8_KEY_UP; break;
                    case 8:  kUp |= P8_KEY_DOWN; break;
                    case 9:  kUp |= P8_KEY_LEFT; break;
                    case 10: kUp |= P8_KEY_RIGHT; break;
                }
                break;
            case SDL_QUIT:
                quit = 1;
                break;
        }
    }

    currKHeld |= currKDown;
    currKHeld ^= kUp;
    
    return InputState_t {
        currKDown,
        currKHeld,
        0, 0, 0, // No mouse/touch for now
        false, "" // No keyboard for now
    };
}

vector<string> Host::listcarts(){
    vector<string> carts;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (_cartDirectory.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (isCartFile(ent->d_name)){
                carts.push_back(ent->d_name);
            }
        }
        closedir (dir);
    }
    
    return carts;
}

std::vector<std::string> Host::listdirs() {
    std::vector<std::string> dirs;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (_cartDirectory.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_name[0] == '.') {
                continue;
            }
            std::string fullPath = _cartDirectory + "/" + ent->d_name;
            DIR* testDir = opendir(fullPath.c_str());
            if (testDir != NULL) {
                closedir(testDir);
                dirs.push_back(ent->d_name);
            }
        }
        closedir (dir);
    }
    
    return dirs;
}
