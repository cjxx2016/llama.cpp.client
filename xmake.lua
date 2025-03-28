add_rules("mode.debug", "mode.release")

target("llama.cpp.client")
    set_kind("binary")
    set_symbols("debug")
    add_files("Test.cc")
    add_files("LlamaClient/LlamaClient.cc")

    after_build(function (target)
        os.cp(target:targetfile(), "./".. path.filename(target:targetfile()))
    end)

-- switch to debug with `xmake f -m debug`
-- switch to release with `xmake f -m release`
-- build with `xmake`
-- run with `./llama.cpp.client`
