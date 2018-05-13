#pragma once
#include <CommProto/commproto.h>
#include <WinSock2.h>
#include <ProtoPackets\ProtoPacketsV2.pb.h>
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

#pragma comment(lib, "Ws2_32.lib")

using namespace google::protobuf::io;
using namespace std;

extern int hsock;
bool sendPacket(int& socket, char* packet, int pktSize, ProtoPackets::Packet& payload);

error_t ArmCommandCallback(const comnet::Header& header, const ngcp::ArmCommand& packet, comnet::Comms& node);
error_t ArmPositionCallback(const comnet::Header& header, const ngcp::ArmPosition& packet, comnet::Comms& node);
error_t VehicleGlobalPositionCallback(const comnet::Header& header, const ngcp::VehicleGlobalPosition& packet, comnet::Comms& node);
error_t VehicleInertialStateCallback(const comnet::Header& header, const ngcp::VehicleInertialState& packet, comnet::Comms& node);
