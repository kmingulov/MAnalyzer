MAnalyzer
=========

**MAnalyzer** is morph analyzer.

Compilation
------------

Just:

    cd analyzer;
    make;
To see debug information while MAnalyzer using use `make debug=true` instead of `make`.

Also you can copy `libmanalyzer.so` file to `/usr/lib/` directory (or just use `-L` flag of g++ compiler).

Dictionary
----------

First of all you need dictionary. In create_dics/ directory there is script,
that will make dictionary from sqlite-json-dictionaries for pymorphy. You can
download them [here](https://bitbucket.org/kmike/pymorphy/downloads).

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

TODO
----
* Id of prefixes ПО and НАИ are hardcoded now.
