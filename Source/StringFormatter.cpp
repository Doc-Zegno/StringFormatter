#include "StringFormatter.h"


namespace Handmada {
    StringFormatter operator"" _sf(const char* pattern, size_t size)
    {
        return StringFormatter(std::string(pattern, size));
    }
}