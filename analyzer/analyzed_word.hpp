#ifndef ANALYZED_WORD_HPP
#define ANALYZED_WORD_HPP

/*
    Analyzed word. Used in static analyzer's functions.
    Contains information about word.
*/
struct AnalyzedWord;

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
