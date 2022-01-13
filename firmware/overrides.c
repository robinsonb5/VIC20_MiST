#include "statusword.h"
#include "minfat.h"

/* Override the autoboot function to load VIC20.ROM, attempt to load MEGACART.ROM
   and attempt to mount MEGACART.NV */
extern unsigned char romtype;
int LoadROM(const char *fn);
int loadimage(char *filename,int unit);

extern fileTYPE file;

static int rom,mc,mcnv;

static void load()
{
	// Attempt to load ROM, Megacart ROM and mount Megacart NV...
	romtype=0;
	if(!rom)
		rom=LoadROM("VIC20   ROM");
	if(!mcnv)
		mcnv=loadimage("MEGACARTNV ",'1');
	romtype=2;
	if(!mc)
		mc=LoadROM("MEGACARTROM");
}

char *autoboot()
{
	char *result=0;
	rom=0; mc=0; mcnv=0;
	load();

	// Now attempt to change directory to "CHAM20"
	if(FileOpen(&file,"CHAM20     "))
		ChangeDirectoryByCluster(file.cluster);

	// And have another go at loading anything that failed first time...
	load();

	ChangeDirectoryByCluster(0);

	if(mc)
		statusword|=(1<<15);	// Enable Megacart if the ROM loaded successfully.
	sendstatus();

	if(!rom)
		result="ROM failed";
	return(result);
}

