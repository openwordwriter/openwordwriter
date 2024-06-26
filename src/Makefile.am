#AUTOMAKE_OPTIONS = subdir-objects

SUBDIRS = af plugins text wp

libabiword_cpp = 
opt_libs = 
platform_ldflags = 
resources = 

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = \
	-v --warn-all --accept-unprefixed \
	--symbol-prefix=abi --symbol-prefix=libabiword
INTROSPECTION_COMPILER_ARGS = \
	--includedir=$(srcdir) \
    --includedir=.

if HAVE_INTROSPECTION
SUBDIRS += gi-overrides
endif

if TOOLKIT_COCOA
main_cpp = wp/main/cocoa/CocoaMain.cpp
include $(top_srcdir)/src/wp/main/cocoa/bundle.mk

install-data-hook:
	install_name_tool \
			-id @executable_path/../Frameworks/libabiword-@ABIWORD_SERIES@.dylib \
			$(DESTDIR)/$(frameworksdir)/libabiword-@ABIWORD_SERIES@.dylib
	install_name_tool -change $(frameworksdir)/libabiword-@ABIWORD_SERIES@.dylib \
		@executable_path/../Frameworks/libabiword-@ABIWORD_SERIES@.dylib \
		$(DESTDIR)/$(macosdir)/AbiWord
endif

if TOOLKIT_GTK_ALL
libabiword_cpp += wp/main/gtk/libabiword.cpp
main_cpp = wp/main/gtk/UnixMain.cpp
endif

if TOOLKIT_QT
libabiword_cpp += wp/main/qt/libabiwordQt.cpp
main_cpp = wp/main/qt/QtMain.cpp
endif

if TOOLKIT_WIN
main_cpp = wp/main/win/Win32Main.cpp
opt_libs += Win32Resources.lo
platform_ldflags += -mwindows -municode
resources += Win32Resources.lo
endif

if TOOLKIT_COCOA
frameworks_LTLIBRARIES = libabiword-@ABIWORD_SERIES@.la
else
lib_LTLIBRARIES = libabiword-@ABIWORD_SERIES@.la
endif

libabiword_@ABIWORD_SERIES@_la_CPPFLAGS = \
	$(WP_CPPFLAGS) \
	-DABIWORD_DATADIR="\"$(ABIWORD_DATADIR)\""

libabiword_@ABIWORD_SERIES@_la_LDFLAGS = \
	$(DEPS_LIBS) \
	$(CHAMPLAIN_LIBS) \
	-no-undefined \
	-avoid-version \
	-export-dynamic


if TOOLKIT_COCOA
libabiword_@ABIWORD_SERIES@_la_LDFLAGS += -headerpad_max_install_names
endif

libabiword_@ABIWORD_SERIES@_la_LIBADD = \
	plugins/libplugins.la \
	text/libtext.la \
	wp/ap/libap.la \
	wp/impexp/libimpexp.la \
	af/xap/libxap.la \
	af/gr/libgr.la \
	af/util/libutil.la \
	af/ev/libev.la \
	$(opt_libs)

if TOOLKIT_GTK_ALL
## Resources compilation
abi-resources.c: wp/main/gtk/resources.xml
	$(AM_V_GEN) glib-compile-resources \
                --sourcedir=$(srcdir) --target=$@ \
                --generate-source --c-name abi $(srcdir)/wp/main/gtk/resources.xml

abi-resources.h: wp/main/gtk/resources.xml
	$(AM_V_GEN) glib-compile-resources                                  \
                --sourcedir=$(srcdir) --target=$@ \
		--generate-header --c-name abi $(srcdir)/wp/main/gtk/resources.xml

BUILT_SOURCES = \
        abi-resources.c \
        abi-resources.h

libabiword_cpp += abi-resources.c abi-resources.h
## End of resources

# headers are GTK only.
libabiword_@ABIWORD_SERIES@_includedir = @ABIWORD_HEADERSDIR@
libabiword_@ABIWORD_SERIES@_include_HEADERS = \
	wp/ap/gtk/abiwidget.h \
	wp/main/gtk/libabiword.h \
	af/xap/gtk/xap_UnixTableWidget.h
