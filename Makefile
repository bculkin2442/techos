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

CFLAGS := -std=c11 -O0 -g -Wall -Wextra -Wpedantic

.PHONY: all clean run

all: techos.o
	# Replace with a patsub of srcs
	gcc -O0 -g -o techos techos.o commands.c
	
run: all
	./techos

clean: 
	rm $(wildcard *.o)
	rm techos

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))
