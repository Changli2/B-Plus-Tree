BIN=./main
DATA=B-TREE_FILE POSTINGSFILE TEXTFILE
HANDIN=$(USER)-HW3.tar.gz

all: load test

$(BIN):
	cd SRC; make; cp main .. ; cp parms ..

$(DATA):
	\rm -f $(DATA)
	$(BIN) < import_dict.inp > /dev/null

load: $(BIN) $(DATA)

print: load
	$(BIN) < Tests/test_print.inp > print.mysol

test: test_search test_sanity test_predecessors test_successors
	@echo -e "\033[0;31m" all tests passed "\033[0m"

# make sure your output format is correct
test_sanity: load
	$(BIN) < Tests/test_sanity.inp > sanity.mysol
	@echo -e "\033[0;31m" $@ makes sure you are using identical output format, \
		 so pass me first"\033[0m"
	diff sanity.mysol Tests/sanity.sol
	@echo -e "\033[0;31m" $@ passed "\033[0m"

# for the answers we asked in the handout
test_predecessors: load
	$(BIN) < Tests/test_predecessors.inp > predecessors.mysol
	diff predecessors.mysol Tests/predecessors.sol
	@echo -e "\033[0;31m" $@ passed "\033[0m"

# for the answers we asked in the handout
test_successors: load
	$(BIN) < Tests/test_successors.inp > successors.mysol
	diff successors.mysol Tests/successors.sol
	@echo -e "\033[0;31m" $@ passed "\033[0m"

test_search: load
	$(BIN) < Tests/test_search.inp > search.mysol
	diff search.mysol Tests/search.sol
	@echo -e "\033[0;31m" $@ passed "\033[0m"

grade: load
	$(BIN) < ../../grade.inp > my_answers.sol
	diff my_answers.sol ../../correct.sol

clean:
	\rm -f $(BIN)
	cd SRC; make clean

spotless: clean
	\rm -f $(DATA)
	cd SRC; make spotless
	\rm -f $(BIN) parms
	\rm -f $(HANDIN)
	\rm -f *.mysol

handin: spotless
	tar czvfh $(HANDIN) \
		--exclude=*.o \
		--exclude=*.out \
		--exclude=.svn \
		--exclude=functional_tests.py \
		--exclude=import_dict.inp \
		--exclude=load_dict.sh \
		DOC/ README SRC/ Tests/ makefile Datafiles/

release: spotless
	tar cvfh btree.tar \
		--exclude=*.o \
		--exclude=*.out \
		--exclude=.svn \
		--exclude=functional_tests.py \
		DOC/ README SRC/ Tests/ makefile Datafiles/ import_dict.inp load_dict.sh
