
noinst_LTLIBRARIES = lib@TOOLKIT@.la

# The pdf exporter wants to call EditMethods, so we need WP_CPPFLAGS
AM_CPPFLAGS = \
	$(WP_CPPFLAGS)

lib@TOOLKIT@_la_SOURCES = \
	ie_exp_PDF.cpp \
	ie_exp_PDF.h \
	ie_impexp_RegisterUnix.cpp \
	ie_impGraphic_GdkPixbuf.cpp \
	ie_impGraphic_GdkPixbuf.h

EXTRA_DIST = \
	$(conditional_src)

