
noinst_LTLIBRARIES = libev.la

AM_CPPFLAGS = \
	$(WP_CPPFLAGS) \
	$(AF_CPPFLAGS)

libev_la_SOURCES = \
	xp/ev_EditBinding.cpp \
	xp/ev_EditBinding.h \
	xp/ev_EditBits.h \
	xp/ev_EditEventMapper.cpp \
	xp/ev_EditEventMapper.h \
	xp/ev_EditMethod.cpp \
	xp/ev_EditMethod.h \
	xp/ev_Keyboard.cpp \
	xp/ev_Keyboard.h \
	xp/ev_Menu_Actions.cpp \
	xp/ev_Menu_Actions.h \
	xp/ev_Menu.cpp \
	xp/ev_Menu.h \
	xp/ev_Menu_Labels.cpp \
	xp/ev_Menu_Labels.h \
	xp/ev_Menu_Layouts.cpp \
	xp/ev_Menu_Layouts.h \
	xp/ev_Mouse.cpp \
	xp/ev_Mouse.h \
	xp/ev_MouseListener.h \
	xp/ev_NamedVirtualKey.cpp \
	xp/ev_NamedVirtualKey.h \
	xp/ev_Toolbar_Actions.cpp \
	xp/ev_Toolbar_Actions.h \
	xp/ev_Toolbar_Control.cpp \
	xp/ev_Toolbar_Control.h \
	xp/ev_Toolbar.cpp \
	xp/ev_Toolbar.h \
	xp/ev_Toolbar_Labels.cpp \
	xp/ev_Toolbar_Labels.h \
	xp/ev_Toolbar_Layouts.cpp \
	xp/ev_Toolbar_Layouts.h

if TOOLKIT_GTK_ALL
libev_la_SOURCES += \
	gtk/ev_UnixFontCombo.cpp \
	gtk/ev_UnixFontCombo.h \
	gtk/ev_UnixKeyboard.cpp \
	gtk/ev_UnixKeyboard.h \
	gtk/ev_UnixKeysym2ucs.cpp \
	gtk/ev_UnixKeysym2ucs.h \
	gtk/ev_UnixMenuBar.h \
	gtk/ev_UnixMenu.cpp \
	gtk/ev_UnixMenu.h \
	gtk/ev_UnixMenuPopup.h \
	gtk/ev_UnixMouse.cpp \
	gtk/ev_UnixMouse.h \
	gtk/ev_UnixToolbar.cpp \
	gtk/ev_UnixToolbar.h \
	gtk/ev_UnixToolbar_ViewListener.cpp \
	gtk/ev_UnixToolbar_ViewListener.h
endif

if TOOLKIT_COCOA
AM_CPPFLAGS += \
	-x objective-c++

libev_la_SOURCES += \
	cocoa/ev_CocoaKeyboard.h \
	cocoa/ev_CocoaKeyboard.cpp \
	cocoa/ev_CocoaMenu.h \
	cocoa/ev_CocoaMenu.cpp \
	cocoa/ev_CocoaMenuBar.h \
	cocoa/ev_CocoaMenuPopup.h \
	cocoa/ev_CocoaMouse.h \
	cocoa/ev_CocoaMouse.cpp \
	cocoa/ev_CocoaToolbar.h \
	cocoa/ev_CocoaToolbar.cpp \
	cocoa/ev_CocoaToolbar_ViewListener.h \
	cocoa/ev_CocoaToolbar_ViewListener.cpp
endif

if TOOLKIT_WIN
libev_la_SOURCES += \
	win/ev_Win32Keyboard.cpp \
	win/ev_Win32Keyboard.h \
	win/ev_Win32Menu.cpp \
	win/ev_Win32Menu.h \
	win/ev_Win32Mouse.cpp \
	win/ev_Win32Mouse.h \
	win/ev_Win32Toolbar.cpp \
	win/ev_Win32Toolbar.h \
	win/ev_Win32Toolbar_ViewListener.cpp \
	win/ev_Win32Toolbar_ViewListener.h
endif

if TOOLKIT_QT
include ${top_srcdir}/moc.mk

$(libqt_la_OBJECTS) : \
	moc_ev_QtMenuSlot.cpp \
	$(NULL)

libev_la_SOURCES += \
	qt/ev_QtKeyboard.cpp \
	qt/ev_QtKeyboard.h \
	qt/ev_QtMouse.cpp \
	qt/ev_QtMouse.h \
	qt/ev_QtMenu.cpp \
	qt/ev_QtMenu.h \
	qt/ev_QtMenuBar.cpp \
	qt/ev_QtMenuBar.h \
	qt/ev_QtToolbar.cpp \
	qt/ev_QtToolbar.h \
	qt/ev_QtMenuSlot.cpp \
	qt/ev_QtMenuSlot.h \
	$(NULL)

endif
