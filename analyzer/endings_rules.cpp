#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "endings_rules.hpp"

//******************************************************************************
// STRUCT
//******************************************************************************

struct EndingsRules
{
    int count;
    unsigned short int ** rules;
};

//******************************************************************************
// LEMMAS RULES READING/FREEING
//******************************************************************************

EndingsRules * endings_rules_fread(const char * filename)
{
    EndingsRules * rules = (EndingsRules *) malloc(sizeof(EndingsRules));

    FILE * file = fopen(filename, "r");

    // Reading count of them.
    fscanf(file, "%d", &(rules -> count));
    rules -> rules = (unsigned short int **) malloc(sizeof(unsigned short int *) * rules -> count);

    char * buffer = (char *) calloc(1024, sizeof(char));

    for(int i = 0; i < rules -> count; i++)
    {
        // Reading count of rules for this lemma.
        int count = 0;
        fscanf(file, "%d", &count);

        // +1 for count.
        rules -> rules[i] = (unsigned short int *) malloc(sizeof(unsigned short int) * (count + 1));
        rules -> rules[i][0] = (unsigned short int) count;

        for(int j = 0; j < count; j++)
            fscanf(file, "%d", &(rules -> rules[i][j + 1]));
    }

    fclose(file);
    free(buffer);

    return rules;
}

void endings_rules_free(EndingsRules * rules)
{
    for(int i = 0; i < rules -> count; i++)
        free(rules -> rules[i]);

    free(rules -> rules);
    free(rules);
}


//******************************************************************************
// ACCESS DATA
//******************************************************************************

unsigned short int * endings_rules_get(EndingsRules * rules, unsigned int lemma_id)
{
    return rules -> rules[lemma_id];
}
