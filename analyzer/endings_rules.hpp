#ifndef ENDINGS_RULES_HPP
#define ENDINGS_RULES_HPP

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
    Gets amount of rules for the ending.
*/
unsigned short int endings_rules_get_rules_count(EndingsRules * rules, unsigned int ending_id);

/*
    Gets array with rules' ids.
*/
unsigned short int * endings_rules_get_rules_ids(EndingsRules * rules, unsigned int ending_id);

/*
    Gets array with endings lengths. All lengths greater than or equal 0. Array
    ends with -1.
    Unsigned int id is index number, not rule id!
*/
char * endings_rules_get_endings_lens(EndingsRules * rules, unsigned int ending_id, unsigned int id);

#endif
