#include "ByteCompiler.h"


ByteCompiler::ByteCompiler() {}


ByteCompiler::ByteCompiler(const std::string& fileNameToCompile)
{
	std::ifstream fileToCompile(fileNameToCompile);

	if (!fileToCompile.is_open())
	{
		std::cout << "Couldn't open file: " << fileNameToCompile << "\n";
		return;
	}

	while (fileToCompile.good())
	{
		std::string line;
		std::getline(fileToCompile, line);
		handleInput(line);
	}
	fileToCompile.close();
}

void ByteCompiler::handleInput(std::string& input)
{
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
	std::string word;
	for (const auto& x : input)
	{
		if (x == ',') {}
		else if (x == ' ')
		{ 
			code.push_back(word);
			word = "";
		}
		else
		{
			word += (unsigned char)x;
		}
	}
	code.push_back(word);
}

void ByteCompiler::compile(const std::string& fileNameOUT)
{
	for (int i = 0; i < code.size(); i++)
	{
		auto result = std::find(commands.begin(), commands.end(), uppercase(code[i]));
		if (result == commands.end())
		{
			try
			{
				int value = std::stoi(code[i]);
				compiledBytes += (char)value;
				continue;
			}
			catch (std::exception e)
			{
				std::cout << code[i] << " is not a valid command!\n";
				return;
			}
		}
		else
		{
			if (*result == "MOV")
			{
				if (uppercase(code[i + 2]) == "AL" || uppercase(code[i + 2]) == "BL" || uppercase(code[i + 2]) == "CL")
				{
					compiledBytes += 3;
					continue;
				}
				else 
				{
					compiledBytes += 2;
					continue;
				}
			}
			compiledBytes += (result - commands.begin());
		}
	}
	std::ofstream file(fileNameOUT, std::fstream::trunc);
	for (const auto& x : compiledBytes)
	{
		file.put(x);
	}
	file.close();
	std::cout << "Compilation suceeded succesfully.\n";
}

