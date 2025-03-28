#pragma once

#include "Base/Inc/NLBaseInc.h"

// increase max payload length to allow use of larger context size
#define CPPHTTPLIB_FORM_URL_ENCODED_PAYLOAD_MAX_LENGTH 1048576
// disable Nagle's algorithm
#define CPPHTTPLIB_TCP_NODELAY true

#include "Depend/httplib/httplib.h"
#include "Depend/nlohmann/json.hpp"

class LlamaClient {
public:
    enum Role {
        SYS = 0,
        USER = 1,
        BOT = 2
    };
    struct RoleContent {
        Role role;
        str content;
    };

public:
    // constructor
    LlamaClient();
    LlamaClient(const str& host, uint16 port);
    // destructor
    ~LlamaClient();

public:
    // 
    void WekeUp(const str& host, uint16 port);
    
    // 
    void ToSleep();

public:
    // send request and get response
    str Talk(const str& req, const fcn<void(const str& rsp, bool bLast)>& cb = nullptr);

public:
    // get now messages
    arr<RoleContent>& Get();

    // set now messages
    void Set(const arr<RoleContent>& messages);

    // add message
    void Add(const RoleContent& message);

    // clear messages
    void Clear();

private:
    // prepare history messages
    void PrepareHistoryMessages();

    // set steam
    void SetStream(bool bStream);

private:
    httplib::Client* pClient = nullptr;
    nlohmann::json params;

    arr<RoleContent> messages;
};