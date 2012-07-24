#include <cstdlib>
#include <cstdio>

#include "analyzer.hpp"

Forms * forms_new(int count)
{
    Forms * forms = (Forms *) malloc(sizeof(Forms));

    forms -> count = count;
    forms -> current = 0;
    forms -> ids = (char *) malloc(sizeof(char) * 2 * count);
    forms -> prefixes = (char *) malloc(sizeof(char) * count);

    return forms;
}

void forms_free(Forms * forms)
{
    free(forms -> ids);
    free(forms -> prefixes);
    free(forms);
}

int forms_add_form(Forms * forms, const char * id, const char prefix)
{
    for(int i = 0; i < 2; i++)
        forms -> ids[2 * forms -> current + i] = id[i];

    forms -> prefixes[forms -> current] = prefix;

    forms -> current++;

    return forms -> current - 1;
}

Forms * forms_fread(const char * filename)
{
    FILE * file = fopen(filename, "r");

    int count;
    char buffer[3];

    fscanf(file, "%d", &count);
    fgetc(file); // Skipping the newline.

    Forms * forms = forms_new(count);

    for(int i = 0; i < count; i++)
    {
        // Reading id.
        for(int j = 0; j < 2; j++)
            buffer[j] = fgetc(file);

        // Skiping the space.
        fgetc(file);

        // Reading the prefix type.
        buffer[2] = fgetc(file);

        // Skiping the newline.
        fgetc(file);

        // Adding the form.
        forms_add_form(forms, &buffer[0], buffer[2]);
    }

    fclose(file);

    return forms;
}
