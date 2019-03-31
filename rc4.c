#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

unsigned char * gen_random( size_t size );
void freeString( void *str );
void RC4( unsigned char *S, unsigned char const *key, int keylen, unsigned char *plain_t, unsigned char *cipher_t );

unsigned char S[256];
/* key generated by `gen_random()` */
unsigned char key[]     = "YOUR SECRET";
unsigned char plain_t[] = "This world has gone to shit! yeah...";
unsigned char *cipher_t = NULL;

int main( void ) {
	/* allocate memory for the cipher text equal to the size of the plain text */
	cipher_t = (unsigned char *) malloc( ( strlen( plain_t ) * sizeof( char ) ) + 1 );
	
	/* encipher */
	RC4( S, key, strlen( key ), plain_t, cipher_t );
	printf( "%s\n", cipher_t );

	/* decipher */
	RC4( S, key, strlen( key ), cipher_t, plain_t );
	printf( "%s\n", plain_t );
	freeString( cipher_t );
	return 0;
}

void RC4( unsigned char *S, unsigned char const *key, int keylen, unsigned char *plain_t, unsigned char *cipher_t ) {
	int i, j, x, temp;
	unsigned char K;

	for ( i = 0 ; i < 256 ; i++ ) {
		S[i] = i;
	}
	j = 0;
	for ( i = 0 ; i < 256 ; i++ ) {
		j    = ( j + S[i] + key[ i % keylen ] ) % 256;
		temp = S[i];
		S[i] = S[j];
		S[j] = temp;
	}

	i = 0;
	j = 0;
	x = 0;
	while ( plain_t[x] != '\x00' ) {
		i    = ( i + 1 ) % 256;
		j    = ( j + S[i] ) % 256;
		temp = S[i];
		S[i] = S[j];
		S[j] = temp;
		K           = S[ ( S[i] + S[j] ) % 256 ];
		cipher_t[x] = plain_t[x] ^ K;
		x++;
	}
	cipher_t[x] = '\x00';
}

unsigned char * gen_random( size_t size ) {
	int i, chars_len;
	char c;
	char chars[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static unsigned char *randStr;
	
	chars_len = strlen( chars );
	if ( size ) {
		randStr = (unsigned char *) malloc( (size * sizeof( char )) + 1 );
		if ( randStr ) {
			for ( i = 0 ; i < size ; i++ ) {
				c = chars[ rand() % chars_len ];
				randStr[i] = c;
			}
			randStr[i] = '\x00';
			return randStr;
		}
		return (unsigned char *) "[Error] - Couldn't allocate memory for random string...";
	}
	return (unsigned char *) "[Error] - Size provided is invalid!";
}

void freeString( void *str ) {
	free( str );
}
