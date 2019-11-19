#ifndef FUNDAMENTALS_H_INCLUDED
#define FUNDAMENTALS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <SDL.h>

Uint32 rmask, gmask, bmask, amask;

#define PHI         1.618033988749894848204586834365638117720309179805762862135
#define TWO_PI      6.283185307179586476925286766559005768394338798750211641949
#define PI          3.141592653589793238462643383279502884197169399375105820974
#define HALF_PI     1.570796326794896619231321691639751442098584699687552910487
#define THIRD_PI    1.047197551196597746154214461093167628065723133125035273658
#define QUARTER_PI  0.785398163397448309615660845819875721049292349843776455243
#define ONE_OVER_PI 0.318309886183790671537767526745028724068919291480912897495

typedef char bool;

double sq( double a );

int rand_int( int min, int max );

float lerp( float start, float stop, float amt );

float map( float value, float start1, float stop1,  float start2, float stop2 );

int constrain( int a, int min, int max );
float constrainf( float a, float min, float max );
double constrainD( double v, double min, double max);

double degrees( double radians );
double radians( double degrees );

// String split
void strspl( char *string, const char *delimiters, char ***list, int *size );
// String Count character
int strcchr( char *string, char C );
// sub-string
char * substr( char *string, int start, int stop );

bool fseek_lines( FILE* f, int N );
bool fseek_string( FILE *f, char *str );

bool cursor_in_rect( SDL_MouseButtonEvent button, SDL_Rect rect );
bool coordinates_in_rect( float x, float y, SDL_Rect rect );

bool is_alphanumeric( char c );
bool is_lower_case( char c );
bool is_upper_case( char c );
bool is_numeral( char c );

typedef struct {
    int i, j;
} index2d;

#endif