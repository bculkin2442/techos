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
SRCS := techos.c commands.c

CFLAGS := -std=c11 -O0 -g -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter -Wno-implicit-function-declaration -Wno-unused-but-set-variable

.PHONY: all clean run

all: techos


techos: $(patsubst %.c,%.o,$(SRCS))
	gcc -O0 -g -o techos $(patsubst %.c,%.o,$(SRCS))
	
run: techos
	./techos

clean: 
	rm $(wildcard *.o)
	rm techos

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))
