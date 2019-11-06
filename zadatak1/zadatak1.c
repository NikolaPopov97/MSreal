#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	FILE *fp,*fs,*fb;
	float percentage;
	long int period = 20000L;
	char *str,*str1;
        char sval1,sval2,sval3,sval4,tval1,tval2,tval3,tval4;
        size_t num_of_bytes = 6,num_of_bytes1 = 6;
	float increment = 0;
	int sd = 0;


	while(1) 
	{

		//Citanje vrednosti tastera
		fs = fopen ("/dev/switch", "r");
		if(fs==NULL)
		{
			puts("Problem pri otvaranju /dev/switch");
			return -1;
		}


		str = (char *)malloc(num_of_bytes+1); 
		getline(&str, &num_of_bytes, fs); 

		if(fclose(fs))
		{
			puts("Problem pri zatvaranju /dev/switch");
			return -1;
		}


		sval1 = str[2] - 48;
		sval2 = str[3] - 48;
		sval3 = str[4] - 48;
		sval4 = str[5] - 48;
		free(str);
		printf("Vrednosti prekidaca: %d %d %d %d \n",sval1,sval2,sval3,sval4);
		if((sval1 == 0) && (sval2==0)){
			increment = 0;
		}
		else if((sval1 == 1) && (sval2==0)){
			increment = 0.1;
		}
		else if((sval1 == 0) && (sval2==1)){
			increment = 0.2;
		}
		else{
			increment = 0.3;
		}
		
		//dugmad
		fb = fopen ("/dev/button", "r");
		if(fb==NULL)
		{
			puts("Problem pri otvaranju /dev/button");
			return -1;
		}


		str1 = (char *)malloc(num_of_bytes1+1); 
		getline(&str1, &num_of_bytes1, fb); 

		if(fclose(fb))
		{
			puts("Problem pri zatvaranju /dev/button");
			return -1;
		}


		tval1 = str1[2] - 48;
		tval2 = str1[3] - 48;
		tval3 = str1[4] - 48;
		tval4 = str1[5] - 48;
		free(str1);

		printf("Vrednosti tastera: %d %d %d %d \n",tval1,tval2,tval3,tval4);
		
		if(((tval1 == 1) || (tval2 == 1)) && (sd == 0) ){
			usleep(10);
			sd = 1;
			puts("cao");
			if(tval1 == 1){
				
				if((percentage += increment) > 1 ){
				 	percentage = 1;
				}
 				puts("a mars");
			}
			if(tval2 == 1){
				if((percentage -= increment) < 0){
				 	percentage  = 0;
				}
			}

		}else if(tval1==0 && tval2 == 0) 
		{
			sd = 0;
		}
		
		printf("%f",percentage);
		// Upali diode
		fp = fopen("/dev/led", "w");
		if(fp == NULL)
		{
			printf("Problem pri otvaranju /dev/led\n");
			return -1;
		}
		fputs("0x0F\n", fp);
		if(fclose(fp))
		{
			printf("Problem pri zatvaranju /dev/led\n");
			return -1;
		}
		usleep(percentage*period);

		// Ugasi diode
		fp = fopen("/dev/led", "w");
		if(fp == NULL)
		{
			printf("Problem pri otvaranju /dev/led\n");
			return -1;
		}
		fputs("0x00\n", fp);
		if(fclose(fp))
		{
			printf("Problem pri zatvaranju /dev/led\n");
			return -1;
		}
		usleep((1-percentage)*period);
	}
	return 0;
}

