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
	//uint16_t VX = (instruction >> 8) & 0xF;
	//uint16_t VY = (instruction >> 4) & 0x000F;
	printf("Executing instruction %x\n", instruction);
	switch (opcode){
		case 0x2:{
			uint16_t addr = instruction & 0xFFF;
			push_stack(cpu->interconnect, cpu->reg_PC +2);
			cpu->reg_PC = addr;
			return;
		}
		break;
		case 0x6:{

		}
		break;
		case 0xa:{
			uint16_t addr = instruction & 0xFFF;
			cpu->reg_I = addr;
		}
		break;
		default:
			halt_invalid_instruction(opcode, instruction);
	}

	cpu->reg_PC += 2;
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
void run(Cpu* cpu){
	while(1){
		run_instructtion(cpu);
	}
}


