#include "LlamaClient/LlamaClient.h"

int main() {

    LlamaClient client("127.0.0.1", 9001);

    std::cout << client.Ask("Hello world!") << std::endl;

    return std::cin.get();
}