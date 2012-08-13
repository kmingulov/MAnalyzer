ifeq ($(debug), true)
	DEBUG_MODE = -g -DMANALYZER_DEBUG=1
else
	DEBUG_MODE =
endif

CFLAGS = -c -fPIC -O3 $(DEBUG_MODE)

OBJS = analyzer.o analyzed_word.o lemmas_rules.o forms.o rules.o word_infos.o normal_forms.o endings_rules.o

libmanalyzer.so: $(OBJS)
	g++ -shared -o libmanalyzer.so $(OBJS)

analyzer.o: analyzer.cpp
	g++ $(CFLAGS) analyzer.cpp

analyzed_word.o: analyzed_word.cpp
	g++ $(CFLAGS) analyzed_word.cpp

lemmas_rules.o: lemmas_rules.cpp
	g++ $(CFLAGS) lemmas_rules.cpp

endings_rules.o: endings_rules.cpp
	g++ $(CFLAGS) endings_rules.cpp

rules.o: rules.cpp
	g++ $(CFLAGS) rules.cpp

forms.o: forms.cpp
	g++ $(CFLAGS) forms.cpp

word_infos.o: word_infos.cpp
	g++ $(CFLAGS) word_infos.cpp

normal_forms.o: normal_forms.cpp
	g++ $(CFLAGS) normal_forms.cpp

clean:
	rm -f *.o
