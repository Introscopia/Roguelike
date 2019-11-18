#include "fundamentals.h"

double sq( double a ){
	return a * a;
}

/* RANDOM: min inclusive, max not inclusive
	int co  [7];
    for (int i = 0; i < 7; ++i) co[i] = 0;
    for (int i = 0; i < 1000; ++i){
       int x = random(1, 8);
       co[x-1]++;
    }
    for (int i = 0; i < 7; ++i) printf("%d: %d, ", i+1, co[i] );
    puts(".");
*/
int random( int min, int max ){
    return (rand() % (max-min)) + min;
}

float lerp(float start, float stop, float amt) {
    return start + (stop-start) * amt;
}

float map(float value, float start1, float stop1,  float start2, float stop2) {
	return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

int constrain( int a, int min, int max ){
	if( a < min ) return min;
	else if( a > max ) return max;
	else return a;
}
float constrainf( float a, float min, float max ){
	if( a < min ) return min;
	else if( a > max ) return max;
	else return a;
}
double constrainD( double v, double min, double max){
    if( v < min ) return min;
    else if( v > max ) return max;
    return v;
}

double degrees( double radians ){
	return radians * 57.29577951308232087679815481410517033240547246656432154916;//180 over PI;
}
double radians( double degrees ){
    return degrees * 0.017453292519943295769236907684886127134428718885417254560;// PI over 180
}


void strspl( char *string, const char *delimiters, char ***list, int *size ){
	int ss = strlen( string );
	*list = (char**) malloc( ceil(0.5*ss) * sizeof(char*) );
	int sd = strlen( delimiters );
	*size = 0;

	bool *checks = (bool*) malloc( ss * sizeof(bool) );
	for( int i = 0; i < ss; ++i ){
		checks[i] = 0;
		for (int j = 0; j < sd; ++j){
			if( string[i] == delimiters[j] ){
				checks[i] = 1;
				break;
			}
		}
	}

	bool looking_for_first = 1;
	for( int i = 0; i < ss; ++i ){
		if( looking_for_first ){
			if( checks[i] ){
				string[i] = '\0';
			}
			else{
				(*list)[0] = string+i;
				(*size)++;
				looking_for_first = 0;
			}
		}
		else{
			if( checks[i] ){
				string[i] = '\0';
				if( i < ss-1 ){
					if( !checks[i+1] ){
						(*list)[*size] = string+i+1;
						(*size)++;
					}
				}
			}
		}
	}
	*list = (char**) realloc( *list, (*size) * sizeof(char*) );
	//apparently for strtok the string MUST be declared as "char str[]" in the calling function
	// it can't be a literal and it can't be "char *string"...
	/* TEST:
	char **list;
	int size = 0;
	char string[] = "split me baby one more time";
	split_string( string, " ", &list, &size );
	for (int i = 0; i < size; ++i){
	    printf("%s\n", list[i] );
	}
	*/
	// char * p = strtok (string, delimiters);
	// int i = 0;
	// while (p != NULL){
	// 	(*list)[i] = p;
	// 	p = strtok (NULL, delimiters);
	// 	++i;
	// }
}

int strcchr( char *string, char C ){ // String Count character
	int i = 0;
	int count = 0;
	while( string[i] != '\0' ){
		if( string[i] == C ) ++count;
	}
	return count;
}

// sub-string
char * substr( char *string, int start, int stop ){
	char *sub = (char*) calloc( stop-start +1, sizeof(char) );
	for (int i = start; i < stop; ++i){
		sub[i-start] = string[i];
	}
	sub[ stop-start ] = '\0';
	return sub;
}

bool fseek_lines( FILE* f, int N ){
	char c = getc( f );
	while( c != EOF ){
		if( c == '\n' ){
			N -= 1;
			if( N == 0 ) return 1;
		}
		c = getc( f );
	}
	return 0;
}

bool fseek_string( FILE *f, char *str ){
	char c = getc( f );
	int i = 0;
	while( c != EOF ){
		if( c == str[i] ){
			i++;
			if( str[i] == '\0' ) return 1;
		}
		else{
			i = 0;
		}
		c = getc( f );
	}
	return 0;
}

bool cursor_in_rect( SDL_MouseButtonEvent button, SDL_Rect rect ){
	return ( button.x > rect.x && button.x < rect.x + rect.w ) && ( button.y > rect.y && button.y < rect.y + rect.h );
}

bool coordinates_in_rect( float x, float y, SDL_Rect rect ){
	return ( x > rect.x && x < rect.x + rect.w ) && ( y > rect.y && y < rect.y + rect.h );
}

bool is_alphanumeric( char c ){
  if( is_lower_case( c ) || is_upper_case( c ) || is_numeral( c ) || c == '-' ) return 1;// (int)c == 32 || // 32 is ' '
  else return 0;
}
bool is_lower_case( char c ){
  if( c >= 97 && c <= 122 ) return 1;
  else return 0;
}
bool is_upper_case( char c ){
  if( c >= 65 && c <= 90 ) return 1;
  else return 0;
}
bool is_numeral( char c ){
  if( c >= 48 && c <= 57 ) return 1; // || c == '.'
  else return 0;
}