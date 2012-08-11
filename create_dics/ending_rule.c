#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE         1024
#define SMALL_BUFFER_SIZE   64

int main()
{
    char * buffer = (char *) calloc(BUFFER_SIZE, sizeof(char));

    buffer[BUFFER_SIZE - 1] = '\0'; // Excess, but...
    int current = BUFFER_SIZE - 2;

    // Reading the ending.
    while(buffer[current + 1] != '|')
    {
        buffer[current--] = fgetc(stdin);
    }

    char * ending = &buffer[current + 2];
    printf("%s ", ending);

    // Reading {.
    fgetc(stdin);

    int * nums = (int *) calloc(SMALL_BUFFER_SIZE, sizeof(int));
    int nums_current = 0;
    int ** ids = (int **) calloc(SMALL_BUFFER_SIZE, sizeof(int *));

    // Reading integers.
    int break_at_all = 0;
    while(!break_at_all)
    {
        // Unexpected end. Normally it might be ".
        if(fgetc(stdin) == '}')
            break;

        // Reading the number.
        current = 1;
        while(buffer[current - 1] != '"')
            buffer[current++] = fgetc(stdin);

        buffer[current - 1] = '\0';
        nums[nums_current] = atoi(&buffer[1]);
        ids[nums_current] = (int *) calloc(SMALL_BUFFER_SIZE, sizeof(int));
        int ids_current = 1;

        // Reading ': ['.
        for(int i = 0; i < 3; i++)
            fgetc(stdin);

        // Reading ids.
        current = 0;
        char letter;
        while(1)
        {
            letter = fgetc(stdin);

            if(letter != ',' && letter != ']')
                buffer[current++] = letter;
            else
            {
                buffer[current] = '\0';
                ids[nums_current][ids_current++] = atoi(&buffer[0]);
                current = 0;

                // Space after ',' or ','/'}' after ']'.
                letter = fgetc(stdin);

                if(letter == ',')
                {
                    // Reading space.
                    fgetc(stdin);
                    break;
                }
                else if(letter == '}')
                {
                    break_at_all = 1;
                    break;
                }
            }
        }

        // Remember count.
        ids[nums_current][0] = ids_current - 1;
        nums_current++;
    }

    // Printing result.
    printf("%d ", nums_current);

    for(int i = 0; i < nums_current; i++)
    {
        printf("%d %d ", nums[i], ids[i][0]);
        for(int j = 0; j < ids[i][0]; j++)
            printf("%d ", ids[i][j + 1]);
    }

    printf("\n");

    free(buffer);
}
