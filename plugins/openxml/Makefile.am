
if OPENXML_BUILTIN
noinst_LTLIBRARIES = openxml.la

else

plugindir = $(ABIWORD_PLUGINSDIR)
plugin_LTLIBRARIES = openxml.la

endif

AM_CPPFLAGS = \
	$(OPENXML_CFLAGS) \
	-I$(srcdir)/common/xp \
	-I$(srcdir)/imp/xp -I$(srcdir)/exp/xp \
	-DABI_BUILD_VERSION=\"$(VERSION)\"

openxml_la_LIBADD = \
	$(OPENXML_LIBS)

openxml_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined
if TOOLKIT_COCOA
openxml_la_LDFLAGS += -headerpad_max_install_names
endif # TOOLKIT_COCOA

openxml_la_SOURCES = \
	common/xp/ie_impexp_OpenXML.cpp \
	common/xp/OXML_Document.cpp \
	common/xp/OXML_Document.h \
	common/xp/OXML_Element.cpp \
	common/xp/OXML_Element.h \
	common/xp/OXML_Element_Run.cpp \
	common/xp/OXML_Element_Run.h \
	common/xp/OXML_Element_Text.cpp \
	common/xp/OXML_Element_Text.h \
	common/xp/OXML_Element_Paragraph.cpp \
	common/xp/OXML_Element_Paragraph.h \
	common/xp/OXML_Element_Table.cpp \
	common/xp/OXML_Element_Table.h \
	common/xp/OXML_Element_Row.cpp \
	common/xp/OXML_Element_Row.h \
	common/xp/OXML_Element_Cell.cpp \
	common/xp/OXML_Element_Cell.h \
	common/xp/OXML_Element_List.cpp \
	common/xp/OXML_Element_List.h \
	common/xp/OXML_Element_Math.cpp \
	common/xp/OXML_Element_Math.h \
	common/xp/OXML_Element_Image.cpp \
	common/xp/OXML_Element_Image.h \
	common/xp/OXML_Element_Hyperlink.cpp \
	common/xp/OXML_Element_Hyperlink.h \
	common/xp/OXML_Element_Bookmark.cpp \
	common/xp/OXML_Element_Bookmark.h \
	common/xp/OXML_Element_Field.cpp \
	common/xp/OXML_Element_Field.h \
	common/xp/OXML_Element_TextBox.cpp \
	common/xp/OXML_Element_TextBox.h \
	common/xp/OXML_FontManager.cpp \
	common/xp/OXML_LangToScriptConverter.h \
	common/xp/OXML_FontManager.h \
	common/xp/OXML_ObjectWithAttrProp.cpp \
	common/xp/OXML_ObjectWithAttrProp.h \
	common/xp/OXML_Section.cpp \
	common/xp/OXML_Section.h \
	common/xp/OXML_Style.cpp \
	common/xp/OXML_Style.h \
	common/xp/OXML_List.h \
	common/xp/OXML_List.cpp \
	common/xp/OXML_Image.h \
	common/xp/OXML_Image.cpp \
	common/xp/OXML_Theme.cpp \
	common/xp/OXML_Theme.h \
	common/xp/OXML_Types.h \
	imp/xp/ie_imp_OpenXML.cpp \
	imp/xp/ie_imp_OpenXML.h \
	imp/xp/ie_imp_OpenXML_Sniffer.cpp \
	imp/xp/ie_imp_OpenXML_Sniffer.h \
	imp/xp/OXMLi_ListenerState_Common.cpp \
	imp/xp/OXMLi_ListenerState_Common.h \
	imp/xp/OXMLi_ListenerState_DocSettings.cpp \
	imp/xp/OXMLi_ListenerState_DocSettings.h \
	imp/xp/OXMLi_ListenerState.cpp \
	imp/xp/OXMLi_ListenerState.h \
	imp/xp/OXMLi_ListenerState_HdrFtr.cpp \
	imp/xp/OXMLi_ListenerState_HdrFtr.h \
	imp/xp/OXMLi_ListenerState_MainDocument.cpp \
	imp/xp/OXMLi_ListenerState_MainDocument.h \
	imp/xp/OXMLi_ListenerState_Math.cpp \
	imp/xp/OXMLi_ListenerState_Math.h \
	imp/xp/OXMLi_ListenerState_Styles.cpp \
	imp/xp/OXMLi_ListenerState_Styles.h \
	imp/xp/OXMLi_ListenerState_Theme.cpp \
	imp/xp/OXMLi_ListenerState_Theme.h \
	imp/xp/OXMLi_ListenerState_Numbering.cpp \
	imp/xp/OXMLi_ListenerState_Numbering.h \
	imp/xp/OXMLi_ListenerState_Table.cpp \
	imp/xp/OXMLi_ListenerState_Table.h \
	imp/xp/OXMLi_ListenerState_Field.cpp \
	imp/xp/OXMLi_ListenerState_Field.h \
	imp/xp/OXMLi_ListenerState_Footnote.cpp \
	imp/xp/OXMLi_ListenerState_Footnote.h \
	imp/xp/OXMLi_ListenerState_Endnote.cpp \
	imp/xp/OXMLi_ListenerState_Endnote.h \
	imp/xp/OXMLi_ListenerState_Image.cpp \
	imp/xp/OXMLi_ListenerState_Image.h \
	imp/xp/OXMLi_ListenerState_Textbox.cpp \
	imp/xp/OXMLi_ListenerState_Textbox.h \
	imp/xp/OXMLi_ListenerState_Valid.cpp \
	imp/xp/OXMLi_ListenerState_Valid.h \
	imp/xp/OXMLi_PackageManager.cpp \
	imp/xp/OXMLi_PackageManager.h \
	imp/xp/OXMLi_StreamListener.cpp \
	imp/xp/OXMLi_StreamListener.h \
	imp/xp/OXMLi_Namespace_Common.cpp \
	imp/xp/OXMLi_Namespace_Common.h \
	imp/xp/OXMLi_Types.h \
	exp/xp/ie_exp_OpenXML.cpp \
	exp/xp/ie_exp_OpenXML.h \
	exp/xp/ie_exp_OpenXML_Sniffer.cpp \
	exp/xp/ie_exp_OpenXML_Sniffer.h \
	exp/xp/ie_exp_OpenXML_Listener.cpp \
	exp/xp/ie_exp_OpenXML_Listener.h
	$(NULL)


if TOOLKIT_COCOA
install-data-hook:
	install_name_tool -change $(ABIWORD_LIBDIR)/libabiword-@ABIWORD_SERIES@.dylib \
		@executable_path/../Frameworks/libabiword-@ABIWORD_SERIES@.dylib \
		$(DESTDIR)/$(plugindir)/openxml.so
endif # TOOLKIT_COCOA

common/xp/OXML_LangToScriptConverter.h: $(srcdir)/common/xp/OXML_LangToScriptConverter.gperf
	gperf $< >$@

OMMLXSLT = \
	omml_xslt/mml2omml.xsl \
	omml_xslt/omml2mml.xsl

ommlxslt_DATA = $(OMMLXSLT)
ommlxsltdir = $(ABIWORD_OMMLXSLTDIR)

EXTRA_DIST = \
	$(OMMLXSLT) \
	common/xp/converter.pl \
	common/xp/OXML_LangToScriptConverter.gperf \
	plugin.m4

