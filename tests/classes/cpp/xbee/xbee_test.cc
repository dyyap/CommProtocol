#include <CommProto/architecture/os/comm_mutex.h>
#include <CommProto/comms.h>
#include <CommProto/callback.h>
#include <CommProto/architecture/os/comm_thread.h>
#include <iostream>

/*
#Note 
CommProtocol Library requires libxbee3 to execute copy the file to your bin folder (ie .exe)
-Windows {build dir}\libxbee3\(debug or release)\libxbee3.dll
-Linux: {build dir}\libxbee3\libxbee3.o
*/

class Cat {
};
// Ping testing.
class Ping : INHERITS_ABSPACKET {
public:
  Ping() : CHAIN_ABSPACKET(Ping) { }
  Ping(std::string cc)
    : CHAIN_ABSPACKET(Ping)
    , cat(cc) { }

  void Pack(REF_OBJECTSTREAM obj) {
    obj << cat;  // add cat
  }

  void Unpack(REF_OBJECTSTREAM obj) {
    obj >> cat;
  }

  ABSPACKET* Create() {
    return new Ping();
  }

  const std::string& GetCat() const { return cat; }
private:

  std::string cat;
};
class ArmCommand : INHERITS_ABSPACKET {
	/**
	Creates an instance
	*/
public:
	ArmCommand(uint8_t id = 0,
		int32_t position = 0)
		: CHAIN_ABSPACKET(ArmCommand),
		id(id),
		position(position)
	{
	}



	/**
	Pack data into the stream for sending out.
	*/
	void Pack(comnet::ObjectStream &obj) override {
		obj << id;
		obj << position;
	}


	/**
	Unpack data back into this packet when receiving data.
	*/
	void Unpack(comnet::ObjectStream &obj) override {
		obj >> position;
		obj >> id;
	}


