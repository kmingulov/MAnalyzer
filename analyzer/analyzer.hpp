#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <map>
#include <dawgdic/dictionary.h>

//******************************************************************************
// WORDS INFO
//******************************************************************************

struct WordInfo
{
    // Normal form and it's gram. type.
    char * n_form;
    unsigned short int n_form_id;

    // Gram. type of word.
    unsigned short int form_id;
};

struct WordInfos
{
    // Current size and max size of array.
    unsigned int size;
    unsigned int max_size;

    // Flag "prepend error": cannot add new element, because array is full.
    bool prepend_error;

    // Flag "prediction": these results was get by prediction. This word is
    // unknown.
    bool prediction;

    WordInfo * infos;
};

/*
    Create/destroy infos array.
*/
WordInfos * infos_new(unsigned int max_size);
void infos_free(WordInfos * wi);

/*
    Prepend word to infos array.
*/
bool infos_prepend_word(WordInfos * wi, char * form, unsigned short int n_form_id, unsigned short int form_id);

//******************************************************************************
// LEMMAS RULES
//******************************************************************************

struct LemmasRules
{
    int count;
    short int ** rules;
};

/*
    Reads lemmas rules from file.
*/
LemmasRules lemmas_rules_fread(const char * filename);

/*
    Frees allocated memory in lemmas rules. (They become useless.)
*/
void lemmas_rules_free(LemmasRules rules);

//******************************************************************************
// FORMS
//******************************************************************************

struct FormInfo
{
    char prefix;
    unsigned short int id;
};

struct Forms
{
    int count;
    char * counts;
    FormInfo ** forms;
};

/*
    Creates new array with forms.
*/
Forms * forms_new(int count);

/*
    Frees forms' array.
*/
void forms_free(Forms * forms);

/*
    Loads forms from file.
*/
Forms * forms_fread(const char * filename);

//******************************************************************************
// RULES
//******************************************************************************

struct Rules
{
    // All forms.
    Forms * forms;

    // Dics (with endings) for rules.
    // TODO Constant?
    dawgdic::Dictionary dics[2766];
};

/*
    Loads rules from _directory_. Each rule is in *.dawgdic file. Forms are in
    forms file.
*/
Rules * rules_dread(const char * dirname);

/*
    Frees rules.
*/
void rules_free(Rules * rules);

//******************************************************************************
// ANALYZER
//******************************************************************************

/*
    Morph analyzer's struct.
*/
struct Analyzer
{
    // Prefixes' and lemmas' dictionaries.
    dawgdic::Dictionary predict_prefixes;
    dawgdic::Dictionary lemmas;

    // Rules for lemmas (array with id of rules for each lemma).
    LemmasRules l_rules;

    // Rules.
    Rules * rules;
};

/*
    Loads analyzer.
*/
Analyzer * analyzer_new();

/*
    Unloads analyzer.
*/
void analyzer_free(Analyzer * analyzer);

/*
    Searches lemmas for word.
    Returns true, if anything found.
*/
bool analyzer_search_lemmas(Analyzer * analyzer, char * word, int word_size, char prefix, WordInfos * buffer);

/*
    Analyzer's main function for morph. analysis.
*/
bool analyzer_get_word_info(Analyzer * analyzer, char * word, int word_size, WordInfos * buffer);

#endif

