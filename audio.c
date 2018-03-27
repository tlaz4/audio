#include <stdio.h>

int main(){
	FILE *f1, *f2;
	char c;
	int i;

	f1 = fopen("Suspense_420819_010_The_Cave_of_Ali_Baba_-128-44-_27789_29m33s.mp3", "rb");

	f2 = fopen("file.mp3", "wb");

	fseek(f1, 84172, SEEK_SET);

	while(1){
		i = 0;
		c = fgetc(f1);
   		while (i < 1400){
        		fputc(c, f2);
        		c = fgetc(f1);
			i++;
    		}
		fseek(f1, -1, SEEK_CUR); 
		printf("file\n");
	}

	

	fclose(f1);
	fclose(f2);
}
