all: lemmas_and_prefixes
	mkdir ../dics
	# Lemmas and prefixes.
	./lemmas_and_prefixes
	# Lemmas rules. Just copy.
	cp src/lemmas_rules ../dics/

lemmas_and_prefixes: lemmas_and_prefixes.cpp
	g++ -o lemmas_and_prefixes lemmas_and_prefixes.cpp

clean:
	rm -rf ../dics
	rm -f lemmas_and_prefixes
