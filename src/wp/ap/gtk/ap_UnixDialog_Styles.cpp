/* -*- mode: C++; tab-width: 4; c-basic-offset: 4;  indent-tabs-mode: t -*- */
/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (c) 2009-2023 Hubert Figuière
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#include <stdlib.h>

#include "ut_string.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "xap_UnixDialogHelper.h"

#include "xap_App.h"
#include "xap_UnixApp.h"
#include "xap_Frame.h"

#include "ap_Strings.h"
#include "ap_Dialog_Id.h"
#include "ap_UnixDialog_Styles.h"
#include "fl_DocLayout.h"
#include "fl_BlockLayout.h"
#include "fv_View.h"
#include "pd_Style.h"
#include "ut_string_class.h"
#include "pt_PieceTable.h"

#include "gr_UnixCairoGraphics.h"

// define to 0 to popup dialogs on top of each other, 1 to hide them
#define HIDE_MAIN_DIALOG 0

XAP_Dialog * AP_UnixDialog_Styles::static_constructor(XAP_DialogFactory * pFactory,
						      XAP_Dialog_Id id)
{
	AP_UnixDialog_Styles * p = new AP_UnixDialog_Styles(pFactory,id);
	return p;
}

AP_UnixDialog_Styles::AP_UnixDialog_Styles(XAP_DialogFactory * pDlgFactory,
										 XAP_Dialog_Id id)
  : AP_Dialog_Styles(pDlgFactory,id), m_selectedStyle(nullptr), m_whichType(AP_UnixDialog_Styles::USED_STYLES)
{
	m_windowMain = nullptr;

	m_btApply = nullptr;
	m_btClose = nullptr;
	m_wGnomeButtons = nullptr;
	m_wParaPreviewArea = nullptr;
	m_pParaPreviewWidget = nullptr;
	m_wCharPreviewArea = nullptr;
	m_pCharPreviewWidget = nullptr;

	m_listStyles = nullptr;
	m_tvStyles = nullptr;
	m_rbList1 = nullptr;
	m_rbList2 = nullptr;
	m_rbList3 = nullptr;
	m_lbAttributes = nullptr;

	m_wModifyDialog = nullptr;
	m_wStyleNameEntry = nullptr;
	m_wBasedOnCombo = nullptr;
	m_wBasedOnEntry = nullptr;
	m_wFollowingCombo = nullptr;
	m_wFollowingEntry = nullptr;
	m_wStyleTypeCombo = nullptr;
	m_wStyleTypeEntry = nullptr;
	m_wLabDescription = nullptr;

	m_pAbiPreviewWidget = nullptr;
	m_wModifyDrawingArea = nullptr;

	m_wModifyOk = nullptr;
	m_wModifyCancel = nullptr;
	m_wFormatMenu = nullptr;
	m_wModifyShortCutKey = nullptr;

	m_bBlockModifySignal = false;
}

AP_UnixDialog_Styles::~AP_UnixDialog_Styles(void)
{
	DELETEP (m_pParaPreviewWidget);
	DELETEP (m_pCharPreviewWidget);
	DELETEP (m_pAbiPreviewWidget);
}

/*****************************************************************/

static void
s_tvStyles_selection_changed (GtkTreeSelection *selection,
		gpointer d)
{
	AP_UnixDialog_Styles * dlg = static_cast <AP_UnixDialog_Styles *>(d);
	dlg->event_SelectionChanged(selection);
}

static void
s_typeslist_changed (GtkWidget *w, gpointer d)
{
	AP_UnixDialog_Styles * dlg = static_cast <AP_UnixDialog_Styles *>(d);
	dlg->event_ListClicked (gtk_button_get_label (GTK_BUTTON(w)));
}

static void
s_deletebtn_clicked (GtkWidget * /*w*/, gpointer d)
{
	AP_UnixDialog_Styles * dlg = static_cast <AP_UnixDialog_Styles *>(d);
	dlg->event_DeleteClicked ();
}

static void
s_modifybtn_clicked (GtkWidget * /*w*/, gpointer d)
{
	AP_UnixDialog_Styles * dlg = static_cast <AP_UnixDialog_Styles *>(d);
	dlg->event_ModifyClicked ();
}

static void
s_newbtn_clicked (GtkWidget * /*w*/, gpointer d)
{
	AP_UnixDialog_Styles * dlg = static_cast <AP_UnixDialog_Styles *>(d);
	dlg->event_NewClicked ();
}

static void
s_applybtn_clicked (GtkWidget * /*w*/, gpointer d)
{
	AP_UnixDialog_Styles * dlg = static_cast <AP_UnixDialog_Styles *>(d);
	dlg->event_Apply ();
}

static void
s_closebtn_clicked (GtkWidget * /*w*/, gpointer d)
{
	AP_UnixDialog_Styles * dlg = static_cast <AP_UnixDialog_Styles *>(d);
	dlg->event_Close ();
}

static void s_remove_property(GtkWidget * widget, AP_UnixDialog_Styles * me)
{
	UT_UNUSED(widget);
	UT_ASSERT(widget && me);
	me->event_RemoveProperty();
}

static void s_style_name(GtkWidget * widget, AP_UnixDialog_Styles * me)
{
	UT_UNUSED(widget);
	UT_ASSERT(widget && me);
	me->new_styleName();
}


static void s_basedon(GtkWidget * widget, AP_UnixDialog_Styles * me)
{
	UT_UNUSED(widget);
	UT_ASSERT(widget && me);
	if(me->isModifySignalBlocked())
		return;
	me->event_basedOn();
}


static void s_followedby(GtkWidget * widget, AP_UnixDialog_Styles * me)
{
	UT_UNUSED(widget);
	UT_ASSERT(widget && me);
	if(me->isModifySignalBlocked())
		return;
	me->event_followedBy();
}


static void s_styletype(GtkWidget * widget, AP_UnixDialog_Styles * me)
{
	UT_UNUSED(widget);
	UT_ASSERT(widget && me);
	if(me->isModifySignalBlocked())
		return;
	me->event_styleType();
}

static gboolean s_paraPreview_draw(GtkWidget * widget, gpointer /* data */, AP_UnixDialog_Styles * me)
{
	UT_UNUSED(widget);
	UT_ASSERT(widget && me);
	me->event_paraPreviewDraw();
	return FALSE;
}


static gboolean s_charPreview_draw(GtkWidget * widget, gpointer /* data */, AP_UnixDialog_Styles * me)
{
	UT_UNUSED(widget);
	UT_ASSERT(widget && me);
	me->event_charPreviewDraw();
	return FALSE;
}


static gboolean s_modifyPreview_draw(GtkWidget * widget, gpointer /* data */, AP_UnixDialog_Styles * me)
{
	UT_UNUSED(widget);
	UT_ASSERT(widget && me);
	me->event_ModifyPreviewDraw();
	return FALSE;
}

static void s_modify_format_cb(GtkWidget * widget, 
			     AP_UnixDialog_Styles * me)
{
	gint active = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
	if(active) {
		gtk_combo_box_set_active(GTK_COMBO_BOX(widget), 0);
	}
	switch(active) {
	case 1:
		me->event_ModifyParagraph();
		break;
	case 2:
		me->event_ModifyFont();
		break;
	case 3:
		me->event_ModifyTabs();
		break;
	case 4:
		me->event_ModifyNumbering();
		break;
	case 5:
		me->event_ModifyLanguage();
		break;
	default:
		break;
	}
}


/*****************************************************************/

