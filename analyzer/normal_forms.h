/**
 * @file normal_forms.h
 * @brief Contains declarations of NormalForms' functions.
 */

#ifndef NORMAL_FORMS_HPP
#define NORMAL_FORMS_HPP

/**
 * @struct NormalForms
 * @brief Contains information about normal forms (for any rule), each of them
 *        contains ending for normal form, ending's length and grammar type.
 */
struct NormalForms;

/**
 * @brief Reads normal forms from file.
 * @param filename path to forms file
 * @return pointer to NormalForms
 */
NormalForms * normal_forms_fread(const char * filename);

/**
 * @brief Frees NormalForms.
 * @param nf pointer to NormalForms
 */
void normal_forms_free(NormalForms * nf);

/**
 * @brief Gets normal form ending by rule's id.
 * @param nf pointer to NormalForms
 * @param id rule's id
 * @return pointer to normal form ending
 */
char * normal_forms_get_ending(NormalForms * nf, unsigned int id);

/**
 * @brief Gets normal form ending's length by rule's id.
 * @param nf pointer to NormalForms
 * @param id rule's id
 * @return length of normal form ending's
 */
unsigned short int normal_forms_get_ending_len(NormalForms * nf, unsigned int id);

/**
 * @brief Gets normal form's grammar type.
 * @param nf pointer to NormalForms
 * @param id rule's id
 * @return normal form's grammar type
 */
unsigned short int normal_forms_get_type(NormalForms * nf, unsigned int id);

#endif
