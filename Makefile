ifeq ($(d), 1)
	DEBUG_MODE = -DANALYZER_DEBUG=1
else ifeq ($(d), 2)
	DEBUG_MODE = -DQUIET_ANALYZER_DEBUG=1
else
	DEBUG_MODE =
endif

KEYS = -O3

ma: main.o analyzer.o lemmas_rules.o forms.o rules.o
	g++ -g $(KEYS) -o ma main.o analyzer.o lemmas_rules.o forms.o rules.o

main.o: main.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c main.cpp

analyzer.o: analyzer/analyzer.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c analyzer/analyzer.cpp

lemmas_rules.o: analyzer/lemmas_rules.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c analyzer/lemmas_rules.cpp

rules.o: analyzer/rules.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c analyzer/rules.cpp

forms.o: analyzer/forms.cpp
	g++ $(KEYS) $(DEBUG_MODE) -g -c analyzer/forms.cpp

clean:
	rm -f *.o ma
