#include <stdio.h>
int check(){
	int i=1;
	char *pi=(char*)&i;

	return *pi==0;
}

int main(){
	if (check()==1){
		printf("big\n");
	}
	else{
		printf("little\n");
	}

	return 0;
}
