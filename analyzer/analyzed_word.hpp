/*
    Copyright (c) 2012 K. Mingulov.
    See the LICENSE file for copying permission.
*/

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
    char * lemma, * ending;

    // Lengths of word's parts.
    int prefix_len, lemma_len, ending_len;

    // Id of the lemma.
    int lemma_id;

    // Prefix id.
    int prefix_id;

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

/*
    Clean up functions. They delete specified data from word.
    Note: prefix_clean_up() will call lemma_clean_up(), lemma_clean_up() will 
    call ending_clean_up(). I.e. when you delete data about prefix,
    lemma and endings data become useless -- because they are based on prefix
    data.
*/
void analyzed_word_prefix_clean_up(AnalyzedWord * aw);
void analyzed_word_lemma_clean_up(AnalyzedWord * aw);
void analyzed_word_ending_clean_up(AnalyzedWord * aw);

#endif
