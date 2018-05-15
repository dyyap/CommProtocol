//#include "stdafx.h"
#include <ProtoPackets\ProtoPacketsV2.pb.h>
#include <WinSock2.h>
#include <iostream>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#pragma comment(lib, "Ws2_32.lib")
#include <CommProto/commproto.h>

#include <XbeePackets\AirVehicleGroundRelativeState.hpp>
#include <XbeePackets\ArmCommand.hpp>
#include <XbeePackets\ArmPosition.hpp>
#include <XbeePackets\Battery.hpp>
#include <XbeePackets\TargetAcknowledgement.hpp>
#include <XbeePackets\TargetDesignationCommand.hpp>
#include <XbeePackets\TargetStatus.hpp>
#include <XbeePackets\VehicleAttitude.hpp>
#include <XbeePackets\VehicleAuthorizationReply.hpp>
#include <XbeePackets\VehicleAuthorizationRequest.hpp>
#include <XbeePackets\VehicleBodySensedState.hpp>
#include <XbeePackets\VehicleGlobalPosition.hpp>
#include <XbeePackets\VehicleIdentification.hpp>
#include <XbeePackets\VehicleInertialState.hpp>
#include <XbeePackets\VehicleModeCommand.hpp>
#include <XbeePackets\VehicleSystemStatus.hpp>
#include <XbeePackets\VehicleTelemetryCommand.hpp>
#include <XbeePackets\VehicleTerminationCommand.hpp>
#include <XbeePackets\VehicleWaypointCommand.hpp>

using namespace google::protobuf::io;
using namespace std;

void* SocketHandler(void* lp, comnet::Comms& );
bool initSocket(int& hsocket, char* host_name, int host_port);
bool sendPacket(int& socket, char* packet, int pktSize, ProtoPackets::Packet& payload);// , CodedOutputStream *coded_output);
int hsock;
int UGV_DESTID = 2;
// Test against an xbee on another machine.

int main(int argc, const char * argv[])
{

	std::cout << "argc:" << argc << std::endl;
	std::cout << "argv:"  << std::endl;

	for (int i = 0; i < argc; ++i)
	{
		std::cout << argv[i] << std::endl;

	}
	const char* destMac = "0013A2004105C6AA";
	const char* port = "COM9";
	// test date
	std::cout << "Test: 5/11/2018" << std::endl;
	//Disables Pinging to make reading output easier
	comnet::constate::ConnectionStateManager::ConStateEnabled = false;

	std::condition_variable cond;
	std::cout << sizeof(comnet::Header) << std::endl;
	// CommNode 1
	comnet::Comms comm1(1);
	//comm1.LoadKey("01234567890ABCDEF");

	comnet::architecture::os::CommMutex mut;
	comnet::architecture::os::CommLock commlock(mut);
	// This will cause the thread to wait for a few milliseconds, causing any other thread to wait.
	comnet::architecture::os::WaitForMilliseconds(commlock, cond, 1000);

	std::cout << "Test complete!" << std::endl;
	// CommNode 1 init and add Connection.
	std::cout << "Init connection succeeded: "
		<< std::boolalpha
		<< comm1.InitConnection(ZIGBEE_LINK, port, "", 57600)
		<< std::endl;
	std::cout << "Connected to address: "
		<< std::boolalpha
		<< comm1.AddAddress(UGV_DESTID, destMac)
		<< std::endl;
	// NOTE(All): Be sure to run the nodes! If not, the threads won't execute!
	comm1.Run();
    /* Coded output stram */
    int defaultSize = 1024;
    //ProtoPackets::ArmCommand payload;
    int host_port = 7000;
    char* host_name = "127.0.0.1";
    char *pkt = new char[defaultSize];

    if (initSocket(hsock, host_name, host_port) == false)
    {
        delete pkt;
        closesocket(hsock);
        WSACleanup();
        return -1;
    }
	if (listen(hsock, 10) == -1) {
		fprintf(stderr, "Error listening %d\n", errno);
		return -1;
	}
	sockaddr_in sadr;
	int addr_size = sizeof(sockaddr_in);
	int *csock = 0;
	while (true) {
		printf("\nwaiting for a connection\n");
		csock = (int*)malloc(sizeof(int));
		if ((*csock = accept(hsock, (sockaddr*)&sadr, &addr_size)) != -1) {
			printf("---------------------\nReceived connection from %s\n", inet_ntoa(sadr.sin_addr));
			SocketHandler((void*)csock, comm1);
		}
		else {
			fprintf(stderr, "Error accepting %d\n", errno);
		}
	}
    delete pkt;
//FINISH:
    closesocket(hsock);
    WSACleanup();
    cin.get();
    return 0;
}


