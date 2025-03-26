#include "LlamaClient/LlamaClient.h"

int main() {

    LlamaClient client;

    std::wcout << client.Request(L"Hello world!") << std::endl;

    std::cin.get();

    return 0;
}