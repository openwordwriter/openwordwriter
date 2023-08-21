if TOOLKIT_GTK

if WITH_BUILTIN_AIKSAURUS_GTK
SUBDIRS = aiksaurusgtk3
platform_lib = aiksaurusgtk3/libAiksaurusGtk3.la
endif

endif

AM_CPPFLAGS = \
	$(AIKSAURUS_CFLAGS) \
	-I$(srcdir) \
	-DABI_BUILD_VERSION=\"$(VERSION)\"

aiksaurus_la_SOURCES = \
	xp/AiksaurusABI.cpp

if TOOLKIT_WIN
aiksaurus_la_SOURCES += \
	win/AiksaurusApp.cpp \
	win/AiksaurusApp.h \
	win/AiksaurusDlg.cpp \
	win/AiksaurusDlg.h \
	win/resource.h

Aiksaurus.lo: $(srcdir)/win/Aiksaurus.rc
	$(top_srcdir)/lt-compile-resource $< $@
platform_lib = Aiksaurus.lo
endif

plugindir = $(ABIWORD_PLUGINSDIR)
plugin_LTLIBRARIES = aiksaurus.la

aiksaurus_la_LIBADD = \
	$(platform_lib) \
	$(AIKSAURUS_LIBS)

aiksaurus_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined

EXTRA_DIST = \
	win/Aiksaurus.ico \
	win/Aiksaurus.rc \
	win/back_hover.ico \
	win/back.ico \
	win/forward_hover.ico \
	win/forward.ico \
	win/readme.txt \
	win/search_hover.ico \
	win/search.ico \
	plugin.m4
