# List of used software. Default *nix commands like mkdir, cat, sort and sed
# aren't added.
SQLITE = sqlite3
RECODE = recode
C = gcc
C++ = g++

all: lemmas_and_prefixes lemmas_rules rules_split_and_sort rules_dics normal_forms
	mkdir -p ../dics
	mkdir -p temp

	# Preparing data from sqlite-base.
	# Lemmas, predict prefixes and just prefixes.
	$(SQLITE) sqlite/lemmas.sqlite "SELECT key FROM shelf ORDER BY key;" > temp/lemmas
	$(SQLITE) sqlite/misc.sqlite "SELECT value FROM shelf WHERE key = 'prefixes';" | sed s/", "/\\n/g | sed s/[\]\[\"]//g > temp/predict_prefixes
	$(SQLITE) sqlite/misc.sqlite "SELECT value FROM shelf WHERE key = 'possible_rule_prefixes'" | sed s/", "/\\n/g | sed s/[\]\[\"]//g > temp/prefixes
	# Lemmas rules.
	$(SQLITE) sqlite/lemmas.sqlite "SELECT value FROM shelf ORDER BY key;" | sed s/[\]\[,]//g > temp/lemmas_rules
	# Raw rules.
	$(SQLITE) sqlite/rules.sqlite "SELECT value FROM shelf;" | sed s/\"\"/*/g | sed s/[\]\[\",]//g > temp/raw_rules

	# Prepare lemmas lexicon.
	$(RECODE) utf8..cp1251 temp/lemmas
	# Prepare predict prefixes lexicon.
	sort temp/predict_prefixes > temp/predict_prefixes_sorted
	$(RECODE) utf8..cp1251 temp/predict_prefixes_sorted
	# Creating dawgdics.
	./lemmas_and_prefixes

	# Making lemmas_rules
	./lemmas_rules
	# Add count of rules to the first line.
	sed -i "1i`wc -l ../dics/lemmas_rules | cut -f1 --delimiter=' '`" ../dics/lemmas_rules

	# Making gramtab.
	$(SQLITE) sqlite/misc.sqlite "SELECT value FROM shelf WHERE key='gramtab'" | \
	# Add newlines. \
	sed s/],\ \"/]\\n/g | \
	# Remove quotes. \
	sed s/\ \"//g | sed s/\"://g | \
	# And at the begining and the end too. \
	sed s/{\"// | sed s/]}/]/ > temp/raw_gramtab
	# Sort and enumerate each line.
	sort temp/raw_gramtab | cat -n > temp/gramtab

	# Making rules.
	# Add count of forms to each line and prepare file with normal forms.
	cat temp/raw_rules | while read line; do \
		forms_count=`(echo -n \`echo "$$line" | wc -w\`; echo "/3";) | bc -q`; \
		only_first=`echo "$$line" | cut --delimiter=' ' -f1,2,3`; \
		\
		echo "$$forms_count $$line" >> temp/rules_with_counts; \
		echo "$$only_first" >> temp/normal_forms; \
	done;
	# Make file with normal forms and add amount of them to the first line.
	./normal_forms
	sed -i "1i`wc -l ../dics/normal_forms | cut -f1 --delimiter=' '`" ../dics/normal_forms
	$(RECODE) -f utf8..cp1251 ../dics/normal_forms
	# Add count of rules to the first line.
	sed -i "1i`wc -l temp/rules_with_counts | cut -f1 --delimiter=' '`" temp/rules_with_counts
	# Splitting raw rules into files and sort them.
	mkdir -p temp/splitted/
	./rules_split_and_sort
	# Convert each rule to dawgdic.
	$(RECODE) -f utf8..cp1251 temp/splitted/*
	mkdir -p ../dics/rules/
	./rules_dics
	# Add count of forms to the first line.
	sed -i "1i`wc -l ../dics/rules/forms | cut -f1 --delimiter=' '`" ../dics/rules/forms

	# Remove temp.
	rm -r temp/

lemmas_and_prefixes: lemmas_and_prefixes.cpp
	$(C++) -g -o lemmas_and_prefixes lemmas_and_prefixes.cpp

lemmas_rules: lemmas_rules.c
	$(C) -g -std=c99 -o lemmas_rules lemmas_rules.c

rules_split_and_sort: rules_split_and_sort.cpp
	$(C++) -g -o rules_split_and_sort rules_split_and_sort.cpp

rules_dics: rules_dics.cpp
	$(C++) -g -o rules_dics rules_dics.cpp

normal_forms: normal_forms.cpp
	$(C++) -g -o normal_forms normal_forms.cpp

clean:
	rm -rf ../dics
	rm -f lemmas_and_prefixes lemmas_rules rules_split_and_sort rules_dics
