#include <SDL2/SDL.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

struct CustomWindow {
    //The window we'll be rendering to
	SDL_Window* window = nullptr;

    CustomWindow () {
        if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	    } else {
		    //Create window
		    window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		    
            if ( window == nullptr )
		    {
		    	printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
                return;
		    }
	    }
    }

    ~CustomWindow () {
        if (window != nullptr) {
            SDL_DestroyWindow(window);
        }
    }
};