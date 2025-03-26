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
    // constructor
    LlamaClient(const str& host, uint16 port);
    // destructor
    ~LlamaClient();

public:
    // send request and get response
    wstr Request(const wstr& req, const fcn<void(const wstr& rsp, bool bLast)>& cb = nullptr);
    
private:
    httplib::Client* pClient = nullptr;
    nlohmann::json params;
};