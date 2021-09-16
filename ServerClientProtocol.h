// ----------------------------------------
// Communication between server (ESP-32) and client (ESP-8266)
// ----------------------------------------
#ifndef _SERVER_CLIENT_PROTOCOL_
#define _SERVER_CLIENT_PROTOCOL_

#include <Arduino.h>

typedef uint8_t password_t;

enum packet_t : uint8_t
{
    BEACON = 0,
    REQ_REG,
    RESPOND_REG,
    RECV_ANSW,
    RESPOND_ANSW,
    TIMEOUT
};

// @NguyenVux add bit shift value here
enum btn_t : uint8_t
{
    NO_ANSW,
    BTN_1,
    BTN_2,
    BTN_3,
    BTN_4
};

// May be replaced by GameInfo
enum server_stat_t : int8_t
{
    IDLE = -1,
    TIMEOUT,
    PLAYING
};

struct GameInfo 
{
    uint8_t unanswered;
    // Negative value can replace server_stat_t
    int32_t milliRemain; 
};

// Act as a header
struct BasePacket
{
    const packet_t type;
    BasePacket(packet_t type) : type(type) {}
};

// Unicast/Broadcast from server to clients
struct BeaconPacket : BasePacket
{
    password_t password; // Not sure broadcast plaintext password is a good idea
    GameInfo gameInfo;
    BeaconPacket() : BasePacket(packet_t::BEACON) {}
    BeaconPacket(password_t password) : BeaconPacket() { this->password = password; }
};

// An unregistered client send this packet when:
// 1. Receives BeaconPacket and BeaconPacket.password == userInputPassword
// 2. (FAILSAFE) Listen for beacon timeout and either no server found or 
//    no password matched user input -> Server out of range or incorrect password
//    Broadcast this packet
struct RequestRegisterPacket : BasePacket
{
    password_t password;
    RequestRegisterPacket() : BasePacket(packet_t::REQ_REG) {}
    RequestRegisterPacket(password_t password) : RequestRegisterPacket() { this->password = password; }
};

struct RespondRegisterPacket : BasePacket
{
    uint8_t id;
    RespondRegisterPacket() : BasePacket(packet_t::RESPOND_REG) {}
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