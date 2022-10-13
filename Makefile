FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE

asm:
	g++ $(FLAGS) ASSEMBLER/assembler_main.cpp ASSEMBLER/assembler_functions.cpp -o ass.exe
cpu:
	g++ $(FLAGS) PROCESSOR/processor_main.cpp PROCESSOR/processor_functions.cpp STACK/stack_functions.cpp STACK/error_functions.cpp -o cpu.exe
stack:
	g++ $(FLAGS) STACK/main.cpp STACK/stack_functions.cpp STACK/error_functions.cpp -o STACK/do.exe	