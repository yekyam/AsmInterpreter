#include "AsmI.h"

AsmI::AsmI()
{
	int i = 0;
	for (const auto& x : registerNames)
	{
		registers[i].registerID = getRegisterID(x);
		registers[i].registerValue = 0;
		i++;
	}
}

void AsmI::handleInput(const std::string& input)
{
	std::string word;
	for (const auto& x : input)
	{
		if (x == ',') {}
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

void AsmI::command()
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

int AsmI::getRegisterID(const std::string& name)
{
	int result = 0;
	for (const auto& x : name)
		result += static_cast<int>(x);
	return result;
}

void AsmI::setRegister(const std::string& registerName, u8 value)
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

void AsmI::printRegisters()
{
	int i = 0;
	for (const auto& registerName : registers)
	{
		std::cout << "Name: " << registerNames[i] << "\tRegister Value: " << (ui)registerName.registerValue << "\tBits:"
			<< std::bitset<8>(registerName.registerValue) << '\n';
		i++;
	}
}

void AsmI::printBits(const std::string& registerName)
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

void AsmI::mov(const std::string& arg1, const std::string& arg2)
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

void AsmI::orRegister(const std::string& arg1, const std::string& arg2)
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

void AsmI::xorRegister(const std::string& arg1, const std::string& arg2)
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

void AsmI::andRegister(const std::string& arg1, const std::string& arg2)
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

void AsmI::shlRegister(const std::string& arg1, const std::string& arg2)
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

void AsmI::shrRegister(const std::string& arg1, const std::string& arg2)
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

void AsmI::addRegister(const std::string& arg1, const std::string& arg2)
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

void AsmI::subRegister(const std::string& arg1, const std::string& arg2)
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

void AsmI::mulRegister(const std::string& arg1, const std::string& arg2)
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

void AsmI::divRegister(const std::string& arg1, const std::string& arg2)
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

void AsmI::cmpRegister(const std::string& arg1, const std::string& arg2)
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

void AsmI::pushRegister(const std::string& arg)
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

void AsmI::popRegister(const std::string& registerName)
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

void AsmI::printFlags()
{
	isEqual == true ? std::cout << "Equal flag enabled\n" : std::cout << "Equal flag disabled\n";
}

void AsmI::printHelp()
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