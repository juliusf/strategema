#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "util.h"
#include "interconnect.h"
#include "cpu.h"
#include <string.h>

int main(int argc, const char* argv[]){
    if(argc < 2 || argc > 3) {
        fprintf(stderr, "Missing first argument (the rom to load). Alternatively use --debug to enable debug mode\n");
        return 1;
    }

    uint64_t fileLen = 0;
    unsigned char* rom = NULL;
    int8_t debug_enabled = FALSE;

    if(strcmp(argv[1], "--debug") == 0){
        debug_enabled = TRUE;
        read_from_disk(argv[2], &fileLen, &rom);
    }else{
        read_from_disk(argv[1], &fileLen, &rom);
    }

    Interconnect* interconnect = NULL;
    initialize_interconnect(&interconnect);
    load_rom(interconnect, fileLen, rom);
    free(rom);

    Cpu* cpu = NULL;
    initialize_cpu(&cpu, interconnect);

    run(cpu, debug_enabled);
    return 0;
}

