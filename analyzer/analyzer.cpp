#include <fstream>
#include <sstream>

#include <cstdlib>
#include <cstring>
#include <ctime>

#include "forms.hpp"
#include "rules.hpp"
#include "lemmas_rules.hpp"
#include "endings_rules.hpp"
#include "normal_forms.hpp"
#include "analyzed_word.hpp"

#include "analyzer.hpp"

//******************************************************************************
// DEBUG DEFINES.
//******************************************************************************
#ifdef MANALYZER_DEBUG
    #define MA_DEBUG(...) { fprintf(stderr, "[%d] ", time(NULL)); fprintf(stderr, __VA_ARGS__); }
    #define MA_DEBUG_(...) { fprintf(stderr, __VA_ARGS__); }
#else
    #define MA_DEBUG(...) {}
    #define MA_DEBUG_(...) {}
#endif

//******************************************************************************
// STRUCT.
//******************************************************************************

struct Analyzer
{
    // Prefixes', lemmas' and endings' dictionaries.
    dawgdic::Dictionary predict_prefixes;
    dawgdic::Dictionary lemmas;
    dawgdic::Dictionary endings;

    // Rules for lemmas and endings (array with id of rules for each lemma/
    // ending).
    LemmasRules * l_rules;
    EndingsRules * e_rules;

    // Array with normal forms.
    NormalForms * n_forms;

    // All forms.
    Forms * forms;

    // Rules.
    Rules * rules;
};

//******************************************************************************
// CREATING/DESTROYING
//******************************************************************************

Analyzer * analyzer_new()
{
    MA_DEBUG("[MANALYZER] MAnalyzer debug mode enabled.\n");
    MA_DEBUG("[MANALYZER] Starting manalyzer.\n");

    Analyzer * result = new Analyzer;

    // Reading dictionaries from files.
    // Predict prefixes...
    std::ifstream predict_prefixes_file("dics/predict_prefixes.dawgdic", std::ios::binary);
    result -> predict_prefixes.Read(&predict_prefixes_file);

    // ...lemmas...
    std::ifstream lemmas_file("dics/lemmas.dawgdic", std::ios::binary);
    result -> lemmas.Read(&lemmas_file);

    // ...and endings.
    std::ifstream endings_file("dics/endings.dawgdic", std::ios::binary);
    result -> endings.Read(&endings_file);

    // Reading lemmas and endings rules.
    result -> l_rules = lemmas_rules_fread("dics/lemmas_rules");
    result -> e_rules = endings_rules_fread("dics/endings_rules");

    // Reading all forms from file.
    result -> forms = forms_fread("dics/rules/forms");

    // Reading rules from directory.
    result -> rules = rules_dread("dics/rules");

    // Reading normal forms.
    result -> n_forms = normal_forms_fread("dics/normal_forms");

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

// Searches endings for word from lemma's rules.
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
            MA_DEBUG("[ANALYSIS] \tFound ending in rule %d.\n", rules[i + 1]);

            int count = forms_get_length(analyzer -> forms, value);
            FormInfo * forms = forms_get_form_infos(analyzer -> forms, value);
            // Check for each prefix and forming result.
            for(int j = 0; j < count; j++)
                if(aw -> prefix_type == forms[j].prefix)
                {
                    // Counting length of normal_form word.
                    char * nf_ending = normal_forms_get_ending(analyzer -> n_forms, rules[i + 1]);
                    int nf_ending_len = normal_forms_get_ending_len(analyzer -> n_forms, rules[i + 1]);
                    int nf_len = aw -> predict_prefix_len + aw -> lemma_len + nf_ending_len;

                    // Creating new char string -- word in normal form.
                    char * nf = (char *) malloc(sizeof(char) * (nf_len + 1));

                    // Copying predict_prefix, lemma and new ending.
                    memcpy(nf, aw -> word, aw -> predict_prefix_len * sizeof(char));
                    memcpy(&nf[aw -> predict_prefix_len], aw -> lemma, aw -> lemma_len * sizeof(char));
                    memcpy(&nf[aw -> predict_prefix_len + aw -> lemma_len], nf_ending, nf_ending_len * sizeof(char));
                    nf[nf_len] = '\0';

                    infos_prepend_word(aw -> infos,
                        nf,
                        normal_forms_get_type(analyzer -> n_forms, rules[i + 1]),
                        forms[j].id);

                    result = true;

                    MA_DEBUG("[ANALYSIS] \t\tAnalysis succeed. Type = %d, normal form = '%s' (%d).\n",
                        forms[j].id,
                        nf,
                        normal_forms_get_type(analyzer -> n_forms, rules[i + 1]));

                    MA_DEBUG("[ANALYSIS] \t\tWord's design:\n");

                    #ifdef MANALYZER_DEBUG
                        char old_char;

                        MA_DEBUG("[ANALYSIS] \t\t\t%s = ", aw -> word);

                        old_char = aw -> word[aw -> predict_prefix_len];
                        aw -> word[aw -> predict_prefix_len] = '\0';
                        MA_DEBUG_("%s+", aw -> word);
                        aw -> word[aw -> predict_prefix_len] = old_char;

                        old_char = aw -> prefix[aw -> prefix_len];
                        aw -> word[aw -> prefix_len] = '\0';
                        MA_DEBUG_("%s(%d)+", aw -> prefix, aw -> prefix_type);
                        aw -> word[aw -> prefix_len] = old_char;

                        old_char = aw -> lemma[aw -> lemma_len];
                        aw -> word[aw -> lemma_len] = '\0';
                        MA_DEBUG_("%s(%d)+", aw -> lemma, aw -> lemma_id);
                        aw -> word[aw -> lemma_len] = old_char;

                        old_char = aw -> ending[aw -> ending_len];
                        aw -> word[aw -> ending_len] = '\0';
                        MA_DEBUG_("%s\n", aw -> ending);
                        aw -> word[aw -> ending_len] = old_char;
                    #endif
                }
        }
    }

    return result;
}

