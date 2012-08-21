#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>

using namespace std;

int main(int argc, char ** argv)
{
    if(argc < 3)
    {
        fprintf(stderr, "Error: too few args.\n");
        return 1;
    }

    // Loading gramtab (to move id by short int).
    map <string, unsigned short int> ids;

    unsigned short int number;
    string id, info;

    ifstream gramtab("temp/gramtab");
    while(gramtab >> number >> id >> info)
        ids[id] = number;
    gramtab.close();

    // Make file with normal forms.
    ifstream input(argv[1]);
    ofstream output(argv[2]);

    string ending, pref;
    int prefix;

    while(input >> ending >> id >> pref)
    {
        if(pref == "*")
            prefix = 1;
        else if(pref == "ПО")
            prefix = 2;
        else if(pref == "НАИ")
            prefix = 3;
        else
            prefix = 0;

        map <string, unsigned short int> :: iterator find = ids.find(id);
        if(find != ids.end())
            number = find -> second;
        else
            number = 0;

        output << ending << " " << number << " " << prefix << endl;
    }

    return 0;
}
