TARGET:= D2DGRter
SRCS:= main.cpp
CFLAGS:= -std=c++11
INPUTS := ./testcase0/testcase0.gmp ./testcase1/testcase1.gmp ./testcase2/testcase2.gmp
INPUTS2 := ./testcase0/testcase0.gcl ./testcase1/testcase1.gcl ./testcase2/testcase2.gcl
INPUTS3 := ./testcase0/testcase0.cst ./testcase1/testcase1.cst ./testcase2/testcase2.cst
OUTS := ./testcase0/testcase0.lg ./testcase1/testcase1.lg ./testcase2/testcase2.lg


.PHONY: all  degug run runall evaluate evaluateall clean
all:$(TARGET)

$(TARGET):$(SRCS)
	g++ $(CFLAGS) $(SRCS) -lm -O3 -o $(TARGET)

debug: $(SRCS)
	g++ $(CFLAGS) $(SRCS) -lm -g -o $(TARGET)
	gdb --args ./$(TARGET) ./testcase0/testcase0.gmp ./testcase0/testcase0.gcl ./testcase0/testcase0.cst ./testcase0/testcase0.lg

run: $(TARGET)
	time ./$(TARGET) ./testcase0/testcase0.gmp ./testcase0/testcase0.gcl ./testcase0/testcase0.cst ./testcase0/testcase0.lg

runall: $(TARGET)
	@for i in $(shell seq 0 2); do \
		input=$$(echo $(INPUTS) | cut -d' ' -f$$((i+1))); \
		input2=$$(echo $(INPUTS2) | cut -d' ' -f$$((i+1))); \
		input3=$$(echo $(INPUTS3) | cut -d' ' -f$$((i+1))); \
		output=$$(echo $(OUTS) | cut -d' ' -f$$((i+1))); \
		echo "Running with $$input and $$input2 and $$input3, output to $$output..."; \
		time ./$(TARGET) $$input $$input2 $$input3 $$output; \
	done

evaluate: $(TARGET)
	./Evaluator	./testcase0 testcase0

evaluateall: $(TARGET)
	./Evaluator	./testcase0 testcase0
	./Evaluator	./testcase1 testcase1
	./Evaluator	./testcase2 testcase2

clean:
	rm -f $(TARGET) $(OUTS)
