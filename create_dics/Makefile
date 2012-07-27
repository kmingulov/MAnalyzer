SQLITE = sqlite3
RECODE = recode
C = gcc
C++ = g++

all: lemmas_and_prefixes lemmas_rules
	mkdir -p ../dics
	mkdir -p temp

	# Preparing data from sqlite-base.
	# Lemmas and prefixes.
	$(SQLITE) sqlite/lemmas.sqlite "SELECT key FROM shelf ORDER BY key;" > temp/lemmas
	$(SQLITE) sqlite/misc.sqlite "SELECT value FROM shelf WHERE key = 'prefixes';" | sed s/", "/\\n/g | sed s/[\]\[\"]//g > temp/prefixes
	# Lemmas rules.
	$(SQLITE) sqlite/lemmas.sqlite "SELECT value FROM shelf ORDER BY key;" | sed s/[\]\[,]//g > temp/lemmas_rules
	# Raw rules.
	$(SQLITE) sqlite/rules.sqlite "SELECT value FROM shelf;" | sed s/\"\"/*/g | sed s/[\]\[\",]//g > temp/raw_rules

	# Prepare lemmas lexicon.
	$(RECODE) utf8..cp1251 temp/lemmas
	# Prepare predict prefixes lexicon.
	sort temp/prefixes > temp/prefixes_sorted
	$(RECODE) utf8..cp1251 temp/prefixes_sorted
	# Creating dawgdics.
	./lemmas_and_prefixes

	# Making lemmas_rules
	./lemmas_rules
	# Add count of rules to the first line.
	sed -i "1i`wc -l ../dics/lemmas_rules | cut -f1 --delimiter=' '`" ../dics/lemmas_rules

	# Making rules.
	# Add count of forms to each line;
	cat temp/raw_rules | while read line; do \
		forms_count=`(echo -n \`echo "$$line" | wc -w\`; echo "/3";) | bc -q`; \
		echo "$$forms_count $$line"; \
	done > temp/rules_with_counts;
	# Add count of rules to the first line.
	sed -i "1i`wc -l temp/rules_with_counts | cut -f1 --delimiter=' '`" temp/rules_with_counts
	# Splitting raw rules into files (to sort each rule).
	# Sort each rule.
	# Merge rules back into one file.

	# Remove temp.
	#~ rm -r temp/

lemmas_and_prefixes: lemmas_and_prefixes.cpp
	g++ -g -o lemmas_and_prefixes lemmas_and_prefixes.cpp

lemmas_rules: lemmas_rules.c
	gcc -g -std=c99 -o lemmas_rules lemmas_rules.c

clean:
	rm -rf ../dics
	rm -f lemmas_and_prefixes lemmas_rules
