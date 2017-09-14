#include <stdlib.h>
#include <stdio.h>
#include <cstdio>

//RGB Pixel Struct to represent each pixel
typedef struct RGBPixel{
	unsigned char r, g, b;
} RGBPixel;

//Functions in File
int getHeader(FILE *inputFile);
void readP3(FILE *input);
void readP6(FILE *input);
void writeP3();
void writeP6();

//Global Variables
FILE *input;
FILE *output;
RGBPixel *pixel;
int conversion, height, width, magicNum, colorValue;

int main (int argc, char *argv[]){

//Ensures correct number of arugments
if(argc != 4) {
	printf("Error:Incorect Number of Arugments");
	return -1;
}

conversion = atoi(argv[1]);		//3 or 6
input =  fopen( argv[2], "rb");	//Open Input File
output = fopen( argv[3], "w");	//Determine Output File

//Ensures input file exists
if(!input){
	printf("Error:Input file does not exist");
	return -1;
}

//Ensures the conversion specification is 3(P3) or 6(P6)
if(conversion != 3 && conversion != 6){	//If not 3 and not 6
	printf("Error: Invalid Parameter 1 in Cmd Line Arugment");
	return -1;
}

//Gathers Information from Header
int response = getHeader(input);
if(response != 0){
	printf("Error:Invalid Header");
}

//Calls the Read and Write function for Conversions
if(conversion == 3 ){
	if( magicNum == 3){ 
		readP3(input);
		//writeP3
	}
	else{
	//callP6ToP3(Give input file, give output file)
	}
	//Close files
	return 0;
	}//End '3' If
else{
	if( magicNum == 6){ 
		//copyP6ToP6(input, output);
	}
	else{
	//callP3ToP6(Give input file, give output file)
	}
	//Close files
	return 0;
}//End '6' Else

}//End Main


/*Reads the Header of the File. Gathers MagicNum, width, height,
 * and colorValue. Also checks if the MagicNum and colorValue are 
 * set to correct values.
 *@Returns -1 for Error & 0 for Correct
 */
 int getHeader(FILE *inputFile){
	char element;
	
	//Find Magic Number
	fscanf( inputFile, "P%c\n", &element); // I, (look for)P6\n, (Save to) element
	
	//Ensure it is P3 or P6
	if( element != '3' && element != '6'){
		printf("Error: Invalid Magic Number\n");
		return -1;	
	}
	magicNum = atoi(&element);	//Set the Magic Number to ('3' or '6')
	
	//Skip Comments
	element = fgetc(inputFile);
	while (element == '#') {
    do {
      element = fgetc(inputFile);
    }
    while (element != '\n');
    }	
	
	//Gets Width and Height
	fscanf(inputFile, "%d%d\n", &width, &height);
	
	//Gets and checks Color Value
	fscanf(inputFile, "%d\n", &colorValue);
	if( colorValue > 255 || colorValue < 0){ 
		printf("Error: Color Value Available is 0-255\n");
		return -1;
	}
	//printf("%d\n", width);
	//printf("%d\n", height);
	return 0;
//Scan pointer is at the raster
}//End getHeader
