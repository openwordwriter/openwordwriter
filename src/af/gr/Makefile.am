AM_CPPFLAGS = \
	$(AF_CPPFLAGS)

noinst_LTLIBRARIES = libgr.la

libgr_la_SOURCES = \
	xp/gr_Caret.cpp \
	xp/gr_Caret.h \
	xp/gr_CharWidthsCache.cpp \
	xp/gr_CharWidthsCache.h \
	xp/gr_CharWidths.cpp \
	xp/gr_CharWidths.h \
	xp/gr_Control.h \
	xp/gr_DrawArgs.cpp \
	xp/gr_DrawArgs.h \
	xp/gr_EmbedManager.cpp \
	xp/gr_EmbedManager.h \
	xp/gr_Graphics.cpp \
	xp/gr_Graphics.h \
	xp/gr_Image.cpp \
	xp/gr_Image.h \
	xp/gr_Painter.cpp \
	xp/gr_Painter.h \
	xp/gr_RenderInfo.cpp \
	xp/gr_RenderInfo.h \
	xp/gr_Transform.cpp \
	xp/gr_Transform.h \
	xp/gr_VectorImage.cpp \
	xp/gr_VectorImage.h


if WITH_CAIRO
libgr_la_SOURCES += \
	xp/gr_CairoGraphics.h \
	xp/gr_CairoGraphics.cpp \
	xp/gr_CairoNullGraphics.h \
	xp/gr_CairoNullGraphics.cpp
endif

if TOOLKIT_GTK_ALL
libgr_la_SOURCES += \
	gtk/gr_CairoPrintGraphics.cpp \
	gtk/gr_CairoPrintGraphics.h \
	gtk/gr_UnixCairoGraphics.h \
	gtk/gr_UnixCairoGraphics.cpp \
	gtk/gr_UnixImage.cpp \
	gtk/gr_UnixImage.h \
	gtk/gr_CairoImage.h \
	gtk/gr_CairoImage.cpp
endif

if TOOLKIT_COCOA
AM_CPPFLAGS += \
	-x objective-c++
libgr_la_SOURCES += \
	cocoa/gr_CoreGraphicsUtils.h \
	cocoa/gr_CocoaGraphics.h \
	cocoa/gr_CocoaGraphics.cpp \
	cocoa/gr_CocoaFont.h \
	cocoa/gr_CocoaFont.cpp \
	cocoa/gr_CocoaImage.h \
	cocoa/gr_CocoaImage.cpp
endif

if TOOLKIT_WIN
libgr_la_SOURCES += \
	win/gr_Win32CharWidths.cpp \
	win/gr_Win32CharWidths.h \
	win/gr_Win32Graphics.cpp \
	win/gr_Win32Graphics.h \
	win/gr_Win32Image.cpp \
	win/gr_Win32Image.h \
	win/gr_Win32Uniscribe.h \
	win/gr_Win32USPGraphics.cpp \
	win/gr_Win32USPGraphics.h
endif

if TOOLKIT_QT
libgr_la_SOURCES += \
	qt/gr_QtGraphics.cpp \
	qt/gr_QtGraphics.h
endif
