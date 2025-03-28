#include "LlamaClient/LlamaClient.h"

int main() {
    // init
    LlamaClient client("127.0.0.1", 9001);

    // sync
    std::cout << client.Talk("Hello world!") << std::endl;

    // stream
    client.Talk("Nice to meet you!🥲", [](const str& rsp, bool last) { 
        std::cout << rsp << std::flush; 
    });

    return std::cin.get();
}