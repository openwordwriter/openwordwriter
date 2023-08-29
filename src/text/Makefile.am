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

AM_CPPFLAGS = \
	$(WP_CPPFLAGS)

noinst_LTLIBRARIES = libtext.la

libtext_la_SOURCES = \
	fmt/xp/fb_Alignment.cpp \
	fmt/xp/fb_Alignment.h \
	fmt/xp/fb_ColumnBreaker.cpp \
	fmt/xp/fb_ColumnBreaker.h \
	fmt/xp/fb_LineBreaker.cpp \
	fmt/xp/fb_LineBreaker.h \
	fmt/xp/fg_Graphic.cpp \
	fmt/xp/fg_Graphic.h \
	fmt/xp/fg_GraphicRaster.cpp \
	fmt/xp/fg_GraphicRaster.h \
	fmt/xp/fg_GraphicVector.cpp \
	fmt/xp/fg_GraphicVector.h \
	fmt/xp/fl_AutoLists.cpp \
	fmt/xp/fl_AutoLists.h \
	fmt/xp/fl_AutoNum.cpp \
	fmt/xp/fl_AutoNum.h \
	fmt/xp/fl_BlockLayout.cpp \
	fmt/xp/fl_BlockLayout.h \
	fmt/xp/fl_ContainerLayout.cpp \
	fmt/xp/fl_ContainerLayout.h \
	fmt/xp/fl_DocLayout.cpp \
	fmt/xp/fl_DocLayout.h \
	fmt/xp/fl_DocListener.cpp \
	fmt/xp/fl_DocListener.h \
	fmt/xp/fl_FootnoteLayout.cpp \
	fmt/xp/fl_FootnoteLayout.h \
	fmt/xp/fl_FrameLayout.cpp \
	fmt/xp/fl_FrameLayout.h \
	fmt/xp/fl_Layout.cpp \
	fmt/xp/fl_Layout.h \
	fmt/xp/fl_PartOfBlock.cpp \
	fmt/xp/fl_PartOfBlock.h \
	fmt/xp/fl_SectionLayout.cpp \
	fmt/xp/fl_SectionLayout.h \
	fmt/xp/fl_SelectionPreserver.cpp \
	fmt/xp/fl_SelectionPreserver.h \
	fmt/xp/fl_Squiggles.h \
	fmt/xp/fl_TableLayout.cpp \
	fmt/xp/fl_TableLayout.h \
	fmt/xp/fl_TestRoutines.cpp \
	fmt/xp/fl_TOCLayout.cpp \
	fmt/xp/fl_TOCLayout.h \
	fmt/xp/fp_AnnotationRun.cpp \
	fmt/xp/fp_AnnotationRun.h \
	fmt/xp/fp_RDFAnchorRun.cpp \
	fmt/xp/fp_RDFAnchorRun.h \
	fmt/xp/fp_Column.cpp \
	fmt/xp/fp_Column.h \
	fmt/xp/fp_ContainerObject.cpp \
	fmt/xp/fp_ContainerObject.h \
	fmt/xp/fp_DirectionMarkerRun.cpp \
	fmt/xp/fp_DirectionMarkerRun.h \
	fmt/xp/fp_EmbedRun.cpp \
	fmt/xp/fp_EmbedRun.h \
	fmt/xp/fp_FieldListLabelRun.cpp \
	fmt/xp/fp_FieldListLabelRun.h \
	fmt/xp/fp_Fields.h \
	fmt/xp/fp_FieldTableSumRun.cpp \
	fmt/xp/fp_FieldTOCNum.cpp \
	fmt/xp/fp_FieldTOCNum.h \
	fmt/xp/fp_FmtMarkRun.cpp \
	fmt/xp/fp_FmtMarkRun.h \
	fmt/xp/fp_FootnoteContainer.cpp \
	fmt/xp/fp_FootnoteContainer.h \
	fmt/xp/fp_FrameContainer.cpp \
	fmt/xp/fp_FrameContainer.h \
	fmt/xp/fp_Line.cpp \
	fmt/xp/fp_Line.h \
	fmt/xp/fp_MathRun.cpp \
	fmt/xp/fp_MathRun.h \
	fmt/xp/fp_Page.cpp \
	fmt/xp/fp_Page.h \
	fmt/xp/fp_PageSize.cpp \
	fmt/xp/fp_PageSize.h \
	fmt/xp/fp_Run.cpp \
	fmt/xp/fp_Run.h \
	fmt/xp/fp_TableContainer.cpp \
	fmt/xp/fp_TableContainer.h \
	fmt/xp/fp_TestRoutines.cpp \
	fmt/xp/fp_TextRun.cpp \
	fmt/xp/fp_TextRun.h \
	fmt/xp/fp_TOCContainer.cpp \
	fmt/xp/fp_TOCContainer.h \
	fmt/xp/fp_types.h \
	fmt/xp/fv_Base.cpp \
	fmt/xp/fv_Base.h \
	fmt/xp/fv_FrameEdit.cpp \
	fmt/xp/fv_FrameEdit.h \
	fmt/xp/fv_InlineImage.cpp \
	fmt/xp/fv_InlineImage.h \
	fmt/xp/fv_Selection.cpp \
	fmt/xp/fv_Selection.h \
	fmt/xp/fv_SelectionHandles.cpp \
	fmt/xp/fv_SelectionHandles.h \
	fmt/xp/fv_View_cmd.cpp \
	fmt/xp/fv_View.cpp \
	fmt/xp/fv_View.h \
	fmt/xp/fv_View_protected.cpp \
	fmt/xp/fv_View_TestRoutines.cpp \
	fmt/xp/fv_VisualDragText.cpp \
	fmt/xp/fv_VisualDragText.h \
	fmt/xp/fv_ViewDoubleBuffering.cpp \
	fmt/xp/fv_ViewDoubleBuffering.h \
	ptbl/xp/fd_Field.cpp \
	ptbl/xp/fd_Field.h \
	ptbl/xp/pd_Document.cpp \
	ptbl/xp/pd_Document.h \
	ptbl/xp/pd_Document_TestRoutines.cpp \
	ptbl/xp/pd_DocumentRDF.h \
	ptbl/xp/pd_DocumentRDF.cpp \
	ptbl/xp/pd_RDFQuery.h \
	ptbl/xp/pd_RDFQuery.cpp \
	ptbl/xp/pd_RDFSupport.h \
	ptbl/xp/pd_RDFSupport.cpp \
	ptbl/xp/pd_Iterator.cpp \
	ptbl/xp/pd_Iterator.h \
	ptbl/xp/pd_Style.cpp \
	ptbl/xp/pd_Style.h \
	ptbl/xp/pf_Frag.cpp \
	ptbl/xp/pf_Frag_FmtMark.cpp \
	ptbl/xp/pf_Frag_FmtMark.h \
	ptbl/xp/pf_Frag.h \
	ptbl/xp/pf_Fragments.cpp \
	ptbl/xp/pf_Fragments.h \
	ptbl/xp/pf_Frag_Object.cpp \
	ptbl/xp/pf_Frag_Object.h \
	ptbl/xp/pf_Frags_TestRoutines.cpp \
	ptbl/xp/pf_Frag_Strux_Block.cpp \
	ptbl/xp/pf_Frag_Strux_Block.h \
	ptbl/xp/pf_Frag_Strux.cpp \
	ptbl/xp/pf_Frag_Strux.h \
	ptbl/xp/pf_Frag_Strux_Section.cpp \
	ptbl/xp/pf_Frag_Strux_Section.h \
	ptbl/xp/pf_Frag_TestRoutines.cpp \
	ptbl/xp/pf_Frag_Text.cpp \
	ptbl/xp/pf_Frag_Text.h \
	ptbl/xp/pl_Listener.h \
	ptbl/xp/pl_ListenerCoupleCloser.h \
	ptbl/xp/pl_ListenerCoupleCloser.cpp \
	ptbl/xp/po_Bookmark.cpp \
	ptbl/xp/po_Bookmark.h \
	ptbl/xp/pp_AttrProp.cpp \
	ptbl/xp/pp_AttrProp.h \
	ptbl/xp/pp_Property.cpp \
	ptbl/xp/pp_Property.h \
	ptbl/xp/pp_PropertyMap.cpp \
	ptbl/xp/pp_PropertyMap.h \
	ptbl/xp/pp_Revision.cpp \
	ptbl/xp/pp_Revision.h \
	ptbl/xp/pp_TableAttrProp.cpp \
	ptbl/xp/pp_TableAttrProp.h \
	ptbl/xp/pt_PieceTable.cpp \
	ptbl/xp/pt_PieceTable.h \
	ptbl/xp/pt_PT_Append.cpp \
	ptbl/xp/pt_PT_ChangeObject.cpp \
	ptbl/xp/pt_PT_ChangeSpan.cpp \
	ptbl/xp/pt_PT_ChangeStrux.cpp \
	ptbl/xp/pt_PT_DeleteObject.cpp \
	ptbl/xp/pt_PT_DeleteSpan.cpp \
	ptbl/xp/pt_PT_DeleteStrux.cpp \
	ptbl/xp/pt_PT_FmtMark.cpp \
	ptbl/xp/pt_PT_Glob.cpp \
	ptbl/xp/pt_PT_InsertObject.cpp \
	ptbl/xp/pt_PT_InsertSpan.cpp \
	ptbl/xp/pt_PT_InsertStrux.cpp \
	ptbl/xp/pt_PT_Listener.cpp \
	ptbl/xp/pt_PT_Styles.cpp \
	ptbl/xp/pt_PT_TestRoutines.cpp \
	ptbl/xp/pt_PT_Undo.cpp \
	ptbl/xp/pt_Types.h \
	ptbl/xp/pt_VarSet.cpp \
	ptbl/xp/pt_VarSet.h \
	ptbl/xp/px_ChangeHistory.cpp \
	ptbl/xp/px_ChangeHistory.h \
	ptbl/xp/px_ChangeRecord.cpp \
	ptbl/xp/px_ChangeRecord.h \
	ptbl/xp/px_CH_TestRoutines.cpp \
	ptbl/xp/px_CR_DataItem.cpp \
	ptbl/xp/px_CR_DataItem.h \
	ptbl/xp/px_CR_DocProp.cpp \
	ptbl/xp/px_CR_DocProp.h \
	ptbl/xp/px_CR_FmtMarkChange.cpp \
	ptbl/xp/px_CR_FmtMarkChange.h \
	ptbl/xp/px_CR_FmtMark.cpp \
	ptbl/xp/px_CR_FmtMark.h \
	ptbl/xp/px_CR_Glob.cpp \
	ptbl/xp/px_CR_Glob.h \
	ptbl/xp/px_CR_ObjectChange.cpp \
	ptbl/xp/px_CR_ObjectChange.h \
	ptbl/xp/px_CR_Object.cpp \
	ptbl/xp/px_CR_Object.h \
	ptbl/xp/px_CR_SpanChange.cpp \
	ptbl/xp/px_CR_SpanChange.h \
	ptbl/xp/px_CR_Span.cpp \
	ptbl/xp/px_CR_Span.h \
	ptbl/xp/px_CR_StruxChange.cpp \
	ptbl/xp/px_CR_StruxChange.h \
	ptbl/xp/px_CR_Strux.cpp \
	ptbl/xp/px_CR_Strux.h \
	ptbl/xp/px_CR_Style.cpp \
	ptbl/xp/px_CR_Style.h \
	ptbl/xp/px_CR_TestRoutines.cpp \
	ptbl/xp/pp_Author.h \
	ptbl/xp/pp_Author.cpp

