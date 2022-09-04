cc = g++-12 -std=c++23 -fmodules-ts
target = game

src_files = $(wildcard src/*.cc)
obj_files = $(patsubst src/%.cc,out/%.o,$(src_files))

bin/main: out/main.o 
	$(cc) $(obj_files) -o $@

$(obj_files): out/%.o : src/%.cc
	$(cc) -c $< -o $@

out/main.o: out/sha1.o

.PHONY: run test

run: bin/main 
	@./bin/main

r: run

check: $(src_files) 
	$(cc) -fsyntax-only $(src_files)

c: check