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
#include "XbeeCallbacks.h"


using namespace google::protobuf::io;
using namespace std;

bool initSocket(int& hsocket, char* host_name, int host_port);
bool sendPacket(int& socket, char* packet, int pktSize, ProtoPackets::Packet& payload);// , CodedOutputStream *coded_output);
void linkCallbacks(comnet::Comms& comm1);
int hsock;

// Test against an xbee on another machine.
void singlexbeeTest()
{
	//const char* destMac = "0013A20040A54318";
	const char* destMac = "0013A2004105C6AF";
	const char* portName = "COM8";
	// test date
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
		<< comm1.InitConnection(ZIGBEE_LINK, portName, "", 57600)
		<< std::endl;
	std::cout << "Connected to address: "
		<< std::boolalpha
		<< comm1.AddAddress(2, destMac)
		<< std::endl;

	// link to callback function.
	linkCallbacks(comm1);


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

void doublexbeeTest()
{
	//const char* destMac = "0013A20040A54318";
	const char* UGVMAC = "0013A2004105C6AF";
	const char* GCSMAC = "0013A2004105C6AF";
	const char* GCSPortName = "COM8";
	const char* UGVPortName = "COM9";
	const int GCSID = 1;
	const int UGVID = 2;
	// test date
	//Disables Pinging to make reading output easier
	comnet::constate::ConnectionStateManager::ConStateEnabled = false;
	std::condition_variable cond;
	//std::cout << sizeof(comnet::Header) << std::endl;
	// CommNode 1
	comnet::Comms GCSComm(GCSID);
	comnet::Comms UGVComm(UGVID);
	//comm1.LoadKey("01234567890ABCDEF");

	comnet::architecture::os::CommMutex mut;
	comnet::architecture::os::CommLock commlock(mut);
	// This will cause the thread to wait for a few milliseconds, causing any other thread to wait.
	comnet::architecture::os::WaitForMilliseconds(commlock, cond, 1000);

	std::cout << "Test complete!" << std::endl;
	// CommNode 1 init and add Connection.
	std::cout << "Init connection succeeded: "
		<< std::boolalpha
		<< GCSComm.InitConnection(ZIGBEE_LINK, GCSPortName, "", 57600)
		<< std::endl;
	std::cout << "Connected to address: "
		<< std::boolalpha
		<< GCSComm.AddAddress(UGVID, UGVMAC)
		<< std::endl;

	// CommNode 1 init and add Connection.
	std::cout << "Init connection succeeded: "
		<< std::boolalpha
		<< UGVComm.InitConnection(ZIGBEE_LINK, UGVPortName, "", 57600)
		<< std::endl;
	std::cout << "Connected to address: "
		<< std::boolalpha
		<< UGVComm.AddAddress(GCSID, GCSMAC)
		<< std::endl;

	// link to callback function.
	linkCallbacks(GCSComm);
	linkCallbacks(UGVComm);



	// Test packet. 

	// NOTE(All): Be sure to run the nodes! If not, the threads won't execute!
	GCSComm.Run();
	UGVComm.Run();

	//// Loop. To exit, Click the red button on the top left (Windows Visual Studio) OR 
	//// CNTRL+C (Linux). 
	int id = 77;
	int pos = 1000;
	int pos1 = 20;
	int pos2 = 30;
	int pos3 = 40;
	int pos4 = 50;
	while (true) {
		std::cout << "Sleeping..." << std::endl;
		// comm1 will be sending the packet.
		std::string word;
		std::cout << "enter message:";
		std::cin >> word;
		ngcp::ArmCommand amc(id, pos);
		ngcp::ArmPosition amp(pos1, pos2, pos3, pos4);
		UGVComm.Send(amp, GCSID);
		UGVComm.Send(amc, GCSID);
		//UGVComm.Send(message, 2);
		id++;
		pos++;
		pos1++;
		pos2++;
		pos3++;
		pos4++;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}
	//std::cin.ignore();
	//while (true) {
	//std:string word;
	//	std::cin >> word;
	//	ngcp::ArmCommand amc(22, 1337);
	//	comm1.Send(amc, 2);
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//}
}
//test two xbees on same machine.
void localTcpTest(int& hsocket)
{

    const uint8_t com1ID = 1;
    const uint8_t com2ID = 2;

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

	// Link callbacks 
	linkCallbacks(comm1);


    // Test packet.
    ngcp::ArmCommand amc(22, 7777);
	ngcp::ArmPosition amp(11, 22,33,44);
	ngcp::VehicleGlobalPosition vgp(777,100.1, 200.2, 300.3, 400.4, 500.5, 600.6);
	ngcp::VehicleInertialState vis(888,1000.1, 2000.2, 3000.3, 4000.4, 5000.5, 6000.6, 7000.7, 8000.8, 9000.9,10000.10, 11000.11, 12000.12,
		13000.13, 14000.14, 15000.15);
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
		comm2.Send(vgp, com1ID);
		comm2.Send(vis, com1ID);
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

    /* Coded output stram */

    int defaultSize = 1024;
    //ProtoPackets::ArmCommand payload;
    int host_port = 6969;
    char* host_name = "127.0.0.1";
    //cout << "size after serilizing is " << payload.ByteSize() << endl;
    //int siz = payload.ByteSize() + 4;
    char *pkt = new char[defaultSize];

    //google::protobuf::io::ArrayOutputStream aos(pkt, siz);
    //CodedOutputStream *coded_output = new CodedOutputStream(&aos);
    //coded_output->WriteVarint32(payload.ByteSize());
    //payload.SerializeToCodedStream(coded_output);

    //char buffer[1024];
    //int bytecount;
    //int buffer_len = 0;
    if (initSocket(hsock, host_name, host_port) == false)
    {
        delete pkt;
        closesocket(hsock);
        WSACleanup();
        return -1;
    }

    localTcpTest(hsock);
	//xbeeTest();
	//doublexbeeTest();
    delete pkt;
    closesocket(hsock);
    WSACleanup();
    cin.get();
    return 0;
}
void linkCallbacks(comnet::Comms& comm1)
{
	// link to callback function.
	comm1.LinkCallback(new ngcp::AirVehicleGroundRelativeState(), new comnet::Callback((comnet::callback_t)AirVehicleGroundRelativeStateCallback));
	comm1.LinkCallback(new ngcp::ArmCommand(), new comnet::Callback((comnet::callback_t)ArmCommandCallback));
	comm1.LinkCallback(new ngcp::ArmPosition(1, 1, 1, 1), new comnet::Callback((comnet::callback_t)ArmPositionCallback));
	comm1.LinkCallback(new ngcp::Battery(), new comnet::Callback((comnet::callback_t)BatteryCallback));
	comm1.LinkCallback(new ngcp::TargetAcknowledgement(), new comnet::Callback((comnet::callback_t)TargetAcknowledgementCallback));
	comm1.LinkCallback(new ngcp::TargetDesignationCommand(), new comnet::Callback((comnet::callback_t)TargetDesignationCommandCallback));
	comm1.LinkCallback(new ngcp::TargetStatus(), new comnet::Callback((comnet::callback_t)TargetStatusCallback));
	comm1.LinkCallback(new ngcp::VehicleAttitude(), new comnet::Callback((comnet::callback_t)VehicleAttitudeCallback));
	comm1.LinkCallback(new ngcp::VehicleAuthorizationReply(), new comnet::Callback((comnet::callback_t)VehicleAuthorizationReplyCallback));
	comm1.LinkCallback(new ngcp::VehicleAuthorizationRequest(), new comnet::Callback((comnet::callback_t)VehicleAuthorizationRequestCallback));
	comm1.LinkCallback(new ngcp::VehicleBodySensedState(), new comnet::Callback((comnet::callback_t)VehicleBodySensedStateCallback));
	comm1.LinkCallback(new ngcp::VehicleGlobalPosition(), new comnet::Callback((comnet::callback_t)VehicleGlobalPositionCallback));
	comm1.LinkCallback(new ngcp::VehicleIdentification(), new comnet::Callback((comnet::callback_t)VehicleIdentificationCallback));
	comm1.LinkCallback(new ngcp::VehicleInertialState(), new comnet::Callback((comnet::callback_t)VehicleInertialStateCallback));
	comm1.LinkCallback(new ngcp::VehicleModeCommand(), new comnet::Callback((comnet::callback_t)VehicleModeCommandCallback));
	comm1.LinkCallback(new ngcp::VehicleSystemStatus(), new comnet::Callback((comnet::callback_t)VehicleSystemStatusCallback));
	comm1.LinkCallback(new ngcp::VehicleTelemetryCommand(), new comnet::Callback((comnet::callback_t)VehicleTelemetryCommandCallback));
	comm1.LinkCallback(new ngcp::VehicleTerminationCommand(), new comnet::Callback((comnet::callback_t)VehicleTerminationCommandCallback));
	comm1.LinkCallback(new ngcp::VehicleWaypointCommand(), new comnet::Callback((comnet::callback_t)VehicleWaypointCommandCallback));
}

bool initSocket(int& hsocket, char* host_name, int host_port)
{
    struct sockaddr_in my_addr;
    int *p_int;
    int err;

    WSADATA Data;
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &Data) != 0)
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
    if (connect(hsocket, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1)
    {
        if ((err = errno) != EINPROGRESS)
        {
            fprintf(stderr, "Error connecting socket %d\n", errno);
            return false;
        }
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

