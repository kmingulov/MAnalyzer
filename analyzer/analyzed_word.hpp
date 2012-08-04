#ifndef ANALYZED_WORD_HPP
#define ANALYZED_WORD_HPP

#include "word_infos.hpp"

/*
    Analyzed word. Used in static analyzer's functions.
    Contains information about word.
*/
struct AnalyzedWord
{
    // Whole word and size.
    char * word;
    unsigned int word_size;

    // Pointers to the beginings of the parts.
    char * prefix, * lemma, * ending;

    // Lengths of word's parts.
    int predict_prefix_len, prefix_len, lemma_len, ending_len;

    // Prefix type.
    int prefix_type;

    // Array with infos.
    WordInfos * infos;
};

/*
    Creates new analyzed word.
*/
AnalyzedWord * analyzed_word_new(char * word, unsigned int word_size, WordInfos * buffer);

/*
    Frees analyzed word.
    This function won't free word (arg in analyzed_word_new function)!
*/
void analyzed_word_free(AnalyzedWord * aw);

#endif
