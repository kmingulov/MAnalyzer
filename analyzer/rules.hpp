#ifndef RULES_HPP
#define RULES_HPP

#include <dawgdic/dictionary.h>

#include "forms.hpp"

struct Rules
{
    // All forms.
    Forms * forms;

    // Dics (with endings) for rules.
    // TODO Constant?
    dawgdic::Dictionary dics[2766];
};

/*
    Loads rules from _directory_. Each rule is in *.dawgdic file. Forms are in
    forms file.
*/
Rules * rules_dread(const char * dirname);

/*
    Frees rules.
*/
void rules_free(Rules * rules);

#endif

