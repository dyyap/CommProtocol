/*
  This is a rough implementation of how we will be able to dynamically declare and define 
  classes and packets into CommProto. It is done through extension of the AbstractPacket
  class, which will hold much more valuable information. In order to properly extend our 
  abstract class (AbstractPacket) you must define the pure virtual functions pack() and 
  unpack(). Next, ALWAYS, for your constructor, ALWAYS call AbstractPacket( with string ) so that the
  class will be able to generate a discrete number that will be used to identify the
  packet with a unique number. This number is intended to distinguish packets, and therefore
  help in determining the proper procedures in taking care of the packet.
  
  NOTE: This is only a rough implementation of how packets will be defined. This file will
  act as a file generated by the USER and not the LIBRARY. So the User must define this, or 
  have one of our scripts generate this for them ( We may need to use scripts here ).
*/
#ifndef __PACKETS_H
#define __PACKETS_H

#include <CommProto/AbstractPacket.h>


namespace Comnet {


/**
   This class must inherit AbstractPacket if it wants to be included in message protocol.
*/
class Ping : public AbstractPacket {
public:
  Ping();
  Ping(uint32_t num);
  
  ~Ping() { }
  /**
     Must implement pack and unpack.
  */
  void pack(ObjectStream& obj);
  void unpack(ObjectStream& obj);

  AbstractPacket* create();
public:
 uint32_t num;
};

/**
   Pong class created.
*/
class Pong : public AbstractPacket {
public:
  Pong(char letter);
  
  ~Pong() { }

  void pack(ObjectStream& obj);
  void unpack(ObjectStream& obj);

  AbstractPacket* create();
private:
  char letter;
};

} // Comnet


#endif // __PACKETS_H