void AP_UnixDialog_Styles::runModal(XAP_Frame * pFrame)
{

//
// Get View and Document pointers. Place them in member variables
//

	setFrame(pFrame);
	setView(static_cast<FV_View *>(pFrame->getCurrentView()));
	UT_ASSERT(getView());

	setDoc(getView()->getLayout()->getDocument());

	UT_ASSERT(getDoc());

	// Build the window's widgets and arrange them
	m_windowMain = _constructWindow();
	UT_ASSERT(m_windowMain);

	abiSetupModalDialog(GTK_DIALOG(m_windowMain), pFrame, this, GTK_RESPONSE_CLOSE);

	// *** this is how we add the gc for the para and char Preview's ***
	// attach a new graphics context to the drawing area

	UT_ASSERT(m_wParaPreviewArea && XAP_HAS_NATIVE_WINDOW(m_wParaPreviewArea));

	// make a new Unix GC
	DELETEP (m_pParaPreviewWidget);
	{
		GR_UnixCairoAllocInfo ai(m_wParaPreviewArea);
		m_pParaPreviewWidget =
		    (GR_CairoGraphics*) XAP_App::getApp()->newGraphics(ai);
	}

	// let the widget materialize

	GtkAllocation allocation;
	gtk_widget_get_allocation(m_wParaPreviewArea, &allocation);
	_createParaPreviewFromGC(m_pParaPreviewWidget,
				 static_cast<UT_uint32>(allocation.width),
				 static_cast<UT_uint32>(allocation.height));


	UT_ASSERT(m_wCharPreviewArea && XAP_HAS_NATIVE_WINDOW(m_wCharPreviewArea));

	// make a new Unix GC
	DELETEP (m_pCharPreviewWidget);
	{
		GR_UnixCairoAllocInfo ai(m_wCharPreviewArea);
		m_pCharPreviewWidget =
		    (GR_CairoGraphics*) XAP_App::getApp()->newGraphics(ai);
	}

	// let the widget materialize

	gtk_widget_get_allocation(m_wCharPreviewArea, &allocation);
	_createCharPreviewFromGC(m_pCharPreviewWidget,
				 static_cast<UT_uint32>(allocation.width),
				 static_cast<UT_uint32>(allocation.height));

	// Populate the window's data items
	_populateWindowData();

	// the expose event of the preview
	g_signal_connect(G_OBJECT(m_wParaPreviewArea),
			 "draw",
			 G_CALLBACK(s_paraPreview_draw),
			 reinterpret_cast<gpointer>(this));

	g_signal_connect(G_OBJECT(m_wCharPreviewArea),
			 "draw",
			 G_CALLBACK(s_charPreview_draw),
			 reinterpret_cast<gpointer>(this));

	// connect the select_row signal to the clist
	g_signal_connect (G_OBJECT (gtk_tree_view_get_selection(GTK_TREE_VIEW(m_tvStyles))), "changed",
			  G_CALLBACK (s_tvStyles_selection_changed), reinterpret_cast<gpointer>(this));

	// main loop for the dialog
	gint response;
	while(true)
	{
		response = abiRunModalDialog(GTK_DIALOG(m_windowMain), false);
		if (response == GTK_RESPONSE_APPLY)
			event_Apply();
		else
		{
			event_Close();
			break; // exit the loop
		}
	}

	DELETEP (m_pParaPreviewWidget);
	DELETEP (m_pCharPreviewWidget);

	abiDestroyWidget(m_windowMain);
}

/*****************************************************************/

void AP_UnixDialog_Styles::event_Apply(void)
{
	// TODO save out state of radio items
	m_answer = AP_Dialog_Styles::a_OK;
	const gchar * szStyle = getCurrentStyle();
	if(szStyle && *szStyle)
	{
		getView()->setStyle(szStyle);
	}
}

void AP_UnixDialog_Styles::event_Close(void)
{
	m_answer = AP_Dialog_Styles::a_CANCEL;
}

void AP_UnixDialog_Styles::event_WindowDelete(void)
{
	m_answer = AP_Dialog_Styles::a_CANCEL;
}

void AP_UnixDialog_Styles::event_paraPreviewDraw(void)
{
	if (m_pParaPreview) {
		m_pParaPreview->drawImmediate();
	}
}


void AP_UnixDialog_Styles::event_charPreviewInvalidate(void)
{
	if (m_pCharPreview) {
		event_charPreviewUpdated();
	}
}

void AP_UnixDialog_Styles::event_charPreviewDraw(void)
{
	if (m_pCharPreview) {
		m_pCharPreview->drawImmediate();
	}
}

void AP_UnixDialog_Styles::event_DeleteClicked(void)
{
	if (m_selectedStyle)
    {
		m_sNewStyleName = "";
		gchar * style = nullptr;

		GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(m_tvStyles));
		GtkTreeIter iter;
		gtk_tree_model_get_iter(model, &iter, m_selectedStyle);
		gtk_tree_model_get(model, &iter, 1, &style, -1);

		if (!style)
			return; // ok, nothing's selected. that's fine

		UT_DEBUGMSG(("DOM: attempting to delete style %s\n", style));

		if (!getDoc()->removeStyle(style)) // actually remove the style
		{
			const XAP_StringSet * pSS = m_pApp->getStringSet();
			std::string s;
			pSS->getValueUTF8 (AP_STRING_ID_DLG_Styles_ErrStyleCantDelete,s);
		
			getFrame()->showMessageBox (s.c_str(),
										XAP_Dialog_MessageBox::b_O,
										XAP_Dialog_MessageBox::a_OK);
			return;
		}

		g_free(style);

		getFrame()->repopulateCombos();
		_populateWindowData(); // force a refresh
		getDoc()->signalListeners(PD_SIGNAL_UPDATE_LAYOUT);
    }
}

void AP_UnixDialog_Styles::event_NewClicked(void)
{
	setIsNew(true);
	modifyRunModal();
	if(m_answer == AP_Dialog_Styles::a_OK)
	{
		m_sNewStyleName = getNewStyleName();
		createNewStyle(m_sNewStyleName.c_str());
		_populateCList();
	}
}

void AP_UnixDialog_Styles::event_SelectionChanged(GtkTreeSelection * selection)
{
	GtkTreeView *tree = gtk_tree_selection_get_tree_view(selection);
	GtkTreeModel *model = gtk_tree_view_get_model(tree);
	GList *list = gtk_tree_selection_get_selected_rows(selection, &model);

	gpointer item = g_list_nth_data(list, 0);
	m_selectedStyle = reinterpret_cast<GtkTreePath *>(item);

	// refresh the previews
	_populatePreviews(false);
}

void AP_UnixDialog_Styles::event_ListClicked(const char * which)
{
	const XAP_StringSet * pSS = m_pApp->getStringSet();
	std::string s;
	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_LBL_InUse, s);

	if (s == which)
	{
		m_whichType = USED_STYLES;
	}
	else
	{
		pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_LBL_UserDefined, s);
		if (s == which)
			m_whichType = USER_STYLES;
		else
			m_whichType = ALL_STYLES;
	}

	// force a refresh of everything
	_populateWindowData();
}

/*****************************************************************/

