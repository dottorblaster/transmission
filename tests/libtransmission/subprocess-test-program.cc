// This file Copyright (C) 2017-2022 Mnemosyne LLC.
// It may be used under GPLv2 (SPDX: GPL-2.0-only), GPLv3 (SPDX: GPL-3.0-only),
// or any future license endorsed by Mnemosyne LLC.
// License text can be found in the licenses/ folder.

#include <libtransmission/file.h> // tr_sys_dir_get_current()
#include <libtransmission/utils.h> // tr_env_get_string()

#include <fmt/core.h>

#include <cstdio>
#include <string>

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        return 1;
    }

    auto const result_path = std::string{ argv[1] };
    auto const test_action = std::string{ argv[2] };
    auto const tmp_result_path = result_path + ".tmp";

    FILE* out = std::fopen(tmp_result_path.c_str(), "w+");
    if (out == nullptr)
    {
        return 1;
    }

    if (test_action == "--dump-args")
    {
        for (int i = 3; i < argc; ++i)
        {
            fmt::print(out, "{:s}\n", argv[i]);
        }
    }
    else if (test_action == "--dump-env")
    {
        for (int i = 3; i < argc; ++i)
        {
            fmt::print(out, "{:s}\n", tr_env_get_string(argv[i], "<null>"));
        }
    }
    else if (test_action == "--dump-cwd")
    {
        auto cwd = tr_sys_dir_get_current(nullptr);

        if (std::empty(cwd))
        {
            cwd = "<null>";
        }

        fmt::print(out, "{:s}\n", cwd);
    }
    else
    {
        std::fclose(out);
        std::remove(tmp_result_path.c_str());
        return 1;
    }

    std::fclose(out);
    tr_sys_path_rename(tmp_result_path.c_str(), result_path.c_str());
    return 0;
}
