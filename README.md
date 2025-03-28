# llama.cpp.client

   c++ client of llama.cpp's llama-server

## Build

   xmake

## Usage

```cpp
   int main() {

      LlamaClient client("127.0.0.1", 9001);

      std::cout << client.Ask("Hello world!") << std::endl;

      return std::cin.get();
   }
```  

   Have fun.