GtkWidget * AP_UnixDialog_Styles::_constructWindow(void)
{
	const XAP_StringSet * pSS = m_pApp->getStringSet();

	// load the dialog from the UI file
	GtkBuilder* builder = newDialogBuilderFromResource("ap_UnixDialog_Styles.ui");

	GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "ap_UnixDialog_Styles"));
	std::string s;
	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_StylesTitle, s);
	gtk_window_set_title (GTK_WINDOW (window), s.c_str());

	// list of styles goes in the top left
	localizeLabelMarkup(GTK_WIDGET(gtk_builder_get_object(builder, "lbStyles")), pSS, AP_STRING_ID_DLG_Styles_Available);
	
	// treeview
	m_tvStyles = GTK_WIDGET(gtk_builder_get_object(builder, "tvStyles"));
	gtk_tree_selection_set_mode (gtk_tree_view_get_selection (GTK_TREE_VIEW (m_tvStyles)), GTK_SELECTION_SINGLE);

	localizeLabelMarkup(GTK_WIDGET(gtk_builder_get_object(builder, "lbList")), pSS, AP_STRING_ID_DLG_Styles_List);

	m_rbList1 = GTK_WIDGET(gtk_builder_get_object(builder, "rbList1"));
	localizeButton(m_rbList1, pSS, AP_STRING_ID_DLG_Styles_LBL_InUse);
	m_rbList2 = GTK_WIDGET(gtk_builder_get_object(builder, "rbList2"));
	localizeButton(m_rbList2, pSS, AP_STRING_ID_DLG_Styles_LBL_All);
	m_rbList3 = GTK_WIDGET(gtk_builder_get_object(builder, "rbList3"));
	localizeButton(m_rbList3, pSS, AP_STRING_ID_DLG_Styles_LBL_UserDefined);
	
	// previewing and description goes in the top right

	localizeLabelMarkup(GTK_WIDGET(gtk_builder_get_object(builder, "lbParagraph")), pSS, AP_STRING_ID_DLG_Styles_ParaPrev);
	GtkWidget *frameParaPrev = GTK_WIDGET(gtk_builder_get_object(builder, "frameParagraph"));
	m_wParaPreviewArea = gtk_drawing_area_new();
	gtk_widget_set_size_request(m_wParaPreviewArea, 300, 70);
	gtk_container_add(GTK_CONTAINER(frameParaPrev), m_wParaPreviewArea);
	gtk_widget_show(m_wParaPreviewArea);

	localizeLabelMarkup(GTK_WIDGET(gtk_builder_get_object(builder, "lbCharacter")), pSS, AP_STRING_ID_DLG_Styles_CharPrev);
	GtkWidget *frameCharPrev = GTK_WIDGET(gtk_builder_get_object(builder, "frameCharacter"));
	m_wCharPreviewArea = gtk_drawing_area_new();
	gtk_widget_set_size_request(m_wCharPreviewArea, 300, 50);
	gtk_container_add(GTK_CONTAINER(frameCharPrev), m_wCharPreviewArea);
	gtk_widget_show(m_wCharPreviewArea);

	localizeLabelMarkup(GTK_WIDGET(gtk_builder_get_object(builder, "lbDescription")), pSS, AP_STRING_ID_DLG_Styles_Description);
	m_lbAttributes = GTK_WIDGET(gtk_builder_get_object(builder, "lbAttributes"));

	// Pack buttons at the bottom of the dialog
	m_btNew = GTK_WIDGET(gtk_builder_get_object(builder, "btNew"));
	m_btDelete = GTK_WIDGET(gtk_builder_get_object(builder, "btDelete"));
	m_btModify = GTK_WIDGET(gtk_builder_get_object(builder, "btModify"));
	localizeButton(m_btModify, pSS, AP_STRING_ID_DLG_Styles_Modify);

	m_btApply = GTK_WIDGET(gtk_builder_get_object(builder, "btApply"));
	m_btClose = GTK_WIDGET(gtk_builder_get_object(builder, "btClose"));

	_connectSignals();

	g_object_unref(G_OBJECT(builder));
	return window;
}

void AP_UnixDialog_Styles::_connectSignals(void) const
{
	// connect signal for this list
	g_signal_connect (G_OBJECT(GTK_BUTTON(m_rbList1)), 
			  "clicked",
			  G_CALLBACK(s_typeslist_changed),
			  (void*)reinterpret_cast<gconstpointer>(this));
	
	g_signal_connect (G_OBJECT(GTK_BUTTON(m_rbList2)), 
			  "clicked",
			  G_CALLBACK(s_typeslist_changed),
			  (void*)reinterpret_cast<gconstpointer>(this));
	
	g_signal_connect (G_OBJECT(GTK_BUTTON(m_rbList3)), 
			  "clicked",
			  G_CALLBACK(s_typeslist_changed),
			  (void*)reinterpret_cast<gconstpointer>(this));
	
	/*
	g_signal_connect (G_OBJECT(GTK_COMBO(m_cbList)->entry), 
			  "changed",
			  G_CALLBACK(s_typeslist_changed),
			  (void*)reinterpret_cast<gconstpointer>(this));
	*/

	// connect signals for these 3 buttons
	g_signal_connect (G_OBJECT(m_btNew),
			  "clicked",
			  G_CALLBACK(s_newbtn_clicked),
			  (void*)reinterpret_cast<gconstpointer>(this));
	
	g_signal_connect (G_OBJECT(m_btModify),
			  "clicked",
			  G_CALLBACK(s_modifybtn_clicked),
			  (void*)reinterpret_cast<gconstpointer>(this));
	
	g_signal_connect (G_OBJECT(m_btDelete),
			  "clicked",
			  G_CALLBACK(s_deletebtn_clicked),
			  (void*)reinterpret_cast<gconstpointer>(this));
	
	// dialog buttons
	g_signal_connect (G_OBJECT(m_btApply),
			  "clicked",
			  G_CALLBACK(s_applybtn_clicked),
			  (void*)reinterpret_cast<gconstpointer>(this));

	g_signal_connect (G_OBJECT(m_btClose),
			  "clicked",
			  G_CALLBACK(s_closebtn_clicked),
			  (void*)reinterpret_cast<gconstpointer>(this));
}

void AP_UnixDialog_Styles::_populateCList(void)
{
	const PD_Style * pStyle;
	const gchar *org_name;

	size_t nStyles = getDoc()->getStyleCount();
	xxx_UT_DEBUGMSG(("DOM: we have %d styles\n", nStyles));
	
	if (m_listStyles == nullptr) {
		m_listStyles = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
		GtkTreeModel *sort = gtk_tree_model_sort_new_with_model (GTK_TREE_MODEL (m_listStyles));
		gtk_tree_sortable_set_sort_column_id (GTK_TREE_SORTABLE (sort), 0, GTK_SORT_ASCENDING);
		gtk_tree_view_set_model (GTK_TREE_VIEW(m_tvStyles), sort);
		g_object_unref (G_OBJECT (sort));
		g_object_unref (G_OBJECT (m_listStyles));
	} else {
		gtk_list_store_clear (m_listStyles);
	}
	
	GtkTreeViewColumn *column = gtk_tree_view_get_column (GTK_TREE_VIEW(m_tvStyles), 0);
	if (!column) 
	{
		column = gtk_tree_view_column_new_with_attributes ("Style", gtk_cell_renderer_text_new (), "text", 0, nullptr);
		gtk_tree_view_append_column(GTK_TREE_VIEW(m_tvStyles), column);
	}

	GtkTreeIter iter, pHighlightIter;
	bool highlight = false;
	UT_GenericVector<PD_Style*> *pStyles = nullptr;
	getDoc()->enumStyles(pStyles);
	for (UT_uint32 i = 0; i < nStyles; i++)
	{
		pStyle = pStyles->getNthItem(i);

		// style has been deleted probably
		if (!pStyle)
			continue;

		org_name = pStyle->getName();

		std::string sLoc;
		pt_PieceTable::s_getLocalisedStyleName(org_name, sLoc);

		if ((m_whichType == ALL_STYLES) ||
			(m_whichType == USED_STYLES && pStyle->isUsed()) ||
			(m_whichType == USER_STYLES && pStyle->isUserDefined()) ||
			(m_sNewStyleName == sLoc)) /* show newly created style anyways */
		{
			gtk_list_store_append(m_listStyles, &iter);
			gtk_list_store_set(m_listStyles, &iter, 0, sLoc.c_str(),
					   1, org_name, -1);

			if (m_sNewStyleName == sLoc) {
				pHighlightIter = iter;
				highlight = true;
			}
		}
	}
	DELETEP(pStyles);

	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_tvStyles));
	if (highlight) {
		// select new/modified
		GtkTreeModel *sort = gtk_tree_view_get_model(GTK_TREE_VIEW(m_tvStyles));
		gtk_tree_model_sort_convert_child_iter_to_iter(GTK_TREE_MODEL_SORT(sort), &iter, &pHighlightIter);
		gtk_tree_selection_select_iter(selection, &iter);
		GtkTreePath *path = gtk_tree_model_get_path(sort, &iter); 
		gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(m_tvStyles), path, nullptr, FALSE, 0, 0);
		gtk_tree_path_free(path);
	}
	else {
		// select first
		GtkTreePath *path = gtk_tree_path_new_from_string("0");
		gtk_tree_selection_select_path(selection, path);
		gtk_tree_path_free(path);
	}
	
	// selection "changed" doesn't fire here, so hack manually
	s_tvStyles_selection_changed (selection, (gpointer)(this));
}

