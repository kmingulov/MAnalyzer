#include <vector>
#include <fstream>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "analyzer/analyzer.hpp"

void analyze_file(Analyzer * analyzer, const char * filename)
{
    std::vector <char *> text;
    std::vector <int> sizes;
    std::ifstream input(filename);
    std::string str;

    // Reading all file to the memory.
    while(input >> str)
    {
        sizes.push_back(str.size());
        char * c = (char *) malloc(sizeof(char) * (str.size() + 1));
        strcpy(c, str.c_str());
        text.push_back(c);
    }

    int len = text.size();
    for(int i = 0; i < len; i++)
        if(analyzer_get_word_info(analyzer, text[i], sizes[i], NULL))
        {
            //~ printf("%d\n", 1);
        }
        else
        {
            printf("%s\n", text[i]);
            //~ printf("%d\n", 0);
        }
}

int main()
{
    Analyzer * analyzer = analyzer_new();

    //~ analyze_file(analyzer, "input2");

    char word[] = {"ÄÎÐÎÃÎÞ"};
    WordInfos * wi = infos_new(1024);
    int word_size = strlen(word);
    if(analyzer_get_word_info(analyzer, word, word_size, wi))
        std::cout << "Found: ";

    for(int i = 0; i < wi -> size; i++)
        std::cout << "\t" << wi -> infos[i].n_form << "\t" << wi -> infos[i].n_form_id << " " << wi -> infos[i].form_id << std::endl;

    infos_free(wi);

    //~ std::cout << "!\n"; std::cin >> word[0];

    analyzer_free(analyzer);

    return 0;
}
