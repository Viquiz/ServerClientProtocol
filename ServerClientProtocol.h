// ----------------------------------------
// Communication between server (ESP-32) and client (ESP-8266)
// ----------------------------------------
#ifndef _SERVER_CLIENT_PROTOCOL_
#define _SERVER_CLIENT_PROTOCOL_

#include <Arduino.h>

// DEPRECATED: Support for password without a screen is too cumbersome
// typedef uint8_t password_t;

enum packet_t : uint8_t
{
    BEACON = 0,
    REQ_REG,
    RESPOND_REG,
    RECV_ANSW,
    RESPOND_ANSW,
    TIMEOUT // UNUSED
};

// @NguyenVux, @Cemu0 add bit shift value here
enum btn_t : uint8_t
{
    NO_ANSW,
    BTN_1,
    BTN_2,
    BTN_3,
    BTN_4
};

// UNUSED, may be replaced by BeaconPacket.milliRemain
enum server_stat_t : int8_t
{
    IDLE = -1,
    PLAYING
};

struct BasePacket
{
    const packet_t type;
    BasePacket(packet_t type) : type(type) {}
};

// Unicast/Broadcast from server to clients
struct BeaconPacket : BasePacket
{
    // Negative value can replace server_stat_t
    int32_t milliRemain; 
    uint8_t unanswered;
    BeaconPacket() : BasePacket(packet_t::BEACON) {}
};

// An unregistered client send this packet when:
// 1. Receives BeaconPacket
// 2. Timeout when listen for beacon and no server found, use broadcast
struct RequestRegisterPacket : BasePacket
{
    RequestRegisterPacket() : BasePacket(packet_t::REQ_REG) {}
};

struct RespondRegisterPacket : BasePacket
{
    uint8_t id;
    RespondRegisterPacket() : BasePacket(packet_t::RESPOND_REG) {}
};

// Server receive this from client(s)
// If timeout and client hasn't pressed a button, send NO_ANSW to the server
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

// UNUSED
struct TimeoutPacket : BasePacket
{
    TimeoutPacket() : BasePacket(packet_t::TIMEOUT) {}
};
#endif // !_SERVER_CLIENT_PROTOCOL_