/*
    Copyright (c) 2012 K. Mingulov.
    See the LICENSE file for copying permission.
*/

#ifndef FORMS_HPP
#define FORMS_HPP

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

#endif
