#include "messages.h"
#include <cstdio>

void InitState(const Config &, InternalState &) {}

void InitStorage(const Config &, Storage &) {}

Answer MakeSuggestions(const Input &, const Storage &, const InternalState &, InternalState &) {
    std::vector<std::string> result;
    char* buf = new char[256];
    for (size_t i = 0; i < 20; ++i) {
        snprintf(buf, 256, "%lu", i);
        result.push_back(buf);
    }
    delete[] buf;
    return Answer { result };
}
