#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "util.h"
#include "interconnect.h"

int main(int argc, const char* argv[]){
    uint64_t fileLen = 0;
    unsigned char* rom = NULL;
    read_from_disk(argv[1], &fileLen, &rom);
    
    Interconnect* interconnect = NULL;
    initialize_interconnect(&interconnect);
    printf("interconnect RAM at 50: %i\n", interconnect->ram[50]);
    load_rom(interconnect, fileLen, rom);
    printf("rom at 200: %c\n", rom[200]);
    printf("RAM at 200: %c\n", interconnect->ram[0x200 + 200]);
    return 0;
}

