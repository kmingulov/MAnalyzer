all: lemmas_and_prefixes
	mkdir ../dics
	./lemmas_and_prefixes

lemmas_and_prefixes: lemmas_and_prefixes.cpp
	g++ -o lemmas_and_prefixes lemmas_and_prefixes.cpp

clean:
	rm -rf ../dics
	rm -f lemmas_and_prefixes
