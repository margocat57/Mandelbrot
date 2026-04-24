COMPILER ?= g++
PKG = $(shell pkg-config --cflags --libs gtk+-3.0)
LIBS = -lm
CFLAGS = -march=native -Wall -Wextra

OPT ?= O2
MODE ?=
ARGS ?=

SRC_DIR = realisations
SRC = $(SRC_DIR)/mandelbrot_main.cpp $(SRC_DIR)/$(TARGET).cpp

DEFS =
ifeq ($(MODE),MEASURE)
DEFS = -DMEASURE
endif
ifeq ($(MODE),MEASURE_DRAW)
DEFS = -DMEASURE_DRAW
endif

$(TARGET): $(SRC)
	$(COMPILER) -$(OPT) $(CFLAGS) $(DEFS) $^ -o $@ $(PKG) $(LIBS)

run: $(TARGET)
	$(if $(MODE), \
		taskset -c 3 ./$(TARGET) $(ARGS) > result.txt && \
		python3 script.py $(TARGET) $(COMPILER) $(OPT) $(MODE) < result.txt, \
		./$(TARGET))

clean:
	rm -f mandelbrot* result.txt

# make TARGET=mandelbrot2 COMPILER=clang++ OPT=O3 run
# make TARGET=mandelbrot2 COMPILER=clang++ OPT=O3 MODE=MEASURE ARGS="30 10" run
# make TARGET=mandelbrot2 COMPILER=clang++ OPT=O3 MODE=MEASURE_DRAW ARGS="30 10" run