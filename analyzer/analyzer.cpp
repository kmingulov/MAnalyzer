/*
    Copyright (c) 2012 K. Mingulov.
    See the LICENSE file for copying permission.
*/

/**
 * @file analyzer.cpp
 * @brief Contains implementations of Analyzer's functions.
 */

#include <fstream>
#include <sstream>

#include <dawgdic/dictionary.h>

#include <cstdlib>
#include <cstring>

#include "forms.h"
#include "rules.h"
#include "lemmas_rules.h"
#include "endings_rules.h"
#include "normal_forms.h"
#include "analyzed_word.h"

#include "analyzer.h"

//******************************************************************************
// DEBUG DEFINES.
//******************************************************************************

/**
 * @def MA_DEBUG
 *
 * MAnalyzer debug macro. If MAnalyzer library compiled with -debug key, debug
 * information will print to stderr while library using.
 *
 * Syntax is similar to printf() syntax.
 */
#ifdef MANALYZER_DEBUG
    #define MA_DEBUG(...) { fprintf(stderr, __VA_ARGS__); }
#else
    #define MA_DEBUG(...) {}
#endif

//******************************************************************************
// STRUCT.
//******************************************************************************

/**
 * @struct Analyzer
*/
struct Analyzer
{
    /// @brief DAWG dictionary with prefixes.
    dawgdic::Dictionary predict_prefixes;
    /// @brief DAWG dictionary with lemmas.
    dawgdic::Dictionary lemmas;
    /// @brief DAWG dictionary with endings.
    dawgdic::Dictionary endings;

    /// @brief The array with ids of rules for each lemma id.
    LemmasRules * l_rules;
    /// @brief The array with ids of rules for each ending id.
    EndingsRules * e_rules;

    /// @brief The array with normal forms for rule and ending.
    NormalForms * n_forms;

    /// @brief The array with DAWG dictionary for each rule. Each DAWG
    /// dictionary contains endings, by the ending we can get an array with
    /// forms for this ending and rule.
    Rules * rules;

    /// @brief The array of all for rule and ending.
    Forms * forms;
};

//******************************************************************************
// CREATING/DESTROYING
//******************************************************************************

Analyzer * analyzer_new(const char * dirname)
{
    MA_DEBUG("[MANALYZER] MAnalyzer debug mode enabled.\n");
    MA_DEBUG("[MANALYZER] Starting manalyzer.\n");
    MA_DEBUG("[MANALYZER] Dictionary dir is '%s'.\n", dirname);

    Analyzer * result = new Analyzer;

    // Preparing buffer (for filename).
    std::string path;

    // Reading dictionaries from files.
    // Predict prefixes...
    path = dirname; path += "/predict_prefixes.dawgdic";
    std::ifstream predict_prefixes_file(path.c_str(), std::ios::binary );
    result -> predict_prefixes.Read(&predict_prefixes_file);

    // ...lemmas...
    path = dirname; path += "/lemmas.dawgdic";
    std::ifstream lemmas_file(path.c_str(), std::ios::binary );
    result -> lemmas.Read(&lemmas_file);

    // ...and endings.
    path = dirname; path += "/endings.dawgdic";
    std::ifstream endings_file(path.c_str(), std::ios::binary );
    result -> endings.Read(&endings_file);

    // Reading lemmas and endings rules.
    path = dirname; path += "/lemmas_rules";
    result -> l_rules = lemmas_rules_fread(path.c_str());
    path = dirname; path += "/endings_rules";
    result -> e_rules = endings_rules_fread(path.c_str());

    // Reading all forms from file.
    path = dirname; path += "/rules/forms";
    result -> forms = forms_fread(path.c_str());

    // Reading rules from directory.
    path = dirname; path += "/rules";
    result -> rules = rules_dread(path.c_str());

    // Reading normal forms.
    path = dirname; path += "/normal_forms";
    result -> n_forms = normal_forms_fread(path.c_str());

    MA_DEBUG("[MANALYZER] Succesfully started MAnalyzer.\n");

    return result;
}

void analyzer_free(Analyzer * analyzer)
{
    MA_DEBUG("[MANALYZER] Finishing MAnalyzer.\n");

    lemmas_rules_free(analyzer -> l_rules);
    endings_rules_free(analyzer -> e_rules);
    forms_free(analyzer -> forms);
    rules_free(analyzer -> rules);
    normal_forms_free(analyzer -> n_forms);
    delete analyzer;

    MA_DEBUG("[MANALYZER] Succesfully finished MAnalyzer.\n");
}

//******************************************************************************
// HELPFULL FOR ANALYSIS
//******************************************************************************

