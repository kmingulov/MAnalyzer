#ifndef ENDINGS_RULES_HPP
#define ENDINGS_RULES_HPP

/*
    Struct of endings rules. This struct compare each ending id to ids of it's
    rules.
*/
struct EndingsRules
{
    int count;

    // Array with counts of rules for each ending.
    unsigned short int * counts;

    // Array with rules ids.
    unsigned short int ** rules;

    // Array with start indexes for endings' lens (for char * lens).
    unsigned int ** indexes;

    // Array with lens.
    char * lens;
};

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
