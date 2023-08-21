
AM_CPPFLAGS = \
	$(EPUB_CFLAGS) \
	-I$(srcdir)/imp/xp \
	-I$(srcdir)/exp/xp \
	-I$(srcdir)/dialogs/xp \
	-I$(srcdir)/dialogs/win \
	-DABI_BUILD_VERSION=\"$(VERSION)\"

if EPUB_BUILTIN
noinst_LTLIBRARIES = epub.la

else

plugindir = $(ABIWORD_PLUGINSDIR)
plugin_LTLIBRARIES = epub.la

endif

epub_la_LIBADD = $(EPUB_LIBS)

epub_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined
if TOOLKIT_COCOA
epub_la_LDFLAGS += -headerpad_max_install_names
endif # TOOLKIT_COCOA

epub_la_SOURCES = \
	common/xp/ie_impexp_EPUB.cpp \
	common/xp/ie_impexp_EPUB.h \
	imp/xp/ie_imp_EPUB.cpp \
	imp/xp/ie_imp_EPUB.h \
	imp/xp/ie_imp_EPUB_Sniffer.cpp \
	imp/xp/ie_imp_EPUB_Sniffer.h \
	exp/xp/ie_exp_EPUB.cpp \
	exp/xp/ie_exp_EPUB.h \
	exp/xp/ie_exp_EPUB_Sniffer.cpp \
	exp/xp/ie_exp_EPUB_Sniffer.h \
	exp/xp/ie_exp_EPUB_EPUB3Writer.cpp \
	exp/xp/ie_exp_EPUB_EPUB3Writer.h

if !TOOLKIT_COCOA
epub_la_SOURCES += \
	dialogs/xp/ap_Dialog_EpubExportOptions.cpp \
	dialogs/xp/ap_Dialog_EpubExportOptions.h
endif

if TOOLKIT_GTK_ALL
epub_la_SOURCES += \
	dialogs/gtk/ap_UnixDialog_EpubExportOptions.cpp \
	dialogs/gtk/ap_UnixDialog_EpubExportOptions.h
endif

if TOOLKIT_WIN
epub_la_SOURCES += \
	dialogs/win/ap_Win32Dialog_EpubExportOptions.cpp \
	dialogs/win/ap_Win32Dialog_EpubExportOptions.h

ap_Win32Res_EpubExportOptions.lo: ap_Win32Res_EpubExportOptions.rc $(libwin_la_OBJECTS)
	CPPFLAGS="$(COLLAB_RCFLAGS)" $(top_srcdir)/lt-compile-resource $< $@
endif

if TOOLKIT_COCOA
install-data-hook:
	install_name_tool -change $(ABIWORD_LIBDIR)/libabiword-@ABIWORD_SERIES@.dylib \
		@executable_path/../Frameworks/libabiword-@ABIWORD_SERIES@.dylib \
		$(DESTDIR)/$(plugindir)/epub.so
endif # TOOLKIT_COCOA

EXTRA_DIST = \
	dialogs/win/ap_Win32Res_EpubExportOptions.rc \
	dialogs/win/ap_Win32Res_EpubExportOptions.rc2
	plugin.m4