/**
 * @brief Searches endings for word.
 * @param analyzer  used analyzer
 * @param aw        analyzing word
 * @return true, if found at least one ending
 * @return false, if failed
 */
static bool analyzer_search_endings(Analyzer * analyzer, AnalyzedWord * aw)
{
    bool result = false;

    short int * rules = lemmas_rules_get(analyzer -> l_rules, aw -> lemma_id);

    // Going through all rules for this lemma and check the ending.
    for(int i = 0; i < rules[0]; i++)
    {
        // Searching ending in rule's dawgdic.
        int value;
        if
        (
            (aw -> ending_len != 0 && rules_find_ending_in_rule(analyzer -> rules, rules[i + 1], aw -> ending, aw -> ending_len, &value)) ||
            (aw -> ending_len == 0 && rules_find_ending_in_rule(analyzer -> rules, rules[i + 1], "*", 1, &value))
        )
        {
            MA_DEBUG("[ANALYSIS] \t\tFound ending in rule %d.\n", rules[i + 1]);

            int count = forms_get_length(analyzer -> forms, value);
            FormInfo * forms = forms_get_form_infos(analyzer -> forms, value);
            // Check for each prefix and forming result.
            for(int j = 0; j < count; j++)
                if(aw -> prefix_id == forms[j].prefix)
                {
                    // Counting length of normal_form word.
                    char * nf_ending = normal_forms_get_ending(analyzer -> n_forms, rules[i + 1]);
                    int nf_ending_len = normal_forms_get_ending_len(analyzer -> n_forms, rules[i + 1]);
                    int nf_len = aw -> prefix_len + aw -> lemma_len + nf_ending_len;

                    // Creating new char string -- word in normal form.
                    char * nf = (char *) malloc(sizeof(char) * (nf_len + 1));

                    // Don't copy prefix with ids != 0 (ПО and НАИ in russian
                    // language).
                    int start = aw -> prefix_id != 0 ? 0 : aw -> prefix_len;

                    // Copying predict_prefix, lemma and new ending.
                    memcpy(nf, aw -> word, start * sizeof(char));
                    memcpy(&nf[start], aw -> lemma, aw -> lemma_len * sizeof(char));
                    memcpy(&nf[start + aw -> lemma_len], nf_ending, nf_ending_len * sizeof(char));
                    nf[nf_len] = '\0';

                    infos_prepend_word(aw -> infos,
                        nf,
                        normal_forms_get_type(analyzer -> n_forms, rules[i + 1]),
                        forms[j].id);

                    result = true;

                    MA_DEBUG("[ANALYSIS] \t\t\tAnalysis succeed. Type = %d, normal form = '%s' (%d).\n",
                        forms[j].id,
                        nf,
                        normal_forms_get_type(analyzer -> n_forms, rules[i + 1]));

                    MA_DEBUG("[ANALYSIS] \t\t\tWord's design:\n");

                    #ifdef MANALYZER_DEBUG
                        char old_char;

                        MA_DEBUG("[ANALYSIS] \t\t\t\t%s = ", aw -> word);

                        old_char = aw -> word[aw -> prefix_len];
                        aw -> word[aw -> prefix_len] = '\0';
                        MA_DEBUG("%s+", aw -> word);
                        aw -> word[aw -> prefix_len] = old_char;

                        old_char = aw -> lemma[aw -> lemma_len];
                        aw -> lemma[aw -> lemma_len] = '\0';
                        MA_DEBUG("%s(%d)+", aw -> lemma, aw -> lemma_id);
                        aw -> lemma[aw -> lemma_len] = old_char;

                        old_char = aw -> ending[aw -> ending_len];
                        aw -> ending[aw -> ending_len] = '\0';
                        MA_DEBUG("%s\n", aw -> ending);
                        aw -> ending[aw -> ending_len] = old_char;
                    #endif
                }
        }
    }

    return result;
}

/**
 * @brief Searches lemmas by word and spawn analyzer_search_endings() for each
 *        lemma.
 * @param analyzer  used analyzer
 * @param aw        analyzing word
 * @return true, if found at least one lemma
 * @return false, if failed
 */