void AP_UnixDialog_Styles::_populateWindowData(void)
{
	_populateCList();
	_populatePreviews(false);
}

void AP_UnixDialog_Styles::setDescription(const char * desc) const
{
	UT_ASSERT(m_lbAttributes);
	gtk_label_set_text (GTK_LABEL(m_lbAttributes), desc);
}

const char * AP_UnixDialog_Styles::getCurrentStyle (void) const
{
	static std::string sStyleBuf;

	UT_ASSERT(m_tvStyles);

	if (!m_selectedStyle)
		return nullptr;

	gchar * style = nullptr;

	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(m_tvStyles));
	GtkTreeIter iter;
	gtk_tree_model_get_iter(model, &iter, m_selectedStyle);
	gtk_tree_model_get(model, &iter, 1, &style, -1);

	if (!style)
		return nullptr;

	sStyleBuf = style;
	g_free(style);
	return sStyleBuf.c_str();
}

GtkWidget *  AP_UnixDialog_Styles::_constructModifyDialog(void)
{
	GtkWidget *modifyDialog;
	const XAP_StringSet * pSS = m_pApp->getStringSet();

	std::string title;

	if(!isNew())
		pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyTitle,title);
	else
		pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_NewTitle,title);

	modifyDialog = abiDialogNew("modify style dialog", TRUE, title.c_str());
	XAP_gtk_widget_set_margin(modifyDialog, 5);
	gtk_window_set_resizable(GTK_WINDOW(modifyDialog), FALSE);

	_constructModifyDialogContents(gtk_dialog_get_content_area(GTK_DIALOG (modifyDialog)));

	m_wModifyDialog = modifyDialog;

//
// Gnome buttons
//
	_constructGnomeModifyButtons();
//
// Connect signals
//

	_connectModifySignals();
	return modifyDialog;
}

void  AP_UnixDialog_Styles::_constructModifyDialogContents(GtkWidget * container)
{

	GtkWidget *dialog_vbox1 = nullptr;
	GtkWidget *OverallVbox = nullptr;
	GtkWidget *comboTable  = nullptr;
	GtkWidget *nameLabel  = nullptr;
	GtkWidget *basedOnLabel  = nullptr;
	GtkWidget *followingLabel = nullptr;
	GtkWidget *styleTypeLabel = nullptr;
	GtkWidget *styleNameEntry = nullptr;
	GtkWidget *basedOnCombo = nullptr;
	GtkWidget *basedOnEntry = nullptr;
	GtkWidget *followingCombo = nullptr;
	GtkWidget *followingEntry = nullptr;
	GtkWidget *styleTypeCombo = nullptr;
	GtkWidget *styleTypeEntry = nullptr;
	GtkWidget *previewFrame = nullptr;
	GtkWidget *modifyDrawingArea = nullptr;
	GtkWidget *DescriptionText = nullptr;
	GtkWidget *checkBoxRow = nullptr;
	GtkWidget *checkAddTo = nullptr;
	GtkWidget *checkAutoUpdate = nullptr;
	GtkWidget *deletePropCombo = nullptr;
	GtkWidget *deletePropEntry = nullptr;
	GtkWidget *deletePropButton = nullptr;
	const XAP_StringSet * pSS = m_pApp->getStringSet();

	dialog_vbox1 = container;
	gtk_widget_show (dialog_vbox1);

	OverallVbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_show(OverallVbox);
	gtk_box_pack_start(GTK_BOX(dialog_vbox1), OverallVbox, TRUE, TRUE, 0);
	XAP_gtk_widget_set_margin(OverallVbox, 5);

	comboTable = gtk_grid_new ();
	gtk_widget_set_hexpand (comboTable, TRUE);
	gtk_widget_show(comboTable);
	gtk_box_pack_start(GTK_BOX(OverallVbox), comboTable, TRUE, TRUE, 2);
	XAP_gtk_widget_set_margin(comboTable, 2);
	gtk_grid_set_column_spacing(GTK_GRID(comboTable), 2);

	std::string s;
	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyName,s);
	nameLabel = gtk_label_new(s.c_str());
	g_object_set(G_OBJECT(nameLabel),
                                    "xalign", 0.0, "yalign", 0.5,
                                    "justify", GTK_JUSTIFY_LEFT,
                                    "xpad", 2, "ypad", 2, "hexpand", TRUE, nullptr);
	gtk_widget_show (nameLabel);
	gtk_grid_attach(GTK_GRID (comboTable), nameLabel, 0, 0, 1, 1);

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyType,s);
	styleTypeLabel = gtk_label_new(s.c_str());
	g_object_set(G_OBJECT(styleTypeLabel),
                                        "xalign", 0.0, "yalign", 0.5,
                                        "justify", GTK_JUSTIFY_LEFT,
                                        "xpad", 2, "ypad", 2, "hexpand", TRUE, nullptr);
	gtk_widget_show (styleTypeLabel);
	gtk_grid_attach (GTK_GRID (comboTable), styleTypeLabel, 1, 0, 1, 1);

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyBasedOn,s);
	basedOnLabel = gtk_label_new(s.c_str());
	g_object_set(G_OBJECT(basedOnLabel),
                                       "xalign", 0.0, "yalign", 0.5,
                                       "justify", GTK_JUSTIFY_LEFT,
                                       "xpad", 2, "ypad", 2, nullptr);
	gtk_widget_show (basedOnLabel);
	gtk_grid_attach (GTK_GRID (comboTable), basedOnLabel, 0, 2, 1, 1);

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyFollowing,s);
	followingLabel = gtk_label_new(s.c_str());
	g_object_set(G_OBJECT(followingLabel),
                                         "xalign", 0.0, "yalign", 0.5,
                                         "xpad", 2, "ypad", 2, nullptr);
	gtk_widget_show (followingLabel);
	gtk_grid_attach (GTK_GRID (comboTable), followingLabel, 1, 2, 1, 1);

	styleNameEntry = gtk_entry_new ();
	gtk_widget_show (styleNameEntry);
	gtk_grid_attach (GTK_GRID (comboTable), styleNameEntry, 0, 1, 1, 1);
	gtk_widget_set_size_request (styleNameEntry, 158, -1);

	basedOnCombo = gtk_combo_box_text_new_with_entry ();
	gtk_widget_show (basedOnCombo);
	gtk_grid_attach (GTK_GRID (comboTable), basedOnCombo, 0, 3, 1, 1);
		
	basedOnEntry = gtk_bin_get_child(GTK_BIN(basedOnCombo));
	gtk_widget_show (basedOnEntry);
	gtk_widget_set_size_request (basedOnEntry, 158, -1);

	followingCombo = gtk_combo_box_text_new_with_entry();
	gtk_widget_show(followingCombo);
	gtk_grid_attach(GTK_GRID(comboTable), followingCombo, 1, 3, 1, 1);

	followingEntry = gtk_bin_get_child(GTK_BIN(followingCombo));
	gtk_widget_show (followingEntry);
	gtk_widget_set_size_request (followingEntry, 158, -1);
