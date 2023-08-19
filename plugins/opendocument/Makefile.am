AUTOMAKE_OPTIONS = subdir-objects

SUBDIRS = 

if OPENDOCUMENT_BUILTIN
noinst_LTLIBRARIES = opendocument.la

else

plugindir = $(ABIWORD_PLUGINSDIR)
plugin_LTLIBRARIES = opendocument.la

endif # OPENDOCUMENT_BUILTIN

AM_CPPFLAGS = \
	-I$(srcdir)/common/xp/crypto \
	-I$(srcdir)/common/xp \
	$(OPENDOCUMENT_CFLAGS) \
	-DABI_BUILD_VERSION=\"$(VERSION)\"

opendocument_la_LIBADD = \
	$(OPENDOCUMENT_LIBS) \
	$(LIBGCRYPT_LIBS)

opendocument_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined
if TOOLKIT_COCOA
opendocument_la_LDFLAGS += -headerpad_max_install_names
endif # TOOLKIT_COCOA

opendocument_la_SOURCES = \
        common/xp/crypto/gc-pbkdf2-sha1.c \
        common/xp/crypto/gc-pbkdf2-sha1.h \
        common/xp/crypto/hmac-sha1.c \
        common/xp/crypto/hmac.h \
        common/xp/crypto/memxor.c \
        common/xp/crypto/memxor.h \
        common/xp/crypto/sha1.c \
        common/xp/crypto/sha1.h \
        common/xp/ie_impexp_OpenDocument.cpp \
        common/xp/ODc_Crypto.cpp \
        common/xp/ODc_Crypto.h \
        common/xp/ODc_util.cpp \
        common/xp/ODc_util.h \
	exp/xp/ie_exp_OpenDocument.cpp \
	exp/xp/ie_exp_OpenDocument.h \
	exp/xp/ie_exp_OpenDocument_Sniffer.cpp \
	exp/xp/ie_exp_OpenDocument_Sniffer.h \
	exp/xp/ODe_AbiDocListener.cpp \
	exp/xp/ODe_AbiDocListener.h \
	exp/xp/ODe_AbiDocListenerImpl.cpp \
	exp/xp/ODe_AbiDocListenerImpl.h \
	exp/xp/ODe_AutomaticStyles.cpp \
	exp/xp/ODe_AutomaticStyles.h \
	exp/xp/ODe_AuxiliaryData.cpp \
	exp/xp/ODe_AuxiliaryData.h \
	exp/xp/ODe_Common.cpp \
	exp/xp/ODe_Common.h \
	exp/xp/ODe_DefaultStyles.cpp \
	exp/xp/ODe_DefaultStyles.h \
	exp/xp/ODe_DocumentData.cpp \
	exp/xp/ODe_DocumentData.h \
	exp/xp/ODe_FontFaceDecls.cpp \
	exp/xp/ODe_FontFaceDecls.h \
	exp/xp/ODe_Frame_Listener.cpp \
	exp/xp/ODe_Frame_Listener.h \
	exp/xp/ODe_HeadingSearcher_Listener.cpp \
	exp/xp/ODe_HeadingSearcher_Listener.h \
	exp/xp/ODe_ListenerAction.h \
	exp/xp/ODe_ListLevelStyle.cpp \
	exp/xp/ODe_ListLevelStyle.h \
	exp/xp/ODe_Main_Listener.cpp \
	exp/xp/ODe_Main_Listener.h \
	exp/xp/ODe_ManifestWriter.cpp \
	exp/xp/ODe_ManifestWriter.h \
	exp/xp/ODe_RDFWriter.cpp \
	exp/xp/ODe_RDFWriter.h \
	exp/xp/ODe_ThumbnailsWriter.cpp \
	exp/xp/ODe_ThumbnailsWriter.h \
	exp/xp/ODe_MetaDataWriter.cpp \
	exp/xp/ODe_MetaDataWriter.h \
	exp/xp/ODe_Note_Listener.cpp \
	exp/xp/ODe_Note_Listener.h \
	exp/xp/ODe_PicturesWriter.cpp \
	exp/xp/ODe_PicturesWriter.h \
	exp/xp/ODe_SettingsWriter.cpp \
	exp/xp/ODe_SettingsWriter.h \
	exp/xp/ODe_Style_List.cpp \
	exp/xp/ODe_Style_List.h \
	exp/xp/ODe_Style_MasterPage.cpp \
	exp/xp/ODe_Style_MasterPage.h \
	exp/xp/ODe_Style_PageLayout.cpp \
	exp/xp/ODe_Style_PageLayout.h \
	exp/xp/ODe_Styles.cpp \
	exp/xp/ODe_Styles.h \
	exp/xp/ODe_Style_Style.cpp \
	exp/xp/ODe_Style_Style.h \
	exp/xp/ODe_Table_Listener.cpp \
	exp/xp/ODe_Table_Listener.h \
	exp/xp/ODe_Text_Listener.cpp \
	exp/xp/ODe_Text_Listener.h \
	exp/xp/ODe_TOC_Listener.cpp \
	exp/xp/ODe_TOC_Listener.h \
	imp/xp/ie_imp_OpenDocument.cpp \
	imp/xp/ie_imp_OpenDocument.h \
	imp/xp/ie_imp_OpenDocument_Sniffer.cpp \
	imp/xp/ie_imp_OpenDocument_Sniffer.h \
	imp/xp/ODi_Abi_Data.cpp \
	imp/xp/ODi_Abi_Data.h \
	imp/xp/ODi_ContentStream_ListenerState.cpp \
	imp/xp/ODi_ContentStream_ListenerState.h \
	imp/xp/ODi_ContentStreamAnnotationMatcher_ListenerState.cpp \
	imp/xp/ODi_ContentStreamAnnotationMatcher_ListenerState.h \
	imp/xp/ODi_ElementStack.cpp \
	imp/xp/ODi_ElementStack.h \
	imp/xp/ODi_FontFaceDecls.cpp \
	imp/xp/ODi_FontFaceDecls.h \
	imp/xp/ODi_Frame_ListenerState.cpp \
	imp/xp/ODi_Frame_ListenerState.h \
	imp/xp/ODi_ListenerStateAction.cpp \
	imp/xp/ODi_ListenerStateAction.h \
	imp/xp/ODi_ListenerState.h \
	imp/xp/ODi_ListLevelStyle.cpp \
	imp/xp/ODi_ListLevelStyleFormats.h \
	imp/xp/ODi_ListLevelStyle.h \
	imp/xp/ODi_ManifestStream_ListenerState.cpp \
	imp/xp/ODi_ManifestStream_ListenerState.h \
	imp/xp/ODi_MetaStream_ListenerState.cpp \
	imp/xp/ODi_MetaStream_ListenerState.h \
	imp/xp/ODi_NotesConfiguration.cpp \
	imp/xp/ODi_NotesConfiguration.h \
	imp/xp/ODi_Office_Styles.cpp \
	imp/xp/ODi_Office_Styles.h \
	imp/xp/ODi_Postpone_ListenerState.cpp \
	imp/xp/ODi_Postpone_ListenerState.h \
	imp/xp/ODi_SettingsStream_ListenerState.h \
	imp/xp/ODi_StartTag.cpp \
	imp/xp/ODi_StartTag.h \
	imp/xp/ODi_StreamListener.cpp \
	imp/xp/ODi_StreamListener.h \
	imp/xp/ODi_Style_List.cpp \
	imp/xp/ODi_Style_List.h \
	imp/xp/ODi_Style_MasterPage.cpp \
	imp/xp/ODi_Style_MasterPage.h \
	imp/xp/ODi_Style_PageLayout.cpp \
	imp/xp/ODi_Style_PageLayout.h \
	imp/xp/ODi_StylesStream_ListenerState.cpp \
	imp/xp/ODi_StylesStream_ListenerState.h \
	imp/xp/ODi_Style_Style.cpp \
	imp/xp/ODi_Style_Style_Family.cpp \
	imp/xp/ODi_Style_Style_Family.h \
	imp/xp/ODi_Style_Style.h \
	imp/xp/ODi_Table_ListenerState.cpp \
	imp/xp/ODi_Table_ListenerState.h \
	imp/xp/ODi_TableOfContent_ListenerState.cpp \
	imp/xp/ODi_TableOfContent_ListenerState.h \
	imp/xp/ODi_TextContent_ListenerState.cpp \
	imp/xp/ODi_TextContent_ListenerState.h \
	imp/xp/ODi_XMLRecorder.cpp \
	imp/xp/ODi_XMLRecorder.h \
	$(NULL)


if TOOLKIT_COCOA
install-data-hook:
	install_name_tool -change $(ABIWORD_LIBDIR)/libabiword-@ABIWORD_SERIES@.dylib \
		@executable_path/../Frameworks/libabiword-@ABIWORD_SERIES@.dylib \
		$(DESTDIR)/$(plugindir)/opendocument.so
endif # TOOLKIT_COCOA

EXTRA_DIST = \
	common/xp/crypto/README \
	plugin.m4

