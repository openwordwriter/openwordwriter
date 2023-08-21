
AM_CPPFLAGS = \
	$(GOFFICE_CFLAGS) \
	-DABI_BUILD_VERSION=\"$(VERSION)\"

plugindir = $(ABIWORD_PLUGINSDIR)
plugin_LTLIBRARIES = goffice.la

goffice_la_LIBADD = \
	$(GOFFICE_LIBS)

goffice_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined

goffice_la_SOURCES = \
	gtk/AbiGOChart.cpp \
	gtk/AbiGOChart.h \
	gtk/AbiGOComponent.cpp \
	gtk/AbiGOComponent.h \
	gtk/AbiGOffice.cpp \
	gtk/AbiGOffice.h \
	gtk/ie_imp_GOChart.cpp \
	gtk/ie_imp_GOChart.h \
	gtk/ie_imp_GOComponent.cpp \
	gtk/ie_imp_GOComponent.h \
	$(NULL)


EXTRA_DIST = \
	plugin.m4

