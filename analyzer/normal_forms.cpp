#include <fstream>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "normal_forms.hpp"

//******************************************************************************
// STRUCT
//******************************************************************************

struct NormalForm
{
    char * ending;
    unsigned short int ending_len;
    unsigned short int id;
    // TODO Remove completely?
    //~ char prefix;
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

        int ending_size = ending.size();

        if(ending == "*")
        {
            nf -> forms[i].ending = NULL;
            nf -> forms[i].ending_len = 0;
        }
        else
        {
            nf -> forms[i].ending = (char *) malloc(sizeof(char) * (ending_size + 1));
            nf -> forms[i].ending_len = ending_size;
            strcpy(nf -> forms[i].ending, ending.c_str());
        }

        nf -> forms[i].id = id;
        //~ nf -> forms[i].prefix = prefix;
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

unsigned short int normal_forms_get_ending_len(NormalForms * nf, unsigned int id)
{
    return nf -> forms[id].ending_len;
}

unsigned short int normal_forms_get_type(NormalForms * nf, unsigned int id)
{
    return nf -> forms[id].id;
}

char normal_forms_get_prefix(NormalForms * nf, unsigned int id)
{
    return 1;
    //~ return nf -> forms[id].prefix;
}
