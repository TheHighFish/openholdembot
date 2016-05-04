//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: StringCRC to guarantee a constant mutex-name for all instances
//
//******************************************************************************
//
// Header file created for crc32hash.c
// which has been found on the net without a header.
// Original source of crc32hash.c released under GPLv2.
//
// crc32hash.c - derived from linux/lib/crc32.c, GNU GPL v2 
// Found at http://www.kernel.org/doc/Documentation/pcmcia/crc32hash.c
//
// Slightly modified by THF to avoid passing the length as a parameter.
//
//******************************************************************************

unsigned int crc32(unsigned char const *p);


/* Usage example:

int main(int argc, char **argv) {
	unsigned int result;
	if (argc != 2) {
		printf("no string passed as argument\n");
		return -1;
	}
	result = crc32((unsigned char const *)argv[1]);
	printf("0x%x\n", result);
	return 0;
}
*/
