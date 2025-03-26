#include "LlamaClient/LlamaClient.h"

int main() {

    LlamaClient client("127.0.0.1", 9001);

    std::cout << client.Request("Hello world!") << std::endl;

    std::cin.get();

    return 0;
}