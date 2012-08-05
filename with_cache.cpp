#include <vector>
#include <fstream>
#include <map>

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "analyzer/word_infos.hpp"
#include "analyzer/analyzer.hpp"

int main()
{
    Analyzer * analyzer = analyzer_new();
    WordInfos * wi = infos_new(1024);
    std::map <std::string, bool> cache;

    std::vector <std::string> text;
    std::vector <int> sizes;
    std::ifstream input("input");
    std::string str;

    // Reading all file to the memory.
    while(input >> str)
    {
        sizes.push_back(str.size());
        text.push_back(str);
    }

    char buffer[1024];
    int len = text.size();
    for(int i = 0; i < len; i++)
    {
        std::map <std::string, bool> :: iterator iter = cache.find(text[i]);

        if(iter != cache.end())
        {
            strcpy(&buffer[0], text[i].c_str());
            analyzer_get_word_info(analyzer, &buffer[0], sizes[i], wi);
            infos_erase(wi);
            cache[text[i]] = true;
        }
    }

    infos_free(wi);
    analyzer_free(analyzer);

    return 0;
}
