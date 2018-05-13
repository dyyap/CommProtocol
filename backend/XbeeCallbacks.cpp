#include "XbeeCallbacks.h"

error_t ArmCommandCallback(const comnet::Header& header, const ngcp::ArmCommand& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;
	std::cout << "Message: " << std::endl;
	std::cout << "Packet contains: ";
	std::cout << "ID: " << packet.id << endl;
	std::cout << "Pos: " << packet.position << endl;
	//packet.print();
	ProtoPackets::ArmCommand *payload = new ProtoPackets::ArmCommand();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_id(packet.id);
	payload->set_position(packet.position);

	masterPayload.set_allocated_armcommand(payload);

	int pktSize = masterPayload.ByteSize() + 4;
	if (sendPacket(hsock, pkt, pktSize, masterPayload) == false) //,coded_output) == false)
	{
		delete pkt;
		closesocket(hsock);
		WSACleanup();
		return -1;
	}
	delete pkt;
	return comnet::CALLBACK_SUCCESS | comnet::CALLBACK_DESTROY_PACKET;
}


error_t ArmPositionCallback(const comnet::Header& header, const ngcp::ArmPosition& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;
	std::cout << "Message: " << std::endl;
	std::cout << "Packet contains: ";
	std::cout << "Pos1: " << packet.position1 << endl;
	std::cout << "Pos2: " << packet.position2 << endl;
	std::cout << "Pos3: " << packet.position3 << endl;
	std::cout << "Pos4: " << packet.position4 << endl;

	//packet.print();
	ProtoPackets::ArmPosition *payload = new ProtoPackets::ArmPosition();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_position1(packet.position1);
	payload->set_position2(packet.position2);
	payload->set_position3(packet.position3);
	payload->set_position4(packet.position4);


	masterPayload.set_allocated_armposition(payload);

	int pktSize = masterPayload.ByteSize() + 4;
	if (sendPacket(hsock, pkt, pktSize, masterPayload) == false) //,coded_output) == false)
	{
		delete pkt;
		closesocket(hsock);
		WSACleanup();
		return -1;
	}
	delete pkt;
	return comnet::CALLBACK_SUCCESS | comnet::CALLBACK_DESTROY_PACKET;
}

error_t VehicleGlobalPositionCallback(const comnet::Header& header, const ngcp::VehicleGlobalPosition& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::VehicleGlobalPosition *payload = new ProtoPackets::VehicleGlobalPosition();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_vehicleid(packet.vehicle_id);
	payload->set_altitude(packet.altitude);
	payload->set_latitude(packet.latitude);
	payload->set_longitude(packet.longitude);
	payload->set_xspeed(packet.x_speed);
	payload->set_yspeed(packet.y_speed);
	payload->set_zspeed(packet.z_speed);

	masterPayload.set_allocated_vehicleglobalposition(payload);

	int pktSize = masterPayload.ByteSize() + 4;
	if (sendPacket(hsock, pkt, pktSize, masterPayload) == false) //,coded_output) == false)
	{
		delete pkt;
		closesocket(hsock);
		WSACleanup();
		return -1;
	}
	delete pkt;
	return comnet::CALLBACK_SUCCESS | comnet::CALLBACK_DESTROY_PACKET;
}


error_t VehicleInertialStateCallback(const comnet::Header& header, const ngcp::VehicleInertialState& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::VehicleInertialState *payload = new ProtoPackets::VehicleInertialState();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_vehicleid(packet.vehicle_id);
	payload->set_altitude(packet.altitude);
	payload->set_latitude(packet.latitude);
	payload->set_longitude(packet.longitude);
	payload->set_roll(packet.roll);
	payload->set_pitch(packet.pitch);
	payload->set_heading(packet.heading);
	payload->set_northspeed(packet.north_speed);
	payload->set_eastspeed(packet.east_speed);
	payload->set_verticalspeed(packet.vertical_speed);
	payload->set_rollrate(packet.roll_rate);
	payload->set_pitchrate(packet.pitch_rate);
	payload->set_yawrate(packet.yaw_rate);
	payload->set_northaccel(packet.north_accel);
	payload->set_eastaccel(packet.east_accel);
	payload->set_verticalaccel(packet.vertical_accel);

	masterPayload.set_allocated_vehicleinertialstate(payload);

	int pktSize = masterPayload.ByteSize() + 4;
	if (sendPacket(hsock, pkt, pktSize, masterPayload) == false) //,coded_output) == false)
	{
		delete pkt;
		closesocket(hsock);
		WSACleanup();
		return -1;
	}
	delete pkt;
	return comnet::CALLBACK_SUCCESS | comnet::CALLBACK_DESTROY_PACKET;
}