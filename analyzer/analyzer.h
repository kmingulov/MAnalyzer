/*
    Copyright (c) 2012 K. Mingulov.
    See the LICENSE file for copying permission.
*/

#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include "word_infos.h"

/*
    Morph analyzer's struct.
*/
struct Analyzer;

/*
    Loads analyzer. Dictionary is located in dirname dir.
*/
Analyzer * analyzer_new(const char * dirname);

/*
    Unloads analyzer.
*/
void analyzer_free(Analyzer * analyzer);

/*
    Analyzer's main function for morph. analysis.
*/
bool analyzer_get_word_info(Analyzer * analyzer, char * word, unsigned int word_size, WordInfos * buffer);

#endif

