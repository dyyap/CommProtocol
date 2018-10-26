#include <iostream>
#include <fstream>
#include <string>
#include "ProtoPackets.pb.h"
#include <WS2tcpip.h>
#include <string.h>
#include <string>

// instructions need PUTTY 
// ssh into 127.0.0.1 with port 6969


// This is a glorified mess

#pragma comment (lib, "ws2_32.lib")

using namespace std;
using namespace google::protobuf::io;

void InitializeWinsock() {

	// Initialize winsock
	WSAData wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) { // simple network error 
		cerr << "Can't Initialize winsock! Quitting" << endl;

	}

}
void ListStates(const ProtoPackets::AirVehicleGroundRelativeState & proto_packets) {

	cout << "uhhhhhh" << proto_packets.angleofattack();

}

void main() {

	InitializeWinsock();

	// Create a socket
	// socket is an endpoint; number ; in unix it is a file descriptor

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0); // socket (ip address family, TCP socket, flag for off)
	if (listening == INVALID_SOCKET) { // if socket doesn't work quit
		cerr << "Can't creat a socket! Quitting" << endl;
		return;
	}

	// Bind the socket to an ip adress and port or vice versa
	sockaddr_in hint; // hint data structure https://patents.google.com/patent/WO2017078732A1/en
	hint.sin_family = AF_INET; // IP4
	hint.sin_port = htons(6969);  // host to network short
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // could also use inet_pton

	//bind to socket
	bind(listening, (sockaddr*)&hint, sizeof(hint));



	// tell winsock the socket is for listening
	listen(listening, SOMAXCONN); //marks it for listening


	// wait for connection
	sockaddr_in client; // make a client
	int clientsieze = sizeof(client); // get the size of client

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsieze);
	if (clientSocket == INVALID_SOCKET) { // if socket doesn't work quit
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	char host[NI_MAXHOST]; // client's remote name , could use DNS lookup, we'll find out
	char service[NI_MAXHOST]; // Service( i.e port) the client is connect on

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);
	//memset(host, 0, NI_MAXHOST); //portable

	//lookup the host name of the client we are working with
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		//DNS lookup
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port " <<
			ntohs(client.sin_port) << endl;
	}
	// close listening socket
	closesocket(listening);

	
	
	//close the socket
	closesocket(clientSocket);
	//Shutdown winsock
	WSACleanup();

}