bool initSocket(int& hsocket, char* host_name, int host_port)
{

    struct sockaddr_in my_addr;
    int *p_int;
    int err;
	int result;
    WSADATA Data;
    printf("\nInitialising Winsock...");
    if ((result = WSAStartup(MAKEWORD(2, 2), &Data)) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    hsocket = socket(AF_INET, SOCK_STREAM, 0);
    if (hsocket == -1)
    {
        printf("Error initializing socket %d\n", errno);
        return false;
    }

    p_int = (int*)malloc(sizeof(int));
    *p_int = 1;

    if ((setsockopt(hsocket, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1) ||
            (setsockopt(hsocket, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1))
    {
        printf("Error setting options %d\n", errno);
        free(p_int);
        return false;
    }
    free(p_int);

    my_addr.sin_addr.s_addr = inet_addr(host_name);
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(host_port);

    memset(&(my_addr.sin_zero), 0, 8);
    //if (connect(hsocket, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1)
    //{
    //    if ((err = errno) != EINPROGRESS)
    //    {
    //        fprintf(stderr, "Error connecting socket %d\n", errno);
    //        return false;
    //    }
    //}

	if (::bind(hsock, (SOCKADDR *)&my_addr, sizeof(my_addr)) == SOCKET_ERROR) {
		fprintf(stderr, "Error binding to socket, make sure nothing else is listening on this port %d\n", errno);
		return false;
	}
    return true;
}

bool sendPacket(int& socket, char* packet, int pktSize, ProtoPackets::Packet& payload)//, CodedOutputStream * unused_coded_output)
{

    google::protobuf::io::ArrayOutputStream aos(packet, pktSize);
    CodedOutputStream *coded_output = new CodedOutputStream(&aos);
    coded_output->WriteVarint32(payload.ByteSize());
    payload.SerializeToCodedStream(coded_output);

    int bytecount = 0;
    if ((bytecount = send(socket, packet, pktSize, 0)) == -1)
    {
        fprintf(stderr, "Error sending data %d\n", errno);
        return false;
    }

    return true;
}

google::protobuf::uint32 readHdr(char *buf)
{
	google::protobuf::uint32 size;
	google::protobuf::io::ArrayInputStream ais(buf, 4);
	CodedInputStream coded_input(&ais);
	coded_input.ReadVarint32(&size);//Decode the HDR and get the size
	cout << "size of payload is " << size << endl;
	return size;
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
	
	ProtoPackets::Packet::PacketCase cases =  payload.packet_case();
	std::cout << "packet case is: " << payload.packet_case();
	switch (cases)
	{
	case 1: //AirVehicleGroundRelativeState = 1,
			{
				ProtoPackets::AirVehicleGroundRelativeState p = payload.airvehiclegroundrelativestate();
				ngcp::AirVehicleGroundRelativeState pkt(p.vehicleid(), p.angleofattack(), p.angleofsideslip(), p.trueairspeed(),
					p.indicatedairspeed(), p.northwindspeed(), p.eastwindspeed(), p.northgroundspeed(),
					p.eastgroundspeed(), p.barometricpressure(), p.barometricaltitude());
				comm.Send(pkt, UGV_DESTID);
				break;
			}
	case 2: //ArmCommand = 2,
	{
		ProtoPackets::ArmCommand p = payload.armcommand();
		ngcp::ArmCommand pkt(p.id(),p.position());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 3: //ArmPosition = 3,
	{
		ProtoPackets::ArmPosition p = payload.armposition();
		ngcp::ArmPosition pkt(p.position1(),p.position2(),p.position3(),p.position4());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 4://Battery = 4,
	{
		ProtoPackets::Battery p = payload.battery();
		ngcp::Battery pkt(p.batterypercentage());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 5://TargetAcknowledgement = 5,
	{
		ProtoPackets::TargetAcknowledgement p = payload.targetacknowledgement();
		ngcp::TargetAcknowledgement pkt(p.targetstatus());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 6://TargetDesignationCommand = 6,
	{
		ProtoPackets::TargetDesignationCommand p = payload.targetdesignationcommand();
		ngcp::TargetDesignationCommand pkt(p.vehicleid(),p.payloadid(),p.targetid(),p.targettype(),
											p.longitude(),p.latitude(),p.altitude());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 7://TargetStatus = 7,
	{
		ProtoPackets::TargetStatus p = payload.targetstatus();
		ngcp::TargetStatus pkt(p.targetradius(),p.targetangle(),p.targetaltitude());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 8://VehicleAttitude = 8,
	{
		ProtoPackets::VehicleAttitude p = payload.vehicleattitude();
		ngcp::VehicleAttitude pkt(p.vehicleid(),p.roll(),p.pitch(),p.yaw());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 9://VehicleAuthorizationReply = 9,
	{
		ProtoPackets::VehicleAuthorizationReply p = payload.vehicleauthorizationreply();
		ngcp::VehicleAuthorizationReply pkt(p.vehicleid(),p.vehicletype(),p.authorizedservices(),
											p.grantedservices());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 10://VehicleAuthorizationRequest = 10,
	{
		ProtoPackets::VehicleAuthorizationRequest p = payload.vehicleauthorizationrequest();
		ngcp::VehicleAuthorizationRequest pkt(p.vehicleid(),p.vehicletype(),
											p.authorizedservices(),p.grantedservices());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 11://VehicleBodySensedState = 11,
	{
		ProtoPackets::VehicleBodySensedState p = payload.vehiclebodysensedstate();
		ngcp::VehicleBodySensedState pkt(p.vehicleid(),p.xaccel(),p.yaccel(),p.zaccel(),
										p.rollrate(),p.pitchrate(),p.yawrate());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 12://VehicleGlobalPosition = 12,
	{
		ProtoPackets::VehicleGlobalPosition p = payload.vehicleglobalposition();
		ngcp::VehicleGlobalPosition pkt(p.vehicleid(), p.longitude(), p.latitude(), p.altitude(),
			p.xspeed(), p.yspeed(), p.zspeed());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 13://VehicleIdentification = 13,
	{
		ProtoPackets::VehicleIdentification p = payload.vehicleidentification();
		ngcp::VehicleIdentification pkt(p.vehicleid(), p.vehicletype());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 14://VehicleInertialState = 14,
	{
		ProtoPackets::VehicleInertialState p = payload.vehicleinertialstate();
		ngcp::VehicleInertialState pkt(p.vehicleid(),p.longitude(),p.latitude(),p.altitude(),
										p.roll(),p.pitch(),p.heading(),p.northspeed(),p.eastspeed(),
										p.verticalspeed(),p.rollrate(),p.pitchrate(),p.yawrate(),
										p.northaccel(),p.eastaccel(),p.verticalaccel());
		
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 15://VehicleModeCommand = 15,
	{
		ProtoPackets::VehicleModeCommand p = payload.vehiclemodecommand();
		ngcp::VehicleModeCommand pkt(p.vehicleid(), p.vehiclemode());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 16://VehicleSystemStatus = 16,
	{
		ProtoPackets::VehicleSystemStatus p = payload.vehiclesystemstatus();
		ngcp::VehicleSystemStatus pkt(p.vehicleid(), p.vehiclemode(), p.vehiclestate());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 17://VehicleTelemetryCommand = 17,
	{
		ProtoPackets::VehicleTelemetryCommand p = payload.vehicletelemetrycommand();
		ngcp::VehicleTelemetryCommand pkt(p.vehicleid(),p.telemetryselect(),p.telemetryrate());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 18://VehicleTerminationCommand = 18,
	{
		ProtoPackets::VehicleTerminationCommand p = payload.vehicleterminationcommand();
		ngcp::VehicleTerminationCommand pkt(p.vehicleid(),p.terminationmode());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 19://VehicleWaypointCommand = 19,
	{
		ProtoPackets::VehicleWaypointCommand p = payload.vehiclewaypointcommand();
		ngcp::VehicleWaypointCommand pkt(p.vehicleid(), p.longitude(), p.latitude(), p.altitude());
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	default: // case == 0, means no case set, something is wrong.
		break;
	}

	delete buffer;

}

void* SocketHandler(void* lp, comnet::Comms& comm) {
	int *csock = (int*)lp;

	char buffer[4];
	int bytecount = 0;
	string output, pl;
	//log_packet logp;

	memset(buffer, '\0', 4);

	//while (true) {
		//Peek into the socket and get the packet size
		if ((bytecount = recv(*csock,
			buffer,
			4, MSG_PEEK)) == -1) {
			fprintf(stderr, "Error receiving data %d\n", errno);
		}
		//else if (bytecount == 0)
			//break;
		cout << "First read byte count is " << bytecount << endl;
		readBody(*csock, readHdr(buffer),comm);
	//}
	//free(csock);
	return 0;
}

