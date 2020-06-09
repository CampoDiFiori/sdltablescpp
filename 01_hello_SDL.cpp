/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <string>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

struct CustomSurface {
    SDL_Surface *sdl_surface_ptr = nullptr;

    CustomSurface() : sdl_surface_ptr(nullptr) {
        std::cout << "Created from null" << std::endl;
    }

    CustomSurface(SDL_Surface *sdlSurface) {
        std::cout << "Created" << std::endl;
        sdl_surface_ptr = sdlSurface;
    }

    CustomSurface &operator=(CustomSurface &&other) {
        if (sdl_surface_ptr != nullptr) {
            std::cout << "Dropped" << std::endl;
            SDL_FreeSurface(sdl_surface_ptr);
        }
        sdl_surface_ptr = other.sdl_surface_ptr;
        other.sdl_surface_ptr = nullptr;
        std::cout << "Moved" << std::endl;
    }

    auto loadSurface(std::string path) -> void {
        sdl_surface_ptr = SDL_LoadBMP(path.c_str());

        if (sdl_surface_ptr == nullptr) {
            throw SDL_GetError();
        }
    }

    ~CustomSurface() {
        if (sdl_surface_ptr != nullptr) {
            std::cout << "Dropped" << std::endl;
            SDL_FreeSurface(sdl_surface_ptr);
        }
        sdl_surface_ptr = nullptr;
    }
};

class CustomWindow {
    // The window we'll be rendering to
    SDL_Window *window;
    CustomSurface screenSurface;
    CustomSurface imageSurface;

  public:
    CustomWindow() : window(nullptr), screenSurface(nullptr), imageSurface(nullptr) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw SDL_GetError();
        }
        // Create window
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (window == nullptr) {
            throw SDL_GetError();
        }

        screenSurface = SDL_GetWindowSurface(window);

        if (screenSurface.sdl_surface_ptr == nullptr) {
            throw SDL_GetError();
        }

        imageSurface = SDL_GetWindowSurface(window);

        if (imageSurface.sdl_surface_ptr == nullptr) {
            throw SDL_GetError();
        }
    }

    auto loadNewImage(std::string path) -> void {
        // auto tmp = std::move(CustomSurface());
        imageSurface.loadSurface(path);

        // converting 24 bit surface loaded by default to whatever our screen supports
        imageSurface.sdl_surface_ptr = SDL_ConvertSurface(imageSurface.sdl_surface_ptr, screenSurface.sdl_surface_ptr->format, 0);

        if (imageSurface.sdl_surface_ptr == nullptr) {
            throw SDL_GetError();
        }
    }

    auto draw() -> void {

        //Apply the image stretched
        SDL_Rect stretchRect;
        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = SCREEN_WIDTH;
        stretchRect.h = SCREEN_HEIGHT;

        // Apply the image
        if (SDL_BlitSurface(imageSurface.sdl_surface_ptr, nullptr, screenSurface.sdl_surface_ptr, nullptr) < 0) {
            std::cerr << SDL_GetError() << std::endl;
        }

        //Update the surface
        SDL_UpdateWindowSurface(window);
    }

    ~CustomWindow() {
        if (window != nullptr) {
            SDL_DestroyWindow(window);
        }

        window = nullptr;

        SDL_Quit();
    }
};

int main(int argc, char *args[]) {
    try {
        CustomWindow custom_window{};
        auto quit = false;
        SDL_Event e;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }

                switch (e.key.keysym.sym) {
                case SDLK_UP:
                    custom_window.loadNewImage("barbara_gray.bmp");
                    break;
                case SDLK_DOWN:
                    custom_window.loadNewImage("lena_gray.bmp");
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                default:
                    break;
                }
            }

            custom_window.draw();
        }
    } catch (std::string e) {
        std::cout << e << std::endl;
        return 1;
    }
    return 0;
}