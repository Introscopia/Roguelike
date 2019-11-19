#include "fundamentals.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "SDL2_gfx/SDL2_gfxPrimitives.h"
#include "SDL2_gfx/SDL2_framerate.h"

#ifdef WIN32
#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif

#include "structs.h"
#include "map.h"


Uint32 time_passed = 0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~O~~~~~~~~~~| M A I N |~~~~~~~~~~~O~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char *argv[]){

    //HWND hwnd_win = GetConsoleWindow();
    //ShowWindow(hwnd_win,SW_HIDE);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    int width = 640;
    int height = 480;
    int cx, cy;
    bool loop = 1;
    int zoomI = 0;
    double zoom = 1;
    double tx = 0, ty = 0;
    int mouseX, mouseY, pmouseX, pmouseY;

    Uint32 then, now, frames;
    FPSmanager fpsm;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }
    SDL_MaximizeWindow( window );
    SDL_GetWindowSize( window, &width, &height );
    cx = width / 2;
    cy = height / 2;

    srand (time(NULL));// prime the random number generator

    IMG_Init(IMG_INIT_PNG);

    const SDL_Color black = {0, 0, 0, 255};
    const SDL_Color white = {255, 255, 255, 255};
    //Uint32 rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif


    Map *map = load_tmx( "Assets/gardens.tmx" );

    SDL_Surface *surf = IMG_Load( "Assets/Kenney 1-bit colored_transparent.png" );
    if( surf == NULL ){
    	printf( "load failed: %s", IMG_GetError() );
    	return 0;
    }
    SDL_Texture *tilesheet_color = SDL_CreateTextureFromSurface( renderer, surf );
    SDL_FreeSurface( surf );
    SDL_Texture *tilesheet_mono;

    // I'm harcoding a lot of the tilesheet loading stuff here... Sorry!
    // I'm also assuming they're square.
    int tilewidth = 16;
    int margin = 0;
    int spacing = 1;
    int total = 1024;
    int row = 32;
    SDL_Rect* srcrects = (SDL_Rect*) malloc( total * sizeof(SDL_Rect) );
    int xi = 0, yi = 0;
    int e = tilewidth + spacing;
    for (int i = 0; i < total; ++i){
    	srcrects[i] = (SDL_Rect){ margin + xi*e, margin + yi*e, tilewidth, tilewidth };
    	++xi;
    	if( xi == row ){
    		xi = 0;
    		++yi;
    	}
    }

    Entity player;
    player.sprite_ID = 25;
    player.x = 17;
    player.y = 40;


    //evs = edges_vertices( 0, 0, row, vrow, R );
    //vas = vertex_adjacencies( 0, 0, row, vrow, centers_length );
    zoomI = 10;
    zoom = 2.593742;
    bool up = 0, down = 0, left = 0, right = 0;
    int walkcooldown = 0;// ideally this would be done with an actual timer...
    float E = srcrects[0].w * zoom;
    
    SDL_initFramerate(&fpsm);
    SDL_setFramerate(&fpsm, 60);
    puts("<<Entering Loop>>");
    while ( loop ) { //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> L O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
        while( SDL_PollEvent(&event) ){
            switch (event.type) {
                case SDL_QUIT:
                    loop = 0;
                    break;
                case SDL_KEYDOWN:
                         if( event.key.keysym.sym == 'w' || event.key.keysym.sym == SDLK_UP )     up = 1;
                    else if( event.key.keysym.sym == 's' || event.key.keysym.sym == SDLK_DOWN )   down = 1;
                    else if( event.key.keysym.sym == 'a' || event.key.keysym.sym == SDLK_LEFT )   left = 1;
                    else if( event.key.keysym.sym == 'd' || event.key.keysym.sym == SDLK_RIGHT )  right = 1;
                    break;
                case SDL_KEYUP:
                         if( event.key.keysym.sym == 'w' || event.key.keysym.sym == SDLK_UP )     up = 0;
                    else if( event.key.keysym.sym == 's' || event.key.keysym.sym == SDLK_DOWN )   down = 0;
                    else if( event.key.keysym.sym == 'a' || event.key.keysym.sym == SDLK_LEFT )   left = 0;
                    else if( event.key.keysym.sym == 'd' || event.key.keysym.sym == SDLK_RIGHT )  right = 0;
                    if( up + down + left + right == 0 ) walkcooldown = 0;
                    break;
                case SDL_MOUSEMOTION:
                	pmouseX = mouseX;
        			pmouseY = mouseY;
                    mouseX = event.motion.x;
                    mouseY = event.motion.y;
                    /*if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE) ){
                        tx += event.motion.xrel;
                        ty += event.motion.yrel;
                    }*/
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    break;
                case SDL_MOUSEBUTTONUP:
                    break;
                case SDL_MOUSEWHEEL:;
                    /*double xrd = (mouseX - tx) / zoom;
                    double yrd = (mouseY - ty) / zoom;
                    zoomI -= event.wheel.y;
                    //I = constrain(I, minI, maxI);
                    zoom = pow(1.1, zoomI);
                    tx = mouseX - xrd * zoom;
                    ty = mouseY - yrd * zoom;*/
                	zoomI -= event.wheel.y;
                    zoom = pow(1.1, zoomI);
                    E = srcrects[0].w * zoom;
                    break;
            }
        }

        if( walkcooldown <= 0 ){
        	char horz = right - left;
        	char vert = down - up;
        	if( horz != 0 && vert != 0 ){//diagonal movement
        		if( map->heights[player.x + horz][player.y + vert] == 0 ){
			        player.x += horz;
			        player.y += vert;
			    	walkcooldown = 6;
			    }
			    else goto slide;
        	}
        	else{
        		slide:
		        if( map->heights[player.x + horz][player.y] == 0 ){//horizontal
			        player.x += horz;
			    	walkcooldown = 6;
			    }
			    if( map->heights[player.x][player.y + vert] == 0 ){//vertical
			    	player.y += vert;
			    	walkcooldown = 6;
			    }//                        these need to be separate so you can drag yourself along walls
			}//                            and not get caught on them
		}
		else walkcooldown -= 1;

        tx = cx - (zoom * player.x * srcrects[0].w);
        ty = cy - (zoom * player.y * srcrects[0].w);
        

        SDL_RenderClear( renderer );
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_RenderFillRect( renderer, NULL );

        //puts("goin into the render");
        renderDrawMap( renderer, map, tilesheet_color, srcrects, tx, ty, zoom );

        
        SDL_RenderCopyF( renderer, tilesheet_color, srcrects + player.sprite_ID, &(SDL_FRect){tx + player.x * E, ty + player.y * E, E, E} );

        SDL_RenderPresent(renderer);
        //printf("%d\n", time_passed );
        time_passed = SDL_framerateDelay(&fpsm);

    }//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> / L O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    free_map( map );
    SDL_DestroyTexture( tilesheet_color );

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

