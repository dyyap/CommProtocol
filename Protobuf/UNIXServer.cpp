// Server side C/C++ for UNIX

#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "ProtoPacketsV2.pb.h"
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>


#define PORT 59000
using namespace google::protobuf::io;

void addVehicles(ProtoPackets::AirVehicleGroundRelativeState*);

int main(int argc, char const *argv[]){
  //Add a vehicle
//  ProtoPackets::AirVehicleGroundRelativeState vehicle;
//  addVehicles(&vehicle);
  
 // std::cout << "\nSize after serilizing is " << vehicle.ByteSize() << std::endl;
  

//  int size = vehicle.ByteSize()+4;
 // char *pkt = new char [size];
  
 // ArrayOutputStream aos(pkt,size);
 // CodedOutputStream *output = new CodedOutputStream(&aos);
 // output->WriteVarint32(vehicle.ByteSize());
 // vehicle.SerializeToCodedStream(output);

  //Creating socket and send

  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  int* csock;
  int bytecount;
  
  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    std::cerr << "Creating socket file descriptor failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  /*// Forcefully attaching socket to the port 59000
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    std::cerr << "setsockopt fails" << std::endl;
    exit(EXIT_FAILURE);
  }*/
  
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  memset(&(address.sin_zero), 0, 8);

  //Bind to port 59000
  if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
    std::cerr << "bind failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 5) < 0) {
    std::cerr << "listening error" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  std::cout << "Waiting for client" << std::endl; 
if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        std::cerr << "Fail accepting connection" << std::endl;
        exit(EXIT_FAILURE);
    }
ProtoPackets::AirVehicleGroundRelativeState vechicle;
addVehicles(&vechicle);
int size = vechicle.ByteSize()+4;
char *pkt = new char [size];
ArrayOutputStream aos(pkt,size);
CodedOutputStream *output = new CodedOutputStream(&aos);
output->WriteVarint32(vechicle.ByteSize());
vechicle.SerializeToCodedStream(output);
std::cout << "\nSize after serilizing is " << vechicle.ByteSize() << std::endl;
    

  
  if((bytecount = send(new_socket, (void *) pkt,size,0)) < 0 ) {
    std::cerr << "Error sending data" << std::endl;
    exit(EXIT_FAILURE);
  } else {
    std::cout << "\nSent protobuf package!" << std::endl;
    std::cout << bytecount << " bytes were sent" << std::endl;
  }

  google::protobuf::ShutdownProtobufLibrary();
  close(server_fd);
  
  return 0;
}

void addVehicles(ProtoPackets::AirVehicleGroundRelativeState* vehicle) {
  //ID
  std::cout << "Enter vehicle id: ";
  int id;
  std::cin >> id;
  vehicle->set_vehicleid(id);
  std::cin.ignore(256, '\n');

  //angleofattack
  std::cout << "Angle of attack: ";
  float angle_att;
  std::cin >> angle_att;
  vehicle->set_angleofattack(angle_att);
  std::cin.ignore(256, '\n');

  //angleofsideslip
  std::cout << "Angle of sideslip: ";
  float angle_sl;
  std::cin >> angle_sl;
  vehicle->set_angleofsideslip(angle_sl);
  std::cin.ignore(256, '\n');

  //trueairspeed
  std::cout << "True air speed: ";
  float air_speed;
  std::cin >> air_speed;
  vehicle->set_trueairspeed(air_speed);
  std::cin.ignore(256, '\n');

  //indicatedairspeed
  std::cout << "Indicated air speed: ";
  float indicated_air;
  std::cin >> indicated_air;
  vehicle->set_indicatedairspeed(indicated_air);
  std::cin.ignore(256, '\n');

  //northwindspeed
  std::cout << "North wind speed: ";
  float nw_speed;
  std::cin >> nw_speed;
  vehicle->set_northwindspeed(nw_speed);
  std::cin.ignore(256, '\n');

  //eastwindspeed
  std::cout << "East wind speed: ";
  float ew_speed;
  std::cin >> ew_speed;
  vehicle->set_eastwindspeed(ew_speed);
  std::cin.ignore(256, '\n');

  //northgroundspeed
  std::cout << "North ground speed: ";
  float ng_speed;
  std::cin >> ng_speed;
  vehicle->set_northgroundspeed(ng_speed);
  std::cin.ignore(256, '\n');

  //eastgroundspeed
  std::cout << "East ground speed: ";
  float eg_speed;
  std::cin >> eg_speed;
  vehicle->set_eastgroundspeed(eg_speed);
  std::cin.ignore(256, '\n');

  //barometricpressure
  std::cout << "Barometric pressure: ";
  float pressure;
  std::cin >> pressure;
  vehicle->set_barometricpressure(pressure);
  std::cin.ignore(256, '\n');

  //barometricaltitude
  std::cout << "Barometrical altitude: ";
  float altitude;
  std::cin >> altitude;
  vehicle->set_barometricaltitude(altitude);
  std::cin.ignore(256, '\n');
}
