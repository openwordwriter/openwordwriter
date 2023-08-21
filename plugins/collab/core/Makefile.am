AM_CPPFLAGS = \
	$(COLLAB_CFLAGS) \
	-I$(srcdir)/ \
	-I$(srcdir)/.. \
	-I$(srcdir)/dialogs \
	-I$(srcdir)/packet \
	-I$(srcdir)/plugin \
	-I$(srcdir)/session \
	-DABI_BUILD_VERSION=\"$(VERSION)\"

noinst_LTLIBRARIES = libcore.la

libcore_la_LIBADD =

libcore_la_SOURCES =

COLLAB_RCFLAGS = -I$(srcdir)

uidir=$(ABIWORD_UIDIR)
ui_gtk3 =
ui_win32 =

if TOOLKIT_WIN
libcore_la_SOURCES += \
	dialogs/win/ap_Win32Dialog_CollaborationAccounts.cpp \
	dialogs/win/ap_Win32Dialog_CollaborationAccounts.h \
	dialogs/win/ap_Win32Dialog_CollaborationAddAccount.cpp \
	dialogs/win/ap_Win32Dialog_CollaborationAddAccount.h \
	dialogs/win/ap_Win32Dialog_CollaborationAddBuddy.cpp \
	dialogs/win/ap_Win32Dialog_CollaborationAddBuddy.h \
	dialogs/win/ap_Win32Dialog_CollaborationEditAccount.cpp \
	dialogs/win/ap_Win32Dialog_CollaborationEditAccount.h \
	dialogs/win/ap_Win32Dialog_CollaborationJoin.cpp \
	dialogs/win/ap_Win32Dialog_CollaborationJoin.h \
	dialogs/win/ap_Win32Dialog_CollaborationShare.cpp \
	dialogs/win/ap_Win32Dialog_CollaborationShare.h

libcore_la_LIBADD += ap_Win32Res_CollaborationDialogs.lo
ap_Win32Res_CollaborationDialogs.lo:  $(srcdir)/dialogs/win/ap_Win32Res_CollaborationDialogs.rc $(libcore_la_OBJECTS)
	CPPFLAGS="$(COLLAB_RCFLAGS)" $(top_srcdir)/lt-compile-resource $< $@

ui_win32 += \
	dialogs/win/ap_Win32Res_CollaborationDialogs.rc \
	dialogs/win/ap_Win32Res_DlgCollaborationAccounts.rc2 \
	dialogs/win/ap_Win32Res_DlgCollaborationAddAccount.rc2 \
	dialogs/win/ap_Win32Res_DlgCollaborationAddBuddy.rc2 \
	dialogs/win/ap_Win32Res_DlgCollaborationEditAccount.rc2 \
	dialogs/win/ap_Win32Res_DlgCollaborationJoin.rc2 \
	dialogs/win/ap_Win32Res_DlgCollaborationShare.rc2
endif

if TOOLKIT_GTK_ALL
libcore_la_SOURCES += \
	dialogs/unix/ap_UnixDialog_CollaborationAccounts.cpp \
	dialogs/unix/ap_UnixDialog_CollaborationAccounts.h \
	dialogs/unix/ap_UnixDialog_CollaborationAddAccount.cpp \
	dialogs/unix/ap_UnixDialog_CollaborationAddAccount.h \
	dialogs/unix/ap_UnixDialog_CollaborationAddBuddy.cpp \
	dialogs/unix/ap_UnixDialog_CollaborationAddBuddy.h \
	dialogs/unix/ap_UnixDialog_CollaborationEditAccount.cpp \
	dialogs/unix/ap_UnixDialog_CollaborationEditAccount.h \
	dialogs/unix/ap_UnixDialog_CollaborationJoin.cpp \
	dialogs/unix/ap_UnixDialog_CollaborationJoin.h \
	dialogs/unix/ap_UnixDialog_CollaborationShare.cpp \
	dialogs/unix/ap_UnixDialog_CollaborationShare.h

ui_gtk3+ = \
	dialogs/unix/ap_UnixDialog_CollaborationAccounts.ui \
	dialogs/unix/ap_UnixDialog_CollaborationAddAccount.ui \
	dialogs/unix/ap_UnixDialog_CollaborationAddBuddy.ui \
	dialogs/unix/ap_UnixDialog_CollaborationEditAccount.ui \
	dialogs/unix/ap_UnixDialog_CollaborationJoin.ui \
	dialogs/unix/ap_UnixDialog_CollaborationShare.ui
endif

ui_DATA = $(ui_gtk3)

libcore_la_SOURCES += \
	account/xp/AccountEvent.cpp \
	account/xp/AccountEvent.h \
	account/xp/AccountHandler.cpp \
	account/xp/AccountHandler.h \
	account/xp/BackendPackets.cpp \
	account/xp/Buddy.cpp \
	account/xp/Buddy.h \
	account/xp/DocHandle.h \
	account/xp/DocTreeItem.h \
	account/xp/Event.h \
	account/xp/EventListener.h \
	account/xp/SessionEvent.cpp \
	account/xp/SessionEvent.h \
	dialogs/xp/ap_Dialog_CollaborationAccounts.cpp \
	dialogs/xp/ap_Dialog_CollaborationAccounts.h \
	dialogs/xp/ap_Dialog_CollaborationAddAccount.cpp \
	dialogs/xp/ap_Dialog_CollaborationAddAccount.h \
	dialogs/xp/ap_Dialog_CollaborationAddBuddy.cpp \
	dialogs/xp/ap_Dialog_CollaborationAddBuddy.h \
	dialogs/xp/ap_Dialog_CollaborationEditAccount.cpp \
	dialogs/xp/ap_Dialog_CollaborationEditAccount.h \
	dialogs/xp/ap_Dialog_CollaborationJoin.cpp \
	dialogs/xp/ap_Dialog_CollaborationJoin.h \
	dialogs/xp/ap_Dialog_CollaborationShare.cpp \
	dialogs/xp/ap_Dialog_CollaborationShare.h \
	packet/xp/AbiCollab_Packet.cpp \
	packet/xp/AbiCollab_Packet.h \
	packet/xp/CorePackets.cpp \
	packet/xp/EventPacket.h \
	packet/xp/Serialization.cpp \
	packet/xp/Serialization.h \
	plugin/xp/AbiCollab_Command.cpp \
	plugin/xp/AbiCollab_Command.h \
	plugin/xp/AbiCollab_Plugin.cpp \
	plugin/xp/AbiCollab_Plugin.h \
	plugin/xp/AbiCollab_Regression.cpp \
	plugin/xp/AbiCollab_Regression.h \
	session/xp/AbiCollab.cpp \
	session/xp/AbiCollab_Export.cpp \
	session/xp/AbiCollab_Export.h \
	session/xp/AbiCollab.h \
	session/xp/AbiCollab_Import.cpp \
	session/xp/AbiCollab_Import.h \
	session/xp/AbiCollab_ImportRuleSet.cpp \
	session/xp/AbiCollab_ImportRuleSet.h \
	session/xp/AbiCollabSessionManager.cpp \
	session/xp/AbiCollabSessionManager.h \
	session/xp/DiskSessionRecorder.cpp \
	session/xp/DiskSessionRecorder.h \
	sync/xp/lock.h \
	sync/xp/SynchronizedQueue.h \
	sync/xp/Synchronizer.cpp \
	sync/xp/Synchronizer.h \
	$(NULL)

EXTRA_DIST = \
	$(ui_win32) \
	$(ui_gtk3)
