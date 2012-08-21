# Language (ru or en).
DICS_LANG			= en

# Paths.
SQLITE_BASE_DIR 	= sqlite_$(DICS_LANG)
DICS_DIR			= ../dics_$(DICS_LANG)

# List of used software. Default *nix commands like mkdir, cat, sort and sed
# aren't added.
SQLITE 			 	= sqlite3
RECODE 				= recode -f
C 					= gcc
C++ 				= g++

# If language is english, we don't use recode.
ifeq ($(DICS_LANG), en)
	RECODE			= \# recode -f
endif

all: make_dawgdics make_rules rules_split_and_sort rules_dics normal_forms ending_rule add_endings_lens
	rm -rf $(DICS_DIR) temp/

	mkdir -p $(DICS_DIR)
	mkdir -p temp

	# Preparing data from sqlite-base.
	# Lemmas, predict prefixes and just prefixes.
	$(SQLITE) $(SQLITE_BASE_DIR)/lemmas.sqlite "SELECT key FROM shelf ORDER BY key;" > temp/lemmas
	$(SQLITE) $(SQLITE_BASE_DIR)/misc.sqlite "SELECT value FROM shelf WHERE key = 'prefixes';" | sed s/", "/" 0\n"/g | sed s/[\]\[\"]//g > temp/predict_prefixes
	$(SQLITE) $(SQLITE_BASE_DIR)/misc.sqlite "SELECT value FROM shelf WHERE key = 'possible_rule_prefixes'" | sed s/", "/\\n/g | sed s/[\]\[\"]//g | (while read line; do counter=`echo "$counter+1" | bc -ql`; echo "$$line $$counter"; done;) > temp/possible_prefixes
	# Lemmas rules.
	$(SQLITE) $(SQLITE_BASE_DIR)/lemmas.sqlite "SELECT value FROM shelf ORDER BY key;" | sed s/[\]\[,]//g > temp/lemmas_rules
	# Raw rules.
	$(SQLITE) $(SQLITE_BASE_DIR)/rules.sqlite "SELECT value FROM shelf;" | sed s/\"\"/*/g | sed s/[\]\[\",]//g > temp/raw_rules
	# Endings (for prediction).
	$(SQLITE) $(SQLITE_BASE_DIR)/endings.sqlite "SELECT * FROM shelf ORDER BY key;" > temp/endings

	# Prepare lemmas lexicon.
	$(RECODE) utf8..cp1251 temp/lemmas
	# Prepare predict prefixes lexicon.
	cat temp/possible_prefixes >> temp/predict_prefixes
	sort temp/predict_prefixes > temp/predict_prefixes_sorted
	$(RECODE) utf8..cp1251 temp/predict_prefixes_sorted
	# Prepare endings lexicon. (See make_dawgdics.cpp for some notes.)
	$(RECODE) utf8..cp1251 temp/endings;
	# Make reversed endings, add counts and delete excess chars.
	cat temp/endings | while read line; do \
		new_line=`echo "$$line" | ./ending_rule`; \
		echo "$$new_line" >> temp/endings_reversed; \
	done;
	# Sort.
	$(RECODE) cp1251..utf8 temp/endings_reversed;
	sort temp/endings_reversed > temp/endings_sorted;
	# Split to 2 files.
	cat temp/endings_sorted | while read line; do \
		end=`echo "$$line" | cut -d' ' -f1`; \
		rules=`echo "$$line" | cut -d' ' -f2-`; \
		echo "$$end" >> temp/ends; \
		echo "$$rules" >> temp/endings_rules; \
	done;
	$(RECODE) utf8..cp1251 temp/ends;
	# Creating dawgdics.
	./make_dawgdics $(DICS_DIR)

	# Making lemmas_rules and preparing endings_rules
	./make_rules temp/lemmas_rules $(DICS_DIR)/lemmas_rules
	# Add count of rules to the first line.
	sed -i "1i`wc -l $(DICS_DIR)/lemmas_rules | cut -f1 --delimiter=' '`" $(DICS_DIR)/lemmas_rules
	sed -i "1i`wc -l temp/endings_rules | cut -f1 --delimiter=' '`" temp/endings_rules

	# Making gramtab.
	$(SQLITE) $(SQLITE_BASE_DIR)/misc.sqlite "SELECT value FROM shelf WHERE key='gramtab'" | \
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
	# Add count of rules to the first line.
	sed -i "1i`wc -l temp/rules_with_counts | cut -f1 --delimiter=' '`" temp/rules_with_counts

	# Making ending_rules.
	./add_endings_lens $(DICS_DIR)/endings_rules

	# Make file with normal forms and add amount of them to the first line.
	./normal_forms temp/normal_forms $(DICS_DIR)/normal_forms
	sed -i "1i`wc -l $(DICS_DIR)/normal_forms | cut -f1 --delimiter=' '`" $(DICS_DIR)/normal_forms
	$(RECODE) utf8..cp1251 $(DICS_DIR)/normal_forms
	# Splitting raw rules into files and sort them.
	mkdir -p temp/splitted/
	./rules_split_and_sort
	# Convert each rule to dawgdic.
	$(RECODE) -f utf8..cp1251 temp/splitted/*
	mkdir -p $(DICS_DIR)/rules/
	./rules_dics temp/splitted $(DICS_DIR)/rules
	cp temp/splitted/info $(DICS_DIR)/rules/info
	# Add count of forms to the first line.
	sed -i "1i`wc -l $(DICS_DIR)/rules/forms | cut -f1 --delimiter=' '`" $(DICS_DIR)/rules/forms

	# Copying gramtab.
	cp temp/gramtab $(DICS_DIR)/gramtab
	$(RECODE) utf8..cp1251 $(DICS_DIR)/gramtab

	# Remove temp.
	#~ rm -r temp/

make_dawgdics: make_dawgdics.cpp
	$(C++) -g -o make_dawgdics make_dawgdics.cpp

make_rules: make_rules.c
	$(C) -g -std=c99 -o make_rules make_rules.c

ending_rule: ending_rule.c
	$(C) -g -std=c99 -o ending_rule ending_rule.c

rules_split_and_sort: rules_split_and_sort.cpp
	$(C++) -g -o rules_split_and_sort rules_split_and_sort.cpp

rules_dics: rules_dics.cpp
	$(C++) -g -o rules_dics rules_dics.cpp

normal_forms: normal_forms.cpp
	$(C++) -g -o normal_forms normal_forms.cpp

add_endings_lens: add_endings_lens.cpp
	$(C++) -g -o add_endings_lens add_endings_lens.cpp

clean:
	rm -rf $(DICS_DIR) temp/
	rm -f lemmas_and_prefixes lemmas_rules rules_split_and_sort rules_dics
