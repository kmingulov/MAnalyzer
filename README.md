MAnalyzer
=========
**SimpleCiv** is morph analyzer.

Compilation
------------
Just `make`. To use debug information use `make debug=true`.

Note: this will compile whole program including MAnalyzer, not a dynamic library.

Usage
-----
First of all you need dictionary. In create_dics/ directory there is script,
that will make dictionary from sqlite-json-dictionaries for pymorphy. You can
download them [here](https://bitbucket.org/kmike/pymorphy/downloads).

Usage is simple. Include two header files:

    #include "analyzer/word_infos.hpp"
    #include "analyzer/analyzer.hpp"

Add somewhere add these lines:

    // Creating analyzer.
    Analyzer * analyzer = analyzer_new("dics");
    // Array with results.
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
