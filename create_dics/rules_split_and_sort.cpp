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

    ifstream gramtab("temp/gramtab");
    while(gramtab >> number >> id >> info)
        ids[id] = number;
    gramtab.close();

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
        output << forms_count << endl;

        for(int j = 0; j < forms_count; j++)
        {
            input >> end >> id >> pref;

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

            output << end << " " << number << " " << prefix << endl;
        }

        output.close();

        sprintf(&buffer[0], "cd temp/splitted/; sort -o %d_ %d; mv %d_ %d; cd ../..;", i, i, i, i);
        system(&buffer[0]);
    }

    input.close();

    ofstream output("temp/splitted/info");
    output << count << endl;
    output.close();

    return 0;
}
