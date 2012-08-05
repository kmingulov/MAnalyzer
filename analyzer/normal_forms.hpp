#ifndef NORMAL_FORMS_HPP
#define NORMAL_FORMS_HPP

/*
    This struct contains information about normal forms (each of them contains
    ending, ending length and id).
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
unsigned short int normal_forms_get_ending_len(NormalForms * nf, unsigned int id);
unsigned short int normal_forms_get_type(NormalForms * nf, unsigned int id);
char normal_forms_get_prefix(NormalForms * nf, unsigned int id);

#endif
