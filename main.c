#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//RGB Pixel Struct to represent each pixel
typedef struct Pixel{
	unsigned char r, g, b;
} Pixel;

//Struct for our intermediate step(2/3)
typedef struct Image{
	int width, height, colorValue;
	Pixel* buffer;
} Image;

//Global Variables
FILE *input;
FILE *output;
Image img = {0}; //Sets all variables & pointer(NULL) to 0
int conversion, magicNum;

// This function write a header in the desired format w/o comment
void writeHeader(int magicNum){
    char magicBuffer[2];
    char wbuffer[8];
    char hbuffer[8];
    char colorBuffer[8];
	
    sprintf(magicBuffer, "%d", magicNum);
    sprintf(wbuffer, "%d", img.width);
    sprintf(hbuffer, "%d", img.height);
    sprintf(colorBuffer, "%d", img.colorValue);

	//Puts header elments into output
    fputc('P', output);
    fputs(magicBuffer, output);
    fputc(' ', output);
    fputs(wbuffer, output);
    fputc(' ', output);
    fputs(hbuffer, output);
    fputc(' ', output);
    fputs(colorBuffer, output);
    fputc('\n', output);
}

//Reads over a P3 File 
void readP3(){ //FILE input/output
	int i = 0;
	int length = img.width * img.height;
	int c;
    char rstr[8];
    char gstr[8];
    char bstr[8];
    int rindex = 0;
    int gindex = 0;
    int bindex = 0;

	//Set buffer(Type Pixel) rgb values
	while(i < length){	
		while(1){
			c = fgetc(input);
			if(c == ' ' || c == '\n'){ break;}
            rstr[rindex] = (char)c;
            rindex++;
		}
        rstr[rindex] = '\0';
        rindex = 0;

		while(1){
			c = fgetc(input);
			if(c == ' ' || c == '\n'){ break;}
            gstr[gindex] = (char)c;
            gindex++;
		}
        gstr[gindex] = '\0';
        gindex = 0;

		while(1){
			c = fgetc(input);
			if(c == ' ' || c == '\n'){ break;}
            bstr[bindex] = (char)c;
            bindex++;
		}
        bstr[bindex] = '\0';
        bindex = 0;
		//Set the buffer for rgb
		img.buffer[i].r = atoi(rstr);
		img.buffer[i].g = atoi(gstr);
		img.buffer[i].b = atoi(bstr);
		i++;
	}
}

//This function reads a binary File
void readP6(){
	
	//(pointer to store, size of pixels(x bit long), bit length of file, file to read)
	fread( img.buffer, sizeof(Pixel), img.width * img.height, input);
	
}

//This function writes to P3(ascii) Format
void writeP3(){
	int i;
	char rbuffer[32];
	char gbuffer[32];
	char bbuffer[32];
	for( i = 0 ; i < img.width * img.height; i++){
        sprintf(rbuffer, "%d", img.buffer[i].r);
        sprintf(gbuffer, "%d", img.buffer[i].g);
        sprintf(bbuffer, "%d", img.buffer[i].b);

		fputs(rbuffer, output);
		fputc(' ', output);
		fputs(gbuffer, output);
		fputc(' ', output);
		fputs(bbuffer, output);
		fputc(' ', output);
	}
}

//This function write to P6(Binary)
void writeP6(){
	fwrite( img.buffer, sizeof(Pixel), img.width * img.height, output);
}

/*Reads the Header of the File. Gathers MagicNum, width, height,
 * and colorValue. Also checks if the MagicNum and colorValue are 
 * set to correct values.
 *@Returns -1 for Error & 0 for Correct
 */
 int getHeader(FILE *inputFile){
	char element;
	
	//Find Magic Number
	fscanf( inputFile, "P%c", &element); // I, (look for)P6\n, (Save to) element

	//Ensure it is P3 or P6
	if( element != '3' && element != '6'){
		fprintf(stderr,"Error: Invalid Magic Number\n");
		return -1;	
	}
	magicNum = atoi(&element);	//Set the Magic Number to ('3' or '6')

    // Read until the next line
    element = fgetc(inputFile);
    element = fgetc(inputFile);
	
	//Skip Comments
	if (element == '#') {
    do {
      element = fgetc(inputFile);
    }
    while (element != '\n');
    }	

	//Gets Width and Height
	fscanf(inputFile, "%d%d", &img.width, &img.height);

	//Gets and checks Color Value
	fscanf(inputFile, "%d\n", &img.colorValue);
	if( img.colorValue > 255 || img.colorValue < 1){ 
		fprintf(stderr,"Error: Color Value Available is 0-255\n");
		return -1;
	}

	//Allocates space in mem for Image and Buffer. Sets Width, height, colorvalue in Image
	img.buffer = (Pixel *) malloc(sizeof(Pixel) * img.width * img.height);

	return 0;
//Scan pointer is at the raster
}//End getHeader

/*The main method calls al other methods. Also, contains most of the tests.
 *@returns 0 is All Good, -1 if error
 */
int main (int argc, char *argv[]){

	//Ensures correct number of arugments
	if(argc != 4) {
		fprintf(stderr,"Error:Incorect Number of Arugments");
		return -1;
	}

	conversion = atoi(argv[1]);	
	input =  fopen( argv[2], "r");	
	output = fopen( argv[3], "w");	

	//Ensures input file exists
	if(!input){
		fprintf(stderr,"Error:Input file does not exist");
		return -1;
	}

	//Ensures the conversion specification is 3(P3) or 6(P6)
	if(conversion != 3 && conversion != 6){	//If not 3 and not 6
		fprintf(stderr,"Error: Invalid Parameter 1 in Cmd Line Arugment");
		return -1;
	}

	//Start Read
	//Gathers Information from Header
	int response = getHeader(input);
	if(response != 0){
		fprintf(stderr,"Error:Invalid Header");
	}
	
	//Writes Proper header to output file
    writeHeader(conversion);
	
	//Calls the Read and Write function for Conversions(Raster)
	if(conversion == 3 ){
		if( magicNum == 3){ 
			readP3();
		}
		else{
			readP6();
		}
		writeP3();
		fclose(output);		//close output file
		fclose(input);		//Close input file
		free(img.buffer);	//Free buffer
		return 0;
	}
	else{
		if( magicNum == 6){ 
			readP6();
		}
		else{
			readP3();
		}
		writeP6();
		fclose(output);		//close output file
		fclose(input);		//Close input file
		free(img.buffer);	//Free buffer
		return 0;
	}

}
