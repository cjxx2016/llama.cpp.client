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
        SYSTEM = 0,
        USER = 1,
        ASSISTANT = 2
    };
    struct RoleContent {
        Role role;        
        str content;
    };

public:
    // constructor
    LlamaClient(const str& host, uint16 port);
    // destructor
    ~LlamaClient();

public:
    // send request and get response
    str Request(const str& req, const fcn<void(const str& rsp, bool bLast)>& cb = nullptr);

public:
    // get now messages
    arr<RoleContent>& GetMessages();

    // set now messages
    void SetMessages(const arr<RoleContent>& messages);

    // add message
    void AddMessage(const RoleContent& message);

    // clear messages
    void ClearMessages();

private:
    // prepare history messages
    void PrepareHistoryMessages();

private:
    httplib::Client* pClient = nullptr;
    nlohmann::json params;

    arr<RoleContent> messages;
};