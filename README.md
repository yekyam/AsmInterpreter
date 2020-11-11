# AsmInterpreter
A simple assembly interpreter to help people with understanding the language without the hassle of setup.
This is NOT a fully fledged CPU emulator! This is a learning tool for people to use. The purpose of this tool
is to get more comfortable with assembly syntax, and see the immediate results of operations. All the 
registers are 8 bits long, which is a good length to see how the bits change without being too limiting.

**Registers:** *AL, BL, CL*  

Commands:  
**mov   (to reg), (from reg)**  
**mov   (to reg), (value)**       *:Moves either a value, or a value stored in a register to the specified register*  
**or    (to reg), (from reg)**    *:Performs the bitwise OR operation, with the result stored in the first register*  
**and   (to reg), (from reg)**    *:Performs the bitwise AND operation, with the result stored in the first register*  
**xor   (to reg), (from reg)**    *:Performs the bitwise XOR operation, with the result stored in the first register*  
**cmp   (to reg), (from reg)**    *:Compares two registers by subtraction. If the result is 0, the equal flag is enabled*  
**push  (reg)**        *:Pushes either a value, or a register's value to the stack*  
**pop   (reg)**        *:Pops the most recent stack variable on to the specified register*  
**add   (to reg),**    *:Adds the two specified registers together, with the result stored in the first register*  
**sub   (to reg),**    *:Subtracts the two registers, with the result stored in the first register*  
**mul   (to reg),**    *:Multiplies the two specified registers together, with the result stored in the first register*  
**div   (to reg),**    *:Divides the two specified registers together, with the result stored in the first register* 

## Example Command  
These are all valid examples syntactically

**mov AL, 5**  
**MOV AL  5**  
**mov AL  5**  

The only things that must be capatalized are the register names. Other than that, comamnds can be capitalized, commas seperating the arguments are optional, and you can pass in a variable number of arguments, so no need to pass NULL for every blank argument. 
