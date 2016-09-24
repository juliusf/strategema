#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "util.h"
#include "interconnect.h"
#include "cpu.h"

int main(int argc, const char* argv[]){
    if(argc != 2) {
        fprintf(stderr, "Missing first argument (the rom to load).\n");
        return 1;
    }

    uint64_t fileLen = 0;
    unsigned char* rom = NULL;
    read_from_disk(argv[1], &fileLen, &rom);
    
    Interconnect* interconnect = NULL;
    initialize_interconnect(&interconnect);
    load_rom(interconnect, fileLen, rom);
    free(rom);

    Cpu* cpu = NULL;
    initialize_cpu(&cpu, interconnect);

    //printf("rom at 200: %c\n", rom[200]);

    run(cpu);
    return 0;
}

