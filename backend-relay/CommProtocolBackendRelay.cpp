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
void xbeeTest()
{
	//const char* destMac = "0013A20040A54318";
	const char* destMac = "0013A2004105C6AF";

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
		<< comm1.InitConnection(ZIGBEE_LINK, "COM8", "", 57600)
		<< std::endl;
	std::cout << "Connected to address: "
		<< std::boolalpha
		<< comm1.AddAddress(UGV_DESTID, destMac)
		<< std::endl;


	//comm1.LinkCallback(new ngcp::ArmCommand(), new comnet::Callback((comnet::callback_t)ArmCommandCallback));
	//comm1.LinkCallback(new ngcp::ArmPosition(1,1,1,1), new comnet::Callback((comnet::callback_t)ArmPositionCallback));


	//comm1.LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback));

	// Test packet. 

	// NOTE(All): Be sure to run the nodes! If not, the threads won't execute!
	comm1.Run();

	//// Loop. To exit, Click the red button on the top left (Windows Visual Studio) OR 
	//// CNTRL+C (Linux). 
	//while (true) {
	//	std::cout << "Sleeping..." << std::endl;
	//	// comm1 will be sending the packet.
	//	std::string word;
	//	std::cout << "enter message:";
	//	std::cin >> word;
	//	Ping message(word);
	//	ArmCommand amc(22, 1337);
	//	ArmPosition amp(7, 6, 5, 4);
	//	comm1.Send(amp, 2);
	//	comm1.Send(amc, 2);
	//	comm1.Send(message, 2);
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	//}
	//std::cin.ignore();
	while(true){ 
	std:string word;
		std::cin >> word;
		ngcp::ArmCommand amc(22, 1337);
		comm1.Send(amc, 2);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
	}
}

//test two xbees on same machine.
void localTest(int& hsocket)
{
    const uint8_t com1ID = 1;
    const uint8_t com2ID = 2;
    const uint32_t baudRate = 57600;//57600;

    // test date
    std::cout << "Test: 11/17/2017" << std::endl;
    //Disables Pinging to make reading output easier
    comnet::constate::ConnectionStateManager::ConStateEnabled = false;

    std::condition_variable cond;
    std::cout << sizeof(comnet::Header) << std::endl;
    // CommNode 1
    comnet::Comms comm1(com1ID);
    comm1.LoadKey("01234567890ABCDEF");
    // CommNode 2
    comnet::Comms comm2(com2ID);
    comm2.LoadKey("01234567890ABCDEF");
    comnet::architecture::os::CommMutex mut;
    comnet::architecture::os::CommLock commlock(mut);
    // This will cause the thread to wait for a few milliseconds, causing any other thread to wait.
    comnet::architecture::os::WaitForMilliseconds(commlock, cond, 1000);

    std::cout << "Test complete!" << std::endl;

    // ComNode 1 init and add Connection.
    std::cout << "Init connection succeeded: "
              << std::boolalpha
              << comm2.InitConnection(TCP_LINK, "1338", "127.0.0.1")
              << std::endl;
    std::cout << "Connected to address: "
              << std::boolalpha
              << comm2.AddAddress(1, "127.0.0.1", 1337)
              << std::endl;
    // CommNode 2 init and add Connection.
    std::cout << "Init connection succeeded: "
              << std::boolalpha
              << comm1.InitConnection(TCP_LINK, "1337", "127.0.0.1")
              << std::endl;
    std::cout << "Connected to address: "
              << std::boolalpha
              << comm1.AddAddress(2, "127.0.0.1", 1338)
              << std::endl;

    // Test packet.
    ngcp::ArmCommand amc(22, 7777);
	ngcp::ArmPosition amp(11, 22,33,44);
    // NOTE(All): Be sure to run the nodes! If not, the threads won't execute!
    comm1.Run();
    comm2.Run();

    // Loop. To exit, Click the red button on the top left (Windows Visual Studio) OR
    // CNTRL+C (Linux).
    while (true)
    {
        std::cout << "Sleeping..." << std::endl;
        // comm1 will be sending the packet.
        comm2.Send(amc, com1ID);
		comm2.Send(amp, com1ID);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        ++amc.id;
        ++amc.position;
		++amp.position1;
		++amp.position2;
		++amp.position3;
		++amp.position4;

    }
    std::cin.ignore();
}

int main()
{
	const char* destMac = "0013A2004105C6AF";
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
		<< comm1.AddAddress(2, destMac)
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
				ProtoPackets::AirVehicleGroundRelativeState p = payload.airvehiclegroundrelativestate;
				ngcp::AirVehicleGroundRelativeState pkt(p.vehicleid, p.angleofattack, p.angleofsideslip, p.trueairspeed,
					p.indicatedairspeed, p.northwindspeed, p.eastwindspeed, p.clear_northgroundspeed,
					p.eastgroundspeed, p.barometricpressure, p.barometricaltitude);
				comm.Send(pkt, UGV_DESTID);
				break;
			}
	case 2: //ArmCommand = 2,
		break;
	case 3: //ArmPosition = 3,
		break;
	case 4://Battery = 4,
		break;
	case 5://TargetAcknowledgement = 5,
		break;
	case 6://TargetDesignationCommand = 6,
		break;
	case 7://TargetStatus = 7,
		break;
	case 8://VehicleAttitude = 8,
		break;
	case 9://VehicleAuthorizationReply = 9,
		break;
	case 10://VehicleAuthorizationRequest = 10,
		break;
	case 11://VehicleBodySensedState = 11,
		break;
	case 12://VehicleGlobalPosition = 12,
		break;
	case 13://VehicleIdentification = 13,
		break;
	case 14://VehicleInertialState = 14,
	{
		ProtoPackets::VehicleInertialState p = payload.vehicleinertialstate;
		ngcp::VehicleInertialState pkt(p.vehicleid,p.longitude,p.latitude,p.altitude,
										p.roll,p.pitch,p.heading,p.northspeed,p.eastspeed,
										p.verticalspeed,p.rollrate,p.pitchrate,p.yawrate,
										p.northaccel,p.eastaccel,p.verticalaccel);
		
		comm.Send(pkt, UGV_DESTID);
		break;
	}
	case 15://VehicleModeCommand = 15,
		break;
	case 16://VehicleSystemStatus = 16,
		break;
	case 17://VehicleTelemetryCommand = 17,
		break;
	case 18://VehicleTerminationCommand = 18,
		break;
	case 19://VehicleWaypointCommand = 19,
		break;
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

