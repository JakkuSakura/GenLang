SHELL = /bin/bash

AllDirs := src includes
Sources := $(foreach n,$(AllDirs), $(wildcard $(n)/*.cpp))
Objs := $(patsubst %.cpp,%.o, $(Sources))
Deps := $(patsubst %.cpp,%.d, $(Sources))
StaticLib := #bin/genlang.a
DynamicLib := #bin/ganlang.so
Bin := bin/genlang

#AllLibs : $(DynamicLib)
#AllLibs : $(StaticLib)
AllLibs : $(Bin)

CC = g++
CXXFLAGS = -O2 -Wall -Iincludes -std=c++11
#$(foreach n,$(AllDirs), -I$(n))

LDFLAGS = -lstdc++

$(StaticLib) : $(Objs)
	ar rcs $@ $^

$(DynamicLib) : $(Objs)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

$(Bin) : $(Objs)
	$(CC) $(Objs) -o $@

%.d : %.cpp
	$(CC) -MT"$(<:.cpp=.o) $@" -MM $(CXXFLAGS) $< >$@
run: $(Bin)
	$(Bin)

sinclude $(Deps)

.PHONY : clean
clean:
	rm -f $(Objs) $(Deps) $(StaticLib) $(DynamicLib) $(Bin)
