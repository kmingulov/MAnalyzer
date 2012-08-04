#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <map>
#include <dawgdic/dictionary.h>

//******************************************************************************
// WORDS INFO
//******************************************************************************

/*
    Word's infos. This struct contains information about normal forms of the
    word (forms and their grammar types).
*/
struct WordInfos;

/*
    Creates/destroys infos array.
*/
WordInfos * infos_new(unsigned int max_size);
void infos_free(WordInfos * wi);

/*
    Erases data of the infos array.
*/
void infos_erase(WordInfos * wi);

/*
    Prepends word to infos array.
*/
bool infos_prepend_word(WordInfos * wi, char * form, unsigned short int n_form_id, unsigned short int form_id);

/*
    Check: the array with results is prediction or not.
*/
bool infos_is_prediction(WordInfos * wi);

/*
    Check: the array with results is full and there was an error with adding new
    elements (there is no enough space for them).
*/
bool infos_have_prepend_error(WordInfos * wi);

/*
    Gets amount of added words.
*/
int infos_get_size(WordInfos * wi);

/*
    These functions get info with id from word's infos.
*/
char * infos_get_normal_form(WordInfos * wi, unsigned int id);
unsigned short int infos_get_normal_form_id(WordInfos * wi, unsigned int id);
unsigned short int infos_get_form_id(WordInfos * wi, unsigned int id);

//******************************************************************************
// NORMAL FORM
//******************************************************************************

/*
    This struct contains information about normal forms (each of them contains
    ending, id and required prefix).
*/
struct NormalForms;

/*
    Reads normal forms from file.
*/
NormalForms * normal_forms_fread(const char * filename);

/*
    Frees array with normal forms.
*/
void normal_forms_free(NormalForms * nf);

/*
    Gets data of normal form.
*/
char * normal_forms_get_ending(NormalForms * nf, unsigned int id);
unsigned short int normal_forms_get_type(NormalForms * nf, unsigned int id);
char normal_forms_get_prefix(NormalForms * nf, unsigned int id);

//******************************************************************************
// LEMMAS RULES
//******************************************************************************

/*
    Struct of lemmas rules. This struct compare each lemma id to ids of it's
    rules.
*/
struct LemmasRules;

/*
    Reads lemmas rules from file.
*/
LemmasRules * lemmas_rules_fread(const char * filename);

/*
    Frees allocated memory in lemmas rules. (They become useless.)
*/
void lemmas_rules_free(LemmasRules * rules);

/*
    Gets the pointer to the array with ids of rules.
*/
short int * lemmas_rules_get(LemmasRules * rules, unsigned int lemma_id);

//******************************************************************************
// FORMS
//******************************************************************************

/*
    This struct contains information about form: id and required prefix.
    Endings are stored in dawgdics. By them we access these FormInfos.
*/
struct FormInfo
{
    char prefix;
    unsigned short int id;
};

/*
    This struct contains information about forms loaded from forms file.
    It's an array, each line is an array too, but contains FormInfos.
*/
struct Forms;

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

/*
    Gets length of array with forms.
*/
char forms_get_length(Forms * forms, unsigned int id);

/*
    Gets pointer to the array of FormInfos.
*/
FormInfo * forms_get_form_infos(Forms * forms, unsigned int id);

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
    Analyzed word (used in inner analyzers functions).
*/
struct AnalyzedWord
{
    // Whole word and size.
    char * word;
    unsigned int word_size;

    // Pointers to the beginnings of word's parts.
    char * lemma, ending, prefix;

    // Lengths of word's parts.
    unsigned int predict_prefix_len, prefix_len, lemma_len, ending_len;

    // Prefix type.
    unsigned int prefix_type;

    // Array with infos.
    WordInfos * infos;
};

/*
    Morph analyzer's struct.
*/
struct Analyzer
{
    // Prefixes' and lemmas' dictionaries.
    dawgdic::Dictionary predict_prefixes;
    dawgdic::Dictionary lemmas;

    // Rules for lemmas (array with id of rules for each lemma).
    LemmasRules * l_rules;

    // Array with normal forms.
    NormalForms * n_forms;

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

