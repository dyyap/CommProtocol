#include <CommProto/network/Crc32.h>



namespace Comnet {
namespace Network {
/** CRC32 checksum function*/
unsigned int crc32(unsigned char *message, int length) {
	int i, j;
	unsigned int byte, crc, mask;
	static unsigned int table[256];

	/* Set up the table, if necessary. */

	if (table[1] == 0) {
		for (byte = 0; byte <= 255; byte++) {
			crc = byte;
			for (j = 7; j >= 0; j--) {    // Do eight times.
				mask = -(crc & 1);
				crc = (crc >> 1) ^ (0xEDB88320 & mask);
			}
			table[byte] = crc;
		}
	}

	/* Through with table setup, now calculate the CRC. */
	i = 0;
	crc = 0xFFFFFFFF;
	while (length--) {
		byte = message[i];
		crc = (crc >> 8) ^ table[(crc ^ byte) & 0xFF];
		i = i + 1;
	}
	return ~crc;
}


void appendCrc32(uint8_t* buffer, uint32_t *length){
			
	unsigned int crc = crc32(buffer,*length);//calculate crc32
	unsigned char a = (crc >> 24) & 0xff;//leftmost
	unsigned char b = (crc >> 16) & 0xff;//next byte
	unsigned char c = (crc >> 8) & 0xff;//next byte
	unsigned char d = (crc)& 0xff;//right most

	#ifdef LITTLE_ENDIAN_COMNET
	//swap outter two
	unsigned char e = a;
	a = d;
	d = a;
	//swap middle two
	e = b;
	b = c;
	c = b;
	#endif

	//add crc32
	buffer[(*length)++] = a;
	buffer[(*length)++] = b;
	buffer[(*length)++] = c;
	buffer[(*length)++] = d;
			
}

unsigned int truncateCrc32(uint8_t* buffer, uint32_t *length){
	
	
	unsigned char a = buffer[--(*length)];
	unsigned char b = buffer[--(*length)];
	unsigned char c = buffer[--(*length)];
	unsigned char d = buffer[--(*length)];

	#ifdef LITTLE_ENDIAN_COMNET
	//swap outter two
	unsigned char e = a;
	a = d;
	d = a;
	//swap middle two
	e = b;
	b = c;
	c = b;
	#endif

	//store bytes into crcRecv
	unsigned int crcRecv = 0;
	((char*)&crcRecv)[0] = a;
	((char*)&crcRecv)[1] = b;
	((char*)&crcRecv)[2] = c;
	((char*)&crcRecv)[3] = d;
	
	//return results
	return crcRecv;
	
}
}//end NETWORK namespace
}//end COMNET namespace
