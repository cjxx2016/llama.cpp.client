# llama.cpp.client

   c++ client of llama.cpp's llama-server

## Build

   xmake

## Usage

```cpp
   int main() {

      LlamaClient client("127.0.0.1", 9001);

      client.Add({LlamaClient::SYS, "you are always have a joke for me" });

      std::cout << client.Talk("Hello world!") << std::endl;

      return std::cin.get();
   }
```  

   Have fun.
