#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
    if(argc < 3)
    {
        fprintf(stderr, "Error: too few args.\n");
        return 1;
    }

    FILE * input = fopen(argv[1], "r");
    FILE * output = fopen(argv[2], "w");

    if(input == NULL || output == NULL)
    {
        fprintf(stderr, "Error: cannot open file.\n");
        return 1;
    }

    int buffer[1024];
    int current = 0;
    int value = 0;
    int success = 0;
    char c = 0;

    while(1)
    {
        success = fscanf(input, "%d", &buffer[current++]);

        if(buffer[current - 1] == -1)
            current--;

        if(success == EOF)
            current--;

        c = 0;
        c = fgetc(input);

        if( (c == '\n' || success == EOF) )
        {
            fprintf(output, "%d ", current);
            for(int i = 0; i < current; i++)
            {
                fprintf(output, "%d ", buffer[i]);
            }
            fprintf(output, "\n");
            current = 0;
        }

        if(success == EOF)
            break;
    }

    fclose(output);
    fclose(input);

    return 0;
}
