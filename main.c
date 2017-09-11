#include <stdlib.h>
#include <stdio.h>
#include <cstdio>

int main (int argc, char *argv[]){
FILE *input;
FILE *output;
int conversion;

conversion = atoi(argv[1]);		//3 or 6
input =  fopen( argv[2], "r");	//Open Input File
output = fopen( argv[3], "w");	//Determine Output File

if(conversion != 3 || conversion != 6){	//If not 2 or 6 Throw error
	printf("Error: Invalid Parameter 1 in Cmd Line Arugment");
	return -1;
}
else{
	if(conversion == 3 ){
		//callP3Conversion(Give input file, give output file)
		//convertToP3(input, output);
		//Return ??;
	}
	else{
		//CallP6Conversion(Give input file, give output file)
		//convertToP6(input, output);
		//Return ??;
	}
}
//Test variables & input Parameters
//printf("Parameter 1: %d \n", conversion);  	//Works
//printf("Parameter 2: %s\n", argv[2]);		//Alpha.txt
//printf("Parameter 2P: %p\n", *argv[2]);		//linear pointers 00000060
//printf("Parameter 3P: %p\n", *argv[3]);		//linear pointers 00000061

}//End Main

convertToP3(FILE *input, FILE *output ){ //FILE input/output
	
	//get magic num, height, width, color value
	//if P3 copy into outputfile
	//else P6  convert Binary into Ascii
	
}