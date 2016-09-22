#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void halt_invalid_instruction(uint16_t opcode, uint16_t instruction);

void initialize_cpu(Cpu** cpu, Interconnect* interconnect){
	*cpu = (Cpu*) malloc(sizeof(Cpu));
	if (!*cpu){
		fprintf(stderr, "unable to create cpu\n");
		return;
	}
	(*cpu)->reg_PC = PROGRAM_START;
	(*cpu)->interconnect = interconnect;
}

void run_instructtion(Cpu* cpu)
{
	uint16_t instruction = read_word_from_ram(cpu->interconnect, cpu->reg_PC);
	uint16_t opcode = (instruction >> 12) & 0xF;
	uint16_t VX = (instruction >> 8) & 0xF;
	uint16_t VY = (instruction >> 4) & 0xF;
	printf("Executing instruction %x\n", instruction);
	switch (opcode){
		case 0x0:{
			switch(instruction){ 
				case 0x00EE: { // RET
					cpu->reg_PC = pop_stack(cpu->interconnect);
					return;
				}
				break;
				default:
					halt_invalid_instruction(opcode, instruction); 
			}
		}
		break;
		case 0x2:{ //CALL addr
			uint16_t addr = instruction & 0xFFF;
			push_stack(cpu->interconnect, cpu->reg_PC +2);
			cpu->reg_PC = addr;
			return;
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
		case 0xd:{ // DRW Vx, Vy, nibble
			uint8_t nibble = instruction & 0x00F;
			uint8_t x_coord = read_reg_gpr(cpu, VX);
			uint8_t y_coord = read_reg_gpr(cpu, VY);

			draw_on_screen(cpu->interconnect, x_coord, y_coord, cpu->reg_I, nibble);
			goto DEBUG;
		}
		break;
		default:
			halt_invalid_instruction(opcode, instruction);
	}

	cpu->reg_PC += 2;
	return;

	DEBUG:
		fprintf(stderr, "DEBUG: instruction: %x opcode: %x VX: %x VY: %x\n", instruction, opcode, VX, VY);
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
	if (index !=0xF && index <= NUM_GPR){
		return cpu->reg_gpr[index];
	}else{
		fprintf(stderr, "Read from invalid general purpose register!\n");
		exit(-1);
	}
}

void run(Cpu* cpu){
	while(1){
		run_instructtion(cpu);
	}
}


