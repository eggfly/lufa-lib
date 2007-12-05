/*
             MyUSB Library
     Copyright (C) Dean Camera, 2007.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com

 Released under the GPL Licence, Version 3
*/

#include "Dataflash.h"

void Dataflash_Init(void)
{
	PINB  |= (1 << 0);
	DDRB  |= ((1 << 1) | (1 << 2));
	DDRE  |= DATAFLASH_CHIPCS_MASK;
	PORTE |= DATAFLASH_CHIPCS_MASK;

	SPCR  = ((1 << SPE) | (1 << MSTR) | (1 << CPOL) | (1 << CPHA));
}

void Dataflash_WaitWhileBusy(void)
{
	Dataflash_SendByte(DF_CMD_GETSTATUS);
	
	while (!(Dataflash_SendByte(0x00) & DF_STATUS_READY));
}

uint8_t Dataflash_SendByte(const uint8_t Byte)
{
	SPDR = Byte;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}

bool Dataflash_SelectChipFromPage(const uint16_t PageAddress)
{
	if (PageAddress < DATAFLASH_PAGE_SIZE)
	{
		Dataflash_SelectChip(DATAFLASH_CHIP1);
		return true;
	}
	else if (PageAddress < (DATAFLASH_PAGE_SIZE * 2))
	{
		Dataflash_SelectChip(DATAFLASH_CHIP2);
		return true;
	}
	else
	{
		Dataflash_SelectChip(DATAFLASH_NO_CHIP);
		return false;
	}
}

void Dataflash_SendAddressBytes(const uint16_t PageAddress, const uint16_t BufferByte)
{
	Dataflash_SendByte(PageAddress >> 5);
	Dataflash_SendByte((PageAddress << 3) | (BufferByte >> 8));
	Dataflash_SendByte(BufferByte);
}

void Dataflash_ToggleSelectedChipCS(void)
{
	uint8_t SelectedChipMask = Dataflash_GetSelectedChip();
	
	Dataflash_SelectChip(DATAFLASH_NO_CHIP);
	Dataflash_SelectChip(SelectedChipMask);
}
