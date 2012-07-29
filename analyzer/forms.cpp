#include <cstdlib>
#include <cstdio>

#include "analyzer.hpp"

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
    free(forms -> forms);
    free(forms);
}

Forms * forms_fread(const char * filename)
{
    FILE * file = fopen(filename, "r");

    int count, forms_count;
    unsigned short int id;
    unsigned short int prefix;

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
            forms -> forms[i][j].id = id;
        }
    }

    fclose(file);

    return forms;
}
