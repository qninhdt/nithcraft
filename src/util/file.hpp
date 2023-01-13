#pragma once

#include "core/types.hpp"

namespace nith::util
{
    bool ReadFile(const string &path, string &content);
    string GetFileNameFromPath(const string &path);
} // namespace nith::util
