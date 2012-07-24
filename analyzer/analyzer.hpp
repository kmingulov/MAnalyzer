#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <map>
#include <dawgdic/dictionary.h>

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

struct Forms
{
    int count;
    int current;
    char * ids;
    char * prefixes;
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
    Adds form to forms. Returns id of added form.
*/
int forms_add_form(Forms * forms, const char * id, const char prefix);

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
    Searches lemmas/predict prefixes for word.
*/
void analyzer_search_lemmas(Analyzer * analyzer, char * word, int word_size);
void analyzer_search_predict_prefixes(Analyzer * analyzer, char * word);

#endif

