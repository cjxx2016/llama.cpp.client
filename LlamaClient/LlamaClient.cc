#include "LlamaClient.h"


// constructor
LlamaClient::LlamaClient() {

}
// destructor
LlamaClient::~LlamaClient() {

}

// send request and get response
wstr LlamaClient::Request(const wstr& req, const fcn<void(const wstr& rsp, bool bLast)>& cb/* = nullptr*/) {
    return {};
}