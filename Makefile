ifeq ($(APXS_PATH),)
APXS_PATH=/usr/sbin/apxs
endif

APREQ_CONFIG := apreq2-config

MY_LDFLAGS=-lcurl -lyajl `$(APREQ_CONFIG) --link-libtool`
# Note that gcc flags are passed through apxs, so preface with -Wc
MY_CFLAGS=-Wc,-I. -Wc,-Wall -Wc,-g -Wc,-Wno-unused-function `$(APREQ_CONFIG) --includes`
SRCS=src/mod_authn_persona.c src/cookie.c src/verify.c
HDRS=src/cookie.h src/defines.h src/verify.h
BUILDDIR := build

.SUFFIXES: .c .o .la

all:  build/.libs/mod_authn_persona.so

.PHONY: builddir
builddir: build

$(BUILDDIR):
	@mkdir -p $@

$(BUILDDIR)/bin2c: tools/bin2c.c | $(BUILDDIR)
	@gcc -Wall -g -o $(BUILDDIR)/bin2c $<

$(BUILDDIR)/signin_page.h: src/signin.html $(BUILDDIR)/bin2c | $(BUILDDIR) 
	@$(BUILDDIR)/bin2c $^ > $@

$(BUILDDIR)/.libs/mod_authn_persona.so: $(SRCS) $(HDRS) $(BUILDDIR)/signin_page.h
	@cd $(BUILDDIR) && for file in $(SRCS) $(HDRS) ; do ln -sf ../$$file . ; done
	@cd $(BUILDDIR) && $(APXS_PATH) $(MY_LDFLAGS) $(MY_CFLAGS) -c $(subst src/,,$(SRCS))

install: all
	$(APXS_PATH) -i $(BUILDDIR)/mod_authn_persona.la

clean:
	-rm -rf $(BUILDDIR)
