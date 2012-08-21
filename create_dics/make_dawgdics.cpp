#include <fstream>
#include <algorithm>
#include <dawgdic/dawg-builder.h>
#include <dawgdic/dictionary-builder.h>

#include <cstdlib>

using namespace std;
using namespace dawgdic;

// Creates a dictionary from lexicon file.
void create_dic(const char * input_file, const char * output_file, bool auto_enum = true)
{
    ifstream input(input_file);
    DawgBuilder dawg_builder;

    // Reads words from file stream and adds them to the DAWG.
    string temp;
    int counter = 0;
    while(input >> temp)
        if(auto_enum)
            dawg_builder.Insert(temp.c_str(), counter++);
        else
        {
            input >> counter;
            dawg_builder.Insert(temp.c_str(), counter);
        }

    // Finishes building a dawg.
    Dawg dawg;
    dawg_builder.Finish(&dawg);

    // Builds a dictionary a dawg.
    Dictionary dic;
    DictionaryBuilder :: Build(dawg, &dic);

    // Writes a dictionary into a file.
    ofstream output(output_file, ios::binary);
    dic.Write(&output);
}

int main(int argc, char ** argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Error: too few args.\n");
        return 1;
    }

    char buffer[1024];

    sprintf(buffer, "%s/predict_prefixes.dawgdic", argv[1]);
    create_dic("temp/predict_prefixes_sorted", buffer, false);

    sprintf(buffer, "%s/lemmas.dawgdic", argv[1]);
    create_dic("temp/lemmas", buffer);

    sprintf(buffer, "%s/endings.dawgdic", argv[1]);
    create_dic("temp/ends", buffer);

    return 0;
}
