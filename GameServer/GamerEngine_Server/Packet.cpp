#include "Packet.h"


void Packet::Clear() 
{
	buffer.clear();
	extractionOffset = 0;
}

void Packet::Append(const void* data, uint32_t size) 
{
	buffer.insert(buffer.end(), (char*)data, (char*)data + size);
}

Packet& Packet::operator<<(uint32_t data) 
{
	data = htonl(data);
	Append(&data, sizeof(uint32_t));
	return *this;
}

Packet& Packet::operator>>(uint32_t & data)
{
	data = *reinterpret_cast<uint32_t*>(&buffer[extractionOffset]);
	data = ntohl(data);
	extractionOffset += sizeof(uint32_t);
	return *this;
}