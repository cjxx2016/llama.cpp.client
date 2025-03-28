# llama.cpp.client

   c++ client of llama.cpp's llama-server

## Build

   xmake

## Usage

```cpp
   int main() {

      LlamaClient client("127.0.0.1", 9001);

      client.Add({LlamaClient::SYS, "You often joke with me" });

      std::cout << client.Talk("Hello world!") << std::endl;

      return std::cin.get();
   }
```  

   Have fun.
