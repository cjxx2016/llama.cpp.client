#include "LlamaClient.h"

#include "Base/Inc/NLPowerBuf.h"


// constructor
LlamaClient::LlamaClient(const str& host, uint16 port) {
    this->pClient = new httplib::Client(host, port);

    int waitS = 120;
    this->pClient->set_connection_timeout(waitS);
    this->pClient->set_read_timeout(waitS);
    this->pClient->set_write_timeout(waitS);    

    this->params = nlohmann::json::parse(R"({
        "messages": [{
                "role": "system",
                "content": "You are a helpful assistant."
            }, {
                "role": "user",
                "content": "你好"
            }, {
                "role": "assistant",
                "content": "你好！有什么可以帮助你的吗？"
            }, {
                "role": "user",
                "content": "你叫什么名字?"
            }
        ],
        "stream": true,
        "cache_prompt": true,
        "samplers": "edkypmxt",
        "temperature": 0.8,
        "dynatemp_range": 0,
        "dynatemp_exponent": 1,
        "top_k": 40,
        "top_p": 0.95,
        "min_p": 0.05,
        "typical_p": 1,
        "xtc_probability": 0,
        "xtc_threshold": 0.1,
        "repeat_last_n": 64,
        "repeat_penalty": 1,
        "presence_penalty": 0,
        "frequency_penalty": 0,
        "dry_multiplier": 0,
        "dry_base": 1.75,
        "dry_allowed_length": 2,
        "dry_penalty_last_n": -1,
        "max_tokens": -1,
        "timings_per_token": false
    })");

}
// destructor
LlamaClient::~LlamaClient() {
    if (nullptr == this->pClient) {
        return ;
    }
    delete this->pClient;
    this->pClient = nullptr;
}

// send request and get response
wstr LlamaClient::Request(const wstr& req, const fcn<void(const wstr& rsp, bool bLast)>& cb/* = nullptr*/) {
    static const httplib::Headers headers = {
        {"Connection", "keep-alive"},
        {"Content-Type", "application/json"},
        {"Accept", "text/event-stream"}
        // Add API key or other headers if needed
    };

    // Serialize the json object to a string
    str params_json = this->params.dump();

    NLPowerBuf bufRecv;
    int cutLength = 0;
    
    auto FuncProcessRecvChat = [&](const str& recv, int& dealLength) -> bool {
        if (recv.empty()) {
            return false;
        }
        return true;

    };
        
_re_post_info:
    auto res = this->pClient->Post("/v1/chat/completions", headers, params_json, str("application/json"), [&](const char *data, size_t data_length) -> bool {
        bufRecv.Add(data, data_length);
        FuncProcessRecvChat(bufRecv, cutLength);
        bufRecv.CutFront(cutLength);
        return true;
    });
    if (res/* && res->status == 200*/) {
        FuncProcessRecvChat(res->body, cutLength);
    } else {
        // Handle error response from server
        // std::cerr << "prompt error: " << res.error() << std::endl;
        goto _re_post_info;
    }

    return L"welcome to my world!";
}