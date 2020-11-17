#ifndef ASMI_BCOMPILER
#define ASMI_BCOMPILER
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "AsmI.h"

enum 
{
	FLAGS,
	HELP,
	MOV_V,
	MOV_R,
	OR,
	AND,
	XOR,
	ADD,
	CMP,
	PUSH,
	POP,
	SUB,
	DIV,
	MUL,
	AL_C,
	BL_C,
	CL_C,
};

class ByteCompiler
{
	uint8_t value_stack[1024];
	uint8_t instruction_stack[1024];
	std::string compiledBytes;
	std::vector<std::string> code;
	std::vector<std::string> commands = { "FLAGS",  "HELP", "MOV", "MOV_V", "OR", "AND", "XOR", "ADD", "CMP", "PUSH", "POP", "SUB", "DIV", "MUL", "AL", "BL", "CL", "NULL" };
	
public:

	//Code order is:
	//	Call Ctor, compile code, execute code
	ByteCompiler();

	//Read in lines from a file, calls parser
	ByteCompiler(const std::string& fileNameToCompile);

	//Read in lines, parse lines
	void handleInput(std::string& input);

	//Compiles code to byte code
	void compile(const std::string& fileNameOUT);

	//Runs code, takes in an AsmI reference to run commands and store their result
	void execute(AsmI& asmi, const std::string& fileNameIN);

	//For debugging, prints the bytes that are stores in the compiled bytes string
	void printBytes();
	//For debugging, prints code produced by the parser
	void printCode();
};

#endif
