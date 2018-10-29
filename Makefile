.PHONY: all clean

CFLAGS+= -std=c++11 -g -Wall
LDFLAGS+=-lm     # link to math library

TARGET=train test cal_acc 

all: $(TARGET)
# type make/make all to compile test_hmm

train: train.cpp
	g++ $(CFLAGS) train.cpp -o train
test: test.cpp
	g++ $(CFLAGS) test.cpp -o test
acc: cal_acc.cpp
	g++ $(CFLAGS) cal_acc.cpp -o cal_acc
clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
