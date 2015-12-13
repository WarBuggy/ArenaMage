// These two lines prevents the file from being included multiple
// times in the same source file
#ifndef DATAID_H_
#define DATAID_H_

enum DataID : uint8_t {
    Null,
    LogInRequestClient,
    LogInServerAccept,
    LogInDenyWrongPassword,
    LogInDenyMaxPlayer,
    LogInDenyWrongPacketFormat,
    LogOut,
    RequestArenaInfo,
    ArenaObjects,
    ArenaInfo,
    ArenaReceivedCompleted
};
#endif
