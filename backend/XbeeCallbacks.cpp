#include "XbeeCallbacks.h"


// Note: no need to delete payload pointer at the end, Protobuf automatically deletes it.

error_t AirVehicleGroundRelativeStateCallback(const comnet::Header& header, const ngcp::AirVehicleGroundRelativeState& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::AirVehicleGroundRelativeState *payload = new ProtoPackets::AirVehicleGroundRelativeState();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_vehicleid(packet.vehicle_id);
	payload->set_angleofattack(packet.angle_of_attack);
	payload->set_angleofsideslip(packet.angle_of_sideslip);
	payload->set_trueairspeed(packet.true_airspeed);
	payload->set_indicatedairspeed(packet.indicated_airspeed);
	payload->set_northwindspeed(packet.north_wind_speed);
	payload->set_eastwindspeed(packet.east_wind_speed);
	payload->set_northgroundspeed(packet.north_ground_speed);
	payload->set_eastgroundspeed(packet.east_ground_speed);
	payload->set_barometricpressure(packet.barometric_pressure);
	payload->set_barometricaltitude(packet.barometric_altitude);

	masterPayload.set_allocated_airvehiclegroundrelativestate(payload);

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
	//std::cout << "Message: " << std::endl;
	//std::cout << "Packet contains: ";
	//std::cout << "Pos1: " << packet.position1 << endl;
	//std::cout << "Pos2: " << packet.position2 << endl;
	//std::cout << "Pos3: " << packet.position3 << endl;
	//std::cout << "Pos4: " << packet.position4 << endl;

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

error_t BatteryCallback(const comnet::Header& header, const ngcp::Battery& packet, comnet::Comms& node) {
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::Battery *payload = new ProtoPackets::Battery();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_batterypercentage(packet.batteryPercentage);

	masterPayload.set_allocated_battery(payload);

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

error_t TargetAcknowledgementCallback(const comnet::Header& header, const ngcp::TargetAcknowledgement& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::TargetAcknowledgement *payload = new ProtoPackets::TargetAcknowledgement();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_targetstatus(packet.target_status);

	masterPayload.set_allocated_targetacknowledgement(payload);

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

error_t TargetDesignationCommandCallback(const comnet::Header& header, const ngcp::TargetDesignationCommand& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::TargetDesignationCommand *payload = new ProtoPackets::TargetDesignationCommand();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_vehicleid(packet.vehicle_id);
	payload->set_payloadid(packet.payload_id);
	payload->set_targetid(packet.target_id);
	payload->set_targettype(packet.target_type);
	payload->set_longitude(packet.longitude);
	payload->set_latitude(packet.latitude);
	payload->set_altitude(packet.altitude);

	masterPayload.set_allocated_targetdesignationcommand(payload);

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

error_t TargetStatusCallback(const comnet::Header& header, const ngcp::TargetStatus& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::TargetStatus *payload = new ProtoPackets::TargetStatus();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];

	payload->set_targetradius(packet.target_radius);
	payload->set_targetangle(packet.target_angle);
	payload->set_targetaltitude(packet.target_altitude);

	masterPayload.set_allocated_targetstatus(payload);

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

error_t VehicleAttitudeCallback(const comnet::Header& header, const ngcp::VehicleAttitude& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::VehicleAttitude *payload = new ProtoPackets::VehicleAttitude();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];

	payload->set_vehicleid(packet.vehicle_id);
	payload->set_roll(packet.roll);
	payload->set_pitch(packet.pitch);
	payload->set_yaw(packet.yaw);

	masterPayload.set_allocated_vehicleattitude(payload);

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

error_t VehicleAuthorizationReplyCallback(const comnet::Header& header, const ngcp::VehicleAuthorizationReply& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::VehicleAuthorizationReply *payload = new ProtoPackets::VehicleAuthorizationReply();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];

	payload->set_vehicleid(packet.vehicle_id);
	payload->set_vehicletype(packet.vehicle_type);
	payload->set_authorizedservices(packet.authorized_services);
	payload->set_grantedservices(packet.granted_services);

	masterPayload.set_allocated_vehicleauthorizationreply(payload);

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

error_t VehicleAuthorizationRequestCallback(const comnet::Header& header, const ngcp::VehicleAuthorizationRequest& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::VehicleAuthorizationRequest *payload = new ProtoPackets::VehicleAuthorizationRequest();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];

	payload->set_vehicleid(packet.vehicle_id);
	payload->set_vehicletype(packet.vehicle_type);
	payload->set_authorizedservices(packet.authorized_services);
	payload->set_grantedservices(packet.granted_services);

	masterPayload.set_allocated_vehicleauthorizationrequest(payload);

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

error_t VehicleBodySensedStateCallback(const comnet::Header& header, const ngcp::VehicleBodySensedState& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::VehicleBodySensedState *payload = new ProtoPackets::VehicleBodySensedState();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];

	payload->set_vehicleid(packet.vehicle_id);
	payload->set_xaccel(packet.x_accel);
	payload->set_yaccel(packet.y_accel);
	payload->set_zaccel(packet.z_accel);
	payload->set_rollrate(packet.roll_rate);
	payload->set_pitchrate(packet.pitch_rate);
	payload->set_yawrate(packet.yaw_rate);

	masterPayload.set_allocated_vehiclebodysensedstate(payload);

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


error_t VehicleIdentificationCallback(const comnet::Header& header, const ngcp::VehicleIdentification& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::VehicleIdentification *payload = new ProtoPackets::VehicleIdentification();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_vehicleid(packet.vehicle_id);
	payload->set_vehicletype(packet.vehicle_type);

	masterPayload.set_allocated_vehicleidentification(payload);

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

error_t VehicleModeCommandCallback(const comnet::Header& header, const ngcp::VehicleModeCommand& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::VehicleModeCommand *payload = new ProtoPackets::VehicleModeCommand();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_vehicleid(packet.vehicle_id);
	payload->set_vehiclemode(packet.vehicle_mode);

	masterPayload.set_allocated_vehiclemodecommand(payload);

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

error_t VehicleSystemStatusCallback(const comnet::Header& header, const ngcp::VehicleSystemStatus& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::VehicleSystemStatus *payload = new ProtoPackets::VehicleSystemStatus();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_vehicleid(packet.vehicle_id);
	payload->set_vehiclemode(packet.vehicle_mode);
	payload->set_vehiclestate(packet.vehicle_state);

	masterPayload.set_allocated_vehiclesystemstatus(payload);

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

error_t VehicleTelemetryCommandCallback(const comnet::Header& header, const ngcp::VehicleTelemetryCommand& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::VehicleTelemetryCommand *payload = new ProtoPackets::VehicleTelemetryCommand();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_vehicleid(packet.vehicle_id);
	payload->set_telemetryselect(packet.telemetry_select);
	payload->set_telemetryrate(packet.telemetry_rate);

	masterPayload.set_allocated_vehicletelemetrycommand(payload);

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

error_t VehicleTerminationCommandCallback(const comnet::Header& header, const ngcp::VehicleTerminationCommand& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::VehicleTerminationCommand *payload = new ProtoPackets::VehicleTerminationCommand();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_vehicleid(packet.vehicle_id);
	payload->set_terminationmode(packet.termination_mode);

	masterPayload.set_allocated_vehicleterminationcommand(payload);

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

error_t VehicleWaypointCommandallback(const comnet::Header& header, const ngcp::VehicleWaypointCommand& packet, comnet::Comms& node)
{
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;

	//packet.print();
	ProtoPackets::VehicleWaypointCommand *payload = new ProtoPackets::VehicleWaypointCommand();
	ProtoPackets::Packet masterPayload;
	char *pkt = new char[1024];
	payload->set_vehicleid(packet.vehicle_id);
	payload->set_longitude(packet.longitude);
	payload->set_latitude(packet.latitude);
	payload->set_altitude(packet.altitude);

	masterPayload.set_allocated_vehiclewaypointcommand(payload);

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