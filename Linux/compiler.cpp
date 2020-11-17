#include <iostream>
#include <string>

#include "AsmI.h"
#include "ByteCompiler.h"

//Thin interface for calling the compiler. The arg fileOUT is not needed if mode 2 is selected
int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cout<<"Usage: >compiler (1 for compilation, 2 for execution) fileIN fileOUT\n";
		return 0;
	}
	if (argc >= 3)
	{
		std::string mode(argv[1]);
		std::string fileIN(argv[2]);
		if (mode == "1" && argc == 4)
		{
			std::string out(argv[3]);
			ByteCompiler compiler(fileIN);
			compiler.compile(out);
			return 0;
		}
		if (mode == "2" && argc == 3)
		{
			AsmI interpreter;
			ByteCompiler compiler;
			compiler.execute(interpreter, fileIN);
			interpreter.printRegisters();
			return 0;
		}
		else
		{
			std::cout << "Invalid arguments!\n";
			return 0;
		}
	}
}
