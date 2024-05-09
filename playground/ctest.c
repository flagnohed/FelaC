#include <stdio.h>
#include <stdlib.h>

int main () {
	FILE *fp = fopen ("smallfile.txt", "r");
	long int sz;
	fseek (fp, 0L, SEEK_END);
	sz = ftell (fp);
	rewind (fp);
	printf("%ld\n", sz);
}
