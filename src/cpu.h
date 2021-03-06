#ifndef CPU_H
#define CPU_H

#define NUM_GPR 16
#define MAX_NUM_BREAKPOINTS 32

#include <inttypes.h>
#include "interconnect.h"
#include <stddef.h>
#include "timer.h"

enum breakpointType_t {OPCODE, OPCOUNT};

typedef struct breakpoint_t{
	enum breakpointType_t type;
	uint32_t value;
}Breakpoint;

typedef struct cpu_t{
	uint8_t reg_gpr[NUM_GPR];
	uint16_t reg_I;
	uint16_t reg_PC;
	uint8_t reg_sp;
	//Timer* reg_DT; //TODO convert to correct timer Type
	uint32_t reg_ST; //TODO convert to correct timer Type
	uint32_t op_count;
	Breakpoint breakpoints[MAX_NUM_BREAKPOINTS];
	uint8_t num_breakpoints;
	Interconnect* interconnect;
} Cpu;


void initialize_cpu(Cpu** cpu, Interconnect* interconnect);
void run(Cpu* cpu, uint8_t debug_enabled);
void write_reg_gpr(Cpu* cpu, size_t index, uint8_t value);
uint8_t read_reg_gpr(Cpu* cpu, size_t index);
void print_debug_cpu(Cpu* cpu);
#endif /* CPU_H */
