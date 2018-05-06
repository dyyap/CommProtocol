#pragma once
#pragma once
#include <CommProto/commproto.h>

class ArmCommandXbee : INHERITS_ABSPACKET
{
	/**
	Creates an instance
	*/
public:
	ArmCommandXbee(uint8_t id = 0,
		int32_t position = 0)
		: CHAIN_ABSPACKET(ArmCommandXbee),
		id(id),
		position(position)
	{
	}
	/**
	Pack data into the stream for sending out.
	*/
	void Pack(comnet::ObjectStream &obj) override
	{
		obj << id;
		obj << position;
	}


	/**
	Unpack data back into this packet when receiving data.
	*/
	void Unpack(comnet::ObjectStream &obj) override
	{
		obj >> position;
		obj >> id;
	}
	/**
	Tells CommProtocol how to recreate the ArmCommand packet
	when receiving data.
	*/
	comnet::AbstractPacket *Create() override
	{
		return new ArmCommandXbee();
	}
	const void print() const
	{
		std::cout << "ID: " << id << "\n Position:"
			<< position << "\n";
	}

	/**
	Data.
	*/
	uint8_t id;
	int32_t position;
};