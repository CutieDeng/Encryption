cc = g++-12 -std=c++23 -fmodules-ts
target = game

src_files = $(wildcard includes/*.cc)
obj_files = $(patsubst includes/%.cc,out/%.o,$(src_files))
bin_files = $(wildcard src/**.cc) 
target_object = $(patsubst src/%.cc,out/%.o,$(bin_files)) 
targets = $(patsubst src/%.cc,bin/%,$(bin_files)) 

all: $(targets) 

$(obj_files): out/%.o : includes/%.cc
	$(cc) -c $< -o $@

$(target_object): out/%.o : src/%.cc 
	$(cc) -c $< -o $@

$(targets): bin/% : out/%.o
	$(cc) $(obj_files) $< -o $@

$(target_object) : $(obj_files) 

.PHONY: run test release all clean rebuild

run: bin/main 
	@./bin/main

r: run 

check: $(src_files) $(bin_files) 
	$(cc) -fsyntax-only $(src_files) $(bin_files)

c: check

clean: 
	-rm -rf $(wildcard out/*.o) $(wildcard bin/*) 
	@# -rm -rf $(target_object) $(targets) $(obj_files)

rebuild: 
	-rm -rf gcm.cache 
	./build.py