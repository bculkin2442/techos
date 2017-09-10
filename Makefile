# Dependancy boilerplate
#
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c   = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS)
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

%.o : %.c
%.o : %.c $(DEPDIR)/%.d
	$(COMPILE.c) -c $<
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

#  All our source files
SRCS := $(wildcard *.c)

# All of our documentation
DOCS := $(wildcard help/*.1)

# The flags we wan to pass to the C compiler
CFLAGS := -std=c11 -O0 -g -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter -Wno-implicit-function-declaration -Wno-unused-but-set-variable

# None of these targets correspond to actual files
.PHONY: all clean run

all: techos

docs: $(patsubst %.1,%.pdf,$(DOCS))

# TechOS depends on an .o file for each source file
techos: $(patsubst %.c,%.o,$(SRCS))
	gcc -O0 -g -o techos $(patsubst %.c,%.o,$(SRCS))
	
run: techos
	./techos

# Delete the binary and any object files, as well as the printed documentation
clean: 
	rm $(wildcard *.o)
	rm techos

# Include all of our dependancy info
include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))