void readBody(int csock, google::protobuf::uint32 siz, comnet::Comms& comm)
{
	int bytecount;
	ProtoPackets::Packet payload;
	//log_packet payload;
	char* buffer = new char[siz + 4];//size of the payload and hdr
						 //Read the entire buffer including the hdr
	memset(buffer, '\0', 4);
	//if ((bytecount = recv(csock, buffer, siz + 4, MSG_WAITALL)) == -1) {
	if ((bytecount = recv(csock, buffer, siz + 4, MSG_PUSH_IMMEDIATE)) == -1) {
		fprintf(stderr, "Error receiving data %d\n", errno);
	}
	cout << "Second read byte count is " << bytecount << endl;
	//Assign ArrayInputStream with enough memory 
	google::protobuf::io::ArrayInputStream ais(buffer, siz + 4);
	CodedInputStream coded_input(&ais);
	//Read an unsigned integer with Varint encoding, truncating to 32 bits.
	coded_input.ReadVarint32(&siz);
	//After the message's length is read, PushLimit() is used to prevent the CodedInputStream 
	//from reading beyond that length.Limits are used when parsing length-delimited 
	//embedded messages
	google::protobuf::io::CodedInputStream::Limit msgLimit = coded_input.PushLimit(siz);
	//De-Serialize
	payload.ParseFromCodedStream(&coded_input);
	//Once the embedded message has been parsed, PopLimit() is called to undo the limit
	coded_input.PopLimit(msgLimit);
	//Print the message
	cout << "Message is " << payload.DebugString();

	ProtoPackets::Packet::PacketCase cases = payload.packet_case();
	std::cout << "packet case is: " << payload.packet_case();
	switch (cases)
	{
	case 1: //AirVehicleGroundRelativeState = 1,
	{
		ProtoPackets::AirVehicleGroundRelativeState p = payload.airvehiclegroundrelativestate();
		ngcp::AirVehicleGroundRelativeState pkt(p.vehicleid(), p.angleofattack(), p.angleofsideslip(), p.trueairspeed(),
			p.indicatedairspeed(), p.northwindspeed(), p.eastwindspeed(), p.northgroundspeed(),
			p.eastgroundspeed(), p.barometricpressure(), p.barometricaltitude());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 2: //ArmCommand = 2,
	{
		ProtoPackets::ArmCommand p = payload.armcommand();
		ngcp::ArmCommand pkt(p.id(), p.position());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 3: //ArmPosition = 3,
	{
		ProtoPackets::ArmPosition p = payload.armposition();
		ngcp::ArmPosition pkt(p.position1(), p.position2(), p.position3(), p.position4());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 4://Battery = 4,
	{
		ProtoPackets::Battery p = payload.battery();
		ngcp::Battery pkt(p.batterypercentage());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 5://TargetAcknowledgement = 5,
	{
		ProtoPackets::TargetAcknowledgement p = payload.targetacknowledgement();
		ngcp::TargetAcknowledgement pkt(p.targetstatus());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 6://TargetDesignationCommand = 6,
	{
		ProtoPackets::TargetDesignationCommand p = payload.targetdesignationcommand();
		ngcp::TargetDesignationCommand pkt(p.vehicleid(), p.payloadid(), p.targetid(), p.targettype(),
			p.longitude(), p.latitude(), p.altitude());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 7://TargetStatus = 7,
	{
		ProtoPackets::TargetStatus p = payload.targetstatus();
		ngcp::TargetStatus pkt(p.targetradius(), p.targetangle(), p.targetaltitude());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 8://VehicleAttitude = 8,
	{
		ProtoPackets::VehicleAttitude p = payload.vehicleattitude();
		ngcp::VehicleAttitude pkt(p.vehicleid(), p.roll(), p.pitch(), p.yaw());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 9://VehicleAuthorizationReply = 9,
	{
		ProtoPackets::VehicleAuthorizationReply p = payload.vehicleauthorizationreply();
		ngcp::VehicleAuthorizationReply pkt(p.vehicleid(), p.vehicletype(), p.authorizedservices(),
			p.grantedservices());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 10://VehicleAuthorizationRequest = 10,
	{
		ProtoPackets::VehicleAuthorizationRequest p = payload.vehicleauthorizationrequest();
		ngcp::VehicleAuthorizationRequest pkt(p.vehicleid(), p.vehicletype(),
			p.authorizedservices(), p.grantedservices());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 11://VehicleBodySensedState = 11,
	{
		ProtoPackets::VehicleBodySensedState p = payload.vehiclebodysensedstate();
		ngcp::VehicleBodySensedState pkt(p.vehicleid(), p.xaccel(), p.yaccel(), p.zaccel(),
			p.rollrate(), p.pitchrate(), p.yawrate());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 12://VehicleGlobalPosition = 12,
	{
		ProtoPackets::VehicleGlobalPosition p = payload.vehicleglobalposition();
		ngcp::VehicleGlobalPosition pkt(p.vehicleid(), p.longitude(), p.latitude(), p.altitude(),
			p.xspeed(), p.yspeed(), p.zspeed());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 13://VehicleIdentification = 13,
	{
		ProtoPackets::VehicleIdentification p = payload.vehicleidentification();
		ngcp::VehicleIdentification pkt(p.vehicleid(), p.vehicletype());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 14://VehicleInertialState = 14,
	{
		ProtoPackets::VehicleInertialState p = payload.vehicleinertialstate();
		ngcp::VehicleInertialState pkt(p.vehicleid(), p.longitude(), p.latitude(), p.altitude(),
			p.roll(), p.pitch(), p.heading(), p.northspeed(), p.eastspeed(),
			p.verticalspeed(), p.rollrate(), p.pitchrate(), p.yawrate(),
			p.northaccel(), p.eastaccel(), p.verticalaccel());

		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 15://VehicleModeCommand = 15,
	{
		ProtoPackets::VehicleModeCommand p = payload.vehiclemodecommand();
		ngcp::VehicleModeCommand pkt(p.vehicleid(), p.vehiclemode());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 16://VehicleSystemStatus = 16,
	{
		ProtoPackets::VehicleSystemStatus p = payload.vehiclesystemstatus();
		ngcp::VehicleSystemStatus pkt(p.vehicleid(), p.vehiclemode(), p.vehiclestate());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 17://VehicleTelemetryCommand = 17,
	{
		ProtoPackets::VehicleTelemetryCommand p = payload.vehicletelemetrycommand();
		ngcp::VehicleTelemetryCommand pkt(p.vehicleid(), p.telemetryselect(), p.telemetryrate());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 18://VehicleTerminationCommand = 18,
	{
		ProtoPackets::VehicleTerminationCommand p = payload.vehicleterminationcommand();
		ngcp::VehicleTerminationCommand pkt(p.vehicleid(), p.terminationmode());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	case 19://VehicleWaypointCommand = 19,
	{
		ProtoPackets::VehicleWaypointCommand p = payload.vehiclewaypointcommand();
		ngcp::VehicleWaypointCommand pkt(p.vehicleid(), p.longitude(), p.latitude(), p.altitude());
		comm.Send(pkt, DEST_PLATFORM_ID);
		break;
	}
	default: // case == 0, means no case set, something is wrong.
		break;
	}

	delete buffer;

}






