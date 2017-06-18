#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(){
	srand(time(NULL));
	int n=rand()%71;
	printf("%d",n);
	return 0;
}
