#include "LlamaClient/LlamaClient.h"

int main() {

    LlamaClient client("127.0.0.1", 9001);

    client.Add({LlamaClient::SYS, "You often joke with me"});

    std::cout << client.Ask("Hello world!") << std::endl;

    return std::cin.get();
}