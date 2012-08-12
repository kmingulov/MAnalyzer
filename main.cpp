#include <vector>
#include <fstream>

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "analyzer/word_infos.hpp"
#include "analyzer/analyzer.hpp"

void analyze_file(Analyzer * analyzer, const char * filename)
{
    std::vector <char *> text;
    std::vector <int> sizes;
    std::ifstream input(filename);
    std::string str;

    WordInfos * wi = infos_new(1024);

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
    {
        if(analyzer_get_word_info(analyzer, text[i], sizes[i], wi))
        {
            //~ printf("%d\n", 1);
        }
        else
        {
            printf("%s\n", text[i]);
            //~ printf("%d\n", 0);
        }
        infos_erase(wi);
    }

    infos_free(wi);
}

int main()
{
    Analyzer * analyzer = analyzer_new("dics");

    //~ analyze_file(analyzer, "input");

    char buffer[1024];
    WordInfos * wi = infos_new(1024);

    while(fgets(&buffer[0], 1024, stdin))
    {
        int len = strlen(&buffer[0]) - 1;
        buffer[len] = '\0';

        analyzer_get_word_info(analyzer, &buffer[0], len, wi);
    }

    infos_free(wi);

    analyzer_free(analyzer);

    return 0;
}
