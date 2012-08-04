#include <cstdlib>
#include <cstdio>

#include "forms.hpp"

//******************************************************************************
// STRUCTS
//******************************************************************************

// Defined in forms.hpp file.
struct FormInfo;

struct Forms
{
    int count;
    char * counts;
    FormInfo ** forms;
};

//******************************************************************************
// CREATING/DESTROYING
//******************************************************************************

Forms * forms_new(int count)
{
    Forms * forms = (Forms *) malloc(sizeof(Forms));

    forms -> count = count;
    forms -> counts = (char *) malloc(sizeof(char) * count);
    forms -> forms = (FormInfo **) malloc(sizeof(FormInfo *) * count);

    return forms;
}

void forms_free(Forms * forms)
{
    free(forms -> counts);

    for(int i = 0; i < forms -> count; i++)
        free(forms -> forms[i]);

    free(forms -> forms);
    free(forms);
}

//******************************************************************************
// LOADING
//******************************************************************************

Forms * forms_fread(const char * filename)
{
    FILE * file = fopen(filename, "r");

    int count, forms_count;
    int id, prefix;

    fscanf(file, "%d", &count);

    Forms * forms = forms_new(count);

    for(int i = 0; i < count; i++)
    {
        // Reading amount of forms.
        fscanf(file, "%d", &forms_count);

        forms -> counts[i] = (char) forms_count;
        forms -> forms[i] = (FormInfo *) malloc(sizeof(FormInfo) * forms_count);

        for(int j = 0; j < forms_count; j++)
        {
            fscanf(file, "%d %d", &id, &prefix);
            forms -> forms[i][j].prefix = (char) prefix;
            forms -> forms[i][j].id = (unsigned short int) id;
        }
    }

    fclose(file);

    return forms;
}

//******************************************************************************
// RETRIEVE DATA
//******************************************************************************

char forms_get_length(Forms * forms, unsigned int id)
{
    return forms -> counts[id];
}

FormInfo * forms_get_form_infos(Forms * forms, unsigned int id)
{
    return forms -> forms[id];
}
