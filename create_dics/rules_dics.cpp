#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <map>
#include <dawgdic/dawg-builder.h>
#include <dawgdic/dictionary-builder.h>

using namespace std;
using namespace dawgdic;

//******************************************************************************
// DEFINES
// (only for 2-byte unsigned short int!)
//******************************************************************************

// Mask for next-marker.
#define MASK_NEXT_MARKER            32768

// Masks for prefix.
#define MASK_PREFIX_1               16384
#define MASK_PREFIX_2               8192

//******************************************************************************
// FORMS STRUCT FOR CREATING FORMS FILE
// (it doesn't seem like Forms struct in MAnalyzer source code)
//******************************************************************************

struct Forms
{
    int count;
    int current;
    unsigned short int ** forms;
};

Forms * forms_new(int count)
{
    Forms * forms = (Forms *) malloc(sizeof(Forms));

    forms -> current = 0;
    forms -> forms = (unsigned short int **) calloc(count, sizeof(unsigned short int *));

    return forms;
}

int forms_add_form(Forms * forms, unsigned short int id, const char prefix)
{
    // Create new list.
    unsigned short int * si = (unsigned short int *) malloc(sizeof(unsigned short int));
    // Add id.
    *si = id;
    // There is no next element. So don't mark this element.
    // Add prefix (0,1,2,3 are values, so add (0,1,10,11) 2 bits).
    if(prefix & 2)
        *si += MASK_PREFIX_1;
    if(prefix & 1)
        *si += MASK_PREFIX_2;

    forms -> forms[forms -> current] = si;
    forms -> current += 1;

    return forms -> current - 1;
}

void forms_update_form(Forms * forms, unsigned int index, unsigned short int id, const char prefix)
{
    if(index < forms -> current)
    {
        // Counting forms.
        unsigned short int * list = forms -> forms[index];
        int count = 0;

        for(;;)
        {
            count++;
            if( !(*list & MASK_NEXT_MARKER) )
                break;
            else
                list++;
        }

        *list += MASK_NEXT_MARKER;

        // Allocate new memory.
        list = (unsigned short int *) malloc(sizeof(unsigned short int) * (count + 1));

        // Copy old data.
        memcpy(list, forms -> forms[index], sizeof(unsigned short int) * count);
        free(forms -> forms[index]);
        forms -> forms[index] = list;

        // Add new form.
        list[count] = id;
        if(prefix & 2)
            list[count] += MASK_PREFIX_1;
        if(prefix & 1)
            list[count] += MASK_PREFIX_2;
    }
}

//******************************************************************************
// MAIN()
//******************************************************************************
int main()
{
    //~ Forms * f = forms_new(15);
//~ 
    //~ forms_add_form(f, 1, 0);
    //~ for(int i = 2; i < 10; i++)
        //~ forms_update_form(f, 0, i, 0);
//~ 
    //~ for(int i = 0; i < 9; i++)
        //~ cout << (unsigned short int) f -> forms[0][i] << " " << (unsigned short int) (f -> forms[0][i] - MASK_NEXT_MARKER) << endl;

    char buffer[1024];

    ifstream info("temp/splitted/info");
    int count;
    info >> count;
    info.close();

    // Creating forms array.
    Forms * forms = forms_new(150000);

    string end;
    int forms_count, id, pref;
    for(int i = 0; i < count; i++)
    {
        // Opening the file.
        sprintf(&buffer[0], "temp/splitted/%d", i);
        ifstream input(&buffer[0]);

        // Creating new dawg.
        DawgBuilder dawg_builder;

        // Cache.
        map <std::string, int> cache;

        input >> forms_count;

        for(int j = 0; j < forms_count; j++)
        {
            input >> end >> id >> pref;

            std::map <std::string, int> :: iterator find = cache.find(end);
            if(find != cache.end())
                forms_update_form(forms, find -> second, id, pref);
            else
            {
                int form_id = forms_add_form(forms, id, pref);
                dawg_builder.Insert(end.c_str(), form_id);
                cache[end] = form_id;
            }
        }

        input.close();

        sprintf(&buffer[0], "../dics/rules/%d.dawgdic", i);

        // Create dawgdic.
        Dawg dawg;
        dawg_builder.Finish(&dawg);
        Dictionary dic;
        DictionaryBuilder :: Build(dawg, &dic);

        // Save dic to file.
        ofstream output(&buffer[0], ios::binary);
        dic.Write(&output);
    }

    ofstream output("../dics/rules/forms");

    for(int i = 0; i < forms -> current; i++)
    {
        unsigned short int * si = forms -> forms[i];

        int count = 0;
        for(;;)
        {
            count++;
            if( !(*si & MASK_NEXT_MARKER) )
                break;
            else
                si++;
        }

        output << count << " ";

        for(int j = 0; j < count; j++)
        {
            unsigned short int value = forms -> forms[i][j];
            int prefix = 0;

            if(value & MASK_NEXT_MARKER)
                value -= MASK_NEXT_MARKER;

            if(value & MASK_PREFIX_1)
            {
                value -= MASK_PREFIX_1;
                prefix += 2;
            }

            if(value & MASK_PREFIX_2)
            {
                value -= MASK_PREFIX_2;
                prefix += 1;
            }

            output << (unsigned short int) value << " " << prefix << " ";
        }

        output << endl;
    }

    output.close();

    return 0;
}
