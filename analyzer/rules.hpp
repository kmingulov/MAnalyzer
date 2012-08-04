#ifndef RULES_HPP
#define RULES_HPP

#include <dawgdic/dictionary.h>

/*
    Struct with rules. Each rule is an another dawgdic with endings-keys.
*/
struct Rules;

/*
    Loads rules from _directory_. Each rule is in *.dawgdic file. Forms are in
    forms file.
*/
Rules * rules_dread(const char * dirname);

/*
    Frees rules.
*/
void rules_free(Rules * rules);

/*
    Returns true if ending is in rule with id. If true, buffer will contain
    value of this ending.
*/
bool rules_find_ending_in_rule(Rules * rules, unsigned int id, const char * ending, int ending_len, int * buffer);

#endif

