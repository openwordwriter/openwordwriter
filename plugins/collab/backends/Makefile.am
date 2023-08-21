
DIST_SUBDIRS = telepathy sipsimple
SUBDIRS = 

AM_CPPFLAGS = \
	$(COLLAB_CFLAGS) \
	-I$(srcdir)/.. \
	-I$(srcdir)/../core \
	-I$(srcdir)/fake/xp \
	-I$(srcdir)/service

noinst_LTLIBRARIES = libbackends.la

uidir=$(ABIWORD_UIDIR)
ui_gtk3 =
win_rc =

libbackends_la_SOURCES =
libbackends_la_LIBADD =

if COLLAB_BACKEND_FAKE
libbackends_la_SOURCES += \
	fake/xp/FakeAccountHandler.cpp \
	fake/xp/FakeAccountHandler.h \
	fake/xp/FakeBuddy.h
endif

if COLLAB_BACKEND_SERVICE
certdir=$(ABIWORD_DATADIR)/certs
cert_DATA = \
        service/cacert.pem

if TOOLKIT_WIN
libbackends_la_SOURCES += \
	service/win/ap_Win32Dialog_GenericInput.cpp \
	service/win/ap_Win32Dialog_GenericInput.h \
	service/win/ap_Win32Dialog_GenericProgress.cpp \
	service/win/ap_Win32Dialog_GenericProgress.h \
	service/win/ServiceWin32AccountHandler.cpp \
	service/win/ServiceWin32AccountHandler.h

win_rc += \
	service/win/ap_Win32Res_ServiceDialogs.rc \
	service/win/ap_Win32Res_DlgGenericInput.rc2 \
	service/win/ap_Win32Res_DlgGenericProgress.rc2
endif

if TOOLKIT_GTK_ALL
libbackends_la_SOURCES += \
	service/unix/ap_UnixDialog_GenericInput.cpp \
	service/unix/ap_UnixDialog_GenericInput.h \
	service/unix/ap_UnixDialog_GenericProgress.cpp \
	service/unix/ap_UnixDialog_GenericProgress.h \
	service/unix/ServiceUnixAccountHandler.cpp \
	service/unix/ServiceUnixAccountHandler.h
ui_gtk3 += \
	service/unix/ap_UnixDialog_GenericInput.ui \
	service/unix/ap_UnixDialog_GenericProgress.ui
endif

libbackends_la_SOURCES += \
	service/xp/abicollab_types.h \
	service/xp/AbiCollabSaveInterceptor.cpp \
	service/xp/AbiCollabSaveInterceptor.h \
	service/xp/ap_Dialog_GenericInput.cpp \
	service/xp/ap_Dialog_GenericInput.h \
	service/xp/ap_Dialog_GenericProgress.cpp \
	service/xp/ap_Dialog_GenericProgress.h \
	service/xp/AbiCollabImport.h \
	service/xp/AbiCollabImport.cpp \
	service/xp/AbiCollabService_Export.cpp \
	service/xp/AbiCollabService_Export.h \
	service/xp/AsioRealmProtocol.h \
	service/xp/AsyncWorker.h \
	service/xp/InterruptableAsyncWorker.h \
	service/xp/ProgressiveSoapCall.h \
	service/xp/RealmBuddy.h \
	service/xp/RealmConnection.h \
	service/xp/RealmConnection.cpp \
	service/xp/RealmGrowBuffer.h \
	service/xp/RealmProtocol.cpp \
	service/xp/RealmProtocol.h \
	service/xp/ServiceAccountHandler.cpp \
	service/xp/ServiceAccountHandler.h \
	service/xp/ServiceBuddy.h \
	service/xp/ServiceErrorCodes.cpp \
	service/xp/ServiceErrorCodes.h \
	service/xp/soa.h \
	service/xp/soa_result.cpp \
	service/xp/soa_result.h \
	service/xp/soa_soup.cpp \
	service/xp/soa_soup.h \
	service/xp/soa_types.h \
	service/xp/tls_tunnel.cpp \
	service/xp/tls_tunnel.h \
	$(NULL)
endif

if COLLAB_BACKEND_SUGAR
libbackends_la_SOURCES += \
	sugar/unix/SugarBuddy.h \
	sugar/unix/SugarUnixAccountHandler.cpp \
	sugar/unix/SugarUnixAccountHandler.h
endif

if COLLAB_BACKEND_TCP

if TOOLKIT_WIN
libbackends_la_SOURCES += \
	tcp/win/TCPWin32AccountHandler.cpp \
	tcp/win/TCPWin32AccountHandler.h
endif

if TOOLKIT_GTK_ALL
libbackends_la_SOURCES += \
	tcp/unix/TCPUnixAccountHandler.cpp \
	tcp/unix/TCPUnixAccountHandler.h
endif

libbackends_la_SOURCES += \
	tcp/xp/IOServerHandler.h \
	tcp/xp/Session.h \
	tcp/xp/TCPAccountHandler.cpp \
	tcp/xp/TCPAccountHandler.h \
	tcp/xp/TCPBuddy.h
endif

if COLLAB_BACKEND_XMPP
libbackends_la_SOURCES += \
        xmpp/xp/XMPPAccountHandler.cpp \
        xmpp/xp/XMPPAccountHandler.h \
        xmpp/xp/XMPPBuddy.h \
	$(xmpp_@PLATFORM@_src)
if TOOLKIT_GTK_ALL
libbackends_la_SOURCES += \
	xmpp/unix/XMPPUnixAccountHandler.cpp \
	xmpp/unix/XMPPUnixAccountHandler.h
endif
endif

if COLLAB_BACKEND_TELEPATHY
if TOOLKIT_GTK_ALL
servicefiledir = $(datadir)/dbus-1/services
servicefile_in_file = telepathy/unix/org.freedesktop.Telepathy.Client.AbiCollab.service.in
servicefile_DATA = $(servicefile_in_file:.service.in=.service)
%.service: %.service.in Makefile
	$(AM_V_GEN)sed -e "s|[@]bindir[@]|$(bindir)|" $< > $@

clientfiledir = $(datarootdir)/telepathy/clients
clientfile_DATA = telepathy/unix/AbiCollab.client
libbackends_la_SOURCES += \
	telepathy/unix/TelepathyChatroom.cpp \
	telepathy/unix/TelepathyChatroom.h \
	telepathy/unix/TelepathyBuddy.h \
	telepathy/unix/DTubeBuddy.h \
	telepathy/unix/TelepathyUnixAccountHandler.cpp \
	telepathy/unix/TelepathyUnixAccountHandler.h
endif
endif

if COLLAB_BACKEND_SIPSIMPLE
if TOOLKIT_GTK_ALL
libbackends_la_SOURCES += \
	sipsimple/unix/SIPSimpleUnixAccountHandler.cpp \
	sipsimple/unix/SIPSimpleUnixAccountHandler.h
endif
libbackends_la_SOURCES += \
	sipsimple/xp/SIPSimpleAccountHandler.cpp \
	sipsimple/xp/SIPSimpleAccountHandler.h \
	sipsimple/xp/SIPSimpleBuddy.h
endif

ui_DATA = $(ui_gtk3)

EXTRA_DIST = \
	$(ui_gtk3) \
	$(win_rc) \
        $(cert_DATA) \
	$(servicefile_in_file) \
	$(clientfile_DATA)
