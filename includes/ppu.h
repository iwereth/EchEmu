#ifndef PPU_H
#define PPU_H
#include <stdint.h>
#include <stdbool.h>
#include "memory.h"
/*
PPUCTRL		$2000	VPHB SINN	NMI enable (V), PPU master/slave (P), sprite height (H), background tile select (B), sprite tile select (S), increment mode (I), nametable select (NN)
PPUMASK		$2001	BGRs bMmG	color emphasis (BGR), sprite enable (s), background enable (b), sprite left column enable (M), background left column enable (m), greyscale (G)
PPUSTATUS	$2002	VSO- ----	vblank (V), sprite 0 hit (S), sprite overflow (O); read resets write pair for $2005/$2006
OAMADDR		$2003	aaaa aaaa	OAM read/write address
OAMDATA		$2004	dddd dddd	OAM data read/write
PPUSCROLL	$2005	xxxx xxxx	fine scroll position (two writes: X scroll, Y scroll)
PPUADDR		$2006	aaaa aaaa	PPU read/write address (two writes: most significant byte, least significant byte)
PPUDATA		$2007	dddd dddd	PPU data read/write
OAMDMA		$4014	aaaa aaaa	OAM DMA high address
*/

typedef struct{
	uint8_t ypos;
	uint8_t index;
	uint8_t attr;
	uint8_t xpos;
}oamtype;

typedef struct {
	struct {
		uint8_t PPUCTRL;
		uint8_t PPUMASK;
		uint8_t PPUSTATUS;
		uint8_t OAMADDR;
		uint8_t OAMDATA;
		uint8_t PPUSCROLL;
		uint8_t PPUADDR;
		uint8_t PPUDATA;
	}registers;
	bool oddframe;
	int scanline;
	int tick;
	bool drawflag;
	bool toggle;
	bool innmi;
	uint8_t readbuffer;
	uint8_t*  palette;
	uint8_t* patterntable;
	uint8_t* nametable;
	uint32_t* gfx;
	uint8_t finex,tempfinex;
	uint16_t vramaddr,tempvramaddr;
	uint8_t curr_tile[8];
	long long int framecount;
	oamtype *OAMdata;
	uint8_t* OAMpointer;
	oamtype *currOAM;
	memmap ppumap[6];
}ppu;

/*********************PPUCTRL MACROS************************/
#define BASENAMETABLE		(PPU->registers.PPUCTRL & 0x3)
#define CTRLNMI				(PPU->registers.PPUCTRL & (1<<7))
#define CHRSIZE				(PPU->registers.PPUCTRL & (1<<5))
#define BGPATTERN			(PPU->registers.PPUCTRL & (1<<4))
#define CHRPATTERN			(PPU->registers.PPUCTRL & (1<<3)) 
#define INCMODE 			(PPU->registers.PPUCTRL & (1<<2))

/*******************PPUMASK MACROS**************************/
#define RED_EMPHASIZE		(PPU->registers.PPUMASK & (1<<5))
#define BLUE_EMPHASIZE		(PPU->registers.PPUMASK & (1<<7))
#define GREEN_EMPHASIZE		(PPU->registers.PPUMASK & (1<<6))
#define SHOWCHR				(PPU->registers.PPUMASK & (1<<4)) 
#define SHOWBG				(PPU->registers.PPUMASK & (1<<3))
#define SHOWLFCHR			(PPU->registers.PPUMASK & (1<<2))
#define SHOWLFBG			(PPU->registers.PPUMASK & (1<<1))
#define SHOWGREY			(PPU->registers.PPUMASK & 0x1)
#define RENDER				((SHOWBG || SHOWCHR) && (PPU->registers.PPUSTATUS & (1<<7)))

/*******************PPUSTATUS MACROS*************************/
#define VBLANK 				(PPU->registers.PPUSTATUS & (1<<7))
#define CHR0HIT				(PPU->registers.PPUSTATUS & (1<<6))
#define CHROVERFLOW			(PPU->registers.PPUSTATUS & (1<<5))


#define OAMADDRESS 			(PPU->registers.OAMADDR)
#define OAMDAT 				(PPU->registers.OAMDATA)
#define SCROLL 				(PPU->registers.PPUSCROLL)
#define ADDRESS 			(PPU->registers.PPUADDR)
#define DAT					(PPU->registers.PPUDATA)

#define IFCHANGEREG 		(PPU->registers.FLAG) 

#define VRAMADDR 			PPU->vramaddr
#define PALETTE(num)		ppu_readb(PPU->ppumap, 0x3F00 | palent)

#define FINEX 				PPU->finex

void ppu_init(ppu *PPU);
void ppu_run(ppu* PPU);
void ppu_reset(ppu* PPU);

uint8_t ppu_read_reg(memmap* map, uint16_t addr);
void ppu_write_reg(memmap* map, uint16_t addr, uint8_t byte);
#endif