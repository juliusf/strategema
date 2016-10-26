#define _BSB_SOURCE
#include "cpu.h"
#include "timer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>



void halt_invalid_instruction(uint16_t opcode, uint16_t instruction);
void decode_instruction_to_string(uint16_t instruction, char* output);
void debug_state(Cpu* cpu, uint16_t instruction, uint16_t opcode, uint16_t VX, uint16_t VY);
uint8_t should_break(Cpu* cpu, uint16_t opcode);
void initialize_cpu(Cpu** cpu, Interconnect* interconnect){
	*cpu = (Cpu*) malloc(sizeof(Cpu));
	if (!*cpu){
		fprintf(stderr, "unable to create cpu\n");
		return;
	}
	(*cpu)->reg_PC = PROGRAM_START;
	(*cpu)->interconnect = interconnect;
	(*cpu)->op_count = 0;
	
	(*cpu)->num_breakpoints = 0;


	initialize_timer( &((*cpu)->reg_DT));
	srand(time(NULL));  // initializing random
}

void run_instruction(Cpu* cpu, uint8_t debug_enabled)
{
	const unsigned int sleep_usecs = 500;  // hack for preventing too fast drawing
	usleep(sleep_usecs);
	uint16_t instruction = read_word_from_ram(cpu->interconnect, cpu->reg_PC);
	uint16_t opcode = (instruction >> 12) & 0xF;
	uint16_t VX = (instruction >> 8) & 0xF;
	uint16_t VY = (instruction >> 4) & 0xF;
	
	if (debug_enabled){
			char instruction_name[80];
			decode_instruction_to_string(instruction, instruction_name);
			printf("%i: %s - (0x%x)\n", cpu->op_count, instruction_name, instruction);
		if ( cpu->num_breakpoints == 0 || should_break(cpu, opcode))
		{
			debug_state(cpu, instruction, opcode, VX, VY);
		}
	}



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
					goto DEBUG; 
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
			uint16_t value = (instruction) & 0x00FF;
			if (value == read_reg_gpr(cpu, VX)){
				cpu->reg_PC += 2;
			}
		}
		break;
		case 0x4:{ //SE VX != byte
			uint16_t value = (instruction) & 0x00FF;
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
			uint16_t new_value = read_reg_gpr(cpu, VX) + value;	
			write_reg_gpr(cpu, VX, new_value);
		}
		break;
		case 0x8:{
			switch(instruction & 0xF00F){
				case (0x8000):{ // Vx = Vy
					uint16_t val = read_reg_gpr(cpu, VY);
					write_reg_gpr(cpu, VX, val);
				}
				break;
				case (0x8001):{ //OR VX, VY
					uint16_t VX_val = read_reg_gpr(cpu, VX);
					uint16_t VY_val = read_reg_gpr(cpu, VY);
					uint16_t result = VX_val | VY_val;
					write_reg_gpr(cpu, VX, result);
				}
				break;
				case(0x8002):{ //AND VX, Vy
					uint16_t VX_val = read_reg_gpr(cpu, VX);
					uint16_t VY_val = read_reg_gpr(cpu, VY);
					uint16_t result = VX_val & VY_val;
					write_reg_gpr(cpu, VX, result);
				}
				break;
				case(0x8005):{ //SUB VX, Vy
					uint16_t VX_val = read_reg_gpr(cpu, VX);
					uint16_t VY_val = read_reg_gpr(cpu, VY);
					if (VX_val > VY_val){
						write_reg_gpr(cpu, 15, 1);
					}else
					{
						write_reg_gpr(cpu, 15, 0);
					}
					uint16_t result = VX_val - VY_val;
					write_reg_gpr(cpu, VX, result);
				}
				break;
				default:
				halt_invalid_instruction(opcode, instruction);
				goto DEBUG;
			}
		}
		break;
		case 0xa:{ // LD I, addr
			uint16_t addr = instruction & 0xFFF;
			cpu->reg_I = addr;
		}
		break;
		case 0xc:{ // RND Vx, byte
			uint8_t random = rand(); //TODO FIX	
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
		case 0xe:{
			switch (instruction & 0x00FF){
				case 0x00a1:{ // SKNP Vx
					//TODO INPUT
					cpu->reg_PC += 2;
				}
				break;
				case 0x009e:{ // SKP Vx
					//TODO INPUT

				}
				break;
				default:
				halt_invalid_instruction(opcode, instruction);
				goto DEBUG;
			}
		}
		break;
		case 0xf:{
			switch (instruction & 0xF0FF){ // TODO check this
				case 0xf007:{ // LD VX, DT
					uint8_t timer_val = get_timer_value(cpu->reg_DT);
					write_reg_gpr(cpu, VX, timer_val);
				}
				break;
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
			goto DEBUG;
	
	}

	cpu->reg_PC += 2;
	return;

	DEBUG:
		fprintf(stderr, "DEBUG: instruction: %x opcode: %x VX: %x VY: %x\n", instruction, opcode, VX, VY);
		print_debug_cpu(cpu);
		while(1){}
}

void halt_invalid_instruction(uint16_t opcode, uint16_t instruction){
	fprintf(stderr, "Invalid Instruction! opcode: %x instruction: %x \n", opcode, instruction);
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

void run(Cpu* cpu, uint8_t debug_enabled){
	while(1){
		run_instruction(cpu, debug_enabled);
		cpu->op_count++;
	}	
}

void debug_state(Cpu* cpu, uint16_t instruction, uint16_t opcode, uint16_t VX, uint16_t VY){
	printf("hit breakpoint\n");
	print_debug_cpu(cpu);
	char input = 0;
 	printf("(s)tep | run (u)ntil? | break at (o)p code | (c)ontinue\n");
 	do {
    	input=getchar();
  	}while (input != 's' && input != 'u' && input != 'o' && input !='c' );

	if (input == 'u'){
		printf("Execute until which op count?\n");
		uint32_t next_breakpoint = 0;
		
		while( next_breakpoint == 0  )
		{
			scanf("%i", &next_breakpoint);
		}
		cpu->breakpoints[cpu->num_breakpoints].type = OPCOUNT;
		cpu->breakpoints[cpu->num_breakpoints].value = next_breakpoint;
		cpu->num_breakpoints++;
	}
	if (input == 'o'){
		printf("Execute until which op code?\n");
		uint16_t break_opcode =0;
		while( break_opcode == 0 )
		{
			scanf("%hx", &break_opcode);
		}

		cpu->breakpoints[cpu->num_breakpoints].type = OPCODE;
		cpu->breakpoints[cpu->num_breakpoints].value = break_opcode;
		cpu->num_breakpoints++;
	}
	if (input == 'c'){
		return;
	}
	printf("-----------------------\n");
}

uint8_t should_break(Cpu* cpu, uint16_t opcode){
	for (int i = 0; i < cpu->num_breakpoints; i ++){
		if (cpu->breakpoints[i].type == OPCODE && cpu->breakpoints[i].value == opcode){
			return TRUE;
		}
		if (cpu->breakpoints[i].type == OPCOUNT && cpu->breakpoints[i].value == cpu->op_count){
			return TRUE;
		}

	}
	return FALSE;
}

void decode_instruction_to_string(uint16_t instruction, char* output){
	uint16_t opcode = (instruction >> 12) & 0xF;
	uint16_t VX = (instruction >> 8) & 0xF;
	uint16_t VY = (instruction >> 4) & 0xF;
	uint16_t byte = instruction & 0x00FF;
	uint16_t addr = instruction& 0x0FFF;
	

	switch (opcode){
		case 0x0:{
			switch(instruction){ 
				case 0x00EE: { // RET
					sprintf(output, "RET");
				}
				break;
				case 0x00E0:{ // CLS
					sprintf(output, "CLS");
				}
				break;
				default:
					sprintf(output, "INVALID"); 
			}
		}
		break;
		case 0x1:{ // JMP ADDR
			sprintf(output, "JMP %x", addr);
		}
		break;
		case 0x2:{ //CALL addr
			sprintf(output, "CALL %x", addr);
		}
		break;
		case 0x3:{ //SE Vx == byte
			sprintf(output, "SE %x %x", VX, byte);
		}
		break;
		case 0x4:{ //SE VX != byte
			sprintf(output, "SNE %x %x", VX, byte);
		}
		break;
		case 0x6:{ // LD Vx, byte
			sprintf(output, "LD %x %x", VX, byte);
		}
		break;
		case 0x7:{  //ADD Vx, byte
			sprintf(output, "ADD %x %x", VX, byte);
		}
		break;
		case 0x8:{
			switch(instruction & 0xF00F){
				case (0x8000):{ // Vx = Vy
					sprintf(output, "LD %x %x", VX, VY);
				}
				break;
				case (0x8001):{ //OR VX, VY
					sprintf(output, "OR %x %x", VX, VY);
				}
				break;
				case(0x8002):{ //AND VX, Vy
					sprintf(output, "AND %x %x", VX, VY);
				}
				break;
				case(0x8005):{ //SUB VX, Vy
					sprintf(output, "SUB %x %x", VX, VY);
				}
				break;
				default:
					printf(output, "INVALID");
			}
		}
		break;
		case 0xa:{ // LD I, addr
			sprintf(output, "LD I %x", addr);
		}
		break;
		case 0xc:{ // RND Vx, byte
			sprintf(output, "RND %x %x", VX, byte);
		}
		break;
		case 0xd:{ // DRW Vx, Vy, nibble
			sprintf(output, "DRW %x %x %x", VX, VY, instruction & 0x000F);
		}
		break;
		case 0xe:{
			switch (instruction & 0x00FF){
				case 0x00a1:{ // SKP Vx
					sprintf(output, "SKNP %x", VX);
				}
				break;
				case 0x009e:{ // SKNP Vx
					printf(output, "SKP %x", VX);
				}
				break;
				default:
				printf(output, "INVALID");
			}
		}
		break;
		case 0xf:{
			switch (instruction & 0xF0FF){ // TODO check this
				case 0xf007:{ // LD VX, DT
					printf(output, "LD %x DT", VX);
				}
				break;
				case 0xf01e: { //ADD I, VX
					printf(output, "ADD I, %x", VX);
				}
				break;
				case 0xf015: { //LD DT, VX
					printf(output, "LD DT, %x", VX);
				}
				break;
				default:
					printf(output, "INVALID");
			}
		break;
		}
		default:
			printf(output, "INVALID");
	
	}
}

void print_debug_cpu(Cpu* cpu){
	printf("Register Contents:\n");

	for (int i = 0 ; i < NUM_GPR; i++){
		printf("REG_GPR[%i]: 0x%x\n", i, cpu->reg_gpr[i]);
	}
}

