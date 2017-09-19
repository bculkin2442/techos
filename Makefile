# Dependancy boilerplate
#
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c   = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS)
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

OBJDIR := bin/
$(shell mkdir -p $(OBJDIR) >/dev/null)

%.o : %.c
$(OBJDIR)/%.o : %.c $(DEPDIR)/%.d
	$(COMPILE.c) -c $< -o $@
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

# Customize here
# --------------

# Rules for typesetting manpages
%.ps: %.1
	man -T $< > $@

%.pdf: %.ps
	ps2pdf $< $@

#  All our main source files
SRCS := $(wildcard *.c)

# All of our documentation
DOCS := $(wildcard help/*.1)

# The flags we want to pass to the C compiler
CFLAGS := -std=c11 -O0 -g -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter -Wno-implicit-function-declaration -Wno-unused-but-set-variable

# The flags we want to pass to the linker
LDFLAGS := -Llibs

# The libraries we want to link to
LINKLIBS := -largparser -lintern

# None of these targets correspond to actual files
.PHONY: all clean run libs

all: bin/techos libs


# TechOS depends on an .o file for each source file
bin/techos: $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS)) libs
	gcc $(CFLAGS) $(LDFLAGS) -o bin/techos $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS)) $(LINKLIBS)
	
run: bin/techos
	./bin/techos

# Make libraries
libs: libs/libintern.a libs/libargparser.a

# Make libintern
libs/libintern.a: libs/intern.o
	ar rcs libs/libintern.a libs/intern.o

libs/intern.o: libs/intern.c libs/intern.h
	$(CC) $(CFLAGS) -o libs/intern.o -c libs/intern.c

# Make libargparser
libs/libargparser.a: libs/argparser.o
	ar rcs libs/libargparser.a libs/argparser.o

libs/argparser.o: libs/argparser.c libs/argparser.h
	$(CC) $(CFLAGS) -o libs/argparser.o -c libs/argparser.c 

# Delete the binary and any object/library files, as well as the printed documentation
clean: 
	rm -rf bin/
	rm $(wildcard libs/*.o)
	rm $(wildcard libs/*.a)
	rm techos

docs: $(patsubst %.1,%.pdf,$(DOCS))

# Include all of our dependancy info
include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))
