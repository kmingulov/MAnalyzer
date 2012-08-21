/*
    Copyright (c) 2012 K. Mingulov.
    See the LICENSE file for copying permission.
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "lemmas_rules.hpp"

//******************************************************************************
// STRUCT
//******************************************************************************

struct LemmasRules
{
    int count;
    short int ** rules;
};

//******************************************************************************
// LEMMAS RULES READING/FREEING
//******************************************************************************

LemmasRules * lemmas_rules_fread(const char * filename)
{
    FILE * file = fopen(filename, "r");

    if(file == NULL)
        return NULL;

    LemmasRules * rules = (LemmasRules *) malloc(sizeof(LemmasRules));

    // Reading count of them.
    fscanf(file, "%d", &(rules -> count));
    rules -> rules = (short int **) malloc(sizeof(short int *) * rules -> count);

    char * buffer = (char *) calloc(1024, sizeof(char));

    for(int i = 0; i < rules -> count; i++)
    {
        // Reading count of rules for this lemma.
        int count = 0;
        fscanf(file, "%d", &count);

        // +1 for count.
        rules -> rules[i] = (short int *) malloc(sizeof(short int) * (count + 1));
        rules -> rules[i][0] = (short int) count;

        for(int j = 0; j < count; j++)
            fscanf(file, "%hd", &(rules -> rules[i][j + 1]));
    }

    fclose(file);
    free(buffer);

    return rules;
}

void lemmas_rules_free(LemmasRules * rules)
{
    for(int i = 0; i < rules -> count; i++)
        free(rules -> rules[i]);

    free(rules -> rules);
    free(rules);
}


//******************************************************************************
// ACCESS DATA
//******************************************************************************

short int * lemmas_rules_get(LemmasRules * rules, unsigned int lemma_id)
{
    return rules -> rules[lemma_id];
}
