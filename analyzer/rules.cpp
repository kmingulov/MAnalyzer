#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>

#include "rules.hpp"

Rules * rules_dread(const char * dirname)
{
    Rules * rules = new Rules;

    char path[1024];
    sprintf(&path[0], "%s/forms", dirname);

    rules -> forms = forms_fread(&path[0]);

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
    forms_free(rules -> forms);
    delete rules;
}
