#
# Test main for testing.
#
cmake_minimum_required( VERSION 2.8 )
project(CommBackendRelay)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../XbeeC/xbee_ansic_library/include/)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../CommProto/include/)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../Encryption/cryptopp564/)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../protobuf/include/)
#include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../poco-1.9/include/)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../protobuf/lib)
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../poco-1.9/lib)


add_executable(CommProtoBackendRelay
 ${CMAKE_CURRENT_SOURCE_DIR}/CommProtocolBackendRelay.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/ProtoPackets/ProtoPacketsV2.pb.cc
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/ProtoPackets/ProtoPacketsV2.pb.h
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/AirVehicleGroundRelativeState.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/ArmCommand.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/ArmPosition.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/Battery.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/TargetAcknowledgement.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/TargetDesignationCommand.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/TargetStatus.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/VehicleAttitude.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/VehicleAuthorizationReply.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/VehicleAuthorizationRequest.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/VehicleBodySensedState.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/VehicleGlobalPosition.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/VehicleIdentification.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/VehicleInertialState.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/VehicleModeCommand.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/VehicleSystemStatus.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/VehicleTelemetryCommand.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/VehicleTerminationCommand.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/../Packets/XbeePackets/VehicleWaypointCommand.hpp
 )

if (CMAKE_VERION VERSION_GREATER 2.8.11)
  target_compile_options(CommProtoBackend PUBLIC -std=c++11)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -pthread")
else()
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -std=c++11 -pthread")
endif()
# Given that linux is different from windows
target_link_libraries(CommProtoBackendRelay CommProtocol-Static)
target_link_libraries(CommProtoBackendRelay cryptopp-static)
target_link_libraries(CommProtoBackendRelay libxbee3)
target_link_libraries(CommProtoBackendRelay libprotobuf)