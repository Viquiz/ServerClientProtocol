#ifndef _SERVER_CLIENT_PROTOCOL_
#define _SERVER_CLIENT_PROTOCOL_
#include <Arduino.h>
// ----------------------------------------
// Communication between server (ESP-32) and client (ESP-8266)
// ----------------------------------------
enum packet_t : uint8_t
{
    BEACON = 0,
    RESPOND_BEACON,
    REQ_BEACON,
    RECV_ANSW,
    RESPOND_ANSW,
    TIMEOUT,
};

enum btn_t : uint8_t
{
    NO_ANSW = 0,
    BTN_1,
    BTN_2,
    BTN_3,
    BTN_4
};

// Act as a header
struct BasePacket
{
    const packet_t type;
    BasePacket(packet_t type) : type(type) {}
};

// Unicast/Broadcast server address with password
struct BeaconPacket : BasePacket
{
    uint32_t password;
    BeaconPacket() : BasePacket(packet_t::BEACON) {}
    BeaconPacket(int password) : BeaconPacket() { this->password = password; }
};

// When client receive a BeaconPacket, it will respond a RespondBeaconPacket to the server
struct RespondBeaconPacket : BasePacket
{
    RespondBeaconPacket() : BasePacket(packet_t::RESPOND_BEACON) {}
};

// If for some reason the BeaconPacket can't reach the client,
// client can broadcast this to actively receive the server address
struct RequestBeaconPacket : BasePacket
{
    int password;
    RequestBeaconPacket() : BasePacket(packet_t::REQ_BEACON) {}
    RequestBeaconPacket(int password) : RequestBeaconPacket() { this->password = password; }
};

// Server receive this from client(s).
// If timeout and client hasn't pressed a button, send NO_ANSW to the server.
// A client is considered to be disconnected if it didn't send anything
struct AnswPacket : BasePacket
{
    btn_t button;
    AnswPacket() : BasePacket(packet_t::RECV_ANSW) {}
    AnswPacket(btn_t button) : AnswPacket() { this->button = button; }
};

// Send correct answer to client(s)
struct RespondAnswPacket : BasePacket
{
    btn_t answer;
    RespondAnswPacket() : BasePacket(packet_t::RESPOND_ANSW) {}
    RespondAnswPacket(btn_t answer) : RespondAnswPacket() { this->answer = answer; }
};

struct TimeoutPacket : BasePacket
{
    TimeoutPacket() : BasePacket(packet_t::TIMEOUT) {}
};
#endif // !_SERVER_CLIENT_PROTOCOL_