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

//******************************************************************************
// CREATION/DESTROYING
//******************************************************************************

NormalForm * normal_forms_fread(const char * filename)
{
    std::ifstream input(filename);

    int count;
    input >> count;

    NormalForm * nf = (NormalForm *) malloc(sizeof(NormalForm) * count);

    std::string ending;
    unsigned short int id, prefix;

    for(int i = 0; i < count; i++)
    {
        input >> ending >> id >> prefix;

        nf[i].ending = (char *) malloc(sizeof(char) * (ending.size() + 1));
        strcpy(nf[i].ending, ending.c_str());

        nf[i].id = id;
        nf[i].prefix = prefix;
    }

    return nf;
}

// TODO Write normal_forms_free() function.

//******************************************************************************
// GET DATA.
//******************************************************************************

char * normal_forms_get_ending(NormalForm * nf, unsigned int id)
{
    return nf[id].ending;
}

unsigned short int normal_forms_get_type(NormalForm * nf, unsigned int id)
{
    return nf[id].id;
}

char normal_forms_get_prefix(NormalForm * nf, unsigned int id)
{
    return nf[id].prefix;
}