void ByteCompiler::execute(AsmI& asmi, const std::string& fileNameIN)
{
	std::ifstream file(fileNameIN);
	if (!file.is_open())
	{
		std::cout << "Couldn't open file!\n";
	}
	std::string bytes;
	char c;
	while (file.get(c))
	{
#ifdef ASMI_DEBUG
		std::cout << (unsigned int) c << "\n";
#endif
		bytes += c;
	}
		
	for (int i = 0; i < bytes.size(); i++)
	{
		switch (bytes[i])
		{
		case 0:
			asmi.printFlags();
			break;
		case 1:
			asmi.printHelp();
			break;
		case 2:
			if (bytes[i+1] == AL_C)
			{
				asmi.setRegister("AL", bytes[i + 2]);
				i += 2;
				break;
			}
			else if (bytes[i + 1] == BL_C)
			{
				asmi.setRegister("BL", bytes[i + 2]);
				i += 2;
				break;
			}
			else if (bytes[i + 1] == CL_C)
			{
				asmi.setRegister("CL", bytes[i + 2]);
				i += 2;
				break;
			}
			else 
			{
				std::cout << "Error! Invalid syntax!\n";
			}
		case 3:
			if (bytes[i + 1] == AL_C || bytes[i + 1] == BL_C || bytes[i + 1] == CL_C)
			{
				if (bytes[i + 2] == AL_C || bytes[i + 2] == BL_C || bytes[i + 2] == CL_C)
				{
					asmi.mov(commands[bytes[i+1]-1], commands[bytes[i + 2]-1]);
				}
				else
				{
					std::cout << "Error! Invalid syntax!\n";
				}
			}
			else
			{
				std::cout << "Error! Invalid syntax!\n";
			}
			i += 2;
			break;
		case 4:
			if (bytes[i + 1] == AL_C || bytes[i + 1] == BL_C || bytes[i + 1] == CL_C)
			{
				if (bytes[i + 2] == AL_C || bytes[i + 2] == BL_C || bytes[i + 2] == CL_C)
				{
					asmi.orRegister(commands[bytes[i + 1]-1], commands[bytes[i + 2]-1]);
				}
				else
				{
					std::cout << "Error! Invalid syntax!\n";
				}
			}
			else
			{
				std::cout << "Error! Invalid syntax!\n";
			}
			i += 2;
			break;
		case 5:
			if (bytes[i + 1] == AL_C || bytes[i + 1] == BL_C || bytes[i + 1] == CL_C)
			{
				if (bytes[i + 2] == AL_C || bytes[i + 2] == BL_C || bytes[i + 2] == CL_C)
				{
					asmi.andRegister(commands[bytes[i + 1]], commands[bytes[i + 2]]);
				}
				else
				{
					std::cout << "Error! Invalid syntax!\n";
				}
			}
			else
			{
				std::cout << "Error! Invalid syntax!\n";
			}
			i += 2;
			break;
		case 6:
			if (bytes[i + 1] == AL_C || bytes[i + 1] == BL_C || bytes[i + 1] == CL_C)
			{
				if (bytes[i + 2] == AL_C || bytes[i + 2] == BL_C || bytes[i + 2] == CL_C)
				{
					asmi.xorRegister(commands[bytes[i + 1]], commands[bytes[i + 2]]);
				}
				else
				{
					std::cout << "Error! Invalid syntax!\n";
				}
			}
			else
			{
				std::cout << "Error! Invalid syntax!\n";
			}
			i += 2;
			break;
		case 7:
			if (bytes[i + 1] == AL_C || bytes[i + 1] == BL_C || bytes[i + 1] == CL_C)
			{
				if (bytes[i + 2] == AL_C || bytes[i + 2] == BL_C || bytes[i + 2] == CL_C)
				{
					asmi.addRegister(commands[bytes[i + 1]], commands[bytes[i + 2]]);
				}
				else
				{
					std::cout << "Error! Invalid syntax!\n";
				}
			}
			else
			{
				std::cout << "Error! Invalid syntax!\n";
			}
			i += 2;
			break;
		case 8:
			if (bytes[i + 1] == AL_C || bytes[i + 1] == BL_C || bytes[i + 1] == CL_C)
			{
				if (bytes[i + 2] == AL_C || bytes[i + 2] == BL_C || bytes[i + 2] == CL_C)
				{
					asmi.cmpRegister(commands[bytes[i + 1]], commands[bytes[i + 2]]);
				}
				else
				{
					std::cout << "Error! Invalid syntax!\n";
				}
			}
			else
			{
				std::cout << "Error! Invalid syntax!\n";
			}
			i += 2;
			break;
		case 9:
			if (bytes[i + 1] == AL_C || bytes[i + 1] == BL_C || bytes[i + 1] == CL_C)
			{
				asmi.pushRegister(commands[bytes[i + 1]]);
			}
			else
			{
				std::cout << "Error! Invalid syntax!\n";
			}
			i += 1;
			break;
		case 10:
			if (bytes[i + 1] == AL_C || bytes[i + 1] == BL_C || bytes[i + 1] == CL_C)
			{
				asmi.popRegister(commands[bytes[i + 1]]);
			}
			else
			{
				std::cout << "Error! Invalid syntax!\n";
			}
			i += 1;
			break;
		case 11:
			if (bytes[i + 1] == AL_C || bytes[i + 1] == BL_C || bytes[i + 1] == CL_C)
			{
				if (bytes[i + 2] == AL_C || bytes[i + 2] == BL_C || bytes[i + 2] == CL_C)
				{
					asmi.subRegister(commands[bytes[i + 1]], commands[bytes[i + 2]]);
				}
				else
				{
					std::cout << "Error! Invalid syntax!\n";
				}
			}
			else
			{
				std::cout << "Error! Invalid syntax!\n";
			}
			i += 2;
			break;
		case 12:
			if (bytes[i + 1] == AL_C || bytes[i + 1] == BL_C || bytes[i + 1] == CL_C)
			{
				if (bytes[i + 2] == AL_C || bytes[i + 2] == BL_C || bytes[i + 2] == CL_C)
				{
					asmi.divRegister(commands[bytes[i + 1]], commands[bytes[i + 2]]);
				}
				else
				{
					std::cout << "Error! Invalid syntax!\n";
				}
			}
			else
			{
				std::cout << "Error! Invalid syntax!\n";
			}
			i += 2;
			break;
		case 13:
			if (bytes[i + 1] == AL_C || bytes[i + 1] == BL_C || bytes[i + 1] == CL_C)
			{
				if (bytes[i + 2] == AL_C || bytes[i + 2] == BL_C || bytes[i + 2] == CL_C)
				{
					asmi.mulRegister(commands[bytes[i + 1]], commands[bytes[i + 2]]);
				}
				else
				{
					std::cout << "Error! Invalid syntax!\n";
				}
			}
			else
			{
				std::cout << "Error! Invalid syntax!\n";
			}
			i += 2;
			break;
		case 14:
		case 15:
		case 16:
		default:
			std::cout << "Error, invalid arguments!\n";
		}
	}
}

void ByteCompiler::printBytes()
{
	std::cout << "Length: " << compiledBytes.length() << '\n';
	
	for (auto& x : compiledBytes)
	{
		std::cout << (unsigned int)x << '\n';
	}
}


void ByteCompiler::printCode()
{
	for (auto x : code)
	{
		std::cout << x << '\n';
	}
}
