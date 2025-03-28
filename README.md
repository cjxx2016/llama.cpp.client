# llama.cpp.client

   c++ client of llama.cpp's llama-server

## Build

   xmake

## Usage

```cpp
   int main() {

      LlamaClient client("127.0.0.1", 9001);

      client.AddMessage({LlamaClient::SYSTEM, "you are always have a joke for me" });

      std::cout << client.Request("Hello world!") << std::endl;

      std::cin.get();

      return 0;
   }
```  

   Have fun.
