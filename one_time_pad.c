#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR(string)({ \
	printf("error: %s\nprogram will now exit", string); \
	exit(1);\
	})
#define CHECK_FREE(object)({ \
		if(object != NULL) free(object); \
		else printf("Double free avoided"); })
#define PRINT(integer) printf("%ld\n", integer);

typedef unsigned char byte;
void check_input(int argc, char **argv);
const char* out_file_name_creator(const char *in_file_name, const char *mode);
char *concatenate(char*str1, char *str2);
const long read_into_buffer(byte* buffer, FILE *in_file);
void encrypt(byte* buffer, const long filelen, const int key, FILE* out_file);

int main(int argc, char **argv){
        check_input(argc, argv);
	const char *in_file_name = argv[3], *mode = argv[2], *out_file_name = out_file_name_creator(in_file_name, mode);
	//printf("The concatention of the infile and enc is: %s", concatenate(argv[3], "enc"));
	//if(argc == 4) out = argv[4];
	//key will be the first argument
	const int key = atoi(argv[1]);
	//printf("out_file_name %s\n", out_file_name);
	printf("Opening files...\n");
	FILE *in_file = fopen(in_file_name, "rb"), *out_file = fopen(out_file_name, "w");
	if(in_file == NULL) ERROR("Infile not found");
	else printf("File '%s' found\n", in_file_name);
	fseek(in_file, 0, SEEK_END);
	const long in_file_len = ftell(in_file);
	//printf("The infile length is %ld\n", in_file_len);
	byte* buffer = (byte*)malloc(in_file_len*sizeof(byte));
	read_into_buffer(buffer, in_file);
	encrypt(buffer, in_file_len, key, out_file);
	printf("Closing files..\n");
	fclose(in_file);
	fclose(out_file);
	printf("Exiting program\n");
	return 0;
}

void encrypt(byte* buffer, const long in_file_len, const int key, FILE* out_file){
	//perform XOR operation on each byte and writes it to the file
	printf("Encrypting bytes... \n");
	int pos = 0;
	while(pos < in_file_len){
		byte* encrypted_byte = (byte*)malloc(sizeof(byte));
		*encrypted_byte = buffer[pos]^key;
		//printf("current byte: %u\n", buffer[pos]);
		//printf("encrypted byte %u\n", *encrypted_byte);
		fwrite(encrypted_byte, 1, sizeof(byte), out_file);
		pos++;
		free(encrypted_byte);
	}
	printf("Encryption done\n");
}

const long read_into_buffer(byte* buffer, FILE *in_file){
	//fileptr = fopen("myfile.txt", "rb");  // Open the file in binary mode
	fseek(in_file, 0, SEEK_END);          // Jump to the end of the file
	const long filelen = ftell(in_file);             // Get the current byte offset in the file
	printf("File length: %ld\n", filelen);
	rewind(in_file);                      // Jump back to the beginning of the file
	//(byte *)malloc((filelen)*sizeof(byte)); // Enough memory for file 
	if(buffer == NULL) ERROR("Memory allocation for file buffer failed");
	fread(buffer, sizeof(byte), filelen, in_file);
	//fread(buffer, filelen, 1, in_file); // Read in the entire file
	//fclose(in_file); // Close the file
	return filelen;
}

void check_input(int argc, char **argv){
	if(argc < 2){
		printf("Argument format: ProgramName(one_time_pad) key mode infile\n");
		ERROR("Incorrect command line arguments");
	}
	else if(argc > 4) ERROR("Incorrect command line arguments");
	//else if(strcmp(argv[2], "ENCRYPT") != 0 || strcmp(argv[3], "DECRYPT" != 0) ERROR("Must specify encryption or decryption"); 
	printf("key: %i\nmode: %s\ninfile: %s\noutfile: \n", atoi(argv[1]), argv[2], argv[3]);
}

const char *out_file_name_creator( const char *in_file_name, const char *mode){
	//check if the in_file_name has an extension of the form .something. If it does, save it
	int strlen1 = strlen(in_file_name), has_extension = 0, j = 0;
	char *ex;
	const char enc[5] = { '_', 'e', 'n', 'c' };
	const char dec[5] = { '_', 'd', 'e', 'c'};
	for(int i = 0; i < strlen1; i++){
		if(in_file_name[i] == '.'){
	       		has_extension = 1;
			ex = (char *)calloc(strlen(in_file_name) - i, sizeof(char));
			if(ex == NULL) ERROR("Memory allocation failed");
		}
		if(has_extension){
			ex[j] = in_file_name[i];
			j++;
		}
	}
	const char *extension = ex;
	//printf("The extension is: %s\n", extension);
	//copy over each character from in_file_name to ex to avoid "const char cast to char" warning TODO: learn how to overcome this easier later
	ex = (char *)calloc(strlen(in_file_name) + 1, sizeof(char));
	if(ex == NULL) ERROR("Memory allocation failed");
	for(int i = 0; i < strlen1; i++){
		if(in_file_name[i] == '.') break;
		ex[i] = in_file_name[i];
	}
	//now concatenate ex, which holds the in file name with either "_enc" or "_dec" depening on the mode
	if(strcmp(mode, "ENCRYPT") == 0){ 
		ex = strcat(ex, enc);
	       //	printf("result %s\n", ex);
	}
	else ex = strcat(ex, dec);
	const char* out_file_name = strcat(ex, extension);//strcat(ex, extension);
	return out_file_name;	
}

char *concatenate(char* str1, char* str2){
	char *new_str = (char*)calloc(strlen(str1) + strlen(str2) + 1, sizeof(char));
	if(new_str == NULL) ERROR("Memory allocation failed");
	int l1 = strlen(str1), l2 = strlen(str2), i;
	//copy the string to new_str
	for(i = 0; i < l1 + l2; i++){
		if(i <= l1 )new_str[i] = str1[i];
		else new_str[i] = str2[i];
	}
}
	
//strlen works just like the strlen function from the str library. I just wrote my own to save on import time
/*
int strlen(char *str){
	int i;
	for(i = 0; str[i] != '\0'; i++);
	return i;
}*/
