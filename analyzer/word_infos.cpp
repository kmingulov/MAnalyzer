#include <cstdio>
#include <cstdlib>

#include "analyzer.hpp"

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

void infos_erase(WordInfos * wi)
{
    for(int i = 0; i < wi -> size; i++)
        if(wi -> infos[i].n_form != NULL)
            free(wi -> infos[i].n_form);

    wi -> size = 0;

    wi -> prepend_error = false;
    wi -> prediction = false;
}

bool infos_prepend_word(WordInfos * wi, char * form, unsigned short int n_form_id, unsigned short int form_id)
{
    if(wi -> size == wi -> max_size)
    {
        wi -> prepend_error = true;
        return false;
    }

    wi -> infos[wi -> size].n_form = form;
    wi -> infos[wi -> size].n_form_id = n_form_id;
    wi -> infos[wi -> size].form_id = form_id;

    wi -> size++;

    return true;
}