//
// Cannot modify style type attribute
//	
	if(isNew())
	{
		styleTypeCombo = gtk_combo_box_text_new_with_entry();
		gtk_widget_show (styleTypeCombo);
		gtk_grid_attach (GTK_GRID (comboTable), styleTypeCombo, 1, 1, 1, 1);

		styleTypeEntry = gtk_bin_get_child(GTK_BIN(styleTypeCombo));
		gtk_widget_show (styleTypeEntry);
		gtk_widget_set_size_request (styleTypeEntry, 158, -1);
	}
	else
	{
		styleTypeEntry = gtk_entry_new ();
		gtk_widget_show (styleTypeEntry);
		gtk_grid_attach (GTK_GRID (comboTable), styleTypeEntry, 1, 1, 1, 1);
		gtk_widget_set_size_request (styleTypeEntry, 158, -1);
	}

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyPreview,s);
	s = "<b>" + s + "</b>";
	GtkWidget *lbPrevFrame = gtk_label_new(nullptr);
	gtk_label_set_markup(GTK_LABEL(lbPrevFrame), s.c_str());
	gtk_widget_show(lbPrevFrame);
	previewFrame = gtk_frame_new(nullptr);
	gtk_frame_set_label_widget(GTK_FRAME(previewFrame), lbPrevFrame);
	gtk_frame_set_shadow_type(GTK_FRAME(previewFrame), GTK_SHADOW_NONE);
	gtk_widget_show (previewFrame);
	gtk_box_pack_start (GTK_BOX (OverallVbox), previewFrame, TRUE, TRUE, 0);
	XAP_gtk_widget_set_margin(previewFrame, 3);

	GtkWidget *wDrawFrame = gtk_frame_new(nullptr);
	gtk_frame_set_shadow_type(GTK_FRAME(wDrawFrame), GTK_SHADOW_NONE);
	gtk_widget_show(wDrawFrame);
	gtk_container_add(GTK_CONTAINER(previewFrame), wDrawFrame);
	XAP_gtk_widget_set_margin(wDrawFrame, 6);

	modifyDrawingArea = gtk_drawing_area_new();
	gtk_widget_set_size_request (modifyDrawingArea, -1, 85);
	gtk_container_add (GTK_CONTAINER (wDrawFrame), modifyDrawingArea);
	gtk_widget_show (modifyDrawingArea);

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyDescription,s);
	s = "<b>" + s + "</b>";
	GtkWidget *lbDescrFrame = gtk_label_new(nullptr);
	gtk_label_set_markup(GTK_LABEL(lbDescrFrame), s.c_str());
	gtk_widget_show(lbDescrFrame);
	GtkWidget *descriptionFrame = gtk_frame_new(nullptr);
	g_object_set(G_OBJECT(descriptionFrame),
			   "label-widget", lbDescrFrame,
			   "shadow-type", GTK_SHADOW_NONE,
			   "border-width", 5, nullptr);
	gtk_widget_show (descriptionFrame);
	gtk_box_pack_start (GTK_BOX (OverallVbox), descriptionFrame, FALSE, FALSE, 0);

	DescriptionText = gtk_label_new(nullptr);
	g_object_set(G_OBJECT(DescriptionText),
					  "xpad", 0, "ypad", 6,
					  "wrap", TRUE,
					  "max-width-chars", 64,
					  nullptr);
	gtk_widget_show (DescriptionText);
	gtk_container_add (GTK_CONTAINER (descriptionFrame), DescriptionText);
	gtk_widget_set_size_request(DescriptionText, 438, -1);
//
// Code to choose properties to be removed from the current style.
//
	GtkWidget * deleteRow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_widget_show (deleteRow);
	gtk_box_pack_start (GTK_BOX (OverallVbox), deleteRow, TRUE, TRUE, 0);
	XAP_gtk_widget_set_margin(deleteRow, 2);

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_RemoveLab,s);
	GtkWidget * deleteLabel = gtk_label_new(s.c_str());
	gtk_widget_show (deleteLabel);
	gtk_box_pack_start (GTK_BOX (deleteRow), deleteLabel, TRUE, TRUE, 0);

	GtkListStore * store = gtk_list_store_new(1, G_TYPE_STRING);
	deletePropCombo = gtk_combo_box_new_with_model_and_entry(GTK_TREE_MODEL(store));
	gtk_combo_box_set_entry_text_column(GTK_COMBO_BOX(deletePropCombo), 0);
	gtk_widget_show (deletePropCombo);
	gtk_box_pack_start (GTK_BOX (deleteRow), deletePropCombo, TRUE, TRUE, 0);

	deletePropEntry = gtk_bin_get_child(GTK_BIN(deletePropCombo));
	gtk_widget_show (deletePropEntry);
	gtk_widget_set_size_request (deletePropEntry, 158, -1);

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_RemoveButton,s);
	deletePropButton = gtk_button_new_with_label(s.c_str());
	gtk_widget_show(deletePropButton);
	gtk_box_pack_start (GTK_BOX (deleteRow), deletePropButton, TRUE, TRUE, 0);

	checkBoxRow = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_box_pack_start (GTK_BOX (OverallVbox), checkBoxRow, TRUE, TRUE, 0);
	XAP_gtk_widget_set_margin(checkBoxRow, 2);

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyTemplate,s);
	checkAddTo = gtk_check_button_new_with_label (s.c_str());
	gtk_widget_show (checkAddTo);
	gtk_box_pack_start (GTK_BOX (checkBoxRow), checkAddTo, TRUE, TRUE, 0);

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyAutomatic,s);
	checkAutoUpdate = gtk_check_button_new_with_label (s.c_str());
	gtk_widget_show (checkAutoUpdate);
	gtk_box_pack_start (GTK_BOX (checkBoxRow), checkAutoUpdate, TRUE, TRUE, 0);

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(OverallVbox), box, TRUE, TRUE, 0);
	gtk_widget_show(box);
	GtkWidget* formatMenu = gtk_combo_box_text_new();
	gtk_widget_show(formatMenu);
	gtk_box_pack_end(GTK_BOX(box), formatMenu, FALSE, FALSE, 0);
	_constructFormatList(formatMenu);

//
// Save widget pointers in member variables
//
	m_wStyleNameEntry = styleNameEntry;
	m_wBasedOnCombo = basedOnCombo;
	m_wBasedOnEntry = basedOnEntry;
	m_wFollowingCombo = followingCombo;
	m_wFollowingEntry = followingEntry;
	m_wStyleTypeCombo = styleTypeCombo;
	m_wStyleTypeEntry = styleTypeEntry;
	m_wModifyDrawingArea = modifyDrawingArea;
	m_wLabDescription = DescriptionText;
	m_wDeletePropCombo = deletePropCombo;
	m_wDeletePropEntry = deletePropEntry;
	m_wDeletePropButton = deletePropButton;
	m_wFormatMenu = formatMenu;
}

