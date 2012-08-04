#ifndef LEMMAS_RULES_HPP
#define LEMMAS_RULES_HPP

/*
    Struct of lemmas rules. This struct compare each lemma id to ids of it's
    rules.
*/
struct LemmasRules;

/*
    Reads lemmas rules from file.
*/
LemmasRules * lemmas_rules_fread(const char * filename);

/*
    Frees allocated memory in lemmas rules. (They become useless.)
*/
void lemmas_rules_free(LemmasRules * rules);

/*
    Gets the pointer to the array with ids of rules.
*/
short int * lemmas_rules_get(LemmasRules * rules, unsigned int lemma_id);

#endif
