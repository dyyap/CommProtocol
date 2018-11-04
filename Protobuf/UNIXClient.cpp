
// Client side C/C++ program
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include "ProtoPacketsV2.pb.h"
#include <iostream>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#define PORT 59000

using namespace google::protobuf::io;
void SocketHandler(void*);
google::protobuf::uint32 readHdr(char*);
void readBody(int, google::protobuf::uint32);

int main(int argc, char const *argv[])
{
  struct sockaddr_in address;
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  pthread_t thread_id=0;
  int bytecount;
  char buffer[56];
  int csock;

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "Socket creation error" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
    std::cerr << "\nInvalid address/ Address not supported \n" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cout << "Connection Failed \n" << std::endl;
    exit(EXIT_FAILURE);
  } 
  
  if((bytecount = recv(sock, (void *) buffer, 56, MSG_WAITALL))== -1){
    std::cerr << "Error receiving" << std::endl;
    exit(EXIT_FAILURE);
  } else
    std::cout << bytecount << " bytes were received" << std::endl;
  

  ProtoPackets::AirVehicleGroundRelativeState package;
  google::protobuf::uint32 size;
  google::protobuf::io::ArrayInputStream ais(buffer, 56);
  CodedInputStream coded_input(&ais);
  coded_input.ReadVarint32(&size);
  google::protobuf::io::CodedInputStream::Limit msgLimit = coded_input.PushLimit(size);
  package.ParseFromCodedStream(&coded_input);
  coded_input.PopLimit(msgLimit);

  std::cout << "Size of payload is " << size << std::endl;

  std::cout <<"\n-------Message------\n" << package.DebugString();

  google::protobuf::ShutdownProtobufLibrary();
  close(sock);

  return 0;
}