void   AP_UnixDialog_Styles::_constructGnomeModifyButtons()
{
	GtkWidget *buttonOK;
	GtkWidget *cancelButton;
	GtkWidget *shortCutButton = nullptr;
	const XAP_StringSet * pSS = m_pApp->getStringSet();

	cancelButton = abiAddButton(GTK_DIALOG(m_wModifyDialog),
                                    pSS->getValue(XAP_STRING_ID_DLG_Cancel),
                                    BUTTON_MODIFY_CANCEL);
	buttonOK = abiAddButton(GTK_DIALOG(m_wModifyDialog),
                                pSS->getValue(XAP_STRING_ID_DLG_OK),
                                BUTTON_MODIFY_OK);

#if 0
	shortCutButton = gtk_button_new_with_label (pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyShortCut).c_str());
	gtk_widget_show (shortCutButton);
	gtk_widget_set_sensitive ( shortCutButton, FALSE );
	gtk_box_pack_start (GTK_BOX (bottomButtons), shortCutButton, TRUE, TRUE, 0);
#endif

	m_wModifyOk = buttonOK;
	m_wModifyCancel = cancelButton;
	m_wModifyShortCutKey = shortCutButton;
}


void  AP_UnixDialog_Styles::_constructFormatList(GtkWidget * FormatCombo)
{
	GtkComboBoxText *combo = GTK_COMBO_BOX_TEXT(FormatCombo);
	const XAP_StringSet * pSS = m_pApp->getStringSet();
	std::string s;
	
	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyFormat,s);
	gtk_combo_box_text_append_text(combo, s.c_str());

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyParagraph,s);
	gtk_combo_box_text_append_text(combo, s.c_str());

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyFont,s);
	gtk_combo_box_text_append_text(combo, s.c_str());

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyTabs,s);
	gtk_combo_box_text_append_text(combo, s.c_str());

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyNumbering,s);
	gtk_combo_box_text_append_text(combo, s.c_str());

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyLanguage,s);
	gtk_combo_box_text_append_text(combo, s.c_str());
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
}


void AP_UnixDialog_Styles::_connectModifySignals(void)
{
	g_signal_connect(G_OBJECT(m_wFormatMenu),
					   "changed",
					   G_CALLBACK(s_modify_format_cb),
					   reinterpret_cast<gpointer>(this));

	g_signal_connect(G_OBJECT(m_wModifyDrawingArea),
			 "draw",
			 G_CALLBACK(s_modifyPreview_draw),
			 reinterpret_cast<gpointer>(this));

	g_signal_connect(G_OBJECT(m_wDeletePropButton),
					   "clicked",
					   G_CALLBACK(s_remove_property),
					   static_cast<gpointer>(this));

	g_signal_connect(G_OBJECT(m_wStyleNameEntry),
					   "changed",
					   G_CALLBACK(s_style_name),
					   static_cast<gpointer>(this));

	g_signal_connect(G_OBJECT(m_wBasedOnEntry),
					   "changed",
					   G_CALLBACK(s_basedon),
					   static_cast<gpointer>(this));

	g_signal_connect(G_OBJECT(m_wFollowingEntry),
					   "changed",
					   G_CALLBACK(s_followedby),
					   static_cast<gpointer>(this));

	g_signal_connect(G_OBJECT(m_wStyleTypeEntry),
					   "changed",
					   G_CALLBACK(s_styletype),
					   static_cast<gpointer>(this));
}


bool AP_UnixDialog_Styles::event_Modify_OK(void)
{
  const char * text = XAP_gtk_entry_get_text (GTK_ENTRY (m_wStyleNameEntry));

  if (!text || !strlen (text))
    {
      // error message!
      const XAP_StringSet * pSS = m_pApp->getStringSet ();
      std::string s;
      pSS->getValueUTF8 (AP_STRING_ID_DLG_Styles_ErrBlankName,s);

      getFrame()->showMessageBox (s.c_str(),
				  XAP_Dialog_MessageBox::b_O,
				  XAP_Dialog_MessageBox::a_OK);

      return false;
    }

	// TODO save out state of radio items
	m_answer = AP_Dialog_Styles::a_OK;
	return true;
}

/*!
 * fill the properties vector with the values the given style.
 */
void AP_UnixDialog_Styles::new_styleName(void)
{
	static char message[200];
	const XAP_StringSet * pSS = m_pApp->getStringSet();
	const gchar * psz = XAP_gtk_entry_get_text( GTK_ENTRY( m_wStyleNameEntry));
	std::string s;
	std::string s1;
	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_DefNone,s);

	if(psz && strcmp(psz,s.c_str())== 0)
	{
		// TODO: do a real error dialog
		pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ErrNotTitle1,s);
		pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ErrNotTitle2,s1);
		sprintf(message,"%s%s%s",s.c_str(),psz,s1.c_str());
		messageBoxOK(static_cast<const char *>(message));
		return;
	}

	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_DefCurrent,s);
	if(psz && strcmp(psz,s.c_str())== 0)
	{
		// TODO: do a real error dialog
		pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ErrNotTitle1,s);
		pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ErrNotTitle2,s1);
		sprintf(message,"%s%s%s",s.c_str(),psz,s1.c_str());
		messageBoxOK(static_cast<const char *>(message));
		return;
	}

	g_snprintf(static_cast<gchar *>(m_newStyleName),40,"%s",psz);
	PP_addOrSetAttribute(PT_NAME_ATTRIBUTE_NAME, getNewStyleName(), m_vecAllAttribs);
}

/*!
 * Remove the property from the current style shown in the remove combo box
 */
void AP_UnixDialog_Styles::event_RemoveProperty(void)
{
	const gchar * psz = XAP_gtk_entry_get_text( GTK_ENTRY(m_wDeletePropEntry));
	PP_removeAttribute(psz, m_vecAllProps);
	rebuildDeleteProps();
	updateCurrentStyle();
}

void AP_UnixDialog_Styles::rebuildDeleteProps(void)
{
	GtkComboBox* delCombo = GTK_COMBO_BOX(m_wDeletePropCombo);
	GtkListStore *model = GTK_LIST_STORE(gtk_combo_box_get_model(delCombo));

	gtk_list_store_clear(model);

	UT_sint32 i= 0;
	for(auto iter = m_vecAllProps.cbegin(); iter != m_vecAllProps.cend();
		++iter, ++i) {

		if ((i % 2) == 0) {
			GtkTreeIter gtkiter;
			gtk_list_store_append(model, &gtkiter);
			gtk_list_store_set(model, &gtkiter, 0, iter->c_str(), -1);
		}
	}
}

/*!
 * Update the properties and Attributes vector given the new basedon name
 */
void AP_UnixDialog_Styles::event_basedOn(void)
{
	const XAP_StringSet *pSS = m_pApp->getStringSet();
	const gchar * psz = XAP_gtk_entry_get_text( GTK_ENTRY( m_wBasedOnEntry));
	if (strcmp(psz, pSS->getValue(AP_STRING_ID_DLG_Styles_DefNone)) == 0)
		psz = "None";
	else
		psz = pt_PieceTable::s_getUnlocalisedStyleName(psz);
	g_snprintf(static_cast<gchar *>(m_basedonName),40,"%s",psz);
	PP_addOrSetAttribute("basedon", getBasedonName(), m_vecAllAttribs);
	updateCurrentStyle();
}


/*!
 * Update the Attributes vector given the new followedby name
 */
void AP_UnixDialog_Styles::event_followedBy(void)
{
	const XAP_StringSet *pSS = m_pApp->getStringSet();
	const gchar * psz = XAP_gtk_entry_get_text( GTK_ENTRY(m_wFollowingEntry));
	if (strcmp(psz, pSS->getValue(AP_STRING_ID_DLG_Styles_DefCurrent)) == 0)
		psz = "Current Settings";
	else
		psz = pt_PieceTable::s_getUnlocalisedStyleName(psz);
	g_snprintf(static_cast<gchar *>(m_followedbyName),40,"%s",psz);
	PP_addOrSetAttribute("followedby",getFollowedbyName(), m_vecAllAttribs);
}


/*!
 * Update the Attributes vector given the new Style Type
 */
