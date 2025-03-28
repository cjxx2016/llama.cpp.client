#include "LlamaClient.h"

#include "Base/Data/Buf/NLPowerBuf.h"

// 
void LlamaClient::WekeUp(const str& host, uint16 port) {
    this->pClient = new httplib::Client(host, port);

    int waitS = 120;
    this->pClient->set_connection_timeout(waitS);
    this->pClient->set_read_timeout(waitS);
    this->pClient->set_write_timeout(waitS);    

    this->messages = {
          {Role::SYS, "You are a helpful assistant."}
        // , {Role::USER, "你好"}
        // , {Role::BOT, "你好！有什么可以帮助你的吗？"}
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

// 
void LlamaClient::ToSleep() {
    if (nullptr == this->pClient) {
        return ;
    }
    delete this->pClient;
    this->pClient = nullptr;
}

// constructor
LlamaClient::LlamaClient(const str& host, uint16 port) {
    this->WekeUp(host, port);
}
// destructor
LlamaClient::~LlamaClient() {
    this->ToSleep();
}

// send request and get response
str LlamaClient::Ask(const str& req, const fcn<void(const str& rsp, bool bLast)>& cb/* = nullptr*/) {
    static const httplib::Headers headers = {
        {"Connection", "keep-alive"},
        {"Content-Type", "application/json"},
        {"Accept", "text/event-stream"}
        // Add API key or other headers if needed
    };

    this->Add({Role::USER, req});

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
        
        nlohmann::json data;
        try {
            data = nlohmann::json::parse(recv.substr(5));
        } catch (const std::exception& e) {
            iDealLength = 0;
            return {};
        }

        str ret;

        iDealLength = recv.size();
        
        bool bFinished = false;
        str content;

        if (data.contains("choices")) {
            for (auto& choice : data["choices"]) {
                bFinished = false;
                content.clear();
                if (choice.contains("finish_reason")) {
                    if (!choice["finish_reason"].is_null() && choice["finish_reason"].get<str>() == "stop") {
                        bFinished = true;
                    }
                }
                if (choice.contains("delta")) {
                    if (choice["delta"].contains("content")) {
                        content = choice["delta"]["content"].get<str>();
                        ret.append(content);
                    }
                }
                if (cb) {
                    cb(content, bFinished);
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

    this->Add({Role::BOT, req});
    return ret_s;
}

// get now messages
auto LlamaClient::Get() -> arr<RoleContent>& {
    return this->messages;
}

// set now messages
void LlamaClient::Set(const arr<RoleContent>& messages) {
    this->messages = messages;
}

// add message
void LlamaClient::Add(const RoleContent& message) {
    this->messages.push_back(message);
}

// prepare history messages
void LlamaClient::PrepareHistoryMessages() {
    // 将历史消息添加到params中
    this->params["messages"] = nlohmann::json::array();
    for (auto& message : this->messages) {
        this->params["messages"].push_back({
            {"role", message.role == Role::SYS ? "system" : message.role == Role::USER ? "user" : "assistant"},
            {"content", message.content}
        });
    }
}

// clear messages
void LlamaClient::Clear() {
    this->messages.clear();
}