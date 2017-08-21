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
SRCS := techos.c

CFLAGS := -std=c11 -O0 -g -Wall -Wextra -Wpedantic

all: techos.o
	gcc -o techos techos.o
	
clean: 
	rm $(wildcard *.o)
	rm techos

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))
