#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "fundamentals.h"

typedef struct map_struct{
	int width, height;
	int **sprite_ID;
	char **heights;  // 2 = wall, 1 = obstacle, 0 = floor, -1 = hole
	float **angles;
	SDL_RendererFlip **flips;
	char **gateways; // -1 for normal tiles, for doors, stairs, map edges, contains the ID number of the room it takes you to.
} Map;

Map *alloc_map( int W, int H );

void free_map( Map *map );

Map *load_tmx( char* filename );

void renderDrawMap( SDL_Renderer *renderer, Map *map, SDL_Texture* tilesheet, SDL_Rect* srcrects, float tx, float ty, float zoom );

#endif