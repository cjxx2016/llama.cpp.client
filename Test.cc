#include "LlamaClient/LlamaClient.h"

int main() {

    LlamaClient client("127.0.0.1", 9001);

    client.Add({LlamaClient::SYS, "you are always have a joke for me" });

    std::cout << client.Ask("Hello world!") << std::endl;

    return std::cin.get();
}