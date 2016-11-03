#=================================== config ==================================#
CXX      := g++
CXXFLAGS := -std=c++11
AR       := ar
ARFLAGS  := crs
MAKE     := make

SUBS :=
LIBS :=
MODS :=

#================================== prepare ==================================#

MAIN := $(patsubst %.cpp, %, $(wildcard main.cpp))
TEST := $(patsubst _test/%.cpp, %, $(wildcard _test/*.cpp))

LIBS := $(LIBS:%=%/_bin/lib.a)
MODS := $(MODS:%=%/_bin/mod.a)

ALL  := $(wildcard *.cpp) $(wildcard $(SUBS:%=%/**.cpp))
SRCS := $(filter-out main.cpp, $(ALL)) 
OBJS := $(patsubst %, _bin/%.o, $(basename $(SRCS)))
DEPS := $(patsubst %, _bin/%.d, $(basename $(ALL) $(MAIN) $(TEST)))

#==================================== exe ====================================#

all: $(MAIN) $(TEST)

main: _bin/main.o $(OBJS) $(MODS) $(LIBS)
	@-mkdir -p _bin
	$(CXX) $(CXXFLAGS) -o _bin/main.exe $^

$(TEST): %: _bin/_test/%.o $(OBJS) $(MODS) $(LIBS)
	@-mkdir -p _bin
	$(CXX) $(CXXFLAGS) -o _bin/$@.exe $^

#================================= lib & mod =================================#

.PHONY: $(LIBS) $(MODS)

lib: _bin/lib.a ;

_bin/lib.a: $(OBJS)
	@-mkdir -p _bin
	$(AR) $(ARFLAGS) $@ $^

mod: _bin/mod.a ;

_bin/mod.a: $(OBJS) $(MODS)
	@-mkdir -p _bin
	$(AR) $(ARFLAGS) $@ $^

$(LIBS):
	$(MAKE) lib -C $(patsubst %/_bin/lib.a, %, $@)

$(MODS):
	$(MAKE) mod -C $(patsubst %/_bin/mod.a, %, $@)
	
#================================ objs & deps ================================#

DEPFLAGS     = -MT $@ -MMD -MP -MF _bin/$*.td
POSTCOMPLETE = @mv -f _bin/$*.td _bin/$*.d

_bin/%.o: %.cpp
_bin/%.o: %.cpp _bin/%.d
	@-mkdir -p $(@D)
	$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
	$(POSTCOMPLETE)

_bin/%.d: ;

.PRECIOUS: _bin/%.d

-include $(DEPS)

#=================================== clean ===================================#

.PHONY: clean
clean:
	rm -rf _bin

