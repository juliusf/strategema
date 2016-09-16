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

	switch (opcode){
		default:
			halt_invalid_instruction(opcode, instruction);
	}

	cpu->reg_PC += 2;
}
void halt_invalid_instruction(uint16_t opcode, uint16_t instruction){
	fprintf(stderr, "Invalid Instruction! opcode: %x instruction: %x \n", opcode, instruction);
	exit(-1);
}
void run(Cpu* cpu){
	while(1){
		run_instructtion(cpu);
	}
}


