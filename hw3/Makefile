# The following two variable will be command line determined by TA
# For testing,you could uncomment them
SRIPATH ?= /home/master/06/r06922134/DSP2018FALL/hw3_new/srilm-1.5.10
MACHINE_TYPE ?= i686-m64
LM ?= bigram.lm

CXX = g++
CXXFLAGS = -O3 -I$(SRIPATH)/include -w
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)

TARGET = mydisambig
SRC = mydisambig.cpp
OBJ = $(SRC:.cpp=.o)
TO = ZhuYin-Big5.map
FROM = Big5-ZhuYin.map
.PHONY: all clean map run
#make MACHINE_TYPE=i686-m64 SRIPATH=/home/master/06/r06922134/DSP2018FALL/hw3/srilm-1.5.10_i686-m all

all: $(TARGET)

$(TARGET): $(OBJ) -loolm -ldstruct -lmisc
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

#make MACHINE_TYPE=i686-m64 SRIPATH=/home/master/06/r06922134/DSP2018FALL/hw3/srilm-1.5.10_i686-m run

run: 
	mkdir -p ./result2
	@#TODO How to run your code toward different txt?
	@for i in $(shell seq 1 10) ; do \
		echo "Running $$i.txt"; \
		./mydisambig -text testdata/$$i.txt -map $(TO) -lm $(LM) -order 2 > result2/$$i.txt; \
	done;

map:
	@#TODO how to map?
	@echo "Mapping!"
	@#./mapping $(FROM) $(TO)
	@#matlab mapping.m $(FROM) $(TO)
	@#python mapping.py $(FROM) $(TO)
	@python mapping.py $(FROM) $(TO)
	@#sh mapping.sh $(FROM) $(TO)
	@#perl mapping.pl Big5-ZhuYin.map ZhuYin-Big5.map

clean:
	$(RM) $(OBJ) $(TARGET)

