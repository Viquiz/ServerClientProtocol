// ----------------------------------------
// Communication between server (ESP-32) and client (ESP-8266)
// ----------------------------------------
#ifndef _SERVER_CLIENT_PROTOCOL_
#define _SERVER_CLIENT_PROTOCOL_

#include <Arduino.h>

// DEPRECATED: Support for password without a screen is too cumbersome
// typedef uint8_t password_t;

typedef uint8_t client_t;

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
enum server_state_t : int8_t
{
    IDLE = -1,
    PLAYING
};

struct BasePacket
{
    const packet_t type;
    BasePacket(packet_t type) : type(type) {}
};

// Broadcast from server to clients
struct BeaconPacket : BasePacket
{
    // Negative value can replace server_state_t
    int32_t milliRemain : 24; // Max ~2h
    client_t unanswered;
    BeaconPacket() : BasePacket(packet_t::BEACON) {}
};

// An unregistered client send this packet when receives BeaconPacket
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