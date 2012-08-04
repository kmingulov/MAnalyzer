#include <cstdlib>
#include <cstdio>

#include "analyzed_word.hpp"

//******************************************************************************
// STRUCT (DEFINED IN ANALYZED_WORD.HPP)
//******************************************************************************

struct AnalyzedWord;

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

    // Make 0 everything else. 0 means "not defined".
    aw -> predict_prefix_len = 0;
    aw -> prefix_len = 0;
    aw -> lemma_len = 0;
    aw -> ending_len = 0;
    aw -> prefix_type = 0;

    return aw;
}

void analyzed_word_free(AnalyzedWord * aw)
{
    free(aw);
}