if ENABLE_SPELL
libtext_la_SOURCES += \
	fmt/xp/fl_Squiggles.cpp
endif

if HAVE_REDLAND
libtext_la_SOURCES += \
	ptbl/xp/pd_RDFSupportRed.h
endif

if TOOLKIT_GTK_ALL
AM_CPPFLAGS += \
	$(IMPEXP_CPPFLAGS)

libtext_la_SOURCES += \
	fmt/gtk/fv_UnixFrameEdit.cpp \
	fmt/gtk/fv_UnixFrameEdit.h \
	fmt/gtk/fv_UnixInlineImage.cpp \
	fmt/gtk/fv_UnixInlineImage.h \
	fmt/gtk/fv_UnixVisualDrag.cpp \
	fmt/gtk/fv_UnixVisualDrag.h \
	fmt/gtk/fv_UnixSelectionHandles.cpp \
	fmt/gtk/fv_UnixSelectionHandles.h \
	fmt/gtk/gtktexthandle.cpp \
	fmt/gtk/gtktexthandleprivate.h
endif

EXTRA_DIST = \
	fmt/xp/t/fl_AutoNum.t.cpp \
	ptbl/xp/t/pf_Fragments.t.cpp \
	ptbl/xp/t/pp_PropertyMap.t.cpp \
	ptbl/xp/t/pt_PieceTable.t.cpp
