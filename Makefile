ifeq ($(d), 1)
	DEBUG_MODE = -DANALYZER_DEBUG=1
else ifeq ($(d), 2)
	DEBUG_MODE = -DQUIET_ANALYZER_DEBUG=1
else
	DEBUG_MODE =
endif

KEYS = -O3

ma: main.o analyzer.o analyzed_word.o lemmas_rules.o forms.o rules.o word_infos.o normal_forms.o endings_rules.o
	g++ -g $(KEYS) -o ma main.o analyzer.o analyzed_word.o lemmas_rules.o forms.o rules.o word_infos.o normal_forms.o endings_rules.o

cache: with_cache.o analyzer.o analyzed_word.o lemmas_rules.o forms.o rules.o word_infos.o normal_forms.o endings_rules.o
	g++ -g $(KEYS) -o cache with_cache.o analyzer.o analyzed_word.o lemmas_rules.o forms.o rules.o word_infos.o normal_forms.o endings_rules.o

main.o: main.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c main.cpp

with_cache.o: with_cache.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c with_cache.cpp

analyzer.o: analyzer/analyzer.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c analyzer/analyzer.cpp

analyzed_word.o: analyzer/analyzed_word.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c analyzer/analyzed_word.cpp

lemmas_rules.o: analyzer/lemmas_rules.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c analyzer/lemmas_rules.cpp

endings_rules.o: analyzer/endings_rules.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c analyzer/endings_rules.cpp

rules.o: analyzer/rules.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c analyzer/rules.cpp

forms.o: analyzer/forms.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c analyzer/forms.cpp

word_infos.o: analyzer/word_infos.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c analyzer/word_infos.cpp

normal_forms.o: analyzer/normal_forms.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c analyzer/normal_forms.cpp

clean:
	rm -f *.o ma
