#include "LlamaClient/LlamaClient.h"

int main() {

    LlamaClient client("127.0.0.1", 9001);

    client.AddMessage({LlamaClient::SYSTEM, "you are always have a joke about me" });

    std::cout << client.Request("Hello world!") << std::endl;

    std::cin.get();

    return 0;
}