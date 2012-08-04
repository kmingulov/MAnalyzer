#include <cstdlib>
#include <cstdio>

#include "analyzed_word.hpp"

//******************************************************************************
// STRUCT
//******************************************************************************

struct AnalyzedWord
{
    // Whole word and size.
    char * word;
    unsigned int word_size;

    // Lengths of word's parts.
    int predict_prefix_len, prefix_len, lemma_len, ending_len;

    // Prefix type.
    int prefix_type;

    // Array with infos.
    WordInfos * infos;
};

//******************************************************************************
// CREATION/DESTROYING
//******************************************************************************

AnalyzedWord * analyzed_word_new(char * word, unsigned int word_size, WordInfos * buffer)
{
    AnalyzedWord * aw = (AnalyzedWord *) malloc(sizeof(AnalyzedWord));

    // Add information.
    aw -> word = word;
    aw -> word_size = word_size;
    aw -> infos = buffer;

    // Make -1 everything else. -1 means "not defined".
    aw -> predict_prefix_len = -1;
    aw -> prefix_len = -1;
    aw -> lemma_len = -1;
    aw -> ending_len = -1;
    aw -> prefix_type = -1;

    return aw;
}

void analyzed_word_free(AnalyzedWord * aw)
{
    free(aw);
}
