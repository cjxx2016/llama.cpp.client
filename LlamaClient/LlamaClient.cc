#include "LlamaClient.h"

#include "Base/Inc/NLPowerBuf.h"


// constructor
LlamaClient::LlamaClient(const str& host, uint16 port) {
    this->pClient = new httplib::Client(host, port);

    int waitS = 120;
    this->pClient->set_connection_timeout(waitS);
    this->pClient->set_read_timeout(waitS);
    this->pClient->set_write_timeout(waitS);    

    this->messages = {
          {Role::SYSTEM, "You are a helpful assistant."}
        // , {Role::USER, "你好"}
        // , {Role::ASSISTANT, "你好！有什么可以帮助你的吗？"}
    };

    this->params = nlohmann::json::parse(R"({
        "messages": [],
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

    this->PrepareHistoryMessages();
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
str LlamaClient::Request(const str& req, const fcn<void(const str& rsp, bool bLast)>& cb/* = nullptr*/) {
    static const httplib::Headers headers = {
        {"Connection", "keep-alive"},
        {"Content-Type", "application/json"},
        {"Accept", "text/event-stream"}
        // Add API key or other headers if needed
    };

    this->AddMessage({Role::USER, req});

    this->PrepareHistoryMessages();

    // Serialize the json object to a string
    str params_json = this->params.dump();

    NLPowerBuf bufRecv;
    int cutLength = 0;
    
    auto FuncProcessRecvChat = [cb](const str& recv, int& iDealLength) -> str {
        if (recv.empty()) {
            return {};
        }
        // std::cout << recv << std::endl;
        str ret;
        nlohmann::json data;
        try {
            data = nlohmann::json::parse(recv.substr(5));
        } catch (const std::exception& e) {
            iDealLength = 0;
            return ret;
        }

        iDealLength = recv.size();

        if (data.contains("choices")) {
            for (auto& choice : data["choices"]) {
                if (choice.contains("delta")) {
                    if (choice["delta"].contains("content")) {
                        auto content = choice["delta"]["content"].get<str>();                        
                        if (cb) {
                            cb(content, choice["finish_reason"].get<str>() == "stop");
                        }
                        ret.append(content);
                    }
                }
            }
        }

        return ret;
    };

    str ret_s;
        
_re_post_info:
    auto res = this->pClient->Post("/v1/chat/completions", headers, params_json, str("application/json"), [&](const char *data, size_t data_length) -> bool {
        bufRecv.Add(data, data_length);
        ret_s.append(FuncProcessRecvChat(bufRecv, cutLength));
        bufRecv.CutFront(cutLength);
        
        return true;
    });
    if (res/* && res->status == 200*/) {
        ret_s.append(FuncProcessRecvChat(res->body, cutLength));
    } else {
        // Handle error response from server
        // std::cerr << "prompt error: " << res.error() << std::endl;
        goto _re_post_info;
    }

    return ret_s;
}

// get now messages
auto LlamaClient::GetMessages() -> arr<RoleContent>& {
    return this->messages;
}

// set now messages
void LlamaClient::SetMessages(const arr<RoleContent>& messages) {
    this->messages = messages;
}

// add message
void LlamaClient::AddMessage(const RoleContent& message) {
    this->messages.push_back(message);
}

// prepare history messages
void LlamaClient::PrepareHistoryMessages() {
    // 将历史消息添加到params中
    this->params["messages"] = nlohmann::json::array();
    for (auto& message : this->messages) {
        this->params["messages"].push_back({
            {"role", message.role == Role::SYSTEM ? "system" : message.role == Role::USER ? "user" : "assistant"},
            {"content", message.content}
        });
    }
}

// clear messages
void LlamaClient::ClearMessages() {
    this->messages.clear();
}