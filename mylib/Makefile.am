include $(top_srcdir)/include/Makefile.def

lib_LTLIBRARIES = libmylib.la
libmylib_la_SOURCES = $(MYLIBSOURCES)
libmylib_la_LIBADD = @LIBM@
libmylib_la_LDFLAGS = -no-undefined -version-info 0:1:0

EXTRA_DIST = guidemylibC.tex tcode.tex titre.tex

doc_DATA = guidemylib-old.pdf
