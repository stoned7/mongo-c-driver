EXTRA_DIST =
CLEANFILES =
DISTCLEANFILES =
MAINTAINERCLEANFILES =

bin_PROGRAMS =
lib_LTLIBRARIES =
noinst_LTLIBRARIES =
noinst_PROGRAMS =

if WITH_LIBBSON
SUBDIRS = src/libbson
endif

AM_CTAGSFLAGS = --fields=+l --languages=c

include doc/Makefile.am
include src/Makefile.am

if ENABLE_TESTS
include tests/Makefile.am
endif

if ENABLE_EXAMPLES
include examples/Makefile.am
endif

# for EXTRA_DIST
include build/cmake/Makefile.am

ACLOCAL_AMFLAGS = -I build/autotools/m4 ${ACLOCAL_FLAGS}
DISTCHECK_CONFIGURE_FLAGS = --enable-silent-rules --enable-man-pages --enable-html-doc --enable-sasl --enable-ssl --enable-maintainer-flags --enable-debug --with-libbson=bundled

mongocdocdir = ${docdir}
mongocdoc_DATA = \
	COPYING \
	NEWS \
	README.rst
EXTRA_DIST += $(mongocdoc_DATA)

EXTRA_DIST += \
	CMakeLists.txt \
	CONTRIBUTING.md

dist-hook:
	@if test -d "$(srcdir)/.git"; then                              \
          (cd "$(srcdir)" &&                                            \
           $(top_srcdir)/missing --run git log --stat ) > ChangeLog.tmp \
           && mv -f ChangeLog.tmp $(top_distdir)/ChangeLog              \
           || (rm -f ChangeLog.tmp;                                     \
               echo Failed to generate ChangeLog >&2);                  \
	else                                                            \
	  echo A git checkout is required to generate a ChangeLog >&2;  \
	fi

uninstall-local:
	-rm -r $(mongocdocdir)

dist-rpm: dist-gzip
	cp mongo-c-driver-$(MONGOC_VERSION).tar.gz ~/rpmbuild/SOURCES/
	rpmbuild -bb build/rpm/mongo-c-driver.spec
