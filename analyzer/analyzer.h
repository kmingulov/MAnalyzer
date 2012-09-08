/*
    Copyright (c) 2012 K. Mingulov.
    See the LICENSE file for copying permission.
*/

/**
 * @file analyzer.h
 * @brief Contains declarations of Analyzer's functions.
 */

#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include "word_infos.h"

/**
 * @struct Analyzer
 *
 * @brief Morph analyzer's struct. Implements morph analysis: it allow to find
 *        grammar info of the word and its normal form. There is a prediction
 *        for unknown words.
 */
struct Analyzer;

/**
 * @brief Creates new Analyzer.
 * @param dirname path to the dictionary
 * @return pointer to created Analyzer, if succesed
 */
Analyzer * analyzer_new(const char * dirname);

/**
 * @brief Destroys Analyzer.
 * @param analyzer pointer to an existing Analyzer
 */
void analyzer_free(Analyzer * analyzer);

/**
 * @brief Gets word's grammar information.
 * @param analyzer  pointer to an existing Analyzer
 * @param word      word to analyse
 * @param word_size word's length
 * @param buffer    buffer to place analysis' results to
 * @return true, if word was analyzed succesfully
 * @return false, if analysis and prediction failed
 */
bool analyzer_get_word_info(Analyzer * analyzer, char * word, unsigned int word_size, WordInfos * buffer);

#endif

