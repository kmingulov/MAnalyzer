/**
 * @file rules.cpp
 * @brief Contains implementaions of Rules' functions.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>

#include "forms.h"
#include "rules.h"

//******************************************************************************
// STRUCT
//******************************************************************************

/**
 * @struct Rules
 */
struct Rules
{
    /// Amount of rules (and dictionaries).
    unsigned int count;

    /// Array with dictionaries.
    dawgdic::Dictionary * dics;
};

//******************************************************************************
// CREATION/DESTROYING
//******************************************************************************

Rules * rules_dread(const char * dirname)
{
    Rules * rules = (Rules *) malloc(sizeof(Rules));

    // Buffer for file paths.
    char * path = (char *) malloc(sizeof(char) * 1024);

    // Reading count.
    sprintf(path, "%s/info", dirname);
    std::ifstream info(path);
    info >> rules -> count;
    info.close();

    rules -> dics = new dawgdic::Dictionary [rules -> count];

    for(int i = 0; i < rules -> count; i++)
    {
        sprintf(path, "%s/%d.dawgdic", dirname, i);

        std::ifstream file(path, std::ios::binary);
        rules -> dics[i].Read(&file);
    }

    free(path);

    return rules;
}

void rules_free(Rules * rules)
{
    delete [] rules -> dics;
    free(rules);
}

//******************************************************************************
// GETTING INFO
//******************************************************************************

bool rules_find_ending_in_rule(Rules * rules, unsigned int id, const char * ending, int ending_len, int * buffer)
{
    return rules -> dics[id].Find(ending, ending_len, buffer);
}
