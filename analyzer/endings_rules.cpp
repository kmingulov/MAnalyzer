#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "endings_rules.hpp"

//******************************************************************************
// DEFINES
//******************************************************************************

#define LENS_ARRAY_SIZE                 1024 * 1024

//******************************************************************************
// STRUCT
//******************************************************************************

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

//******************************************************************************
// LEMMAS RULES READING/FREEING
//******************************************************************************

EndingsRules * endings_rules_fread(const char * filename)
{
    EndingsRules * rules = (EndingsRules *) malloc(sizeof(EndingsRules));

    FILE * file = fopen(filename, "r");

    // Reading count of them.
    fscanf(file, "%d", &(rules -> count));

    rules -> counts = (unsigned short int *) malloc(sizeof(unsigned short int) * rules -> count);
    rules -> rules = (unsigned short int **) malloc(sizeof(unsigned short int *) * rules -> count);
    rules -> indexes = (unsigned int **) malloc(sizeof(unsigned int *) * rules -> count);
    rules -> lens = (char *) malloc(sizeof(char) * LENS_ARRAY_SIZE);
    int current = 0;

    char * buffer = (char *) calloc(1024, sizeof(char));

    for(int i = 0; i < rules -> count; i++)
    {
        // Reading count of rules for this lemma.
        int count = 0;
        fscanf(file, "%d", &count);

        rules -> counts[i] = count;

        rules -> rules[i] = (unsigned short int *) malloc(sizeof(unsigned short int) * count);
        rules -> indexes[i] = (unsigned int *) malloc(sizeof(unsigned int) * count);

        for(int j = 0; j < count; j++)
        {
            int id = 0, ends_count = 0;

            fscanf(file, "%d", &id);
            rules -> rules[i][j] = id;

            rules -> indexes[i][j] = current;

            fscanf(file, "%d", &ends_count);

            for(int k = 0; k < ends_count; k++)
            {
                int len = 0;
                fscanf(file, "%d", &len);
                rules -> lens[current++] = len;
            }

            rules -> lens[current++] = -1; // Like \0 for char strings.
        }
    }

    fclose(file);
    free(buffer);

    return rules;
}

void endings_rules_free(EndingsRules * rules)
{
    for(int i = 0; i < rules -> count; i++)
    {
        free(rules -> rules[i]);
        free(rules -> indexes[i]);
    }

    free(rules -> counts);
    free(rules -> rules);
    free(rules -> indexes);
    free(rules -> lens);
    free(rules);
}

//******************************************************************************
// ACCESS DATA
//******************************************************************************

unsigned short int endings_rules_get_rules_count(EndingsRules * rules, unsigned int ending_id)
{
    return rules -> counts[ending_id];
}

unsigned short int * endings_rules_get_rules_ids(EndingsRules * rules, unsigned int ending_id)
{
    return rules -> rules[ending_id];
}

char * endings_rules_get_endings_lens(EndingsRules * rules, unsigned int ending_id, unsigned int id)
{
    unsigned int start_index = rules -> indexes[ending_id][id];
    return &rules -> lens[start_index];
}
