#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>

#include "forms.hpp"
#include "rules.hpp"

//******************************************************************************
// STRUCT
//******************************************************************************

struct Rules
{
    // Dics (with endings) for rules.
    // TODO Constant?
    dawgdic::Dictionary dics[2766];
};

//******************************************************************************
// CREATION/DESTROYING
//******************************************************************************

Rules * rules_dread(const char * dirname)
{
    Rules * rules = new Rules;

    char path[1024];

    // TODO Constant?
    for(int i = 0; i < 2766; i++)
    {
        sprintf(&path[0], "%s/%d.dawgdic", dirname, i);

        std::ifstream file(&path[0], std::ios::binary);
        rules -> dics[i].Read(&file);
    }

    return rules;
}

void rules_free(Rules * rules)
{
    delete rules;
}

//******************************************************************************
// GETTING INFO
//******************************************************************************

bool rules_find_ending_in_rule(Rules * rules, unsigned int id, const char * ending, int ending_len, int * buffer)
{
    return rules -> dics[id].Find(ending, ending_len, buffer);
}
