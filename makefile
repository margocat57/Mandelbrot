CXX = g++
CLANG = clang++
PKG = `pkg-config --cflags --libs gtk+-3.0`
LIBS = -lm

# --- GCC builds for mandelbrot1 and mandelbrot2 ---
%_gcc_O0: %_test_new.cpp
	$(CXX) -O0 $< -o $@ $(PKG) $(LIBS)
%_gcc_O1: %_test_new.cpp
	$(CXX) -O1 $< -o $@ $(PKG) $(LIBS)
%_gcc_O2: %_test_new.cpp
	$(CXX) -O2 $< -o $@ $(PKG) $(LIBS)
%_gcc_O3: %_test_new.cpp
	$(CXX) -O3 $< -o $@ $(PKG) $(LIBS)

%_gcc_test_O0: %_test_new.cpp
	$(CXX) -O0 -DMEASURE_MANDELBROT -DO0_opt $< -o $@ $(PKG) $(LIBS)
%_gcc_test_O1: %_test_new.cpp
	$(CXX) -O1 -DMEASURE_MANDELBROT -DO1_opt $< -o $@ $(PKG) $(LIBS)
%_gcc_test_O2: %_test_new.cpp
	$(CXX) -O2 -DMEASURE_MANDELBROT -DO2_opt $< -o $@ $(PKG) $(LIBS)
%_gcc_test_O3: %_test_new.cpp
	$(CXX) -O3 -DMEASURE_MANDELBROT -DO3_opt $< -o $@ $(PKG) $(LIBS)

# --- Clang builds for mandelbrot1 and mandelbrot2 ---
%_clang_O0: %_test_new.cpp
	$(CLANG) -O0 $< -o $@ $(PKG) $(LIBS)
%_clang_O1: %_test_new.cpp
	$(CLANG) -O1 $< -o $@ $(PKG) $(LIBS)
%_clang_O2: %_test_new.cpp
	$(CLANG) -O2 $< -o $@ $(PKG) $(LIBS)
%_clang_O3: %_test_new.cpp
	$(CLANG) -O3 $< -o $@ $(PKG) $(LIBS)

%_clang_test_O0: %_test_new.cpp
	$(CLANG) -O0 -DMEASURE_MANDELBROT -DO0_opt $< -o $@ $(PKG) $(LIBS)
%_clang_test_O1: %_test_new.cpp
	$(CLANG) -O1 -DMEASURE_MANDELBROT -DO1_opt $< -o $@ $(PKG) $(LIBS)
%_clang_test_O2: %_test_new.cpp
	$(CLANG) -O2 -DMEASURE_MANDELBROT -DO2_opt $< -o $@ $(PKG) $(LIBS)
%_clang_test_O3: %_test_new.cpp
	$(CLANG) -O3 -DMEASURE_MANDELBROT -DO3_opt $< -o $@ $(PKG) $(LIBS)

# --- Special rules for mandelbrot3 (always with -march=native) ---
mandelbrot3_gcc_O0: mandelbrot3_test_new.cpp
	$(CXX) -O0 -march=native $< -o $@ $(PKG) $(LIBS)
mandelbrot3_gcc_O1: mandelbrot3_test_new.cpp
	$(CXX) -O1 -march=native $< -o $@ $(PKG) $(LIBS)
mandelbrot3_gcc_O2: mandelbrot3_test_new.cpp
	$(CXX) -O2 -march=native $< -o $@ $(PKG) $(LIBS)
mandelbrot3_gcc_O3: mandelbrot3_test_new.cpp
	$(CXX) -O3 -march=native $< -o $@ $(PKG) $(LIBS)

mandelbrot3_gcc_test_O0: mandelbrot3_test_new.cpp
	$(CXX) -O0 -DMEASURE_MANDELBROT -march=native -DO0_opt $< -o $@ $(PKG) $(LIBS)
mandelbrot3_gcc_test_O1: mandelbrot3_test_new.cpp
	$(CXX) -O1 -DMEASURE_MANDELBROT -march=native -DO1_opt $< -o $@ $(PKG) $(LIBS)
mandelbrot3_gcc_test_O2: mandelbrot3_test_new.cpp
	$(CXX) -O2 -DMEASURE_MANDELBROT -march=native -DO2_opt $< -o $@ $(PKG) $(LIBS)
mandelbrot3_gcc_test_O3: mandelbrot3_test_new.cpp
	$(CXX) -O3 -DMEASURE_MANDELBROT -march=native -DO3_opt $< -o $@ $(PKG) $(LIBS)

mandelbrot3_clang_O0: mandelbrot3_test_new.cpp
	$(CLANG) -O0 -march=native $< -o $@ $(PKG) $(LIBS)
mandelbrot3_clang_O1: mandelbrot3_test_new.cpp
	$(CLANG) -O1 -march=native $< -o $@ $(PKG) $(LIBS)
mandelbrot3_clang_O2: mandelbrot3_test_new.cpp
	$(CLANG) -O2 -march=native $< -o $@ $(PKG) $(LIBS)
mandelbrot3_clang_O3: mandelbrot3_test_new.cpp
	$(CLANG) -O3 -march=native $< -o $@ $(PKG) $(LIBS)

mandelbrot3_clang_test_O0: mandelbrot3_test_new.cpp
	$(CLANG) -O0 -DMEASURE_MANDELBROT -march=native -DO0_opt $< -o $@ $(PKG) $(LIBS)
mandelbrot3_clang_test_O1: mandelbrot3_test_new.cpp
	$(CLANG) -O1 -DMEASURE_MANDELBROT -march=native -DO1_opt $< -o $@ $(PKG) $(LIBS)
mandelbrot3_clang_test_O2: mandelbrot3_test_new.cpp
	$(CLANG) -O2 -DMEASURE_MANDELBROT -march=native -DO2_opt $< -o $@ $(PKG) $(LIBS)
mandelbrot3_clang_test_O3: mandelbrot3_test_new.cpp
	$(CLANG) -O3 -DMEASURE_MANDELBROT -march=native -DO3_opt $< -o $@ $(PKG) $(LIBS)

# --- Run rules ---
run_%_gcc_O0: %_gcc_O0
	./$<
run_%_gcc_O1: %_gcc_O1
	./$<
run_%_gcc_O2: %_gcc_O2
	./$<
run_%_gcc_O3: %_gcc_O3
	./$<

run_%_gcc_test_O0: %_gcc_test_O0
	taskset -c 4 ./$<
run_%_gcc_test_O1: %_gcc_test_O1
	taskset -c 4 ./$<
run_%_gcc_test_O2: %_gcc_test_O2
	taskset -c 4 ./$<
run_%_gcc_test_O3: %_gcc_test_O3
	taskset -c 4 ./$<

run_%_clang_O0: %_clang_O0
	./$<
run_%_clang_O1: %_clang_O1
	./$<
run_%_clang_O2: %_clang_O2
	./$<
run_%_clang_O3: %_clang_O3
	./$<

run_%_clang_test_O0: %_clang_test_O0
	taskset -c 4 ./$<
run_%_clang_test_O1: %_clang_test_O1
	taskset -c 4 ./$<
run_%_clang_test_O2: %_clang_test_O2
	taskset -c 4 ./$<
run_%_clang_test_O3: %_clang_test_O3
	taskset -c 4 ./$<

# --- Utility ---
clean:
	rm -f mandelbrot*_gcc_* mandelbrot*_clang_* *.o

