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
	AsmI()
	{
		int i = 0;
		for (const auto& x : registerNames)
		{
			registers[i].registerID = getRegisterID(x);
			registers[i].registerValue = 0;
			i++;
		}
	}

	//"Cuts" input into individual strings
	void handleInput(const std::string& input)
	{
		std::string word;
		for (const auto& x : input)
		{
			if (x == ',')  {}
			else if (x == ' ')
			{
				arguments.push_back(word);
				word = "";
			}
			else
			{
				word += x;
			}
		}
		arguments.push_back(word);
	}

	//Handles the cut strings
	void command()
	{
		if (arguments.size() < 1)
		{
			std::cout << "Invalid number of arguments! Must have at least one argument!\n";
		}
		else if (arguments.size() == 1)
		{
			std::string command = lowercase(arguments.at(0));

			if (command == "flags") printFlags();
			else if (command == "help") printHelp();
			else std::cout << "Error! Invalid command!\n";
			arguments.clear();
		}
		else if (arguments.size() == 2)
		{
			std::string command = lowercase(arguments.at(0));
			std::string arg1 = uppercase(arguments.at(1));

			if (command == "push") pushRegister(arg1);
			else if (command == "pop") popRegister(arg1);
			else std::cout << "Error! Invalid command!\n";
			arguments.clear();
		}
		else {
			std::string command = lowercase(arguments.at(0));
			std::string arg1 = uppercase(arguments.at(1));
			std::string arg2 = uppercase(arguments.at(2));

			if (command == "mov") mov(arg1, arg2);
			else if (command == "or") orRegister(arg1, arg2);
			else if (command == "xor") xorRegister(arg1, arg2);
			else if (command == "and") andRegister(arg1, arg2);
			else if (command == "shl") shlRegister(arg1, arg2);
			else if (command == "shr") shrRegister(arg1, arg2);
			else if (command == "add") addRegister(arg1, arg2);
			else if (command == "sub") subRegister(arg1, arg2);
			else if (command == "mul") mulRegister(arg1, arg2);
			else if (command == "div") divRegister(arg1, arg2);
			else if (command == "cmp") cmpRegister(arg1, arg2);
			else std::cout << "Error! Invalid command!\n";
			arguments.clear();
		}
	}

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

	//Generates a register "ID"
	static int getRegisterID(const std::string& name)
	{
		int result = 0;
		for (const auto& x : name)
			result += static_cast<int>(x);
		return result;
	}

	//Sets the value for a specific register
	void setRegister(const std::string& registerName, u8 value)
	{
		for (auto& x : registers)
		{
			if (x.registerID == getRegisterID(registerName))
			{
				x.registerValue = value;
				break;
			}
		}
	}

	//Prints the value for all the registers
	void printRegisters()
	{
		int i = 0;
		for (const auto& registerName : registers)
		{
			std::cout << "Name: " << registerNames[i] << "\tRegister Value: " << (ui)registerName.registerValue <<"\tBits:"
				      << std::bitset<8>(registerName.registerValue) <<'\n';
			i++;
		}
	}

	//Prints the bits of a specific register (for debugging)
	void printBits(const std::string& registerName)
	{
		for (auto& x : registers)
		{
			if (x.registerID == getRegisterID(registerName))
			{
				std::bitset<8> bits(x.registerValue);
				std::cout << bits << "\n";
			}
		}
	}

	//Moves a value into a register, or the value of a register
	void mov(const std::string& arg1, const std::string& arg2)
	{
		try
		{
			int value = std::stoi(arg2);
			if (value > 255)
			{
				std::cout << "Too big!\n";
				return;
			}
			setRegister(arg1, (u8)value);
		}
		catch (std::exception e)
		{
			bool isRegister1 = false;
			u8 value;
			for (auto& x : registers)
			{
				if (x.registerID == getRegisterID(arg2))
				{
					value = x.registerValue;
				}
				if (x.registerID == getRegisterID(arg1))
				{
					isRegister1 = true;
				}
			}
			if (isRegister1)
			{
				setRegister(arg1, value);
			}
		}
	}

	//Bitwise or
	void orRegister(const std::string& arg1, const std::string& arg2)
	{
		u8 value1, value2;
		bool foundReg1 = false, foundReg2 = false;
		for (auto& x : registers)
		{
			if (x.registerID == getRegisterID(arg2))
			{
				foundReg1 = true;
				value2 = x.registerValue;
			}
			if (x.registerID == getRegisterID(arg1))
			{
				foundReg2 = true;
				value1 = x.registerValue;
			}
		}
		if (foundReg1 && foundReg2) 
		{
			setRegister(arg1, value1 | value2);
		}
	}

	//Bitwise xor
	void xorRegister(const std::string& arg1, const std::string& arg2)
	{
		u8 value1, value2;
		bool foundReg1 = false, foundReg2 = false;
		for (auto& x : registers)
		{
			if (x.registerID == getRegisterID(arg2))
			{
				foundReg1 = true;
				value2 = x.registerValue;
			}
			if (x.registerID == getRegisterID(arg1))
			{
				foundReg2 = true;
				value1 = x.registerValue;
			}
		}
		if (foundReg1 && foundReg2)
		{
			setRegister(arg1, value1 ^ value2);
		}
	}

	//Bitwise and
	void andRegister(const std::string& arg1, const std::string& arg2)
	{
		u8 value1, value2;
		bool foundReg1 = false, foundReg2 = false;
		for (auto& x : registers)
		{
			if (x.registerID == getRegisterID(arg2))
			{
				foundReg1 = true;
				value2 = x.registerValue;
			}
			if (x.registerID == getRegisterID(arg1))
			{
				foundReg2 = true;
				value1 = x.registerValue;
			}
		}
		if (foundReg1 && foundReg2)
		{
			setRegister(arg1, value1 & value2);
		}
	}

	//Bit shift left
	void shlRegister(const std::string& arg1, const std::string& arg2)
	{
		try
		{
			bool foundReg = false;
			u8 value = 0;
			int shiftNum = std::stoi(arg2);
			if (shiftNum > 7)
			{
				std::cout << "To big! Max number to shift is only 7!\n";
				return;
			}

			for (auto& x : registers)
			{
				if (x.registerID == getRegisterID(arg1))
				{
					setRegister(arg1, x.registerValue << shiftNum);
				}
			}
		}
		catch (std::exception& e)
		{
			u8 value1, value2;
			bool foundReg1 = false, foundReg2 = false;
			for (auto& x : registers)
			{
				if (x.registerID == getRegisterID(arg2))
				{
					foundReg1 = true;
					value2 = x.registerValue;
				}
				if (x.registerID == getRegisterID(arg1))
				{
					foundReg2 = true;
					value1 = x.registerValue;
				}
			}
			if (foundReg1 && foundReg2)
			{
				setRegister(arg1, value1 << value2);
			}
		}
		
	}

	//Bit shift right
	void shrRegister(const std::string& arg1, const std::string& arg2)
	{
		try
		{
			bool foundReg = false;
			u8 value = 0;
			int shiftNum = std::stoi(arg2);
			if (shiftNum > 7)
			{
				std::cout << "To big! Max number to shift is only 7!\n";
				return;
			}

			for (auto& x : registers)
			{
				if (x.registerID == getRegisterID(arg1))
				{
					setRegister(arg1, x.registerValue >> shiftNum);
				}
			}
		}
		catch (std::exception& e)
		{
			u8 value1, value2;
			bool foundReg1 = false, foundReg2 = false;
			for (auto& x : registers)
			{
				if (x.registerID == getRegisterID(arg2))
				{
					foundReg1 = true;
					value2 = x.registerValue;
				}
				if (x.registerID == getRegisterID(arg1))
				{
					foundReg2 = true;
					value1 = x.registerValue;
				}
			}
			if (foundReg1 && foundReg2)
			{
				setRegister(arg1, value1 >> value2);
			}
		}

	}

	//Adds two registers together
	void addRegister(const std::string& arg1, const std::string& arg2)
	{
		u8 value1, value2;
		bool foundReg1 = false, foundReg2 = false;
		for (auto& x : registers)
		{
			if (x.registerID == getRegisterID(arg2))
			{
				foundReg1 = true;
				value2 = x.registerValue;
			}
			if (x.registerID == getRegisterID(arg1))
			{
				foundReg2 = true;
				value1 = x.registerValue;
			}
		}
		if (foundReg1 && foundReg2)
		{
			setRegister(arg1, value1 + value2);
		}
	}

	//Subtracts two registers from each other
	void subRegister(const std::string& arg1, const std::string& arg2)
	{
		u8 value1, value2;
		bool foundReg1 = false, foundReg2 = false;
		for (auto& x : registers)
		{
			if (x.registerID == getRegisterID(arg2))
			{
				foundReg1 = true;
				value2 = x.registerValue;
			}
			if (x.registerID == getRegisterID(arg1))
			{
				foundReg2 = true;
				value1 = x.registerValue;
			}
		}
		if (foundReg1 && foundReg2)
		{
			setRegister(arg1, value1 - value2);
		}
	}

	//Multiplies two registers together
	void mulRegister(const std::string& arg1, const std::string& arg2)
	{
		u8 value1, value2;
		bool foundReg1 = false, foundReg2 = false;
		for (auto& x : registers)
		{
			if (x.registerID == getRegisterID(arg2))
			{
				foundReg1 = true;
				value2 = x.registerValue;
			}
			if (x.registerID == getRegisterID(arg1))
			{
				foundReg2 = true;
				value1 = x.registerValue;
			}
		}
		if (foundReg1 && foundReg2)
		{
			setRegister(arg1, value1 * value2);
		}
	}

	//Divides two registers 
	void divRegister(const std::string& arg1, const std::string& arg2)
	{
		u8 value1, value2;
		bool foundReg1 = false, foundReg2 = false;
		for (auto& x : registers)
		{
			if (x.registerID == getRegisterID(arg2))
			{
				foundReg1 = true;
				value2 = x.registerValue;
			}
			if (x.registerID == getRegisterID(arg1))
			{
				foundReg2 = true;
				value1 = x.registerValue;
			}
		}
		if (foundReg1 && foundReg2)
		{
			setRegister(arg1, value1 / value2);
		}
	}

	//Subtraction, if the result is 0 the flag is enabled
	void cmpRegister(const std::string& arg1, const std::string& arg2)
	{
		u8 value1, value2;
		for (auto& x : registers)
		{
			if (x.registerID == getRegisterID(arg2))
			{
				value2 = x.registerValue;
			}
			if (x.registerID == getRegisterID(arg1))
			{
				value1 = x.registerValue;
			}
		}
		if ((value1 - value2) == 0)
		{
			isEqual = true;
			return;
		}
		isEqual = false;
	}

	//Pushes a value on to a stack, or the value of a register
	void pushRegister(const std::string& arg)
	{
		try
		{
			int value = std::stoi(arg);
			if (value > 255)
			{
				std::cout << "Value is too big!\n";
				return;
			}
			stack.push((u8)value);
		}
		catch (std::exception e)
		{
			for (auto& x : registers)
			{
				if (getRegisterID(arg) == x.registerID)
				{
					stack.push(x.registerValue);
					x.registerValue = 0;
				}
			}
		}
	}

	//Pops the top value from the stack on to a register
	void popRegister(const std::string& registerName)
	{
		if (stack.size() < 1)
		{
			std::cout << "Stack has no value!\n";
			return;
		}
		for (auto& x : registers)
		{
			if (getRegisterID(registerName) == x.registerID)
			{
				x.registerValue = stack.top();
				stack.pop();
			}
		}
	}

	//Prints any enabled flags
	void printFlags()
	{
		isEqual == true ? std::cout << "Equal flag enabled\n" : std::cout << "Equal flag disabled\n";
	}

	//Prints all available commands
	void printHelp()
	{
		std::cout << "\nWelcome to the Assembly Interpreter!\n\nCommands:\n\nmov   (to reg) (value) \n"
			<< "mov   (to reg) (from reg)\n"
			<< "or    (to reg) (from reg)\n"
			<< "and   (to reg) (from reg)\n"
			<< "xor   (to reg) (from reg)\n"
			<< "shl   (to reg) (num bits)\n"
			<< "shr   (to reg) (num bits)\n"
			<< "add   (to reg) (from reg)\n"
			<< "cmp   (to reg) (from reg)\n"
			<< "add   (to reg) (from reg)\n"
			<< "sub   (to reg) (from reg)\n"
			<< "div   (to reg) (from reg)\n"
			<< "mul   (to reg) (from reg)\n"
			<< "mod   (to reg) (from reg)\n"
			<< "push  (reg)\n"
			<< "pop   (reg)\n\n";
	}
};

int main()
{
	AsmI asmI;
	while (1)
	{
		std::string line = "";
		std::getline(std::cin, line);
		asmI.handleInput(line);
		asmI.command(); 
		asmI.printRegisters();
	}	
}