#include <stdio.h>
#include <locale.h>

int main(){
	printf("%s",setlocale(LC_ALL, "cz-CS"));
}