void AP_UnixDialog_Styles::event_styleType(void)
{
	const XAP_StringSet * pSS = m_pApp->getStringSet();
	std::string s;

	const gchar * psz = XAP_gtk_entry_get_text( GTK_ENTRY(m_wStyleTypeEntry));
	g_snprintf(static_cast<gchar *>(m_styleType),40,"%s",psz);
	const gchar * pszSt = "P";
	pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyCharacter,s);
	if (strstr(m_styleType, s.c_str()) != nullptr)
		pszSt = "C";
	PP_addOrSetAttribute("type", pszSt, m_vecAllAttribs);
}

void AP_UnixDialog_Styles::event_Modify_Cancel(void)
{
	m_answer = AP_Dialog_Styles::a_CANCEL;
}

void AP_UnixDialog_Styles::event_ModifyDelete(void)
{
	m_answer = AP_Dialog_Styles::a_CANCEL;
}

void  AP_UnixDialog_Styles::modifyRunModal(void)
{
//
// OK Construct the new dialog and make it modal.
//
//
// pointer to the widget is stored in m_wModifyDialog
//
// Center our new dialog in its parent and make it a transient

	_constructModifyDialog();

//
// populate the dialog with useful info
//
	if(!_populateModify())
	{
		abiDestroyWidget(m_wModifyDialog);
		return;
	}

        abiSetupModalDialog(GTK_DIALOG(m_wModifyDialog), getFrame(), this, BUTTON_MODIFY_CANCEL);

	// make a new Unix GC

	DELETEP (m_pAbiPreviewWidget);
	GR_UnixCairoAllocInfo ai(m_wModifyDrawingArea);
	m_pAbiPreviewWidget =
	    (GR_CairoGraphics*) XAP_App::getApp()->newGraphics(ai);

	// let the widget materialize

	GtkAllocation allocation;
	gtk_widget_get_allocation(m_wModifyDrawingArea, &allocation);
	_createAbiPreviewFromGC(m_pAbiPreviewWidget,
				static_cast<UT_uint32>(allocation.width),
				static_cast<UT_uint32>(allocation.height));
	_populateAbiPreview(isNew());

	bool inputValid;
	do
	{
		switch(abiRunModalDialog(GTK_DIALOG(m_wModifyDialog), false))
		{
			case BUTTON_MODIFY_OK:
				inputValid = event_Modify_OK();
				break;
			default:
				event_Modify_Cancel();
				inputValid = true;
				break ;
		}
	} while (!inputValid);

	if(m_wModifyDialog && GTK_IS_WIDGET(m_wModifyDialog))
	{
//
// Free the old glists
//
		m_gbasedOnStyles.clear();
		m_gfollowedByStyles.clear();
		m_gStyleType.clear();
		gtk_widget_destroy(m_wModifyDialog); // TOPLEVEL
	}
//
// Have to delete this now since the destructor is not run till later
//
	destroyAbiPreview();
	DELETEP(m_pAbiPreviewWidget);
}

void AP_UnixDialog_Styles::event_ModifyPreviewInvalidate(void)
{
	invalidatePreview();
}

void AP_UnixDialog_Styles::event_ModifyPreviewDraw(void)
{
	if (m_pAbiPreview) {
		m_pAbiPreview->drawImmediate();
	}
}


void AP_UnixDialog_Styles::event_ModifyClicked(void)
{
	PD_Style * pStyle = nullptr;
	const char * szCurrentStyle = getCurrentStyle ();
	m_sNewStyleName = szCurrentStyle ? szCurrentStyle : "";

	if(szCurrentStyle)
		getDoc()->getStyle(szCurrentStyle, &pStyle);

	if (!pStyle)
	{
		// TODO: error message - nothing selected
		return;
	}
//
// Allow built-ins to be modified
//
#if 0
	if (!pStyle->isUserDefined ())
	{
		// can't change builtin, error message
		const XAP_StringSet * pSS = m_pApp->getStringSet();
		std::string s;
		pSS->getValueUTF8 (AP_STRING_ID_DLG_Styles_ErrStyleBuiltin,s);
		const gchar * msg = s.c_str();

		getFrame()->showMessageBox (static_cast<const char *>(msg),
									XAP_Dialog_MessageBox::b_O,
									XAP_Dialog_MessageBox::a_OK);
		return;
	}
#endif

#if HIDE_MAIN_DIALOG
//
// Hide the old window
//
    gtk_widget_hide(m_windowMain);
#endif
//
// fill the data structures needed for the Modify dialog
//
	setIsNew(false);

	modifyRunModal();
	if(m_answer == AP_Dialog_Styles::a_OK)
	{
		applyModifiedStyleToDoc();
		getDoc()->updateDocForStyleChange(getCurrentStyle(),true);
		getDoc()->signalListeners(PD_SIGNAL_UPDATE_LAYOUT);
	}
	else
	{
//
// Do other stuff
//
	}
//
// Restore the values in the main dialog
//

#if HIDE_MAIN_DIALOG
//
// Reveal main window again
//
	gtk_widget_show( m_windowMain);
#endif
}

void  AP_UnixDialog_Styles::setModifyDescription( const char * desc)
{
	UT_ASSERT(m_lbAttributes);
	gtk_label_set_text (GTK_LABEL(m_wLabDescription), desc);
}



static void
setComboContent(GtkComboBoxText * combo, const std::list<std::string> & content)
{
	gtk_combo_box_text_remove_all(combo);
	std::list<std::string>::const_iterator iter(content.begin());
	for(; iter != content.end(); iter++) {
		gtk_combo_box_text_append_text(combo, iter->c_str());
	}
}


bool  AP_UnixDialog_Styles::_populateModify(void)
{
	const XAP_StringSet * pSS = m_pApp->getStringSet();
//
// Don't do any callback while setting up stuff here.
//
	setModifySignalBlocked(true);
	setModifyDescription( m_curStyleDesc.c_str());
//
// Get Style name and put in in the text entry
//
	const char * szCurrentStyle = nullptr;
	std::string s;

	if(!isNew())
	{
		szCurrentStyle= getCurrentStyle();
		if(!szCurrentStyle)
		{
			// TODO: change me to use a real messagebox
			pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ErrNoStyle,s);
			messageBoxOK( s.c_str());
			m_answer = AP_Dialog_Styles::a_CANCEL;
			return false;
		}
		std::string sLoc;
		pt_PieceTable::s_getLocalisedStyleName(getCurrentStyle(), sLoc);
		XAP_gtk_entry_set_text(GTK_ENTRY(m_wStyleNameEntry), sLoc.c_str());
		gtk_editable_set_editable(GTK_EDITABLE(m_wStyleNameEntry),FALSE );
	}
	else
	{
		gtk_editable_set_editable(GTK_EDITABLE(m_wStyleNameEntry),TRUE );
	}
//
// Next interogate the current style and find the based on and followed by
// Styles
//
	const char * szBasedOn = nullptr;
	const char * szFollowedBy = nullptr;
	PD_Style * pBasedOnStyle = nullptr;
	PD_Style * pFollowedByStyle = nullptr;
	if(!isNew())
	{
		PD_Style * pStyle = nullptr;
		if(szCurrentStyle)
			getDoc()->getStyle(szCurrentStyle,&pStyle);
		if(!pStyle)
		{
			// TODO: do a real error dialog
			pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ErrStyleNot,s);
			messageBoxOK( s.c_str());
			m_answer = AP_Dialog_Styles::a_CANCEL;
			return false;
		}
//
// Valid style get the Based On and followed by values
//
	    pBasedOnStyle = pStyle->getBasedOn();
		pFollowedByStyle = pStyle->getFollowedBy();
	}
