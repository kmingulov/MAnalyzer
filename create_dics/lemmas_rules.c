#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE * input = fopen("temp/lemmas_rules", "r");
    FILE * output = fopen("../dics/lemmas_rules", "w");

    int buffer[1024];
    int current = 0;
    int value = 0;
    int success = 0;
    char c = 0;

    while(1)
    {
        success = fscanf(input, "%d", &buffer[current++]);

        if(success == EOF)
            current--;

        c = 0;
        c = fgetc(input);

        if( (c == '\n' || success == EOF) && current != 0 )
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
