#ifndef ASMI
#define ASMI
#include <cstdint>
#include <iostream>
#include <string>
#include <bitset>
#include <stack>
#include <vector>


typedef uint8_t u8;
typedef unsigned int ui;
enum { AL, BL, CL, NUM_REGISTERS };

//mov   (to reg), (from reg)
//mov   (to reg), (value)
//or    (to reg), (from reg) 
//and   (to reg), (from reg) 
//xor   (to reg), (from reg)
//add   (to reg), (from reg)
//cmp   (to reg), (from reg)
//push  (reg),    (NULL)
//pop   (reg),    (NULL)
//add   (to reg), (from reg)
//sub   (to reg), (from reg)
//div   (to reg), (from reg)
//mul   (to reg), (from reg)
//mod   (to reg), (from reg)

//Helper Function, return a lowercase copy of a string
static std::string lowercase(const std::string& string)
{
	if (std::isalpha(string.at(0))) {
		std::string result;
		for (auto& x : string)
		{
			result += std::tolower(x);
		}
		return result;
	}
	else return string;
}

//Helper Function, return an uppercase copy of a string
static std::string uppercase(const std::string& string)
{
	try {
		if (std::isalpha(string.at(0))) {
			std::string result;
			for (auto& x : string)
			{
				result += std::toupper(x);
			}
			return result;
		}
		else return string;
	}
	catch (std::exception e)
	{
		return string; 
	}
}

class AsmI
{
private:
	std::vector<std::string> arguments;
	bool isEqual;
	struct Register
	{
		u8 registerValue;
		int registerID;
	};
	Register registers[NUM_REGISTERS];
	std::string registerNames[NUM_REGISTERS] = { "AL", "BL", "CL" };
	std::stack<u8> stack;
public:
	//Set each register "ID" and set the values to 0. Used loop instead of hardcoded to allow for more registers if desired
	AsmI();

	//"Cuts" input into individual strings
	void handleInput(const std::string& input);

	//Handles the cut strings
	void command();

	//Generates a register "ID"
	int getRegisterID(const std::string& name);

	//Sets the value for a specific register
	void setRegister(const std::string& registerName, u8 value);

	//Prints the value for all the registers
	void printRegisters();

	//Prints the bits of a specific register (for debugging)
	void printBits(const std::string& registerName);

	//Moves a value into a register, or the value of a register
	void mov(const std::string& arg1, const std::string& arg2);

	//Bitwise or
	void orRegister(const std::string& arg1, const std::string& arg2);

	//Bitwise xor
	void xorRegister(const std::string& arg1, const std::string& arg2);

	//Bitwise and
	void andRegister(const std::string& arg1, const std::string& arg2);

	//Bit shift left
	void shlRegister(const std::string& arg1, const std::string& arg2);

	//Bit shift right
	void shrRegister(const std::string& arg1, const std::string& arg2);

	//Adds two registers together
	void addRegister(const std::string& arg1, const std::string& arg2);

	//Subtracts two registers from each other
	void subRegister(const std::string& arg1, const std::string& arg2);

	//Multiplies two registers together
	void mulRegister(const std::string& arg1, const std::string& arg2);

	//Divides two registers 
	void divRegister(const std::string& arg1, const std::string& arg2);

	//Subtraction, if the result is 0 the flag is enabled
	void cmpRegister(const std::string& arg1, const std::string& arg2);

	//Pushes a value on to a stack, or the value of a register
	void pushRegister(const std::string& arg);

	//Pops the top value from the stack on to a register
	void popRegister(const std::string& registerName);

	//Prints any enabled flags
	void printFlags();

	//Prints all available commands
	void printHelp();
};

#endif