//
// Next make a glists of all styles and attach them to the BasedOn and FollowedBy
//
	UT_GenericVector<PD_Style*> * pStyles = nullptr;
	getDoc()->enumStyles(pStyles);
	UT_sint32 nStyles = pStyles->getItemCount();
	for (UT_sint32 i = 0; i < nStyles; i++)
	{
		const PD_Style * pcStyle = pStyles->getNthItem(i);
		const char * name = pcStyle->getName();
		std::string sLoc;
		pt_PieceTable::s_getLocalisedStyleName(name, sLoc);
		if(pBasedOnStyle && pcStyle == pBasedOnStyle)
		{
			szBasedOn = name;
		}
		if(pFollowedByStyle && pcStyle == pFollowedByStyle)
			szFollowedBy = name;
		if(szCurrentStyle && strcmp(name,szCurrentStyle) != 0)
			m_gbasedOnStyles.push_back(sLoc);
		else if(szCurrentStyle == nullptr)
			m_gbasedOnStyles.push_back(sLoc);

		m_gfollowedByStyles.push_back(sLoc);
	}
	DELETEP(pStyles);

	m_gfollowedByStyles.sort();
	m_gfollowedByStyles.push_back(pSS->getValue(AP_STRING_ID_DLG_Styles_DefCurrent));
	m_gbasedOnStyles.sort();
	m_gbasedOnStyles.push_back(pSS->getValue(AP_STRING_ID_DLG_Styles_DefNone));
	m_gStyleType.push_back(pSS->getValue(AP_STRING_ID_DLG_Styles_ModifyParagraph));
	m_gStyleType.push_back(pSS->getValue(AP_STRING_ID_DLG_Styles_ModifyCharacter));

//
// Set the popdown list
//
	setComboContent(GTK_COMBO_BOX_TEXT(m_wBasedOnCombo),m_gbasedOnStyles);
	setComboContent(GTK_COMBO_BOX_TEXT(m_wFollowingCombo),m_gfollowedByStyles);
	if(isNew())
	{
		setComboContent(GTK_COMBO_BOX_TEXT(m_wStyleTypeCombo),m_gStyleType);
	}
//
// OK here we set intial values for the basedOn and followedBy
//
	if(!isNew())
	{
		std::string sLoc;

		if(pBasedOnStyle != nullptr)
		{
			pt_PieceTable::s_getLocalisedStyleName(szBasedOn, sLoc);
			XAP_gtk_entry_set_text(GTK_ENTRY(m_wBasedOnEntry), sLoc.c_str());
		}
		else
		{
			pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_DefNone,s);
			XAP_gtk_entry_set_text (GTK_ENTRY(m_wBasedOnEntry), s.c_str());
		}

		if(pFollowedByStyle != nullptr)
		{
			pt_PieceTable::s_getLocalisedStyleName(szFollowedBy, sLoc);
			XAP_gtk_entry_set_text(GTK_ENTRY(m_wFollowingEntry), sLoc.c_str());
		}
		else
		{
			pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_DefCurrent,s);
			XAP_gtk_entry_set_text (GTK_ENTRY(m_wFollowingEntry), s.c_str());
		}

		const std::string & sType = PP_getAttribute(PT_TYPE_ATTRIBUTE_NAME, m_vecAllAttribs);
		if(sType.find("P") != std::string::npos)
		{
			pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyParagraph,s);
			XAP_gtk_entry_set_text (GTK_ENTRY(m_wStyleTypeEntry),s.c_str());
		}
		else
		{
			pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyCharacter,s);
			XAP_gtk_entry_set_text (GTK_ENTRY(m_wStyleTypeEntry),s.c_str());
		}
	}
	else
	{
//
// Hardwire defaults for "new"
//
		pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_DefNone,s);
		XAP_gtk_entry_set_text (GTK_ENTRY(m_wBasedOnEntry), s.c_str());
		pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_DefCurrent,s);
		XAP_gtk_entry_set_text (GTK_ENTRY(m_wFollowingEntry), s.c_str());
		pSS->getValueUTF8(AP_STRING_ID_DLG_Styles_ModifyParagraph,s);
		XAP_gtk_entry_set_text (GTK_ENTRY(m_wStyleTypeEntry),s.c_str());
	}
	gtk_editable_set_editable(GTK_EDITABLE(m_wFollowingEntry),FALSE );
	gtk_editable_set_editable(GTK_EDITABLE(m_wBasedOnEntry),FALSE );
	gtk_editable_set_editable(GTK_EDITABLE(m_wStyleTypeEntry),FALSE );
//
// Set these in our attributes vector
//
	event_basedOn();
	event_followedBy();
	event_styleType();
	if(isNew())
	{
		fillVecFromCurrentPoint();
	}
	else
	{
		fillVecWithProps(szCurrentStyle,true);
	}
//
// Allow callback's now.
//
	setModifySignalBlocked(false);
//
// Now set the list of properties which can be deleted.
//
	rebuildDeleteProps();
	XAP_gtk_entry_set_text(GTK_ENTRY(m_wDeletePropEntry),"");
	return true;
}

void   AP_UnixDialog_Styles::event_ModifyParagraph()
{
#if HIDE_MAIN_DIALOG
//
// Hide this window
//
    gtk_widget_hide(m_wModifyDialog);
#endif

//
// Can do all this in XP land.
//
	ModifyParagraph();
	rebuildDeleteProps();
#if HIDE_MAIN_DIALOG
//
// Restore this window
//
    gtk_widget_show(m_wModifyDialog);
#endif

//
// This applies the changes to current style and displays them
//
	updateCurrentStyle();
}

void   AP_UnixDialog_Styles::event_ModifyFont()
{
#if HIDE_MAIN_DIALOG
//
// Hide this window
//
    gtk_widget_hide(m_wModifyDialog);
#endif

//
// Can do all this in XP land.
//
	ModifyFont();
	rebuildDeleteProps();
#if HIDE_MAIN_DIALOG
//
// Restore this window
//
    gtk_widget_show(m_wModifyDialog);
#endif

//
// This applies the changes to current style and displays them
//
	updateCurrentStyle();
}

void AP_UnixDialog_Styles::event_ModifyLanguage()
{
#if HIDE_MAIN_DIALOG
	gtk_widget_hide (m_wModifyDialog);
#endif

	ModifyLang();
	rebuildDeleteProps();
#if HIDE_MAIN_DIALOG
	gtk_widget_show (m_wModifyDialog);
#endif

	updateCurrentStyle();
}

void   AP_UnixDialog_Styles::event_ModifyNumbering()
{
#if HIDE_MAIN_DIALOG
//
// Hide this window
//
    gtk_widget_hide(m_wModifyDialog);
#endif

//
// Can do all this in XP land.
//
	ModifyLists();
	rebuildDeleteProps();
#if HIDE_MAIN_DIALOG
//
// Restore this window
//
    gtk_widget_show(m_wModifyDialog);
#endif

//
// This applies the changes to current style and displays them
//
	updateCurrentStyle();

}


void   AP_UnixDialog_Styles::event_ModifyTabs()
{
#if HIDE_MAIN_DIALOG
//
// Hide this window
//
    gtk_widget_hide(m_wModifyDialog);
#endif

//
// Can do all this in XP land.
//
	ModifyTabs();
	rebuildDeleteProps();
#if HIDE_MAIN_DIALOG
//
// Restore this window
//
    gtk_widget_show(m_wModifyDialog);
#endif

//
// This applies the changes to current style and displays them
//
	updateCurrentStyle();
}

bool  AP_UnixDialog_Styles::isModifySignalBlocked(void) const
{
	return m_bBlockModifySignal;
}

void  AP_UnixDialog_Styles::setModifySignalBlocked( bool val)
{
	m_bBlockModifySignal = val;
}
