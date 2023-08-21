
AM_CPPFLAGS = \
	$(PAINT_CFLAGS) \
	-I$(srcdir)/$(PLATFORM) \
	-DABI_BUILD_VERSION=\"$(VERSION)\"

if PAINT_BUILTIN
noinst_LTLIBRARIES = paint.la

else

plugindir = $(ABIWORD_PLUGINSDIR)
plugin_LTLIBRARIES = paint.la

endif

paint_la_LIBADD = \
	$(PAINT_LIBS)

paint_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined

paint_la_SOURCES = \
	xp/AbiGeneric.cpp \
	xp/AbiGeneric.h \
	xp/AbiPaint.cpp \
	xp/AbiPaint.h

if TOOLKIT_GTK_ALL
paint_la_SOURCES += \
	unix/ut_process.h \
	unix/ut_UnixProcess.cpp
endif

if TOOLKIT_WIN
paint_la_SOURCES += \
	win/bmp2png.cpp \
	win/png2bmp.cpp \
	win/pngdib.c \
	win/pngdib.h \
	win/ut_process.h \
	win/ut_Win32Process.cpp
endif

EXTRA_DIST = \
	win/pngdib.readme
	plugin.m4
