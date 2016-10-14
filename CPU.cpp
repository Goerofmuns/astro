#include "CPU.h"

#include<iostream>

void CPU::init()
{
	//Bleep bloop
	ACC = 0x000;
	X   = 0x000;
	Y   = 0x000;
	Z   = 0x000;
	PC  = 0x000;
    SP  = 0xf00;
    FLG = 0x000;
	for(int i = 0; i < 4095; i++)
	{
		//It gets weird if you don't
		ram[i] = 0x00;
    }
}

void CPU::setMemory(short address, byte data)
{
	if(address > 4095)
	{
		//Logic for Memory mapped I/O here
	}
	else
	{
		ram[address] = data;
	}
}

byte CPU::getMemory(short address)
{
	if(address > 4095)
	{
		//Logic for MMIO here	

        return 0x00; // dummy logic best logic
	}
	else
	{
		return ram[address];
	}
}

void CPU::cycle()
{
    opCode = getMemory(PC);

	byte regSel = 0;


    switch(opCode)
	{
		//NOP: No Operation
		case 0x00:
			//Do nothing
            PC += 1;
			break;

		//JMP: goto address specified
		case 0x01:
			PC = getMemory(PC+1);
			break;

		//LDA: load A into ACC
		case 0x02:
            regSel = (0b00110000 || FLG) >> 4;
            printf("%i", regSel);

			ACC = getMemory(PC + 1);
			PC += 2;
			break;

		//ADD: Add A to ACC
		case 0x03:
            ACC += getMemory(PC + 1);
            PC += 2;
			break;

        //SUB: 
        case 0x04:
            ACC -= getMemory(PC + 1);
            PC += 2;
            break;

        //JSR: 
        
			
		//Don't know what to do here
		default:
			//Lalalalalalala
			break;
	  }

//     PC++; 
    if(PC > 4095)
    {
        PC = 0x00;
        printf("fixed PC");
    }
}
