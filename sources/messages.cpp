#include "messages.h"

void InitState(const Config &, InternalState &) {}

void InitStorage(const Config &, Storage &) {}

Answer MakeSuggestions(const Input &input, const Storage &, const InternalState &, InternalState &) {
  return {{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"}};
}
