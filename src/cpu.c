#define _BSB_SOURCE
#include "cpu.h"
#include "timer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>



void halt_invalid_instruction(uint16_t opcode, uint16_t instruction);

void initialize_cpu(Cpu** cpu, Interconnect* interconnect){
	*cpu = (Cpu*) malloc(sizeof(Cpu));
	if (!*cpu){
		fprintf(stderr, "unable to create cpu\n");
		return;
	}
	(*cpu)->reg_PC = PROGRAM_START;
	(*cpu)->interconnect = interconnect;

	initialize_timer( &((*cpu)->reg_DT));
	srand(time(NULL));  // initializing random
}

void run_instructtion(Cpu* cpu)
{
	const unsigned int sleep_usecs = 1 * 1000;
	usleep(sleep_usecs);
	uint16_t instruction = read_word_from_ram(cpu->interconnect, cpu->reg_PC);
	uint16_t opcode = (instruction >> 12) & 0xF;
	uint16_t VX = (instruction >> 8) & 0xF;
	uint16_t VY = (instruction >> 4) & 0xF;

	switch (opcode){
		case 0x0:{
			switch(instruction){ 
				case 0x00EE: { // RET
					cpu->reg_PC = pop_stack(cpu->interconnect);
					return;
				}
				break;
				case 0x00E0:{ // CLS
					clear_screen(cpu->interconnect);
				}
				break;
				default:
					halt_invalid_instruction(opcode, instruction); 
			}
		}
		break;
		case 0x1:{ // JMP ADDR
			uint16_t addr = instruction & 0x0FFF;
			cpu->reg_PC = addr;
			return;
		}
		break;
		case 0x2:{ //CALL addr
			uint16_t addr = instruction & 0xFFF;
			push_stack(cpu->interconnect, cpu->reg_PC +2);
			cpu->reg_PC = addr;
			return;
		}
		break;
		case 0x3:{ //SE Vx == byte
			uint8_t value = (instruction) & 0x00FF;
			if (value == read_reg_gpr(cpu, VX)){
				cpu->reg_PC += 2;
			}
		}
		break;
		case 0x4:{ //SE VX != byte
			uint8_t value = (instruction) & 0x00FF;
			if (value != read_reg_gpr(cpu, VX)){
				cpu->reg_PC += 2;
			}
		}
		break;
		case 0x6:{ // LD Vx, byte
			uint8_t value = instruction & 0x00FF;
			write_reg_gpr(cpu, VX, value);
		}
		break;
		case 0x7:{  //ADD Vx, byte
			uint8_t value = instruction & 0x00FF;
			uint8_t new_value = read_reg_gpr(cpu, VX) + value;	
			write_reg_gpr(cpu, VX, new_value);
		}
		break;
		case 0xa:{ // LD I, addr
			uint16_t addr = instruction & 0xFFF;
			cpu->reg_I = addr;
		}
		break;
		case 0xc:{ // RND Vx, byte
			uint8_t random = rand();
			uint16_t value = (instruction) & 0x00FF;
			write_reg_gpr(cpu, VX, random & value);
		}
		break;
		case 0xd:{ // DRW Vx, Vy, nibble
			uint8_t nibble = instruction & 0x00F;
			uint8_t x_coord = read_reg_gpr(cpu, VX);
			uint8_t y_coord = read_reg_gpr(cpu, VY);
			if ( draw_on_screen(cpu->interconnect, x_coord, y_coord, cpu->reg_I, nibble) ){
				cpu->reg_gpr[15] = 1;
			}else{
				cpu->reg_gpr[15] = 0;
			}
			
		}
		break;
		case 0xf:{
			switch (instruction & 0xF0FF){
				case 0xf01e: { //ADD I, VX
					cpu->reg_I += read_reg_gpr(cpu, VX);
				}
				break;
				case 0xf015: { //LD DT, VX
					set_timer_value(cpu->reg_DT,VX);
				}
				break;
				default:
				halt_invalid_instruction(opcode, instruction);
				goto DEBUG;
			}
		break;
		}
		default:
			halt_invalid_instruction(opcode, instruction);
	
	}

	cpu->reg_PC += 2;
	return;

	DEBUG:
		fprintf(stderr, "DEBUG: instruction: %x opcode: %x VX: %x VY: %x\n", instruction, opcode, VX, VY);
		print_debug_cpu(cpu);
		exit(-1);
}
void halt_invalid_instruction(uint16_t opcode, uint16_t instruction){
	fprintf(stderr, "Invalid Instruction! opcode: %x instruction: %x \n", opcode, instruction);
	exit(-1);
}

void write_reg_gpr(Cpu* cpu, size_t index, uint8_t value){
	if (index != 0xF  && index <= NUM_GPR){
		cpu->reg_gpr[index] = value;
	}else{
		fprintf(stderr, "write to invalid general purpose register!\n");
		exit(-1);
	}
}

uint8_t read_reg_gpr(Cpu* cpu, size_t index){
	if (index <= NUM_GPR){
		return cpu->reg_gpr[index];
	}else{
		fprintf(stderr, "Read from invalid general purpose register: %lu \n", index);
		exit(-1);
	}
}

void run(Cpu* cpu){
	while(1){
		run_instructtion(cpu);
	}
}

void print_debug_cpu(Cpu* cpu){
	printf("Register Contents:\n");

	for (int i = 0 ; i < NUM_GPR; i++){
		printf("REG_GPR[%i]: 0x%x\n", i, cpu->reg_gpr[i]);
	}
}

