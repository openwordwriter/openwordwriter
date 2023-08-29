## Copyright (C) 2023 Hubert Figuière
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
## 02110-1301 USA.

noinst_LTLIBRARIES = libimpexp.la

AM_CPPFLAGS = \
	$(WP_CPPFLAGS)

libimpexp_la_SOURCES = \
	xp/ie_exp_AbiWord_1.cpp \
	xp/ie_exp_AbiWord_1.h \
	xp/ie_exp_AWT.cpp \
	xp/ie_exp_AWT.h \
	xp/ie_exp.cpp \
	xp/ie_exp.h \
	xp/ie_exp_HTML.cpp \
	xp/ie_exp_HTML.h \
	xp/ie_exp_HTML_DocumentWriter.cpp \
	xp/ie_exp_HTML_DocumentWriter.h \
	xp/ie_exp_HTML_StyleTree.cpp \
	xp/ie_exp_HTML_StyleTree.h \
	xp/ie_exp_HTML_util.cpp \
	xp/ie_exp_HTML_util.h \
	xp/ie_exp_HTML_UtilListeners.cpp \
	xp/ie_exp_HTML_UtilListeners.h \
	xp/ie_exp_HTML_Listener.cpp \
	xp/ie_exp_HTML_Listener.h \
	xp/ie_exp_HTML_NavigationHelper.cpp \
	xp/ie_exp_HTML_NavigationHelper.h \
	xp/ie_exp_RTF_AttrProp.cpp \
	xp/ie_exp_RTF_AttrProp.h \
	xp/ie_exp_RTF.cpp \
	xp/ie_exp_RTF.h \
	xp/ie_exp_RTF_listenerGetProps.cpp \
	xp/ie_exp_RTF_listenerGetProps.h \
	xp/ie_exp_RTF_listenerWriteDoc.cpp \
	xp/ie_exp_RTF_listenerWriteDoc.h \
	xp/ie_exp_Text.cpp \
	xp/ie_exp_Text.h \
	xp/ie_exp_XML.cpp \
	xp/ie_exp_XML.h \
	xp/ie_FileInfo.cpp \
	xp/ie_FileInfo.h \
	xp/ie_imp_AbiWord_1.cpp \
	xp/ie_imp_AbiWord_1.h \
	xp/ie_imp.cpp \
	xp/ie_impexp_AbiWord_1.h \
	xp/ie_impexp_HTML.h \
	xp/ie_impexp_MsWord_97.h \
	xp/ie_impexp_Register.cpp \
	xp/ie_impexp_Register.h \
	xp/ie_impexp_RTF.h \
	xp/ie_impexp_Text.h \
	xp/ie_imp_GraphicAsDocument.cpp \
	xp/ie_imp_GraphicAsDocument.h \
	xp/ie_impGraphic.cpp \
	xp/ie_impGraphic.h \
	xp/ie_impGraphic_PNG.cpp \
	xp/ie_impGraphic_PNG.h \
	xp/ie_impGraphic_SVG.cpp \
	xp/ie_impGraphic_SVG.h \
	xp/ie_imp.h \
	xp/ie_imp_MsWord_97.cpp \
	xp/ie_imp_MsWord_97.h \
	xp/ie_imp_MsWord_dump.h \
	xp/ie_imp_PasteListener.cpp \
	xp/ie_imp_PasteListener.h \
	xp/ie_imp_RTF.cpp \
	xp/ie_imp_RTF.h \
	xp/ie_imp_RTFKeywordIDs.h \
	xp/ie_imp_RTFKeywords.h \
	xp/ie_imp_RTFObjectsAndPicts.cpp \
	xp/ie_imp_RTFParse.cpp \
	xp/ie_imp_RTFParse.h \
	xp/ie_imp_Text.cpp \
	xp/ie_imp_Text.h \
	xp/ie_imp_RDF.cpp \
	xp/ie_imp_RDF.h \
	xp/ie_imp_XHTML.cpp \
	xp/ie_imp_XHTML.h \
	xp/ie_imp_XML.cpp \
	xp/ie_imp_XML.h \
	xp/ie_mailmerge.cpp \
	xp/ie_mailmerge.h \
	xp/ie_math_convert.cpp \
	xp/ie_math_convert.h \
	xp/ie_Table.cpp \
	xp/ie_Table.h \
	xp/ie_TOC.cpp \
	xp/ie_TOC.h \
	xp/ie_types.h \
	xp/MathSVGScript.h \
	xp/ie_exp_DocRangeListener.cpp \
        xp/ie_exp_DocRangeListener.h

if TOOLKIT_GTK_ALL
libimpexp_la_SOURCES += \
	gtk/ie_exp_PDF.cpp \
	gtk/ie_exp_PDF.h \
	gtk/ie_impexp_RegisterUnix.cpp \
	gtk/ie_impGraphic_GdkPixbuf.cpp \
	gtk/ie_impGraphic_GdkPixbuf.h
endif

if TOOLKIT_COCOA
AM_CPPFLAGS += \
	-x objective-c++
libimpexp_la_SOURCES += \
	cocoa/ie_impexp_RegisterCocoa.cpp \
	cocoa/ie_impGraphic_Cocoa.cpp \
	cocoa/ie_impGraphic_Cocoa.h
endif

if TOOLKIT_WIN
libimpexp_la_SOURCES += \
	win/ie_impexp_RegisterWin32.cpp \
	win/ie_impGraphic_Win32GDIPlus.cpp \
	win/ie_impGraphic_Win32Native.cpp \
	win/ie_impGraphic_Win32Native.h
endif

if TOOLKIT_QT
libimpexp_la_SOURCES += \
	qt/ie_impexp_RegisterQt.cpp
endif

