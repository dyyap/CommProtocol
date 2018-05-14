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

error_t AirVehicleGroundRelativeStateCallback(const comnet::Header& header, const ngcp::AirVehicleGroundRelativeState& packet, comnet::Comms& node);
error_t ArmCommandCallback(const comnet::Header& header, const ngcp::ArmCommand& packet, comnet::Comms& node);
error_t ArmPositionCallback(const comnet::Header& header, const ngcp::ArmPosition& packet, comnet::Comms& node);
error_t BatteryCallback(const comnet::Header& header, const ngcp::Battery& packet, comnet::Comms& node);
error_t TargetAcknowledgementCallback(const comnet::Header& header, const ngcp::TargetAcknowledgement& packet, comnet::Comms& node);
error_t TargetDesignationCommandCallback(const comnet::Header& header, const ngcp::TargetDesignationCommand& packet, comnet::Comms& node);
error_t TargetStatusCallback(const comnet::Header& header, const ngcp::TargetStatus& packet, comnet::Comms& node);
error_t VehicleAttitudeCallback(const comnet::Header& header, const ngcp::VehicleAttitude& packet, comnet::Comms& node);
error_t VehicleAuthorizationReplyCallback(const comnet::Header& header, const ngcp::VehicleAuthorizationReply& packet, comnet::Comms& node);
error_t VehicleAuthorizationRequestCallback(const comnet::Header& header, const ngcp::VehicleAuthorizationRequest& packet, comnet::Comms& node);
error_t VehicleBodySensedStateCallback(const comnet::Header& header, const ngcp::VehicleBodySensedState& packet, comnet::Comms& node);
error_t VehicleGlobalPositionCallback(const comnet::Header& header, const ngcp::VehicleGlobalPosition& packet, comnet::Comms& node);
error_t VehicleIdentificationCallback(const comnet::Header& header, const ngcp::VehicleIdentification& packet, comnet::Comms& node);
error_t VehicleInertialStateCallback(const comnet::Header& header, const ngcp::VehicleInertialState& packet, comnet::Comms& node);
error_t VehicleModeCommandCallback(const comnet::Header& header, const ngcp::VehicleModeCommand& packet, comnet::Comms& node);
error_t VehicleSystemStatusCallback(const comnet::Header& header, const ngcp::VehicleSystemStatus& packet, comnet::Comms& node);
error_t VehicleTelemetryCommandCallback(const comnet::Header& header, const ngcp::VehicleTelemetryCommand& packet, comnet::Comms& node);
error_t VehicleTerminationCommandCallback(const comnet::Header& header, const ngcp::VehicleTerminationCommand& packet, comnet::Comms& node);
error_t VehicleWaypointCommandCallback(const comnet::Header& header, const ngcp::VehicleWaypointCommand& packet, comnet::Comms& node);