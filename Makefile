CC := gcc
CXX := g++
ifeq ($(OS),Windows_NT)
	RE := rg
else
	RE := grep
endif
FLAGS := -Wall -ggdb -O2
CFLAGS := -std=gnu99 $(FLAGS)
CXXFLAGS := -std=gnu++11 $(FLAGS)
# LDFLAGS := -fuse-ld=mold
LDFLAGS :=
ALL_SRC := $(wildcard src/*.c) $(wildcard src/*/*.c) $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
EX := $(patsubst src/examples/%.cpp, %, $(wildcard src/examples/*.cpp)) $(patsubst src/examples/%.c, %, $(wildcard src/examples/*.c))
C_SRC := $(wildcard src/*.c)
CXX_SRC := $(wildcard src/*.cpp)
C_OBJ := $(patsubst src/%.c, obj/%.o, $(C_SRC))
CXX_OBJ := $(patsubst src/%.cpp, obj/%.opp, $(CXX_SRC))
DEP := $(patsubst src/%.c, dep/%.d, $(C_SRC)) $(patsubst src/%.cpp, dep/%.dpp, $(CXX_SRC))
ALL_OBJ := $(patsubst src/%.cpp, obj/%.opp, $(ALL_SRC)) $(patsubst src/%.c,obj/%.o, $(ALL_SRC))
CLEAN_EX := $(patsubst %, clean/%, $(EX))

.PHONY: default test clean dependencies $(CLEAN_EX) all

default: all

all: | $(EX)

test: arena_quadtree
	${CURDIR}/$<

# If I didn't mix C w/ C++ AND have raylib for two of my examples I'd stick with this
# $(EX): %: src/examples/%.c $(C_OBJ)
# 	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bad_quadtree: src/examples/bad_quadtree.cpp obj/quadtree.opp obj/rando.o obj/vector.o
	$(CXX) $(CXXFLAGS) -o $@ $^ -lraylib $(LDFLAGS)

ok_quadtree: src/examples/ok_quadtree.c obj/quadtree.o obj/rando.o obj/vector.o
	$(CC) $(CFLAGS) -o $@ $^ -lraylib $(LDFLAGS)

arena_quadtree: src/examples/arena_quadtree.c obj/quadtree.o obj/rando.o obj/vector.o obj/arena.o
	$(CC) $(CFLAGS) -o $@ $^ -lraylib $(LDFLAGS)

realloc_danger: %: src/examples/realloc_danger.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

relative_demo: %: src/examples/relative_demo.c obj/arena.o obj/map_wrap.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c dep/%.d | obj dep
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.opp: src/%.cpp dep/%.dpp | obj dep
	$(CXX) $(CXXFLAGS) -c $< -o $@

# used to automatically to detect what needs to be recompiled if one compilation unit depends on another
dependencies: $(DEP)

dep/%.d: src/%.c | dep
	$(CC) -MM -MP -MT obj/$*.o -MF $@ $< $(LIBS)

dep/%.dpp: src/%.cpp | dep
	$(CXX) -x c++ -MM -MP -MT obj/$*.opp -MF $@ $< $(LIBS)

obj:
	@mkdir -p obj
	@mkdir -p obj/examples

dep:
	@mkdir -p dep
	@mkdir -p dep/examples

$(CLEAN_EX): clean/%:
	rm -f $*

clean: | $(CLEAN_EX)
	rm -f list.bin
	rm -rf obj/*
	rm -rf dep/*

compile_commands.json: $(ALL_SRC)
	make $(ALL_OBJ) --always-make --dry-run \
	| $(RE) -w -E '$(CC|CXX)' \
	| $(RE) -w '\-[co]' \
	| jq -nR '[inputs|{command:., directory:".", file: match("\\-c [\\w\\-/]+\\.cp*").string[3:], output: match("\\-o [\\w\\-/]+\\.op*").string[3:]}]' > compile_commands.json
