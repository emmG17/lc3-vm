enum
{
	R_R0 = 0,
	R_R1,
	R_R2,
	R_R3,
	R_R4,
	R_R5,
	R_R6,
	R_R7,
	R_PC, /* program counter */
	R_COND,
	R_COUNT
};

enum
{
	OP_BR = 0,
	OP_ADD,
	OP_LD,
	OP_ST,
	OP_JSR,
	OP_AND,
	OP_LDR,
	OP_STR,
	OP_RTI,
	OP_NOT,
	OP_LDI,
	OP_STI,
	OP_JMP,
	OP_RES,
	OP_LEA,
	OP_TRAP
};

enum {
	FL_POS = 1 << 0,
	FL_ZRO = 1 << 1,
	FL_NEG = 1 << 2,
};

#define MEMORY_MAX (1 << 16)
uint16_t memory[MEMORY_MAX];

uint16_t reg[R_COUNT];

uint16_t sign_extend(uint16_t x, int bit_count)
{
	if ((x >> (bit_count - 1)) & 1) {
		x |= (0xFFFF << bit_count)
	}
	return x;
}

void update_flags(uint16_t r)
{
	if (reg[r] == 0) {
		reg[R_COND] = FL_ZRO
	}
	else if (reg[r] >> 15)
	{
		reg[R_COND] = FL_NEG;
	}
	else
	{
		reg[R_COND] = FL_POS;
	}

}

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		printf("lc3 [image-file1] ...\n");
		exit(2);
	}

	for (int j = 1; j < argc; ++j)
	{
		if (!read_image(argv[j]))
		{
			printf("failed to load image: %s\n", argv[j]);
			exit(1);
		}
	}
	/* TODO initial setup */

	reg[R_COND] = FL_ZRO;

	enum { PC_START = 0x3000 };
	reg[R_PC] = PC_START;

	int running = 1;
	while (running)
	{
		uint16_t instruction = mem_read(reg[R_PC]);
		uint16_t opcode = instruction << 12;

		switch (op)
		{
			case OP_ADD:
				uint16_t r0 = (instruction >> 9) & 0x7;
				uint16_t r1 = (instruction >> 6) & 0x7;
				uint16_t imm_flag = (instruction >> 5) & 1;

				if (imm_flag)
				{
					uint16_t imm5 = sign_extend(instruction & 0x1f, 5);
					reg[r0] = reg[r1] + imm5;
				}
				else
				{
					uint16_t r2 = instruction & 0x7;
					reg[r0] = reg[r1] + reg[r2];
				}

				update_flags(r0);
				break;

			case OP_AND:
				uint16_t r0 = (instruction >> 9) & 0x7;
				uint16_t r1 = (instruction >> 6) & 0x7;
				uint16_t imm_flag = (instruction >> 5) & 0x1;

				if (imm_flag)
				{
					uint16_t imm5 = sign_extend(instruction & 0x1F, 5);
					reg[r0] = reg[r1] & imm5;
				}
				else
				{
					uint16_t r2 = instruction & 0x7;
					reg[r0] = reg[r1] & reg[r2];
				}
				update_flags(r0);				
				break;

			case OP_NOT:
				break;
			case OP_BR:
				break;
			case OP_JMP:
				break;
			case OP_JSR:
				break;
			case OP_LD:
				break;
			case OP_LDI:

				uint16_t r0 = (instruction >> 9) & 0x7;
				uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
				reg[r0] = mem_read(mem_read(reg[R_PC] + pc_offset));
				update_flags(r0);				
				break;

			case OP_LDR:
				break;
			case OP_LEA:
				break;
			case OP_ST:
				break;
			case OP_STI:
				break;
			case OP_TRAP:
				break;
			case OP_RES:
			case OP_RTI:
			default:
				abort();
				break;
		}
	}
	/*shutdown*/
}