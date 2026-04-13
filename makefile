COMPILER ?= g++
PKG = $(shell pkg-config --cflags --libs gtk+-3.0)
LIBS = -lm
CFLAGS = -march=native -Wall -Wextra

OPT ?= O2

MEASURE ?=

SRC_DIR = realisations
SRC = $(SRC_DIR)/mandelbrot_main.cpp $(SRC_DIR)/$(TARGET).cpp

$(TARGET): $(SRC)
	$(COMPILER) -$(OPT) $(CFLAGS) $^ -o $@ $(PKG) $(LIBS)

run: $(TARGET)
	$(if $(MEASURE), \
		taskset -c 3 ./$(TARGET) --measure $(MEASURE) > result.txt && \
		python3 script.py $(TARGET) $(COMPILER) $(OPT) < result.txt, \
		./$(TARGET))


clean:
	rm -f mandelbrot*


# Обычный
# make TARGET=mandelbrot2 COMPILER=clang++ OPT=O3  run

# Тесты
# make TARGET=mandelbrot2 COMPILER=clang++ OPT=O3 MEASURE="50 20" run