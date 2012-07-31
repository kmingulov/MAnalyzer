#include <fstream>
#include <sstream>

#include <cstdlib>
#include <cstring>

#include "analyzer.hpp"

//******************************************************************************
// CREATING/DESTROYING
//******************************************************************************

Analyzer * analyzer_new()
{
    #ifdef ANALYZER_DEBUG
        printf("Analyzer debug mode enabled.\n");
    #endif

    Analyzer * result = new Analyzer;

    // Reading dictionaries from files.
    // Predict prefixes...
    std::ifstream predict_prefixes_file("dics/predict_prefixes.dawgdic", std::ios::binary);
    result -> predict_prefixes.Read(&predict_prefixes_file);

    // ... and lemmas.
    std::ifstream lemmas_file("dics/lemmas.dawgdic", std::ios::binary);
    result -> lemmas.Read(&lemmas_file);

    // Reading lemmas rules (connects each lemma's id to it's rules).
    result -> l_rules = lemmas_rules_fread("dics/lemmas_rules");

    // Reading rules from directory.
    result -> rules = rules_dread("dics/rules");

    return result;
}

void analyzer_free(Analyzer * analyzer)
{
    lemmas_rules_free(analyzer -> l_rules);
    rules_free(analyzer -> rules);
    delete analyzer;
}

//******************************************************************************
// HELPFULL FOR ANALYSIS
//******************************************************************************

// Searches lemmas by word.
// TODO Add buffer for results.
bool analyzer_search_lemmas(Analyzer * analyzer, char * word, int word_size, char prefix)
{
    bool result = false;

    // Root of dictionary.
    dawgdic::BaseType index = analyzer -> lemmas.root();

    // Prefix matching in the lemmas DAWG dic.
    for(char *q = word; *q != '\0'; q++)
    {
        // Following the transition.
        if(!analyzer -> lemmas.Follow(*q, &index))
            break;

        // If found a lemma, trying to get ids for it.
        if(analyzer -> lemmas.has_value(index))
        {
            int value = analyzer -> lemmas.value(index);
            short int * rules = analyzer -> l_rules.rules[value];
            char * ending = q + 1;
            int ending_len = word_size - ((q + 1) - word);

            // Debug information.
            #ifdef ANALYZER_DEBUG
                // Cut off the begining.
                char old_char = *(q + 1);
                *(q + 1) = '\0';

                // Printing lemma.
                printf("Lemma %s (%d). ", word, value);

                // Restore char.
                *(q + 1) = old_char;

                // Printing ending.
                printf("(Possibly) ending is %s (%d).\n", ending, ending_len);
            #endif

            // Going through all rules for this lemma and check the ending.
            for(int i = 0; i < rules[0]; i++)
            {
                #ifdef ANALYZER_DEBUG
                    printf("\tRule %d\n", rules[i + 1]);
                #endif

                // Searching ending in rule's dawgdic.
                int value;
                if
                (
                    (ending_len != 0 && analyzer -> rules -> dics[rules[i + 1]].Find(ending, ending_len, &value)) ||
                    (ending_len == 0 && analyzer -> rules -> dics[rules[i + 1]].Find("*", 1, &value))
                )
                {
                    int count = analyzer -> rules -> forms -> counts[value];
                    FormInfo * forms = analyzer -> rules -> forms -> forms[value];
                    // Check for each prefix and forming result.
                    for(int j = 0; j < count; j++)
                        if(prefix == forms[j].prefix)
                        {
                            result = true;

                            #ifdef ANALYZER_DEBUG
                                printf("\t\tForm %d\n", forms[j].id);
                            #endif

                            #ifdef QUIET_ANALYZER_DEBUG
                                printf("%d ", forms[j].id);
                            #endif
                        }
                }
            }
        }
    }

    #ifdef ANALYZER_DEBUG
        printf("\n");
    #endif

    #ifdef QUIET_ANALYZER_DEBUG
        printf("\n");
    #endif

    return result;
}

//******************************************************************************
// HELPFULL STATIC FUNCTIONS.
//******************************************************************************
static bool analyzer_analyze_lemma(Analyzer * analyzer, char * word, int word_size, char prefix)
{
    // Search lemmas for word.
    if(analyzer_search_lemmas(analyzer, word, word_size, prefix))
        return true;

    // Search lemmas for # + word.
    // TODO Maybe slow (because of strcpy()). Need to write special function.
    char new_word[word_size + 2];
    new_word[0] = '#';
    strcpy(&new_word[1], word);
    if(analyzer_search_lemmas(analyzer, &new_word[0], word_size + 1, prefix))
        return true;

    return false;
}

static bool analyzer_analyze_word(Analyzer * analyzer, char * word, int word_size)
{
    // Analyze word (without searching prefix).
    if(analyzer_analyze_lemma(analyzer, word, word_size, 1))
        return true;

    // Search prefix and analyze the rest part of the word.
    // TODO Brute method. Maybe improve it?
    char old_char = word[2]; word[2] = '\0';
    if(strcmp(word, "ÏÎ") == 0)
    {
        word[2] = old_char;
        if(analyzer_analyze_lemma(analyzer, &word[2], word_size - 2, 2))
            return true;
    }

    word[2] = old_char;

    old_char = word[3]; word[3] = '\0';
    if(strcmp(word, "ÍÀÈ") == 0)
    {
        word[3] = old_char;
        if(analyzer_analyze_lemma(analyzer, &word[3], word_size - 3, 3))
            return true;
    }

    word[3] = old_char;

    return false;
}

//******************************************************************************
// ANALYZER MAIN FUNCTION
//******************************************************************************
bool analyzer_get_word_info(Analyzer * analyzer, char * word, int word_size)
{
    // Analyze whole word.
    if(analyzer_analyze_word(analyzer, word, word_size))
        return true;

    // Analyze word without predict prefix.
    dawgdic::BaseType index = analyzer -> predict_prefixes.root();
    for(char *q = word; *q != '\0'; q++)
    {
        if(!analyzer -> predict_prefixes.Follow(*q, &index))
            break;

        // Found predict prefix.
        if(analyzer -> predict_prefixes.has_value(index))
        {
            // Debug information.
            #ifdef ANALYZER_DEBUG
                // Cutting off the begining.
                char old_char = *(q + 1);
                *(q + 1) = '\0';

                printf("Predict prefix is %s (%d)\n", word, q + 1 - word);

                // Restoring the char.
                *(q + 1) = old_char;
            #endif

            char * new_word = q + 1;
            int new_word_size = word_size - (q + 1 - word);

            if(analyzer_analyze_word(analyzer, new_word, new_word_size))
                return true;
        }
    }

    return false;
}
