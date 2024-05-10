#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main () {
	
	char asscode[64000] = "";
	char *ass = "ass";
	sprintf(asscode, "eat my %s", ass);
	sprintf(asscode, "eat my %s", ass); 
	printf ("%s\nlen: %lu", asscode, strlen (asscode));
	return 0;
}
