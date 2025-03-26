add_rules("mode.debug", "mode.release")

target("llama.cpp.client")
    set_kind("binary")
    add_files("*.cc")
    add_files("LlamaClient/LlamaClient.cc")
