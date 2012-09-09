/**
 * @file doxygen_main_page.h
 * @brief MAnalyzer Doxygen documentation's main page.
 */

#ifndef DOXYGEN_MAIN_PAGE_H
#define DOXYGEN_MAIN_PAGE_H

/**
 * @mainpage
 * @author  K. Mingulov <melkogotto@gmail.com>
 * @version 0.1
 *
 * @section Description
 *
 * **MAnalyzer** is fast morph analyzer library, written in C++. It is based on
 * pymorphy algorithm [(read in russian)]
 * (http://packages.python.org/pymorphy/algo.html) and dictionaries.
 *
 * Features:
 *  * Return grammar info of the word and normal form with grammar info for
 *    the word.
 *  * Prediction for unknown words.
 *
 * MAnalyzer lib released under *MIT License* (look for LICENSE or section
 * License below file). `create_dics/` script and examples in `./main.cpp` and
 * `./cache.cpp` files have nothing in common with MAnalyzer library. They are
 * released under MIT License too.
 *
 * @section Compilation
 *
 * Just:
 *
 *     cd analyzer;
 *     make;
 *
 * To see debug information while MAnalyzer using use `make debug=true` instead
 * of `make`.
 *
 * Also you can copy `libmanalyzer.so` file to `/usr/lib/` directory (or just
 * use `-L` flag of g++ compiler).
 *
 * @section Dictionary
 *
 * First of all you need dictionary. In create_dics/ directory there is script,
 * that will make dictionary from sqlite-json-dictionaries for pymorphy. You can
 * download them [here](https://bitbucket.org/kmike/pymorphy/downloads).
 *
 * Anyway you always can download complete dictionaries from
 * [here](https://github.com/Melkogotto/MAnalyzer/downloads).
 *
 * @section Usage
 *
 * Usage is simple. Include two header files:
 *
 *     #include "analyzer/word_infos.hpp"
 *     #include "analyzer/analyzer.hpp"
 *
 * And somewhere add these lines:
 *
 *     // Creating analyzer. (dics is directory with dictionary).
 *     Analyzer * analyzer = analyzer_new("dics");
 *     // Array with results. (1024 is array size).
 *     WordInfos * wi = infos_new(1024);
 *     // Analyzing word.
 *     analyzer_get_word_info(analyzer, word, word_size, wi);
 *
 * To get information about word use these functions:
 *
 *     // Gets amount of results.
 *     int infos_get_size(WordInfos * wi);
 *     // Gets normal form for result.
 *     char * infos_get_normal_form(WordInfos * wi, unsigned int id);
 *     // Gets normal form's id.
 *     unsigned short int infos_get_normal_form_id(WordInfos * wi, unsigned int id);
 *     // Gets form's id for analyzed word.
 *     unsigned short int infos_get_form_id(WordInfos * wi, unsigned int id);
 *
 * To compile program use `g++ main.cpp -lmanalyzer`.
 *
 * More information you can find out in documentation.
 *
 * @section License
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * @section ToDo
 *
 *  * Refactoring of create_dics/* code is needed!
 *  * Prevent program from crashing when there is no dictionaries.
 */

#endif // DOXYGEN_MAIN_PAGE_H
