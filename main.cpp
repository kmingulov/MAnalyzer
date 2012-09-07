#include <vector>
#include <iostream>
#include <fstream>

#include <cstring>
#include <cstdlib>

#include "analyzer/word_infos.h"
#include "analyzer/analyzer.h"

int main()
{
    Analyzer * analyzer = analyzer_new("dics_ru");
    WordInfos * wi = infos_new(1024);

    std::ifstream input("dics_ru/gramtab");
    std::vector <std::string> gramtab;
    int num;
    std::string id, info;

    while(input >> num >> id >> info)
        gramtab.push_back(info);

    std::string word;
    char * buffer = (char *) malloc(sizeof(char) * 1024);

    while(std::cin >> word)
    {
        strcpy(buffer, word.c_str());
        analyzer_get_word_info(analyzer, buffer, word.size(), wi);

        #ifndef MANALYZER_DEBUG
            std::cout << infos_get_size(wi) << " (prediction = " << infos_is_prediction(wi) << "):\n";
            for(int i = 0; i < infos_get_size(wi); i++)
                std::cout << "\t" << buffer <<
                    " " << gramtab[infos_get_form_id(wi, i) - 1] << " -> " <<
                    infos_get_normal_form(wi, i) <<
                    " " << gramtab[infos_get_normal_form_id(wi, i) - 1] << "\n";
        #endif

        infos_erase(wi);
    }

    free(buffer);
    infos_free(wi);
    analyzer_free(analyzer);

    return 0;
}
