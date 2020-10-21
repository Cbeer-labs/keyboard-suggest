#ifndef KEYBOARD_SUGGEST_MESSAGES_H
#define KEYBOARD_SUGGEST_MESSAGES_H

#include <string>
#include <vector>

#include "state.h"

struct Config;

struct Answer {
  std::vector<std::string> suggestions;
};

struct Input {
  std::string input;
  std::string last_word;
};

Answer MakeSuggestions(const Input &input, const Storage &storage, const InternalState &old_state,
                       InternalState &new_state);

void InitState(const Config &, InternalState &);

void InitStorage(const Config &, Storage &);

#endif  // KEYBOARD_SUGGEST_MESSAGES_H
