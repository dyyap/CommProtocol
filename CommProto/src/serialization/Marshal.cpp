#include <CommProto/serialization/Marshal.h>
#include <stdlib.h>
#include <algorithm>



namespace Comnet {
namespace Serialization {

	template <typename T>
	void swap_endian(T& pX)
	{
		// should static assert that T is a POD

		char& raw = reinterpret_cast<char&>(pX);
		std::reverse(&raw, &raw + sizeof(T));
	}

	template <typename T>
	T swap_endian_copy(T pX)
	{
		swap_endian(pX);
		return pX;
	}

uint32_t packString(string_t data, int16_t len, marshall_t input) 
{
	
	return NULL;
}

string_t unpackString(marshall_t input, int16_t len)
{
	return NULL;
}

uint32_t packByte(uint8_t data, marshall_t input) 
{
	return NULL;
}

uint32_t packUint16(uint16_t data, marshall_t input)
{
	swap_endian(data);
	memcpy(input, &data, sizeof(uint32_t));
	return sizeof(uint32_t);
}

uint16_t unpackUint16(marshall_t input)
{
	uint16_t value;
	memcpy(&value, input, 2);
	swap_endian(value); 
	return value;
}

uint32_t packInt16(int16_t data, marshall_t input)
{
	return NULL;
}

int16_t unpackInt16(marshall_t input)
{
	return NULL;
}



} // Serialization namespace
} // Comnet namespace