	/**
	Tells CommProtocol how to recreate the ArmCommand packet
	when receiving data.
	*/
	comnet::AbstractPacket *Create() override {
		return new ArmCommand();
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
class ArmPosition : INHERITS_ABSPACKET {
	/**
	Creates an instance
	*/
public:
	ArmPosition(int32_t position1 = 1,
		int32_t position2=2,
		int32_t position3=3,
		int32_t position4=4)
		: CHAIN_ABSPACKET(ArmPosition),
		position1(position1),
		position2(position2),
		position3(position3),
		position4(position4)
	{
	}


	/**
	Pack data into the stream for sending out.
	*/
	void Pack(comnet::ObjectStream &obj) override {
		obj << position1;
		obj << position2;
		obj << position3;
		obj << position4;
	}


	/**
	Unpack data back into this packet when receiving data.
	*/
	void Unpack(comnet::ObjectStream &obj) override {
		obj >> position4;
		obj >> position3;
		obj >> position2;
		obj >> position1;
	}

	const void print() const
	{
		std::cout << "VAL: p1: " << position1 << "\n p2:" 
			<< position2 << "\n p3:" << position3 << "\n p4:" << position4 << "\n";
	}
	/**
	Tells CommProtocol how to recreate the ArmPosition packet
	when receiving data.
	*/
	comnet::AbstractPacket *Create() override {
		return new ArmPosition(0, 0, 0, 0);
	}

	/**
	Data.
	*/
	int32_t position1;
	int32_t position2;
	int32_t position3;
	int32_t position4;
};

// Callback function that we will be using to link to Ping packet.
error_t PingCallback(const comnet::Header& header, const Ping& packet, comnet::Comms& node) {
  std::cout << "=::RECEIVED PACKET::=" << std::endl;
  std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;
  std::cout << "Message: " << std::endl;
  std::cout << "Packet contains: " << packet.GetCat() << std::endl;
  return comnet::CALLBACK_SUCCESS | comnet::CALLBACK_DESTROY_PACKET;
}

error_t ArmCommandCallback(const comnet::Header& header, const ArmCommand& packet, comnet::Comms& node) {
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;
	std::cout << "Message: " << std::endl;
	std::cout << "Packet contains: "; 
	packet.print();
	return comnet::CALLBACK_SUCCESS | comnet::CALLBACK_DESTROY_PACKET;
}

error_t ArmPositionCallback(const comnet::Header& header, const ArmPosition& packet, comnet::Comms& node) {
	std::cout << "=::RECEIVED PACKET::=" << std::endl;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;
	std::cout << "Message: " << std::endl;
	std::cout << "Packet contains: ";
	packet.print();
	return comnet::CALLBACK_SUCCESS | comnet::CALLBACK_DESTROY_PACKET;
}


static int packetReceivedCount = 0;
error_t ThroughputPingTestCallback(const comnet::Header& header, const Ping& packet, comnet::Comms& node) {
	
	++packetReceivedCount;
	std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;
	std::cout << "Packet count: " << packetReceivedCount << "\n";
	//std::cout << "Message: " << std::endl;
	//std::cout << "Packet contains: " << packet.GetCat() << std::endl;
	return comnet::CALLBACK_SUCCESS | comnet::CALLBACK_DESTROY_PACKET;
}




// Test against an xbee on another machine.
void isolatedTest()
{
	//const char* destMac = "0013A20040A54318";
	const char* destMac = "0013A2004105C6AA";

	// test date
	std::cout << "Test: 11/17/2017" << std::endl;
	//Disables Pinging to make reading output easier
	comnet::constate::ConnectionStateManager::ConStateEnabled = false;

	std::condition_variable cond;
	std::cout << sizeof(comnet::Header) << std::endl;
	// CommNode 1
	comnet::Comms comm1(2);
	//comm1.LoadKey("01234567890ABCDEF");

	comnet::architecture::os::CommMutex mut;
	comnet::architecture::os::CommLock commlock(mut);
	// This will cause the thread to wait for a few milliseconds, causing any other thread to wait.
	comnet::architecture::os::WaitForMilliseconds(commlock, cond, 1000);

	std::cout << "Test complete!" << std::endl;
	// CommNode 1 init and add Connection.
	std::cout << "Init connection succeeded: "
		<< std::boolalpha
		<< comm1.InitConnection(ZIGBEE_LINK, "COM6", "", 57600)
		<< std::endl;
	std::cout << "Connected to address: "
		<< std::boolalpha
		<< comm1.AddAddress(1, destMac)
		<< std::endl;

	//Ping holder("");
	//comm1.LinkCallback(&holder, new comnet::Callback((comnet::callback_t)PingCallback));

	comm1.LinkCallback(new ArmCommand() , new comnet::Callback((comnet::callback_t)ArmCommandCallback));
	//comm1.LinkCallback(new ArmPosition(), new comnet::Callback((comnet::callback_t)ArmPositionCallback));

	//comm1.LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback));

	// Test packet. 

	//Ping large("CAT NAME IS ANDREW");
	// NOTE(All): Be sure to run the nodes! If not, the threads won't execute!
	comm1.Run();
	int id = 22;
	int pos = 8000;
	// Loop. To exit, Click the red button on the top left (Windows Visual Studio) OR 
	// CNTRL+C (Linux). 

	while (true) {
		//std::cout << "Sleeping..." << std::endl;
		// comm1 will be sending the packet.
		//std::string word;
		//std::cout << "enter message:";
		//std::cin >> word;
		//Ping message(word);
		
		//ArmPosition amp(7, 6, 5, 4);
		//comm1.Send(amp, 2);
		ArmCommand amc(id, pos);
		std::cout << "Packet field ID: " << amc.id << std::endl;
		std::cout << "Packet field Position: " << amc.position <<  std::endl;
		comm1.Send(amc, 1);
		//comm1.Send(message, 2);
		id++;
		pos++;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}
	std::cin.ignore();
}


//test two xbees on same machine.
void localTest()
{
	
	const uint8_t com1ID = 1;
	const uint8_t com2ID = 2;
	const char* com1PortName = "COM6";
	const char* com2PortName = "COM10";
	const char* node1_Mac = "0013A20040917A31";
	const char* node2_Mac = "0013A2004067E4AE";


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
	// CommNode 1 init and add Connection.
	std::cout << "Init connection succeeded: "
		<< std::boolalpha
		<< comm1.InitConnection(ZIGBEE_LINK, com1PortName, "", baudRate)
		<< std::endl;
	std::cout << "Connected to address: "
		<< std::boolalpha
		<< comm1.AddAddress(com2ID, node2_Mac)// "0013A2004067E4AE")
		<< std::endl;
	// ComNode 2 init and add Connection.
	std::cout << "Init connection succeeded: "
		<< std::boolalpha
		<< comm2.InitConnection(ZIGBEE_LINK, com2PortName, "", baudRate)
		<< std::endl;
	std::cout << "Connected to address: "
		<< std::boolalpha
		<< comm2.AddAddress(com1ID, node1_Mac)//"0013A20040917A31")
		<< std::endl;
	// CommNode 2 init and add Connection.
	// CommNode Callback linking.

	comm1.LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback));
	comm2.LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback));


	// Test packet. 
	Ping small("Cats. MEW :3");
	Ping large("The quick brown fox jumps over a lazy dog.");
	// NOTE(All): Be sure to run the nodes! If not, the threads won't execute!
	comm1.Run();
	comm2.Run();

	// Loop. To exit, Click the red button on the top left (Windows Visual Studio) OR 
	// CNTRL+C (Linux). 
	while (true) {
		std::cout << "Sleeping..." << std::endl;
		// comm1 will be sending the packet.
		comm2.Send(large, com1ID);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::cin.ignore();
}

void throughputTest()
{
	const uint8_t comID = 1;
	const char* comPortName = "COM6";

	const uint8_t destComID = 2;
	const char* dest_Mac = "0013A2004067E4AE";


	comnet::Comms comm(comID);
	comm.LoadKey("01234567890ABCDEF");
	comm.InitConnection(ZIGBEE_LINK, comPortName, "", 57600);
	comm.AddAddress(destComID, dest_Mac);

	comm.LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)ThroughputPingTestCallback));


	const int numPacket = 1000;
	std::cout << "Are you sender(1) or receiver(2)?\nEnter 1 or 2:";
	int nodeType;
	std::cin >> nodeType;
	Ping testpacket("TEST");
	if (nodeType == 1)
	{
		
		for (int i = 0; i < numPacket; ++i)
		{
			comm.Send(testpacket, destComID);
		}
	}
	else if (nodeType == 2)
	{
		std::cout << "Waiting for packet...Press anything to end" << std::endl;
		std::cin;
		//while (true) {
		//	
		//	// comm1 will be sending the packet.
		//	
		//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		//}
	}



}

int main(int c, char** args) {


	//localTest();
	isolatedTest();

	return 0;
}