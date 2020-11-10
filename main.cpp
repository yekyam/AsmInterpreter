#include <iostream>
#include <string>
#include <bitset>
#include <stack>

typedef uint8_t u8;
typedef unsigned int ui;
enum { AL, BL, CL, NUM_REGISTERS };

//mov   (to reg), (from reg) C
//mov   (to reg), (value) C
//or    (to reg), (from reg \) C
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

	void command(std::string command, std::string arg1, std::string arg2)
	{
		if (lowercase(command) == "mov") mov(arg1, arg2);
		else if (lowercase(command) == "or") orRegister(arg1, arg2);
		else if (lowercase(command) == "xor") xorRegister(arg1, arg2);
		else if (lowercase(command) == "and") andRegister(arg1, arg2);
		else if (lowercase(command) == "add") addRegister(arg1, arg2);
		else if (lowercase(command) == "sub") subRegister(arg1, arg2);
		else if (lowercase(command) == "mul") mulRegister(arg1, arg2);
		else if (lowercase(command) == "cmp") cmpRegister(arg1, arg2);
		else if (lowercase(command) == "push") pushRegister(arg1);
		else if (lowercase(command) == "pop") popRegister(arg1);
		else if (lowercase(command) == "flags") printFlags();
	}

	static std::string lowercase(std::string string)
	{
		std::string result;
		for (const auto& x : string)
			result += std::tolower(x);
		return result; 
	}

	static int getRegisterID(std::string name)
	{
		int result = 0;
		for (const auto& x : name)
			result += static_cast<int>(x);
		return result;
	}

	void setRegister(std::string registerID, u8 value)
	{
		for (auto& x : registers)
		{
			if (x.registerID == getRegisterID(registerID))
			{
				x.registerValue = value;
			}
		}
	}

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

	void printBits(std::string registerName)
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

	void mov(std::string arg1, std::string arg2)
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

	void orRegister(std::string arg1, std::string arg2)
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

	void xorRegister(std::string arg1, std::string arg2)
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

	void andRegister(std::string arg1, std::string arg2)
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

	void addRegister(std::string arg1, std::string arg2)
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

	void subRegister(std::string arg1, std::string arg2)
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

	void mulRegister(std::string arg1, std::string arg2)
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

	void cmpRegister(std::string arg1, std::string arg2)
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

	void pushRegister(std::string arg)
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

	void popRegister(std::string registerName)
	{
		for (auto& x : registers)
		{
			if (getRegisterID(registerName) == x.registerID)
			{
				x.registerValue = stack.top();
				stack.pop();
			}
		}
	}

	void printFlags()
	{
		isEqual == true ? std::cout << "Equal flag enabled\n" : std::cout << "Equal flag disabled\n";
	}
};

int main()
{
	AsmI asmI;
	while (1)
	{
		std::string command, arg1, arg2;
		std::cin >> command >> arg1 >> arg2;
		asmI.command(command, arg1, arg2);
		asmI.printRegisters();
	}
	
}
