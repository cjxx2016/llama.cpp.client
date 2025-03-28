# llama.cpp.client

    cpp client of llama-server

## Buile

    xmake

## Usage

    ```c++
    int main() {

        LlamaClient client("127.0.0.1", 9001);

        client.AddMessage({LlamaClient::SYSTEM, "you are always have a joke for me" });

        std::cout << client.Request("Hello world!") << std::endl;

        std::cin.get();

        return 0;
    }
    ```
    
    Have fun.