// Searches lemmas by word.
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
                MA_DEBUG_("Searching in rules (possible) ending is '%s' (%d).\n", aw -> ending, aw -> ending_len);
            #endif

            if(analyzer_search_endings(analyzer, aw))
                result = true;
        }
    }

    return result;
}

// This function is very similar to analyzer_search_lemmas(). It doesn't search
// lemmas, it uses special lemma (#). So word is just an ending for this lemma.
static bool analyzer_special_lemma(Analyzer * analyzer, AnalyzedWord * aw)
{
    MA_DEBUG("[ANALYSIS] \tFound lemma '#' (1, 0). Searching in rules (possible) ending '%s' (%d).\n", aw -> ending, aw -> ending_len);

    return analyzer_search_endings(analyzer, aw);
}

//******************************************************************************
// HELPFULL STATIC FUNCTIONS.
//******************************************************************************

// This function starts searching lemmas (or use special lemma).
static bool analyzer_analyze_lemma(Analyzer * analyzer, AnalyzedWord * aw)
{
    // Preparing.
    aw -> ending = NULL;
    aw -> ending_len = 0;

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

// This functions cut off the prefix (if exists) and start analyzing lemmas.
static bool analyzer_analyze_prefix(Analyzer * analyzer, AnalyzedWord * aw)
{
    // Preparing.
    aw -> prefix_type = 1;
    aw -> lemma = aw -> prefix;
    aw -> lemma_len = 0;

    // Analyze word (without searching prefix).
    if(analyzer_analyze_lemma(analyzer, aw))
        return true;

    // Search prefix and analyze the rest part of the word.
    // TODO Brute method. Maybe improve it?
    char * word = aw -> prefix;
    char old_char = word[2]; word[2] = '\0';
    if(strcmp(word, "ÏÎ") == 0)
    {
        word[2] = old_char;

        aw -> prefix_type = 2;
        aw -> prefix_len = 2;
        aw -> lemma = aw -> prefix + 2;
        aw -> lemma_len = 0;

        MA_DEBUG("[ANALYSIS] \tPrefix is ÏÎ (2).\n");

        if(analyzer_analyze_lemma(analyzer, aw))
            return true;
    }

    word[2] = old_char;

    old_char = word[3]; word[3] = '\0';
    if(strcmp(word, "ÍÀÈ") == 0)
    {
        word[3] = old_char;

        aw -> prefix_type = 3;
        aw -> prefix_len = 3;
        aw -> lemma = aw -> prefix + 3;
        aw -> lemma_len = 0;

        MA_DEBUG("[ANALYSIS] \tPrefix is ÍÀÈ (3).\n");

        if(analyzer_analyze_lemma(analyzer, aw))
            return true;
    }

    word[3] = old_char;

    return false;
}

//******************************************************************************
// ANALYZER MAIN FUNCTION
//******************************************************************************

bool analyzer_predict(Analyzer * analyzer, AnalyzedWord * aw);

bool analyzer_get_word_info(Analyzer * analyzer, char * word, unsigned int word_size, WordInfos * buffer)
{
    MA_DEBUG("[ANALYSIS] Start analysis for '%s'.\n", word);

    AnalyzedWord * aw = analyzed_word_new(word, word_size, buffer);
    aw -> prefix = word;

    //~ return analyzer_predict(analyzer, aw);

    // Analyze whole word.
    if(analyzer_analyze_prefix(analyzer, aw))
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
            aw -> prefix = q + 1;
            aw -> predict_prefix_len = q + 1 - word;

            // Debug information.
            #ifdef MANALYZER_DEBUG
                char old_char = *(q + 1);
                *(q + 1) = '\0';
                MA_DEBUG("[ANALYSIS] \tFound predict prefix '%s' (%d).\n", aw -> word, q + 1 - aw -> word);
                *(q + 1) = old_char;
            #endif

            if(analyzer_analyze_prefix(analyzer, aw))
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

    // TODO Check for #+ending?

    // Check the possible endings of the word (with length up to 5 letters).
    // Note: we store endings reversed (i.e. ending abcde stored as edcba). By
    // this trick we can use prefix search (we can call it "reversed ending
    // search").

    char * end = aw -> word + aw -> word_size;
    dawgdic::BaseType index = analyzer -> endings.root();
    for(char * q = end - 1; end - q <= 5 && q >= aw -> word; q--)
    {
        if(!analyzer -> endings.Follow(*q, &index))
            break;

        // Found ending.
        if(analyzer -> endings.has_value(index))
        {
            int ending_id = analyzer -> endings.value(index);
            unsigned short int * rules = endings_rules_get(analyzer -> e_rules, ending_id);

            MA_DEBUG("[PREDICTION] \tFound ending '%s' (%d, %d).\n", q, end - q, ending_id);

            // Go through each rule.
            for(int i = 0; i < rules[0]; i++)
            {
                // Searching ending in rule's dawgdic.
                int value;
                if(rules_find_ending_in_rule(analyzer -> rules, rules[i + 1], q, end - q, &value))
                {
                    MA_DEBUG("[PREDICTION] \t\tFound ending in rule %d.\n", rules[i + 1]);

                    int count = forms_get_length(analyzer -> forms, value);
                    FormInfo * forms = forms_get_form_infos(analyzer -> forms, value);

                    for(int j = 0; j < count; j++)
                    {
                        result = true;

                        // Make normal form.
                        char * n_ending = normal_forms_get_ending(analyzer -> n_forms, rules[i + 1]);
                        int n_ending_len = normal_forms_get_ending_len(analyzer -> n_forms, rules[i + 1]);
                        int nf_len = n_ending_len + (q - aw -> word);
                        char * nf = (char *) malloc(sizeof(char) * (nf_len + 1));

                        memcpy(nf, aw -> word, sizeof(char) * (q - aw -> word));
                        if(n_ending != NULL)
                            strcpy(&nf[q - aw -> word], n_ending);

                        infos_prepend_word(aw -> infos, 
                            nf,
                            normal_forms_get_type(analyzer -> n_forms, rules[i + 1]),
                            forms[j].id);

                        MA_DEBUG("[PREDICTION] \t\t\tPrediction succeed. Type = %d, normal form = '%s' (%d).\n", forms[j].id, nf, normal_forms_get_type(analyzer -> n_forms, rules[i + 1]));
                    }
                }
            }
        }
    }

    if(!result)
        MA_DEBUG("[PREDICTION] Prediction failed. Cannot analyse word '%s'.\n", aw -> word);

    return result;
}
