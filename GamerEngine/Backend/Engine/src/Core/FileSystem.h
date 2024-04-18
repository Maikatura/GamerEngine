#pragma once
#include <filesystem>
#include "Buffer.h"

namespace GamerEngine {

    class FileSystem
    {
    public:
        // TODO: move to FileSystem class
        static Buffer ReadFileBinary(const std::filesystem::path& filepath);
    };

}