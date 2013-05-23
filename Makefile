CFLAGS ?= -O2 -Wall

DEFS = -fPIC -Iinclude

pam_vz_OBJECTS=pam_vz.o ve_enter.o

OUT=pam_vz.so

all: $(OUT)

%.o: %.c
	$(CC) $(DEFS) $(CPPFLAGS) $(CFLAGS) -c $<

pam_vz.so: $(pam_vz_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lpam \
		-shared -Bsymbolic -Wl,-z,defs -Wl,-soname,$@ \
		-Wl,--version-script=pam_vz.lds

ve_enter: ve_enter.c
	$(CC) $(DEFS) $(CPPFLAGS) $(CFLAGS) -DTEST $< -o $@

distclean: clean
clean:
	rm -f Makefile.depend ve_enter $(pam_vz_OBJECTS) $(OUT)

install:
	install -s -o0 -g0 -m755 $(OUT) $(BASEDIR)/lib/security

depend: Makefile.depend
Makefile.depend:
	$(CC) $(DEFS) $(CPPFLAGS) $(CFLAGS) -MM -MG *.c > $@

-include Makefile.depend