bool analyzer_search_lemmas(Analyzer * analyzer, AnalyzedWord * aw)
{
    bool result = false;

    // Root of dictionary.
    dawgdic::BaseType index = analyzer -> lemmas.root();

    // Prefix matching in the lemmas DAWG dic.
    for(char *q = aw -> lemma; *q != '\0'; q++)
    {
        // Following the transition.
        if(!analyzer -> lemmas.Follow(*q, &index))
            break;

        // If found a lemma, trying to get ids for it.
        if(analyzer -> lemmas.has_value(index))
        {
            aw -> lemma_id = analyzer -> lemmas.value(index);
            aw -> lemma_len = q + 1 - aw -> lemma;
            aw -> ending = q + 1;
            aw -> ending_len = aw -> word_size - ((q + 1) - aw -> word);

            // Debug information.
            #ifdef MANALYZER_DEBUG
                char old_char = *(q + 1);
                *(q + 1) = '\0';
                MA_DEBUG("[ANALYSIS] \tFound lemma '%s' (%d, %d). ", aw -> lemma, aw -> lemma_len, aw -> lemma_id);
                *(q + 1) = old_char;
                MA_DEBUG("Searching in rules (possible) ending '%s' (%d).\n", aw -> ending, aw -> ending_len);
            #endif

            if(analyzer_search_endings(analyzer, aw))
                result = true;
        }
    }

    return result;
}

/**
 * @brief Is similar to analyzer_search_lemmas(), but doesn't search any lemmas
 *        and uses special lemma (#) instead. Whole word is the ending in that
 *        case.
 * @param analyzer  used analyzer
 * @param aw        analyzing word
 * @return true, if found word in special lemma's rules
 * @return false, if failed
 */
static bool analyzer_special_lemma(Analyzer * analyzer, AnalyzedWord * aw)
{
    MA_DEBUG("[ANALYSIS] \tFound lemma '#' (1, 0). Searching in rules (possible) ending '%s' (%d).\n", aw -> ending, aw -> ending_len);

    return analyzer_search_endings(analyzer, aw);
}

// This function starts searching lemmas (or use special lemma).
/**
 * @brief First tries to find lemma (by analyzer_search_lemmas() function). If
 *        failed tries to use special lemma (by analyzer_special_lemma()
 *        function). If failed again returns false
 * @param analyzer  used analyzer
 * @param aw        analyzing word
 * @return true, if found any lemma
 * @return false, if failed
 */
static bool analyzer_analyze_lemma(Analyzer * analyzer, AnalyzedWord * aw)
{
    // Clean up.
    analyzed_word_ending_clean_up(aw);

    // Search lemmas for word.
    if(analyzer_search_lemmas(analyzer, aw))
        return true;

    // Preparing.
    aw -> ending = aw -> lemma;
    aw -> ending_len = aw -> word_size - (aw -> ending - aw -> word);
    aw -> lemma_len = 0;
    aw -> lemma_id = 0;

    // Search endings by special lemma (#, len = 0, id = 0, whole word is
    // ending).
    if(analyzer_special_lemma(analyzer, aw))
        return true;

    return false;
}

//******************************************************************************
// ANALYZER MAIN FUNCTION
//******************************************************************************

/**
 * @brief Prediction function. Spawned by analyzer_get_word_info() if analysis
 *        failed. Predicts grammar info and normal form for unknown words.
 * @param analyzer  used analyzer
 * @param aw        analyzing word
 * @return true, if found any lemma
 * @return false, if failed
 */
bool analyzer_predict(Analyzer * analyzer, AnalyzedWord * aw);

bool analyzer_get_word_info(Analyzer * analyzer, char * word, unsigned int word_size, WordInfos * buffer)
{
    MA_DEBUG("[ANALYSIS] Start analysis for '%s'.\n", word);

    AnalyzedWord * aw = analyzed_word_new(word, word_size, buffer);
    aw -> lemma = word;

    // Analyze whole word.
    if(analyzer_analyze_lemma(analyzer, aw))
    {
        MA_DEBUG("[ANALYSIS] Finish analysis.\n");
        analyzed_word_free(aw);
        return true;
    }

    // Analyze word without predict prefix.
    dawgdic::BaseType index = analyzer -> predict_prefixes.root();
    for(char *q = aw -> word; *q != '\0'; q++)
    {
        if(!analyzer -> predict_prefixes.Follow(*q, &index))
            break;

        // Found predict prefix.
        if(analyzer -> predict_prefixes.has_value(index))
        {
            aw -> lemma = q + 1;
            aw -> lemma_len = 0;

            aw -> prefix_len = q + 1 - word;
            aw -> prefix_id = analyzer -> predict_prefixes.value(index);

            // Debug information.
            #ifdef MANALYZER_DEBUG
                char old_char = *(q + 1);
                *(q + 1) = '\0';
                MA_DEBUG("[ANALYSIS] \tFound predict prefix '%s' (%d, %d).\n", aw -> word, q + 1 - aw -> word, aw -> prefix_id);
                *(q + 1) = old_char;
            #endif

            if(analyzer_analyze_lemma(analyzer, aw))
            {
                MA_DEBUG("[ANALYSIS] Finish analysis.\n");
                analyzed_word_free(aw);
                return true;
            }
        }
    }

    MA_DEBUG("[ANALYSIS] Analysis failed. Trying prediction.\n");

    bool result = analyzer_predict(analyzer, aw);
    analyzed_word_free(aw);

    return result;
}

