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
        analyzer_search_lemmas(analyzer, text[i], sizes[i]);
}

int main()
{
    Analyzer * analyzer = analyzer_new();

    char word[] = {"якнбн"};
    int word_size = strlen(word);
    analyzer_search_lemmas(analyzer, word, word_size);

    analyzer_free(analyzer);

    return 0;
}
