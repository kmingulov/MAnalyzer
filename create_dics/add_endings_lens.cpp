#include <fstream>
#include <vector>
using namespace std;

int main()
{
    ifstream rules("temp/rules_with_counts");

    vector < vector <int> > lens;

    int count = 0;
    rules >> count;

    for(int i = 0; i < count; i++)
    {
        int forms_count;
        rules >> forms_count;

        vector <int> temp;

        string end, id, pref;
        for(int j = 0; j < forms_count; j++)
        {
            rules >> end >> id >> pref;
            // temp/rules_with_counts is in UTF8. We use cp1251.
            temp.push_back(end.size() / 2);
        }

        lens.push_back(temp);
    }

    rules.close();

    ifstream input("temp/endings_rules");
    ofstream output("../dics/endings_rules");

    input >> count;
    output << count << endl;
    for(int i = 0; i < count; i++)
    {
        int forms_count;
        int id;
        int endings_count;

        input >> forms_count;
        output << forms_count << " ";

        for(int j = 0; j < forms_count; j++)
        {
            input >> id >> endings_count;
            output << id << " " << endings_count << " ";
            for(int j = 0; j < endings_count; j++)
            {
                int temp;
                input >> temp;
                output << lens[id][temp] << " ";
            }
        }
        output << endl;
    }

    return 0;
}
