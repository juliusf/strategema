#include "util.h"

int32_t load_rom(const char* path, uint64_t* length, char* rom[]){
	
	FILE* file;
	char* buffer;
	uint64_t fileLen;

	file = fopen(path, "r");
	if (!file){
		fprintf(stderr, "unable to open ROM file %s!\n", path);
		return 1;
	}

	fseek(file, 0, SEEK_END);
	fileLen = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = (char *) malloc(fileLen +1);

	if (!buffer){
		fprintf(stderr, "unable to allocate %" PRIu64  " bytes for ROMs\n", fileLen);
		return 1;
	}
	fread(buffer, fileLen, 1, file);
	fclose(file);
	*length = fileLen;
	*rom = buffer;
	    printf("rom at 200: %" PRIu64 "\n",fileLen);
	fprintf(stdout, "successfully loaded ROM file %successfully\n", path);
	return 0;
}
