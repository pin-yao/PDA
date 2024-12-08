TARGET:= Legalizer
SRCS:= main.cpp
CFLAGS:= -std=c++11 -Wall
INPUTS := ./testcase/testcase1_MBFF_LIB_7000.lg ./testcase/testcase1_16900.lg ./testcase/testcase1_ALL0_5000.lg ./testcase/testcase2_100.lg
INPUTS2 := ./testcase/testcase1_MBFF_LIB_7000.opt ./testcase/testcase1_16900.opt ./testcase/testcase1_ALL0_5000.opt ./testcase/testcase2_100.opt
OUTS := testcase1_MBFF_LIB_7000_post.lg testcase1_16900_post.lg testcase1_ALL0_5000_post.lg testcase2_100_post.lg


.PHONY: all debug run runall evaluate evaluateall clean
all:$(TARGET)

$(TARGET):$(SRCS)
	g++ $(CFLAGS) $(SRCS) -lm -O3 -o $(TARGET)

debug: $(TARGET)
	@input=./testcase/testcase1_ALL0_5000.lg; \
    input2=./testcase/testcase1_ALL0_5000.opt; \
    output=testcase1_ALL0_5000_post.lg; \
    echo "Running with $$input and $$input2, output to $$output..."; \
    gdb --args ./$(TARGET) $$input $$input2 $$output;

run: $(TARGET)
	time ./$(TARGET)	./testcase/testcase1_MBFF_LIB_7000.lg	./testcase/testcase1_MBFF_LIB_7000.opt	testcase1_MBFF_LIB_7000_post.lg fordraw.lg

runall: $(TARGET)
	@for i in $(shell seq 0 3); do \
		input=$$(echo $(INPUTS) | cut -d' ' -f$$((i+1))); \
		input2=$$(echo $(INPUTS2) | cut -d' ' -f$$((i+1))); \
		output=$$(echo $(OUTS) | cut -d' ' -f$$((i+1))); \
		echo "Running with $$input and $$input2, output to $$output..."; \
		time ./$(TARGET) $$input $$input2 $$output; \
	done

evaluate: $(TARGET)
	./Evaluator	./testcase/testcase1_MBFF_LIB_7000.lg	./testcase/testcase1_MBFF_LIB_7000.opt	testcase1_MBFF_LIB_7000_post.lg

evaluateall: $(TARGET)
	@for i in $(shell seq 0 3); do \
		input=$$(echo $(INPUTS) | cut -d' ' -f$$((i+1))); \
		input2=$$(echo $(INPUTS2) | cut -d' ' -f$$((i+1))); \
		output=$$(echo $(OUTS) | cut -d' ' -f$$((i+1))); \
		echo "Evaluating $$output..."; \
		./Evaluator $$input $$input2 $$output; \
	done

clean:
	rm -f $(TARGET) $(OUTS) *.png
