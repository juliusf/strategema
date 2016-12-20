#include "interconnect.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "gfx.h"

void initialize_font(Interconnect* interconnect);
void initialize_interconnect(Interconnect** interconnect){
    *interconnect = (Interconnect*) malloc(sizeof(Interconnect));
    if (!*interconnect){
    fprintf(stderr, "unable to create interconnect\n");
    return;
    }
    printf("initializing %i bytes of RAM\n", RAM_SIZE);
    memset((*interconnect)->ram, 42, RAM_SIZE);
    (*interconnect)->stack_ptr = 0;
    (*interconnect)->gfx = NULL;

    initialize_gfx(&((*interconnect)->gfx));
    initialize_font((*interconnect));
}

void load_rom(Interconnect* interconnect, uint64_t rom_len, unsigned char* rom){
    memcpy(interconnect->ram + PROGRAM_START, rom, rom_len);
}

uint16_t read_word_from_ram(Interconnect* interconnect, uint16_t addr){
    if ( ((addr > 0x1ff) && (addr <= 0xfff)) || (addr <= 0x50) ) // 0x0 - 0x50 is character sprite map
    {
        return ((interconnect->ram[addr] << 8) |
                (interconnect->ram[addr+1]));
    } else
    {
        fprintf(stderr, "Invalid Read from address: %x\n", addr);
        exit(-1);
        return 0;
    }
}

void write_byte_to_ram(Interconnect* interconnect, uint16_t addr, uint8_t value){
    if ( ((addr > 0x1ff) && (addr <= 0xfff)) || (addr <= 0x50) ) // apparently some roms modify the system font
    {
        if (addr <= 0x50)
        {
            printf("WARNING: Write to system Memory detected\n");
            return;
        }
        interconnect->ram[addr] = value;
    } else
    {
        fprintf(stderr, "Invalid Write to address: %x\n", addr);
        exit(-1);
    }
}

void push_stack(Interconnect* interconnect, uint16_t addr){
    if (interconnect->stack_ptr +1 == STACK_SIZE){
        fprintf(stderr, "Stack Overflow!\n" );
        exit(0);
    }
    interconnect->stack_ptr += 1;
    interconnect->stack[interconnect->stack_ptr] = addr;
}

uint16_t pop_stack(Interconnect* interconnect){
    if(interconnect->stack_ptr == 0){
        fprintf(stderr, "Stack Underflow!\n" );
        exit(0);
    }
    interconnect->stack_ptr -= 1;
    uint16_t addr = interconnect->stack[interconnect->stack_ptr + 1];
    return addr;
}

uint8_t draw_on_screen(Interconnect* interconnect, uint8_t screen_loc_x, uint8_t screen_loc_y, uint16_t memory_loc, uint8_t len){
    return draw_sprite(interconnect->gfx, screen_loc_x, screen_loc_y, len, &(interconnect->ram[memory_loc]));
}

void clear_screen(Interconnect* interconnect){
    clear_backbuffer(interconnect->gfx);
}


void initialize_font(Interconnect* interconnect){
    unsigned char zero[] =  {0xf0, 0x90, 0x90, 0x90, 0xf0};
    unsigned char one[]  =  {0x20, 0x60, 0x20, 0x20, 0x70};
    unsigned char two[]  =  {0xf0, 0x10, 0xf0, 0x80, 0xf0};
    unsigned char three[]=  {0xf0, 0x10, 0xf0, 0x10, 0xf0};
    unsigned char four[] =  {0x90, 0x90, 0xf0, 0x10, 0x10};
    unsigned char five[] =  {0xf0, 0x80, 0xf0, 0x10, 0xf0};
    unsigned char six[]  =  {0xf0, 0x80, 0xf0, 0x90, 0xf0};
    unsigned char seven[]=  {0xf0, 0x10, 0x20, 0x40, 0x40};
    unsigned char eight[]=  {0xf0, 0x90, 0xf0, 0x90, 0xf0};
    unsigned char nine[] =  {0xf0, 0x90, 0xf0, 0x10, 0xf0};
    unsigned char A[]    =  {0xf0, 0x90, 0xf0, 0x90, 0x90};
    unsigned char B[]    =  {0xe0, 0x90, 0xe0, 0x90, 0xe0};
    unsigned char C[]    =  {0xf0, 0x80, 0x80, 0x80, 0xf0};
    unsigned char D[]    =  {0xe0, 0x90, 0x90, 0x90, 0xe0};
    unsigned char E[]    =  {0xf0, 0x80, 0xf0, 0x80, 0xf0};
    unsigned char F[]    =  {0xf0, 0x80, 0xf0, 0x80, 0x80};
    
    unsigned char* chars[] = {zero, one, two, three, four,
                               five, six, seven, eight, nine,
                               A, B, C, D, E, F};

    for (int character = 0 ; character < 16; character++){
        memcpy( &(interconnect->ram[character*5]), chars[character], 5); 
        interconnect->sprite_lookup_table[character] = character*5;  
    }       
}
