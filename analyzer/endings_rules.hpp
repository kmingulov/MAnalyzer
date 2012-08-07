#ifndef ENDINGS_RULES_HPP
#define ENDINGS_RULES_HPP

// TODO: Identical to LemmasRules. Use templates or not?

/*
    Struct of endings rules. This struct compare each ending id to ids of it's
    rules.
*/
struct EndingsRules;

/*
    Reads lemmas rules from file.
*/
EndingsRules * endings_rules_fread(const char * filename);

/*
    Frees allocated memory in lemmas rules. (They become useless.)
*/
void endings_rules_free(EndingsRules * rules);

/*
    Gets the pointer to the array with ids of rules.
*/
unsigned short int * endings_rules_get(EndingsRules * rules, unsigned int ending_id);

#endif
