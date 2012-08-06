#include <fstream>
#include <algorithm>
#include <dawgdic/dawg-builder.h>
#include <dawgdic/dictionary-builder.h>
using namespace std;
using namespace dawgdic;

// Reverse each word in file.
void reverse_file(const char * input_file, const char * output_file)
{
    ifstream input(input_file);
    ofstream output(output_file);

    string temp;
    while(input >> temp)
    {
        reverse(temp.begin(), temp.end());
        output << temp << endl;
    }

    input.close();
    output.close();
}

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
    // Prepare endings lexicon.
    // Reverse file (in cp-1251 encoding or another 1-byte encoding).
    reverse_file("temp/endings", "temp/endings_reversed");
    // Recode to UTF8 (for sort).
    system("recode -f cp1251..utf8 temp/endings_reversed");
    // Sort.
    system("sort -o temp/endings_sorted temp/endings_reversed");
    // Recode to cp-1251.
    system("recode -f utf8..cp1251 temp/endings_sorted");

    create_dic("temp/predict_prefixes_sorted", "../dics/predict_prefixes.dawgdic");
    create_dic("temp/lemmas", "../dics/lemmas.dawgdic");
    create_dic("temp/endings_sorted", "../dics/endings.dawgdic");

    return 0;
}
