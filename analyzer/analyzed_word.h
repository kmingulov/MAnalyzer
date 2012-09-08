/*
    Copyright (c) 2012 K. Mingulov.
    See the LICENSE file for copying permission.
*/

/**
 * @file analyzed_word.h
 * @brief Contains declarations of AnalyzedWord's functions.
 */

#ifndef ANALYZED_WORD_HPP
#define ANALYZED_WORD_HPP

#include "word_infos.h"

/**
 * @brief Word, which Analyzer analyse. Used in static analyzer's functions.
 *        Contains all analysis inormation about word.
 *        Word is stored by parts (prefix + lemma + ending).
 */
struct AnalyzedWord
{
    /// Pointer to the begining of the word.
    char * word;

    /// Size of whole word.
    unsigned int word_size;

    /// Length of the prefix.
    int prefix_len;

    /// Pointer to the begining of the lemma.
    char * lemma;

    /// Length of the lemma.
    int lemma_len;

    /// Pointer to the begining of the ending.
    char * ending;

    /// Length of the ending.
    int ending_len;

    /// Found lemma's id.
    int lemma_id;

    /// Found prefix's id.
    int prefix_id;

    /// Array with grammar infos.
    WordInfos * infos;
};

/**
 * @brief Creates new analyzed word.
 * @param word      pointer to the word
 * @param word_size length of the word
 * @param buffer    buffer to store grammar infos to
 * @return created AnalyzedWord
 */
AnalyzedWord * analyzed_word_new(char * word, unsigned int word_size, WordInfos * buffer);

/**
 * @brief Frees AnalyzedWord. Note: this function won't free AnalyzedWord::word
 *        (pointer to whole word), by which AnalyzedWord was created. You need
 *        to free it manually.
 * @param aw AnalyzedWord
 */
void analyzed_word_free(AnalyzedWord * aw);

/**
 * @brief Clean up function. Removes all information about prefix. Note: this
 *        function will spawn analyzed_word_lemma_clean_up() and
 *        analyzed_word_ending_clean_up() and clean up all lemma's and ending's
 *        information. This is done because of this information uselessness (it
 *        is based on prefix data).
 * @param aw AnalyzedWord
 */
void analyzed_word_prefix_clean_up(AnalyzedWord * aw);

/**
 * @brief Clean up function. Removes all information about lemma. Note: this
 *        function will spawn analyzed_word_ending_clean_up() (the same reason
 *        as for analyzed_word_prefix_clean_up() function).
 * @param aw AnalyzedWord
 */
void analyzed_word_lemma_clean_up(AnalyzedWord * aw);

/**
 * @brief Clean up function. Removes all information about ending.
 * @param aw AnalyzedWord
 */
void analyzed_word_ending_clean_up(AnalyzedWord * aw);

#endif
