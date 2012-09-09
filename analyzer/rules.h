/**
 * @file rules.h
 * @brief Contains declarations of Rules' functions.
 */

#ifndef RULES_H
#define RULES_H

#include <dawgdic/dictionary.h>

/**
 * @struct Rules
 * @brief An array with dawgdics as elements (index is a rule id). Each dawgdic
 *        is a dictionary of endings, each of which in turn refers to
 *        possible forms in Forms struct.
 */
struct Rules;

/**
 * @brief Loads rules from _directory_. Each rule is in *.dawgdic file, forms
 *        are in forms file.
 * @param dirname path to the directory
 * @return pointer to created rules
 */
Rules * rules_dread(const char * dirname);

/**
 * @brief Frees rules.
 * @param rules pointer to Rules variable
 */
void rules_free(Rules * rules);

/**
 * @brief Searches ending in rule's dawgdic by id.
 * @param rules      pointer to Rules variable
 * @param id         rule's id
 * @param ending     ending to search for
 * @param ending_len ending's length
 * @param buffer     buffer to place ending's value to
 * @return True, if ending is in rule's dictionary. In that case buffer will
 *         contain ending's value.
 * @return False, if ending wasn't find.
 */
bool rules_find_ending_in_rule(Rules * rules, unsigned int id, const char * ending, int ending_len, int * buffer);

#endif

