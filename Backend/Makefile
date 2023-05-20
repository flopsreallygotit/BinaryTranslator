# Configure compiler

CXX ?= g++

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Configure compile flags

CXXFLAGS ?= -ggdb3 -std=c++17 -O0 -Wall -Wextra 							\
			-Wmissing-declarations -Wcast-align -Wcast-qual 			   	\
			-Wchar-subscripts -Wconditionally-supported 				   	\
			-Wconversion -Wempty-body 				   						\
			-Wfloat-equal -Wformat-nonliteral -Wformat-security 		   	\
			-Wformat-signedness -Wformat=2 -Winline -Wlogical-op 		   	\
			-Wopenmp-simd -Woverloaded-virtual 		   						\
			-Wpacked -Wpointer-arith -Winit-self -Wredundant-decls		   	\
			-Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel	\
			-Wstrict-overflow=2 -Wsuggest-attribute=noreturn			   	\
			-Wsuggest-final-methods -Wsuggest-final-types				   	\
			-Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand  	\
			-Wundef -Wunreachable-code -Wunused -Wuseless-cast 			   	\
			-Wvariadic-macros -Wno-literal-suffix 						   	\
			-Wno-missing-field-initializers -Wno-narrowing 				   	\
			-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new 	\
			-fsized-deallocation -fstack-protector -fstrict-overflow 	   	\
			-flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE 	   	\
			-DDEBUG															\

# Configure linker flags

LINKFLAGS ?= 

# Configure program perfomance boost flags

FASTFLAGS ?= # -Ofast -mavx -mavx2 -mavx512dq

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Configure directory flags

DIRFLAGS ?= -I../Universal -I../Config -I../DSL -I../Backend -I../Tree

# Configure sources you need to compile

SOURCES  ?= ../Backend/main.cpp ../Backend/translation.cpp ../Backend/tokenizer.cpp	\
			../Tree/tree.cpp														\
			../Universal/universalUtils.cpp ../Universal/textUtils.cpp				\

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Configure object directory name

OBJDIR ?= ./Objects/Objects/

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Pick executable name

EXECUTABLE ?= backend

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# WARNING: Don't change values and commands below

# Making objects names from sources names

OBJECTS = $(patsubst %.cpp, $(OBJDIR)%.o, $(SOURCES)) 

# Adding directory flags to compiler flags

CXXFLAGS += $(DIRFLAGS)
CXXFLAGS += $(FASTFLAGS)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "Making executable from objects;"
	@$(CXX) $(OBJECTS) -o $@ $(LINKFLAGS)
	@echo "Done;"

$(OBJDIR)%.o: %.cpp
	@mkdir -p $(@D)
	@echo "Making object file from $<;"
	@$(CXX) -c $(CXXFLAGS) $^ -o $@

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.PHONY: clear
clear:
	@echo "Clearing object files and executables;";
	@rm -rf $(OBJDIR) $(EXECUTABLE) Objects

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
