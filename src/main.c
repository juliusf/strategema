#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "util.h"

int main(int argc, const char* argv[]){
    uint64_t fileLen = 0;
    char* rom = NULL;
    load_rom(argv[1], &fileLen, &rom);
    
    printf("rom at 200: %" PRIu64 "\n", fileLen);
    printf("rom at 200: %c\n", rom[200]);
    return 0;
}

