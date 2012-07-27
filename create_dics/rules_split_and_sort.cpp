#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;

int main()
{
    char buffer[1024];

    ifstream input("temp/rules_with_counts");

    int count;
    int forms_count;

    input >> count;

    for(int i = 0; i < count; i++)
    {
        sprintf(&buffer[0], "temp/splitted/%d", i);
        string pref, id, end;

        ofstream output(&buffer[0]);

        input >> forms_count;

        output << forms_count << endl;

        for(int j = 0; j < forms_count; j++)
        {
            input >> end >> id >> pref;

            output << end << " " << id << " " << pref << endl;
        }

        output.close();

        sprintf(&buffer[0], "cd temp/splitted/; sort -o %d_ %d; mv %d_ %d; cd ../..;", i, i, i, i);
        system(&buffer[0]);
    }

    input.close();

    return 0;
}
