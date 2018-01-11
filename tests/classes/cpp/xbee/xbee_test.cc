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


// Callback function that we will be using to link to Ping packet.
error_t PingCallback(const comnet::Header& header, const Ping& packet, comnet::Comms& node) {
  std::cout << "=::RECEIVED PACKET::=" << std::endl;
  std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;
  std::cout << "Message: " << std::endl;
  std::cout << "Packet contains: " << packet.GetCat() << std::endl;
  return comnet::CALLBACK_SUCCESS | comnet::CALLBACK_DESTROY_PACKET;
}




// Test against an xbee on another machine.
void isolatedTest()
{
	//const char* destMac = "0013A20040A54318";
	const char* destMac = "0013A20040A5430F";

	// test date
	std::cout << "Test: 11/17/2017" << std::endl;
	//Disables Pinging to make reading output easier
	comnet::constate::ConnectionStateManager::ConStateEnabled = false;

	std::condition_variable cond;
	std::cout << sizeof(comnet::Header) << std::endl;
	// CommNode 1
	comnet::Comms comm1(2);
	comm1.LoadKey("01234567890ABCDEF");

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
		<< comm1.AddAddress(1, destMac)
		<< std::endl;

	Ping holder("");
	comm1.LinkCallback(&holder, new comnet::Callback((comnet::callback_t)PingCallback));

	//comm1.LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback));

	// Test packet. 

	Ping large("CAT NAME IS ANDREW");
	// NOTE(All): Be sure to run the nodes! If not, the threads won't execute!
	comm1.Run();

	// Loop. To exit, Click the red button on the top left (Windows Visual Studio) OR 
	// CNTRL+C (Linux). 
	while (true) {
		std::cout << "Sleeping..." << std::endl;
		// comm1 will be sending the packet.
		std::string word;
		std::cout << "enter message:";
		std::cin >> word;
		Ping message(word);
		comm1.Send(message, 1);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}
	std::cin.ignore();
}


//test two xbees on same machine.
void localTest()
{
	const char* destMAC1 = "0013A2004067E4A0";
	const char* destMAC2 = "0013A2004067E49F";
	const char* com1PortName = "COM3";
	const char* com2PortName = "COM4";
	const uint8_t com1ID = 1;
	const uint8_t com2ID = 2;
	const uint32_t baudRate = 57600;

	// test date
	std::cout << "Test: 11/17/2017" << std::endl;
	//Disables Pinging to make reading output easier
	comnet::constate::ConnectionStateManager::ConStateEnabled = false;

	std::condition_variable cond;
	std::cout << sizeof(comnet::Header) << std::endl;
	// CommNode 1
	comnet::Comms comm1(1);
	comm1.LoadKey("01234567890ABCDEF");
	// CommNode 2
	comnet::Comms comm2(2);
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
		<< comm1.AddAddress(com2ID, destMAC2)// "0013A2004067E4AE")
		<< std::endl;
	// ComNode 2 init and add Connection.
	std::cout << "Init connection succeeded: "
		<< std::boolalpha
		<< comm2.InitConnection(ZIGBEE_LINK, com1PortName, "", baudRate)
		<< std::endl;
	std::cout << "Connected to address: "
		<< std::boolalpha
		<< comm2.AddAddress(com1ID, destMAC1)//"0013A20040917A31")
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
		comm1.Send(large, com2ID);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::cin.ignore();
}

int main(int c, char** args) {


	//localTest();
	isolatedTest();

	return 0;
}