#include <stdio.h>

int main(){
	FILE *f1, *f2;
	char c;
	int i;

	f1 = fopen("XMinusOne57-03-06091TheSeventhVictim.mp3", "rb");

	f2 = fopen("file.mp3", "wb");

	fseek(f1, 84172, SEEK_SET);

		i = 0;
		c = fgetc(f1);
   		while (i < 140000){
        		fputc(c, f2);
        		c = fgetc(f1);
			i++;
    		}
		fseek(f1, -1, SEEK_CUR); 
		printf("file\n");

	

	fclose(f1);
	fclose(f2);
}
