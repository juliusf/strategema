#ifndef CPU_H
#define CPU_H

#define NUM_GPR 16

#include <inttypes.h>
#include "interconnect.h"
typedef struct cpu_t{
	uint8_t reg_gpr[NUM_GPR];
	uint16_t reg_I;
	uint16_t reg_PC;
	uint8_t reg_sp;
	uint32_t reg_DT; //TODO convert to correct timer Type
	uint32_t reg_ST; //TODO convert to correct timer Type
	Interconnect* interconnect;
} Cpu;

void initialize_cpu(Cpu** cpu, Interconnect* interconnect);
void run(Cpu* cpu);
#endif /* CPU_H */
