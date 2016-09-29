SUBDIRS = po

bin_PROGRAMS = simple-sample advanced-sample i18n-sample
simple_sample_SOURCES = simple-sample.cc cmdlineparse.hh
advanced_sample_SOURCES = advanced-sample.cc cmdlineparse.hh
i18n_sample_SOURCES = i18n-sample.cc cmdlineparse.hh gettext.h

i18n_sample_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"
i18n_sample_LDADD = $(LIBINTL)

dist_doc_DATA = README.md README.ja.md LICENSE
EXTRA_DIST = config.rpath  autogen.sh .dir-locals.el .gitignore

ACLOCAL_AMFLAGS = -I m4

install:
	echo Files can not be installed.
	echo Please copy cmdlineparse.hh to your project directory.
	exit 1
