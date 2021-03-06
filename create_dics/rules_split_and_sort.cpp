#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>

using namespace std;

int main()
{
    // Loading gramtab (to move id by short int).
    map <string, unsigned short int> ids;

    unsigned short int number;
    string id, info;

    ifstream ids_file("temp/gramtab");
    while(ids_file >> number >> id >> info)
        ids[id] = number;
    ids_file.close();

    // Loading possible prefixes (that are enumerated from 1).
    map <string, unsigned short int> prefs;

    ifstream prefs_file("temp/possible_prefixes");
    while(prefs_file >> id >> number)
        prefs[id] = number;
    prefs_file.close();

    // Making rules.
    char buffer[1024];
    ifstream input("temp/rules_with_counts");

    int count;
    int forms_count;

    input >> count;

    string pref, end;
    unsigned short int prefix;

    for(int i = 0; i < count; i++)
    {
        sprintf(&buffer[0], "temp/splitted/%d", i);
        ofstream output(&buffer[0]);

        input >> forms_count;

        for(int j = 0; j < forms_count; j++)
        {
            input >> end >> id >> pref;

            map <string, unsigned short int> :: iterator find = prefs.find(pref);
            if(find != prefs.end())
                prefix = find -> second;
            else
                prefix = 0;

            find = ids.find(id);
            if(find != ids.end())
                number = find -> second;
            else
                number = 0;

            output << end << " " << number << " " << prefix << endl;
        }

        output.close();

        sprintf(&buffer[0], "cd temp/splitted/; sort -o %d_ %d; mv %d_ %d; sed -i \"1i`wc -l %d | cut -f1 --delimiter=' '`\" %d; cd ../..;", i, i, i, i, i, i);
        system(&buffer[0]);
    }

    input.close();

    ofstream output("temp/splitted/info");
    output << count << endl;
    output.close();

    return 0;
}
