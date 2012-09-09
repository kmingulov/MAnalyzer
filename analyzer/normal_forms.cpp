/**
 * @file normal_forms.cpp
 * @brief Contains implementations of NormalForms' functions.
 */

#include <fstream>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "normal_forms.h"

//******************************************************************************
// STRUCT
//******************************************************************************

/**
 * @struct NormalForm
 * @brief Contains information about _one_ normal form.
 */
struct NormalForm
{
    /// Pointer to normal form's ending.
    char * ending;

    /// Ending's length.
    unsigned short int ending_len;

    /// Normal form's grammar type.
    unsigned short int id;
};

/**
 * @struct NormalForms
 */
struct NormalForms
{
    /// Amount of normal forms.
    unsigned int count;

    /// Array with normal forms.
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
