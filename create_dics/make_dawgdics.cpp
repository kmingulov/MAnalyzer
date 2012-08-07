#include <fstream>
#include <algorithm>
#include <dawgdic/dawg-builder.h>
#include <dawgdic/dictionary-builder.h>

#include <cstdlib>

using namespace std;
using namespace dawgdic;

// Creates a dictionary from lexicon file.
void create_dic(const char * input_file, const char * output_file)
{
    ifstream input(input_file);
    DawgBuilder dawg_builder;

    // Reads words from file stream and adds them to the DAWG.
    string temp;
    int counter = 0;
    while(input >> temp)
        dawg_builder.Insert(temp.c_str(), counter++);

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

int main()
{
    create_dic("temp/predict_prefixes_sorted", "../dics/predict_prefixes.dawgdic");
    create_dic("temp/lemmas", "../dics/lemmas.dawgdic");
    create_dic("temp/ends", "../dics/endings.dawgdic");

    return 0;
}
