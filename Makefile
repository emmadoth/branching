default:
	nasm -f elf64 branching.asm
	$(CC) -Wall -Wextra -Wconversion -Wvla -g0 -Ofast -pedantic -no-pie -z noexecstack branching.c branching.o

inline:
	nasm -f elf64 branching.asm
	$(CC) -DINLINE_ASM -Wall -Wextra -Wconversion -Wvla -g0 -Ofast -pedantic -no-pie -z noexecstack branching.c branching.o
