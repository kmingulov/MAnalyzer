MAnalyzer 0.1 {#mainpage}
=============

**MAnalyzer** is fast morph analyzer library, written in C++. It is based on
pymorphy algorithm [(read in russian)]
(http://packages.python.org/pymorphy/algo.html) and dictionaries.

Features:
* Return grammar info of the word and normal form with grammar info for the
  word.
* Prediction for unknown words.

MAnalyzer lib released under *MIT License* (look for LICENSE file).
`create_dics/` script and examples in `./main.cpp` and `./cache.cpp` files have
nothing in common with MAnalyzer library, they are released under MIT License
too.

Compilation
------------

Just:

    cd analyzer
    make

To see debug information while MAnalyzer using use `make debug=true` instead of
`make`.

Also you can copy `libmanalyzer.so` file to `/usr/lib/` directory (or just use
`-L` flag of g++ compiler).

Dictionary
----------

First of all you need dictionary. In create_dics/ directory there is script,
that will make dictionary from sqlite-json-dictionaries for pymorphy. You can
download them [here](https://bitbucket.org/kmike/pymorphy/downloads).

Anyway you always can download complete dictionaries from
[here](https://github.com/Melkogotto/MAnalyzer/downloads).

Usage
-----

Usage is simple. Include two header files:

    #include "analyzer/word_infos.hpp"
    #include "analyzer/analyzer.hpp"

And somewhere add these lines:

    // Creating analyzer. (dics is directory with dictionary).
    Analyzer * analyzer = analyzer_new("dics");
    // Array with results. (1024 is array size).
    WordInfos * wi = infos_new(1024);
    // Analyzing word.
    analyzer_get_word_info(analyzer, word, word_size, wi);

To get information about word use these functions:

    // Gets amount of results.
    int infos_get_size(WordInfos * wi);
    // Gets normal form for result.
    char * infos_get_normal_form(WordInfos * wi, unsigned int id);
    // Gets normal form's id. 
    unsigned short int infos_get_normal_form_id(WordInfos * wi, unsigned int id);
    // Gets form's id for analyzed word.
    unsigned short int infos_get_form_id(WordInfos * wi, unsigned int id);

To compile program use `g++ main.cpp -lmanalyzer`.

More information you can find out in documentation.

ToDo
----

* Add dawgdic source code and compile library using it.
* Finish documentation (LemmasRules', EndingsRules' descriptions and algorithm's
  description).
* Refactoring of create_dics/* code is needed!
* Prevent program from crashing when there is no dictionaries.

ChangeLog
---------

**0.1**
* Initial release.
