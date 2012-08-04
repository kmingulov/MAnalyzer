#include <fstream>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "analyzer.hpp"

//******************************************************************************
// STRUCT
//******************************************************************************

struct NormalForm
{
    char * ending;
    unsigned short int id;
    char prefix;
};

struct NormalForms
{
    unsigned int count;
    NormalForm * forms;
};

//******************************************************************************
// CREATION/DESTROYING
//******************************************************************************

NormalForms * normal_forms_fread(const char * filename)
{
    std::ifstream input(filename);

    unsigned int count;
    input >> count;

    NormalForms * nf = (NormalForms *) malloc(sizeof(NormalForms));
    nf -> count = count;
    nf -> forms = (NormalForm *) malloc(sizeof(NormalForm) * count);

    std::string ending;
    unsigned short int id, prefix;

    for(int i = 0; i < count; i++)
    {
        input >> ending >> id >> prefix;

        nf -> forms[i].ending = (char *) malloc(sizeof(char) * (ending.size() + 1));
        strcpy(nf -> forms[i].ending, ending.c_str());

        nf -> forms[i].id = id;
        nf -> forms[i].prefix = prefix;
    }

    return nf;
}

void normal_forms_free(NormalForms * nf)
{
    for(int i = 0; i < nf -> count; i++)
        free(nf -> forms[i].ending);

    free(nf -> forms);
    free(nf);
}

//******************************************************************************
// GET DATA.
//******************************************************************************

char * normal_forms_get_ending(NormalForms * nf, unsigned int id)
{
    return nf -> forms[id].ending;
}

unsigned short int normal_forms_get_type(NormalForms * nf, unsigned int id)
{
    return nf -> forms[id].id;
}

char normal_forms_get_prefix(NormalForms * nf, unsigned int id)
{
    return nf -> forms[id].prefix;
}
