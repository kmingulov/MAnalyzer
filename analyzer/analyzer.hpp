#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <dawgdic/dictionary.h>

#include "word_infos.hpp"

/*
    Morph analyzer's struct.
*/
struct Analyzer;

/*
    Loads analyzer.
*/
Analyzer * analyzer_new();

/*
    Unloads analyzer.
*/
void analyzer_free(Analyzer * analyzer);

/*
    Searches lemmas for word.
    Returns true, if anything found.
*/
bool analyzer_search_lemmas(Analyzer * analyzer, char * word, int word_size, char prefix, WordInfos * buffer);

/*
    Analyzer's main function for morph. analysis.
*/
bool analyzer_get_word_info(Analyzer * analyzer, char * word, int word_size, WordInfos * buffer);

#endif

