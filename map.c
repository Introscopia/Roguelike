#include "map.h"

Map *alloc_map( int W, int H ){

	Map *M = (Map*) malloc( sizeof(Map) );
	if( M == NULL ) return M;

	M->width = W;
	M->height = H;

	M->sprite_ID =              (int**) malloc( W * sizeof(int*) );
	  M->heights =             (char**) malloc( W * sizeof(char*) );
	   M->angles =            (float**) malloc( W * sizeof(float*) );
	    M->flips = (SDL_RendererFlip**) malloc( W * sizeof(SDL_RendererFlip*) );
	 M->gateways =             (char**) malloc( W * sizeof(char*) );

	int sizes [] = { H * sizeof(int), H * sizeof(char), H * sizeof(float), H * sizeof(SDL_RendererFlip) };
	for (int i = 0; i < W; ++i){
		M->sprite_ID[i] =              (int*) malloc( sizes[0] );
	      M->heights[i] =             (char*) malloc( sizes[1] );
	       M->angles[i] =            (float*) malloc( sizes[2] );	
	        M->flips[i] = (SDL_RendererFlip*) malloc( sizes[3] );
	     M->gateways[i] =             (char*) malloc( sizes[1] );
	}

	return M;
}

void free_map( Map *M ){
	for (int i = 0; i < M->width; ++i){
		free( M->sprite_ID[i] );
	    free( M->heights[i] );
	    free( M->angles[i] );	
	    free( M->flips[i] );
	    free( M->gateways[i] );
	}
	free( M->sprite_ID );
    free( M->heights );
    free( M->angles );	
    free( M->flips );
    free( M->gateways );

    free( M );
}

Map *load_tmx( char* filename ){
	int W, H;
	FILE *f = fopen( filename, "r" );
	// I'm presuming the tiles layer appears before the heights layer...
	fseek_string( f, "name=\"tiles\" width=\"" );
	fscanf(f, "%d", &W);
	fseek_string( f, "height=\"" );
	fscanf(f, "%d", &H);
	

	Map *M = alloc_map( W, H );
	if( M == NULL ) return M;

	float trans_angles [] = { 0, -90, 0, -90, 0, 90, 180, 90 };
	SDL_RendererFlip trans_flips [] = { SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL, SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_NONE, SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL };

	unsigned int N = 0;
	int i = 0, j = 0;
	fseek_lines( f, 2 );
	while( fscanf(f, "%u,", &N) > 0 ){
		if( N > 0x1FFFFFFF ){
			M->sprite_ID[i][j] = (N & 0x1FFFFFFF) - 1;
			//bool flipped_horizontally = N & 0x80000000;
    		//bool flipped_vertically   = N & 0x40000000;
    		//bool flipped_diagonally   = N & 0x20000000;
    		char transform = ((N & 0x20000000)>>29) | ((N & 0x40000000)>>29) | ((N & 0x80000000)>>29);
    		M->angles[i][j] = trans_angles[ transform ];
			M->flips[i][j] = trans_flips[ transform ];
		}
		else{
			M->sprite_ID[i][j] = N - 1;
			M->angles[i][j] = 0;
			M->flips[i][j] = SDL_FLIP_NONE;
		}
		++i;
		if( i == W ){
			i = 0;
			++j;
		}
	}

	fseek_string( f, "name=\"heights\"" );
	fseek_lines( f, 2 );
	i = 0; j = 0;
	while( fscanf(f, "%u,", &N) > 0 ){
		M->heights[i][j] = N-1;
		++i;
		if( i == W ){
			i = 0;
			++j;
		}
	}

	return M;
}

void renderDrawMap( SDL_Renderer *renderer, Map *map, SDL_Texture* tilesheet, SDL_Rect* srcrects, float tx, float ty, float zoom ){
	float E = srcrects[0].w * zoom;
	//float hE = E * 0.5;
	for (int i = 0; i < map->width; ++i){
		for (int j = 0; j < map->height; ++j){
			//printf("%dx%d\n", i, j );
			if( map->sprite_ID[i][j] >= 0 ){
				int x = tx + i*E;
				int y = ty + j*E;
				if( SDL_RenderCopyExF( renderer, tilesheet, srcrects + map->sprite_ID[i][j], 
									   &(SDL_FRect){x, y, E, E}, map->angles[i][j],
									   NULL, map->flips[i][j] ) < 0 ){//&(SDL_Point){x + hE, y + hE}
					puts( SDL_GetError() );
				}
			}
		}
	}
}