endif

libabiword_@ABIWORD_SERIES@_la_DEPENDENCIES = \
	$(libabiword_@ABIWORD_SERIES@_la_LIBADD)

libabiword_@ABIWORD_SERIES@_la_SOURCES = \
	wp/main/xp/abi_ver.cpp \
	$(libabiword_cpp)


# TODO is it possible to track dependencies for that?
WINDRES_CPPFLAGS = -I$(top_srcdir)/src/af/xap/win -I$(top_srcdir)/src/af/util/win -I$(top_srcdir)/src/wp/ap/win
Win32Resources.lo: wp/main/win/Win32Main.rc $(libabiword_@ABIWORD_SERIES@_la_OBJECTS)
	CPPFLAGS="$(WINDRES_CPPFLAGS)" $(top_srcdir)/lt-compile-resource $< $@

bin_PROGRAMS =

if TOOLKIT_COCOA
# MacOS
macos_PROGRAMS = AbiWord

AbiWord_CPPFLAGS = \
	-x objective-c++ \
	$(libabiword_@ABIWORD_SERIES@_la_CPPFLAGS)

AbiWord_LDFLAGS = \
	-no-undefined \
	-avoid-version \
	-export-dynamic \
	-headerpad_max_install_names 

AbiWord_LDADD = \
	libabiword-@ABIWORD_SERIES@.la

AbiWord_SOURCES = \
	$(main_cpp)

nobase_resources_DATA = wp/main/cocoa/MainMenu.nib/*.nib
resources_DATA = wp/main/cocoa/bundle/*.icns \
	wp/main/cocoa/bundle/*.tif \
	wp/main/cocoa/bundle/*.png
contents_DATA = wp/main/cocoa/bundle/Info.plist \
	wp/main/cocoa/bundle/PkgInfo

else
# not MacOS

bin_PROGRAMS += abiword

abiword_CPPFLAGS = \
	$(libabiword_@ABIWORD_SERIES@_la_CPPFLAGS)

abiword_LDFLAGS = \
	$(platform_ldflags)

abiword_LDADD = \
	libabiword-@ABIWORD_SERIES@.la \
	$(resources)

abiword_DEPENDENCIES = \
	libabiword-@ABIWORD_SERIES@.la \
	$(resources)

abiword_SOURCES = \
	$(main_cpp)

endif # TOOLKIT_COCOA

CLEANFILES = \
	abiword.dll.a


if HAVE_INTROSPECTION

INTROSPECTION_GIRS += Abi-@ABIWORD_SERIES@.gir

Abi-@ABIWORD_SERIES@.gir: $(INTROSPECTION_SCANNER) libabiword-@ABIWORD_SERIES@.la Makefile
Abi_@ABI_SUB_SERIES@_gir_INCLUDES = GObject-2.0 Gtk-3.0 Gsf-1
Abi_@ABI_SUB_SERIES@_gir_CFLAGS = $(libabiword_@ABIWORD_SERIES@_la_CPPFLAGS)
Abi_@ABI_SUB_SERIES@_gir_LIBS = libabiword-@ABIWORD_SERIES@.la
Abi_@ABI_SUB_SERIES@_gir_FILES = \
	af/xap/gtk/xap_UnixTableWidget.h \
	af/xap/gtk/xap_UnixTableWidget.cpp \
	wp/ap/gtk/abiwidget.h \
	wp/ap/gtk/abiwidget.cpp \
	wp/main/gtk/libabiword.h \
	wp/main/gtk/libabiword.cpp

girdir = $(GIRDIR)
gir_DATA = $(INTROSPECTION_GIRS)

typelibsdir = $(TYPELIBDIR)
typelibs_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelibs_DATA)

endif # HAVE_INTROSPECTION

EXTRA_DIST = \
	wp/main/win/AbiWord.exe.x86.MANIFEST \
	wp/main/win/AbiWord.exe.amd64.MANIFEST \
	wp/main/win/Win32Main.rc \
	wp/main/cocoa/MainMenu.nib \
	wp/main/cocoa/bundle
