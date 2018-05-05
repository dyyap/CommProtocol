//#include "stdafx.h"
#include "UGVMessage.pb.h"
#include <WinSock2.h>
#include <iostream>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#pragma comment(lib, "Ws2_32.lib")
#include <CommProto/commproto.h>
using namespace google::protobuf::io;
using namespace std;
using namespace UGV;


bool initSocket(int& hsocket, char* host_name, int host_port);
bool sendPacket(int& socket, char* packet, int pktSize, ArmCommand& payload);// , CodedOutputStream *coded_output);
int hsock;

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
        std::cout << "VAL: ID: " << id << "\n Position:"
                  << position << "\n";
    }

    /**
    Data.
    */
    uint8_t id;
    int32_t position;
};

error_t ArmCommandCallback(const comnet::Header& header, const ArmCommandXbee& packet, comnet::Comms& node)
{
    std::cout << "=::RECEIVED PACKET::=" << std::endl;
    std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;
    std::cout << "Message: " << std::endl;
    std::cout << "Packet contains: ";
    packet.print();
    ArmCommand payload;
    char *pkt = new char[1024];
    payload.set_id(packet.id);
    payload.set_position(packet.position);
    int pktSize = payload.ByteSize() + 4;
    if (sendPacket(hsock, pkt, pktSize, payload)==false) //,coded_output) == false)
    {
        delete pkt;
        closesocket(hsock);
        WSACleanup();
        return -1;
    }
    return comnet::CALLBACK_SUCCESS | comnet::CALLBACK_DESTROY_PACKET;
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
              << comm2.AddAddress(1, "127.0.0.1", 6969)
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

    comm1.LinkCallback(new ArmCommandXbee(), new comnet::Callback((comnet::callback_t)ArmCommandCallback));

    // Test packet.
    ArmCommandXbee amc(22, 1337);
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
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        ++amc.id;
        ++amc.position;
    }
    std::cin.ignore();
}

int main()
{

    /* Coded output stram */

    int defaultSize = 1024;
    ArmCommand payload;
    int host_port = 6969;
    char* host_name = "127.0.0.1";

    payload.set_id(777);
    payload.set_position(1010);

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

    localTest(hsock);

    /*for (int i = 0; i<10000; i++) {
    	for (int j = 0; j<10; j++) {

    		if ((bytecount = send(hsock, pkt, siz, 0)) == -1) {
    			fprintf(stderr, "Error sending data %d\n", errno);
    			delete pkt;
    			closesocket(hsock);
    			WSACleanup();
    			return -1;
    		}
    		printf("Sent bytes %d\n", bytecount);
    		Sleep(1);
    	}
    }*/

    //int id = 0;
    //int pos = 1111;
    //for (int i = 0; i < 100000; ++i,++id,++pos)
    //{
    //	payload.set_id(id);
    //	payload.set_position(pos);
    //	int pktSize = payload.ByteSize() + 4;
    //	cout << "count " << i + 1 << endl;
    //	if (sendPacket(hsock, pkt, pktSize, payload)==false) //,coded_output) == false)
    //	{
    //		delete pkt;
    //		closesocket(hsock);
    //		WSACleanup();
    //		return -1;
    //	}
    //	Sleep(50);
    //}


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

bool sendPacket(int& socket, char* packet, int pktSize, ArmCommand& payload)//, CodedOutputStream * unused_coded_output)
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