//******************************************************************************
// PREDICTION FUNCTION
//******************************************************************************

bool analyzer_predict(Analyzer * analyzer, AnalyzedWord * aw)
{
    bool result = false;

    MA_DEBUG("[PREDICTION] Start prediction for '%s'.\n", aw -> word);

    // Check the possible endings of the word (with length up to 5 letters).
    // Note: we store endings reversed (i.e. ending abcde stored as edcba). By
    // this trick we can use prefix search (we can call it "reversed ending
    // search").

    char * end = aw -> word + aw -> word_size;
    dawgdic::BaseType index = analyzer -> endings.root();

    // Start lookup from the end.
    for(char * q = end - 1; end - q <= 5 && q >= aw -> word; q--)
    {
        if(!analyzer -> endings.Follow(*q, &index))
            break;

        // Found ending.
        if(analyzer -> endings.has_value(index))
        {
            int ending_id = analyzer -> endings.value(index);

            unsigned short int rules_count = endings_rules_get_rules_count(analyzer -> e_rules, ending_id);
            unsigned short int * rules_ids = endings_rules_get_rules_ids(analyzer -> e_rules, ending_id);

            MA_DEBUG("[PREDICTION] \tFound ending '%s' (%d, %d).\n", q, end - q, ending_id);

            bool erased = false;

            // Go through each rule.
            for(int i = 0; i < rules_count; i++)
            {
                MA_DEBUG("[PREDICTION] \t\tRule is %d.\n", rules_ids[i]);

                char * l = endings_rules_get_endings_lens(analyzer -> e_rules, ending_id, i);

                while(*l != -1)
                {
                    // Cutting *l chars -- it's an ending.
                    // First of all -- check it's enough letters for it or not.
                    if(aw -> word_size > *l)
                    {
                        char * ending = &aw -> word[aw -> word_size - *l];
                        MA_DEBUG("[PREDICTION] \t\t\tCutting '%s' as ending.\n", ending);

                        int value;
                        if
                        (
                            // Non-null ending.
                            ( *ending != '\0' && rules_find_ending_in_rule(analyzer -> rules, rules_ids[i], ending, end - ending, &value) ) ||
                            // Null ending.
                            ( *ending == '\0' && rules_find_ending_in_rule(analyzer -> rules, rules_ids[i], "*", 1, &value) )
                        )
                        {
                            int count = forms_get_length(analyzer -> forms, value);
                            FormInfo * forms = forms_get_form_infos(analyzer -> forms, value);

                            if(count != 0)
                            {
                                if(!erased)
                                {
                                    erased = true;
                                    infos_erase(aw -> infos);
                                }

                                result = true;
                            }

                            for(int j = 0; j < count; j++)
                            {

                                char * nf_ending = normal_forms_get_ending(analyzer -> n_forms, rules_ids[i]);
                                int nf_ending_len = normal_forms_get_ending_len(analyzer -> n_forms, rules_ids[i]);
                                int w_len = aw -> word_size - (*l);
                                int nf_len = w_len + nf_ending_len;

                                // Creating new char string -- word in normal form.
                                char * nf = (char *) malloc(sizeof(char) * (nf_len + 1));

                                // Copying the rest part of the original word and normal
                                // form ending.
                                memcpy(nf, aw -> word, w_len * sizeof(char));
                                memcpy(&nf[w_len], nf_ending, nf_ending_len * sizeof(char));
                                nf[nf_len] = '\0';

                                infos_prepend_word(aw -> infos,
                                    nf,
                                    normal_forms_get_type(analyzer -> n_forms, rules_ids[i]),
                                    forms[j].id,
                                    true);

                                MA_DEBUG("[PREDICTION] \t\t\t\tPrediction succeed. Type = %d, normal form = '%s' (%d).\n", forms[j].id, nf, normal_forms_get_type(analyzer -> n_forms, rules_ids[i]));
                            }
                        }
                    }

                    l++;
                }
            }
        }
    }

    if(!result)
        MA_DEBUG("[PREDICTION] Prediction failed. Cannot analyse word '%s'.\n", aw -> word);

    return result;
}
