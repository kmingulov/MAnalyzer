#include <cstdio>
#include <cstdlib>

#include "word_infos.hpp"

//******************************************************************************
// STRUCTS
//******************************************************************************

struct WordInfo
{
    // Normal form and it's gram. type.
    char * n_form;
    unsigned short int n_form_id;

    // Grammar type of word.
    unsigned short int form_id;
};

struct WordInfos
{
    // Current size and max size of array.
    unsigned int size;
    unsigned int max_size;

    // Flag "prepend error": cannot add new element, because array is full.
    bool prepend_error;

    // Flag "prediction": these results was get by prediction. This word is
    // unknown.
    bool prediction;

    WordInfo * infos;
};

//******************************************************************************
// CREATION/DESTROYING
//******************************************************************************

WordInfos * infos_new(unsigned int max_size)
{
    WordInfos * wi = (WordInfos *) malloc(sizeof(WordInfos));

    wi -> size = 0;
    wi -> max_size = max_size;

    wi -> prepend_error = false;
    wi -> prediction = false;

    wi -> infos = (WordInfo *) malloc(sizeof(WordInfo) * max_size);

    return wi;
}

void infos_free(WordInfos * wi)
{
    for(int i = 0; i < wi -> size; i++)
        if(wi -> infos[i].n_form != NULL)
            free(wi -> infos[i].n_form);

    free(wi -> infos);
    free(wi);
}

//******************************************************************************
// CHANGING DATA
//******************************************************************************

void infos_erase(WordInfos * wi)
{
    for(int i = 0; i < wi -> size; i++)
        if(wi -> infos[i].n_form != NULL)
            free(wi -> infos[i].n_form);

    wi -> size = 0;

    wi -> prepend_error = false;
    wi -> prediction = false;
}

bool infos_prepend_word(WordInfos * wi, char * form, unsigned short int n_form_id, unsigned short int form_id, bool prediction)
{
    if(wi == NULL)
        return false;

    if(wi -> size == wi -> max_size)
    {
        wi -> prepend_error = true;
        return false;
    }

    if(prediction)
        wi -> prediction = true;

    wi -> infos[wi -> size].n_form = form;
    wi -> infos[wi -> size].n_form_id = n_form_id;
    wi -> infos[wi -> size].form_id = form_id;

    wi -> size++;

    return true;
}

//******************************************************************************
// GETTING DATA
//******************************************************************************

int infos_get_size(WordInfos * wi)
{
    return wi -> size;
}

char * infos_get_normal_form(WordInfos * wi, unsigned int id)
{
    return wi -> infos[id].n_form;
}

unsigned short int infos_get_normal_form_id(WordInfos * wi, unsigned int id)
{
    return wi -> infos[id].n_form_id;
}

unsigned short int infos_get_form_id(WordInfos * wi, unsigned int id)
{
    return wi -> infos[id].form_id;
}

//******************************************************************************
// CHECKS OF WORD'S INFOS ARRAY STATES
//******************************************************************************

bool infos_is_prediction(WordInfos * wi)
{
    return wi -> prediction;
}

bool infos_have_prepend_word(WordInfos * wi)
{
    return wi -> prepend_error;
}
