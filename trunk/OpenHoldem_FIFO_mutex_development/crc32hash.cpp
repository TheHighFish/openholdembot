// crc32hash.c - derived from linux/lib/crc32.c, GNU GPL v2 
// Found at http://www.kernel.org/doc/Documentation/pcmcia/crc32hash.c

#include "stdafx.h"
#include "crc32hash.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

unsigned int crc32(unsigned char const *p)
{
	int i;
	unsigned int crc = 0;
	unsigned int len = strlen((char const *) p); 
	while (len--) {
		crc ^= *p++;
		for (i = 0; i < 8; i++)
			crc = (crc >> 1) ^ ((crc & 1) ? 0xedb88320 : 0);
	}
	return crc;
}

