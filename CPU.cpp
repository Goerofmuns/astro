#include "CPU.h"

#include<iostream>

const short OPCODE_MASK  = 0b111000000000;
const short INDIR_MASK   = 0b000100000000;
const short ZERO_MASK    = 0b000010000000;
const short OPERAND_MASK = 0b000001111111;

const short TWELVE_BIT_MASK  = 0b111111111111;
const short PC_HIGH_BIT_MASK = 0b111000000000;

void CPU::init()
{
    //Bleep bloop
    ACC = 0x000;
    X   = 0x000;
    Y   = 0x000;
    Z   = 0x000;
    I   = 0x00;
    J   = 0x00;
    K   = 0x00;
    PC  = 0x000;
    SP  = 0xf00;
    FLG = 0x000;
    for(int i = 0; i < 4095; i++)
    {
	//It gets weird if you don't
	ram[i] = 0x000;
    }
}

void CPU::setMemory(short address, short data)
{
    std::cout << "SETTING: " << std::bitset<16>(data) << "\n";
    if(address > 4095)
    {
	//Logic for Memory mapped I/O here
	puts("MMIO Write Call");
    }
    else
    {
	ram[address] = data;
    }
    std::cout << "FRESULT: " << std::bitset<16>(getMemory(address)) << "\n";
    std::cout << "DRESULT: " << std::bitset<16>(ram[address]) << "\n";
}

byte CPU::getMemory(short address)
{
    if(address > 4095)
    {
	//Logic for MMIO here	

	puts("MMIO Read Call");
        return 0x00; // dummy logic best logic
    }
    else
    {
	return ram[address];
    }
}

void CPU::cycle()
{
    short curWord = getMemory(PC);

    byte opCode       = (OPCODE_MASK  && curWord) >> 9;
    bool indirect_bit = (INDIR_MASK   && curWord) >> 8;
    bool zero_bit     = (ZERO_MASK    && curWord) >> 7;
    byte arg          = (OPERAND_MASK && curWord);

    if(indirect_bit)
    {
	if(zero_bit)
	{
	    //create new address from arg and zeroes for the 12 extra bits
	    arg = getMemory((short) arg);
	}
	else
	{
	    //create new address from arg and 5 higher order bits of PC
	    arg = getMemory((PC_HIGH_BIT_MASK && PC) + arg);
	}
    }

    std::cout << "=CPU State=\n"
	      << "PC is: " << std::bitset<12>(PC) << "\n"
	      << "curword is: " << std::bitset<16>(curWord) << "\n"
	      << "opCode is: " << std::bitset<3>(opCode) << "\n"
	      << "indirect bit is: " << indirect_bit << "\n"
	      << "zero bit is: " << zero_bit << "\n"
	      << std::endl;

    switch(opCode)
    {
	//AND: bitwise and arg with ACC
    case 0:
	ACC &= arg;
	PC += 1;
	break;

	//JMP: unconditional jump to arg
    case 1:
	//if(
	PC = arg;
	break;
	
	//Don't know what to do here
    default:
	//Lalalalalalala
	break;
	  }

    //Don't overflow past 12 bits
    if(PC > 4095)
    {
        PC = 0x00;
    }
}
