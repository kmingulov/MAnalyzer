#ifndef WORD_INFOS_HPP
#define WORD_INFOS_HPP

/*
    Word's infos. This struct contains information about normal forms of the
    word (forms and their grammar types) and grammar type of the word.
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
    These functions get info (normal form, it's id and word's id) by id from
    word's infos.
*/
char * infos_get_normal_form(WordInfos * wi, unsigned int id);
unsigned short int infos_get_normal_form_id(WordInfos * wi, unsigned int id);
unsigned short int infos_get_form_id(WordInfos * wi, unsigned int id);

#endif
