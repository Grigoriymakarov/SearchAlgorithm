SRCDIR   := code
INCDIR   := include
BUILDDIR := build

CXX      := g++
CPPFLAGS := -I$(INCDIR) -MMD -MP

CXXFLAGS_COMMON := -std=c++17 -Wall -Wextra -Wpedantic -g -O3

CXXFLAGS_OMP := -fopenmp
LDFLAGS_OMP  := -fopenmp


SRC_PROJET := \
	$(SRCDIR)/main.cpp \
	$(SRCDIR)/SeqfileHandler.cpp \
	$(SRCDIR)/PhrfileHandler.cpp \
	$(SRCDIR)/PinfileHandler.cpp \
	$(SRCDIR)/dict.cpp \
	$(SRCDIR)/AlgoSW.cpp

SRC_PROJETOPT := \
	$(SRCDIR)/mainopt.cpp \
	$(SRCDIR)/SeqfileHandleropt.cpp \
	$(SRCDIR)/PhrfileHandler.cpp \
	$(SRCDIR)/PinfileHandler.cpp \
	$(SRCDIR)/dict.cpp \
	$(SRCDIR)/AlgoSWopt.cpp

SRC_PROJETPRELIM := \
	$(SRCDIR)/mainprelim.cpp \
	$(SRCDIR)/SeqfileHandler.cpp \
	$(SRCDIR)/PhrfileHandler.cpp \
	$(SRCDIR)/PinfileHandler.cpp \
	$(SRCDIR)/dict.cpp \
	$(SRCDIR)/AlgoSW.cpp


OBJ_PROJET       := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRC_PROJET))
OBJ_PROJETOPT    := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRC_PROJETOPT))
OBJ_PROJETPRELIM := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRC_PROJETPRELIM))

DEP_PROJET       := $(OBJ_PROJET:.o=.d)
DEP_PROJETOPT    := $(OBJ_PROJETOPT:.o=.d)
DEP_PROJETPRELIM := $(OBJ_PROJETPRELIM:.o=.d)


.PHONY: all
all: projet

.PHONY: projet projetopt projetprelim
projet: $(OBJ_PROJET)
	$(CXX) $(OBJ_PROJET) -o $@

projetprelim: $(OBJ_PROJETPRELIM)
	$(CXX) $(OBJ_PROJETPRELIM) -o $@

projetopt: CXXFLAGS_EXTRA := $(CXXFLAGS_OMP)
projetopt: LDFLAGS_EXTRA  := $(LDFLAGS_OMP)
projetopt: $(OBJ_PROJETOPT)
	$(CXX) $(OBJ_PROJETOPT) $(LDFLAGS_EXTRA) -o $@


$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS_COMMON) $(CXXFLAGS_EXTRA) -c $< -o $@


.PHONY: clean
clean:
	rm -rf $(BUILDDIR) projet projetopt projetprelim

-include $(DEP_PROJET) $(DEP_PROJETOPT) $(DEP_PROJETPRELIM)
