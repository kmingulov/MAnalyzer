all: lemmas_and_prefixes
	mkdir -p ../dics
	mkdir -p temp
	# Prepare lemmas lexicon.
	sqlite3 sqlite/lemmas.sqlite "SELECT key FROM shelf ORDER BY key;" > temp/lemmas
	sort temp/lemmas > temp/lemmas_sorted
	recode utf8..cp1251 temp/lemmas_sorted
	# Prepare predict prefixes lexicon.
	sqlite3 sqlite/misc.sqlite "SELECT value FROM shelf WHERE key = 'prefixes';" | sed s/", "/\\n/g | sed s/[\]\[\"]//g > temp/prefixes
	sort temp/prefixes > temp/prefixes_sorted
	recode utf8..cp1251 temp/prefixes_sorted
	# Creating dics.
	./lemmas_and_prefixes
	# Making lemmas_rules
	sqlite3 sqlite/lemmas.sqlite "SELECT value FROM shelf ORDER BY key;" | sed s/[\]\[,]//g > temp/lemmas_rules
	cat temp/lemmas_rules | (while read line; do echo `echo -n $line | wc -w; echo " $line";` | sed s/\\n//g; done;) > ../dics/lemmas_rules
	# Remove temp.
	rm -r temp/

lemmas_and_prefixes: lemmas_and_prefixes.cpp
	g++ -o lemmas_and_prefixes lemmas_and_prefixes.cpp

clean:
	rm -rf ../dics
	rm -f lemmas_and_prefixes
