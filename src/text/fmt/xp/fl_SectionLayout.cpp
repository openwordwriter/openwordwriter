/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
 * 02111-1307, USA.
 */

#include <stdlib.h>

#include "ut_types.h"
#include "ut_string.h"

#include "fl_SectionLayout.h"
#include "fl_Layout.h"
#include "fl_DocLayout.h"
#include "fl_BlockLayout.h"
#include "fb_LineBreaker.h"
#include "fp_Page.h"
#include "fp_Line.h"
#include "fp_Column.h"
#include "pd_Document.h"
#include "pp_AttrProp.h"
#include "gr_Graphics.h"
#include "pp_Property.h"
#include "px_CR_Strux.h"
#include "px_CR_StruxChange.h"
#include "fv_View.h"

#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "ut_units.h"

fl_SectionLayout::fl_SectionLayout(FL_DocLayout* pLayout, PL_StruxDocHandle sdh, PT_AttrPropIndex indexAP, UT_uint32 iType)
	: fl_Layout(PTX_Section, sdh)
{
	UT_ASSERT(pLayout);

	m_iType = iType;
	
	m_pLayout = pLayout;
	m_pDoc = pLayout->getDocument();
	m_pLB = NULL;
	m_pFirstBlock = NULL;
	m_pLastBlock = NULL;

	m_pNext = NULL;
	m_pPrev = NULL;

	setAttrPropIndex(indexAP);
}

fl_SectionLayout::~fl_SectionLayout()
{
	if (m_pLB)
	{
		delete m_pLB;
	}
}

const char*	fl_SectionLayout::getAttribute(const XML_Char * pszName) const
{
	const PP_AttrProp * pAP = NULL;
	getAttrProp(&pAP);
	
	const XML_Char* pszAtt = NULL;
	pAP->getAttribute(pszName, pszAtt);

	return pszAtt;
}

void fl_SectionLayout::setNext(fl_SectionLayout* pSL)
{
	m_pNext = pSL;
}

void fl_SectionLayout::setPrev(fl_SectionLayout* pSL)
{
	m_pPrev = pSL;
}

FL_DocLayout* fl_SectionLayout::getDocLayout(void) const
{
	return m_pLayout;
}

fl_BlockLayout * fl_SectionLayout::getFirstBlock(void) const
{
	return m_pFirstBlock;
}

fl_BlockLayout * fl_SectionLayout::getLastBlock(void) const
{
	return m_pLastBlock;
}

fl_BlockLayout * fl_SectionLayout::appendBlock(PL_StruxDocHandle sdh, PT_AttrPropIndex indexAP)
{
	return insertBlock(sdh, m_pLastBlock, indexAP);
}

void fl_SectionLayout::addBlock(fl_BlockLayout* pBL)
{
	if (m_pLastBlock)
	{
		UT_ASSERT(m_pLastBlock->getNext() == NULL);
		
		pBL->setNext(NULL);
		pBL->setPrev(m_pLastBlock);
		m_pLastBlock->setNext(pBL);
		m_pLastBlock = pBL;
	}
	else
	{
		UT_ASSERT(!m_pFirstBlock);
		
		pBL->setNext(NULL);
		pBL->setPrev(NULL);
		m_pFirstBlock = pBL;
		m_pLastBlock = m_pFirstBlock;
	}
}

fl_BlockLayout * fl_SectionLayout::insertBlock(PL_StruxDocHandle sdh, fl_BlockLayout * pPrev, PT_AttrPropIndex indexAP)
{
	fl_BlockLayout*	pBL = new fl_BlockLayout(sdh, _getLineBreaker(), pPrev, this, indexAP);
	if (!pBL)
	{
		return pBL;
	}

	if (!m_pLastBlock)
	{
		UT_ASSERT(!m_pFirstBlock);
		m_pFirstBlock = pBL;
		m_pLastBlock = pBL;
	}
	else if (m_pLastBlock == pPrev)
	{
		m_pLastBlock = pBL;
	}
	else if (!pPrev)
	{
		m_pFirstBlock = pBL;
	}

	return pBL;
}

void fl_SectionLayout::removeBlock(fl_BlockLayout * pBL)
{
	UT_ASSERT(pBL);
	UT_ASSERT(m_pFirstBlock);
	
	if (pBL->getPrev())
	{
		pBL->getPrev()->setNext(pBL->getNext());
	}

	if (pBL->getNext())
	{
		pBL->getNext()->setPrev(pBL->getPrev());
	}
	
	if (pBL == m_pFirstBlock)
	{
		m_pFirstBlock = m_pFirstBlock->getNext();
		if (!m_pFirstBlock)
		{
			m_pLastBlock = NULL;
		}
	}

	if (pBL == m_pLastBlock)
	{
		m_pLastBlock = m_pLastBlock->getPrev();
		if (!m_pLastBlock)
		{
			m_pFirstBlock = NULL;
		}
	}

	pBL->setNext(NULL);
	pBL->setPrev(NULL);
}

fb_LineBreaker * fl_SectionLayout::_getLineBreaker(void)
{
	if (!m_pLB)
	{
		fb_LineBreaker* slb = new fb_LineBreaker();

		m_pLB = slb;
	}

	UT_ASSERT(m_pLB);

	return m_pLB;
}

void fl_SectionLayout::_purgeLayout()
{
	fl_BlockLayout*	pBL = m_pFirstBlock;

	while (pBL)
	{
		fl_BlockLayout* pNuke = pBL;

		pBL = pBL->getNext();

		delete pNuke;
	}

	return;
}

UT_Bool fl_SectionLayout::bl_doclistener_populateSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_Span * pcrs, PT_BlockOffset blockOffset, UT_uint32 len)
{
	return pBL->doclistener_populateSpan(pcrs, blockOffset, len);
}

UT_Bool fl_SectionLayout::bl_doclistener_populateObject(fl_BlockLayout* pBL, PT_BlockOffset blockOffset, const PX_ChangeRecord_Object * pcro)
{
	return pBL->doclistener_populateObject(blockOffset, pcro);
}
	
UT_Bool fl_SectionLayout::bl_doclistener_insertSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_Span * pcrs)
{
	return pBL->doclistener_insertSpan(pcrs);
}

UT_Bool fl_SectionLayout::bl_doclistener_deleteSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_Span * pcrs)
{
	return pBL->doclistener_deleteSpan(pcrs);
}

UT_Bool fl_SectionLayout::bl_doclistener_changeSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_SpanChange * pcrsc)
{
	return pBL->doclistener_changeSpan(pcrsc);
}

UT_Bool fl_SectionLayout::bl_doclistener_deleteStrux(fl_BlockLayout* pBL, const PX_ChangeRecord_Strux * pcrx)
{
	return pBL->doclistener_deleteStrux(pcrx);
}

UT_Bool fl_SectionLayout::bl_doclistener_changeStrux(fl_BlockLayout* pBL, const PX_ChangeRecord_StruxChange * pcrxc)
{
	return pBL->doclistener_changeStrux(pcrxc);
}

UT_Bool fl_SectionLayout::bl_doclistener_insertBlock(fl_BlockLayout* pBL, const PX_ChangeRecord_Strux * pcrx,
									PL_StruxDocHandle sdh,
									PL_ListenerId lid,
									void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
															PL_ListenerId lid,
															PL_StruxFmtHandle sfhNew))
{
	return pBL->doclistener_insertBlock(pcrx, sdh, lid, pfnBindHandles);
}

UT_Bool fl_SectionLayout::bl_doclistener_insertSection(fl_BlockLayout* pBL, const PX_ChangeRecord_Strux * pcrx,
									  PL_StruxDocHandle sdh,
									  PL_ListenerId lid,
									  void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
															  PL_ListenerId lid,
															  PL_StruxFmtHandle sfhNew))
{
	return pBL->doclistener_insertSection(pcrx, sdh, lid, pfnBindHandles);
}

UT_Bool fl_SectionLayout::bl_doclistener_insertObject(fl_BlockLayout* pBL, const PX_ChangeRecord_Object * pcro)
{
	return pBL->doclistener_insertObject(pcro);
}

UT_Bool fl_SectionLayout::bl_doclistener_deleteObject(fl_BlockLayout* pBL, const PX_ChangeRecord_Object * pcro)
{
	return pBL->doclistener_deleteObject(pcro);
}

UT_Bool fl_SectionLayout::bl_doclistener_changeObject(fl_BlockLayout* pBL, const PX_ChangeRecord_ObjectChange * pcroc)
{
	return pBL->doclistener_changeObject(pcroc);
}
	
fl_DocSectionLayout::fl_DocSectionLayout(FL_DocLayout* pLayout, PL_StruxDocHandle sdh, PT_AttrPropIndex indexAP)
	: fl_SectionLayout(pLayout, sdh, indexAP, FL_SECTION_DOC)
{
	m_pFirstColumn = NULL;
	m_pLastColumn = NULL;

	m_pHeaderSL = NULL;
	m_pFooterSL = NULL;
	
	_lookupProperties();
}

fl_DocSectionLayout::~fl_DocSectionLayout()
{
	// NB: be careful about the order of these
	_purgeLayout();

	fp_Column* pCol = m_pFirstColumn;
	while (pCol)
	{
		fp_Column* pNext = pCol->getNext();

		delete pCol;

		pCol = pNext;
	}
}

void fl_DocSectionLayout::setHdrFtr(fl_HdrFtrSectionLayout* pHFSL)
{
	const char* pszID = pHFSL->getAttribute("id");

	const char* pszAtt = NULL;

	pszAtt = getAttribute("header");
	if (0 == UT_stricmp(pszAtt, pszID))
	{
		m_pHeaderSL = pHFSL;
		return;
	}
	pszAtt = getAttribute("footer");
	if (0 == UT_stricmp(pszAtt, pszID))
	{
		m_pFooterSL = pHFSL;
		return;
	}
}

fp_Container* fl_DocSectionLayout::getFirstContainer()
{
	return m_pFirstColumn;
}

fp_Container* fl_DocSectionLayout::getLastContainer()
{
	return m_pLastColumn;
}

fp_Container* fl_DocSectionLayout::getNewContainer(void)
{
	/*
	  This is called to create a new column (or row of same).
	*/
	fp_Page* pPage = NULL;
	fp_Column* pLastColumn = (fp_Column*) getLastContainer();
	fp_Column* pAfterColumn = NULL;
	
	if (pLastColumn)
	{
		fp_Page* pTmpPage = pLastColumn->getPage();
		if (pTmpPage->getNext())
		{
			pPage = pTmpPage->getNext();
		}
		else
		{
			pPage = m_pLayout->addNewPage(this);
		}
	}
	else
	{
		/*
		  We currently have no columns.  Time to create some.
		  If there is a previous section, then we need to
		  start our section right after that one.  If not, then
		  we start our section on the first page.  If there is no
		  first page, then we need to create one.
		*/
		fl_DocSectionLayout* pPrevSL = getPrevDocSection();
		if (pPrevSL)
		{
			fp_Page* pTmpPage = pPrevSL->getLastContainer()->getPage();
			if (m_bForceNewPage)
			{
				if (pTmpPage->getNext())
				{
					pPage = pTmpPage->getNext();
				}
				else
				{
					pPage = m_pLayout->addNewPage(this);
				}
			}
			else
			{
				pPage = pTmpPage;
				pAfterColumn = pPage->getNthColumnLeader(pPage->countColumnLeaders()-1);
			}
		}
		else
		{
			if (m_pLayout->countPages() > 0)
			{
				pPage = m_pLayout->getFirstPage();
			}
			else
			{
				pPage = m_pLayout->addNewPage(this);
			}
		}
	}

	UT_ASSERT(pPage);

	fp_Column* pLeaderColumn = NULL;
	fp_Column* pTail = NULL;
	for (UT_uint32 i=0; i<m_iNumColumns; i++)
	{
		fp_Column* pCol = new fp_Column(this);

		if (pTail)
		{
			pCol->setLeader(pLeaderColumn);
			pTail->setFollower(pCol);
			pTail->setNext(pCol);
			pCol->setPrev(pTail);
			
			pTail = pCol;
		}
		else
		{
			pLeaderColumn = pTail = pCol;
			pLeaderColumn->setLeader(pLeaderColumn);
		}
	}

	fp_Column* pLastNewCol = pLeaderColumn;
	while (pLastNewCol->getFollower())
	{
		pLastNewCol = pLastNewCol->getFollower();
	}

	if (m_pLastColumn)
	{
		UT_ASSERT(m_pFirstColumn);

		m_pLastColumn->setNext(pLeaderColumn);
		pLeaderColumn->setPrev(m_pLastColumn);
	}
	else
	{
		UT_ASSERT(!m_pFirstColumn);
		
		m_pFirstColumn = pLeaderColumn;
	}
	
	m_pLastColumn = pLastNewCol;
	UT_ASSERT(!(m_pLastColumn->getNext()));

	pPage->insertColumnLeader(pLeaderColumn, pAfterColumn);
	
	fp_Column* pTmpCol = pLeaderColumn;
 	while (pTmpCol)
	{
		UT_ASSERT(pTmpCol->getPage());
		
		pTmpCol = pTmpCol->getFollower();
	}

	return pLeaderColumn;
}

void fl_DocSectionLayout::format(void)
{
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		pBL->format();
		pBL = pBL->getNext();
	}

	breakSection();
}

void fl_DocSectionLayout::updateLayout(void)
{
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		if (pBL->needsReformat())
		{
			pBL->format();
		}
		
		pBL = pBL->getNext();
	}

	breakSection();
}

UT_Bool fl_DocSectionLayout::doclistener_changeStrux(const PX_ChangeRecord_StruxChange * pcrxc)
{
	UT_ASSERT(pcrxc->getType()==PX_ChangeRecord::PXT_ChangeStrux);

	setAttrPropIndex(pcrxc->getIndexAP());

	_lookupProperties();

	// clear all the columns
	fp_Column* pCol = m_pFirstColumn;
	while (pCol)
	{
		pCol->clearScreen();

		pCol = pCol->getNext();
	}

	// remove all the columns from their pages
	pCol = m_pFirstColumn;
	while (pCol)
	{
		if (pCol->getLeader() == pCol)
		{
			pCol->getPage()->removeColumnLeader(pCol);
		}

		pCol = pCol->getNext();
	}

	// get rid of all the layout information for every block
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		pBL->collapse();

		pBL = pBL->getNext();
	}

	// delete all our columns
	pCol = m_pFirstColumn;
	while (pCol)
	{
		fp_Column* pNext = pCol->getNext();

		delete pCol;

		pCol = pNext;
	}

	m_pFirstColumn = NULL;
	m_pLastColumn = NULL;

	/*
	  TODO to more closely mirror the architecture we're using for BlockLayout, this code
	  should probably just set a flag, indicating the need to reformat this section.  Then,
	  when it's time to update everything, we'll actually do the format.
	*/
	
	format();
	
	FV_View* pView = m_pLayout->getView();
	if (pView)
	{
		pView->notifyListeners(AV_CHG_TYPING | AV_CHG_FMTSECTION);
	}

	return UT_FALSE;
}

void fl_DocSectionLayout::_lookupProperties(void)
{
	const PP_AttrProp* pSectionAP = NULL;
	
	m_pLayout->getDocument()->getAttrProp(m_apIndex, &pSectionAP);

	/*
	  TODO shouldn't we be using PP_evalProperty like
	  the blockLayout does?

	  Yes, since PP_evalProperty does a fallback to the
	  last-chance defaults, whereas the code below is
	  hard-coding its own defaults.  Bad idea.
	*/
	
	const char* pszNumColumns = NULL;
	pSectionAP->getProperty("columns", pszNumColumns);
	if (pszNumColumns && pszNumColumns[0])
	{
		m_iNumColumns = atoi(pszNumColumns);
	}
	else
	{
		m_iNumColumns = 1;
	}

	const char* pszColumnGap = NULL;
	pSectionAP->getProperty("column-gap", pszColumnGap);
	if (pszColumnGap && pszColumnGap[0])
	{
		m_iColumnGap = m_pLayout->getGraphics()->convertDimension(pszColumnGap);
	}
	else
	{
		m_iColumnGap = m_pLayout->getGraphics()->convertDimension("0.25in");
	}

	const char* pszSpaceAfter = NULL;
	pSectionAP->getProperty("section-space-after", pszSpaceAfter);
	if (pszSpaceAfter && pszSpaceAfter[0])
	{
		m_iSpaceAfter = m_pLayout->getGraphics()->convertDimension(pszSpaceAfter);
	}
	else
	{
		m_iSpaceAfter = m_pLayout->getGraphics()->convertDimension("0in");
	}

	const char* pszLeftMargin = NULL;
	const char* pszTopMargin = NULL;
	const char* pszRightMargin = NULL;
	const char* pszBottomMargin = NULL;
	pSectionAP->getProperty("page-margin-left", pszLeftMargin);
	pSectionAP->getProperty("page-margin-top", pszTopMargin);
	pSectionAP->getProperty("page-margin-right", pszRightMargin);
	pSectionAP->getProperty("page-margin-bottom", pszBottomMargin);

	if (
		pszLeftMargin
		&& pszTopMargin
		&& pszRightMargin
		&& pszBottomMargin
		&& pszLeftMargin[0]
		&& pszTopMargin[0]
		&& pszRightMargin[0]
		&& pszBottomMargin[0]
		)
	{
		m_iLeftMargin = m_pLayout->getGraphics()->convertDimension(pszLeftMargin);
		m_iTopMargin = m_pLayout->getGraphics()->convertDimension(pszTopMargin);
		m_iRightMargin = m_pLayout->getGraphics()->convertDimension(pszRightMargin);
		m_iBottomMargin = m_pLayout->getGraphics()->convertDimension(pszBottomMargin);
	}
	else
	{
		m_iLeftMargin = UT_docUnitsFromPaperUnits(m_pLayout->getGraphics(), 100);
		m_iTopMargin = UT_docUnitsFromPaperUnits(m_pLayout->getGraphics(), 100);
		m_iRightMargin = UT_docUnitsFromPaperUnits(m_pLayout->getGraphics(), 100);
		m_iBottomMargin = UT_docUnitsFromPaperUnits(m_pLayout->getGraphics(), 100);
	}
	
	m_bForceNewPage = UT_FALSE;
}

void fl_DocSectionLayout::deleteEmptyColumns(void)
{
	fp_Column* pCol = m_pFirstColumn;
	while (pCol)
	{
		if (pCol->getLeader() == pCol)
		{
			fp_Column* pCol2 = pCol;
			UT_Bool bAllEmpty = UT_TRUE;
			fp_Column* pLastInGroup = NULL;
			
			while (pCol2)
			{
				if (!pCol2->isEmpty())
				{
					bAllEmpty = UT_FALSE;
				}

				if (!(pCol2->getFollower()))
				{
					pLastInGroup = pCol2;
				}
				
				pCol2 = pCol2->getFollower();
			}

			if (bAllEmpty)
			{
				UT_ASSERT(pLastInGroup);

				pCol->getPage()->removeColumnLeader(pCol);

				if (pCol == m_pFirstColumn)
				{
					m_pFirstColumn = pLastInGroup->getNext();
				}

				if (pLastInGroup == m_pLastColumn)
				{
					m_pLastColumn = pCol->getPrev();
				}
				
				if (pCol->getPrev())
				{
					pCol->getPrev()->setNext(pLastInGroup->getNext());
				}

				if (pLastInGroup->getNext())
				{
					pLastInGroup->getNext()->setPrev(pCol->getPrev());
				}

				fp_Column* pCol3 = pCol;
				pCol = pLastInGroup->getNext();
				while (pCol3)
				{
					fp_Column* pNext = pCol3->getFollower();

					delete pCol3;

					pCol3 = pNext;
				}
			}
			else
			{
				pCol = pLastInGroup->getNext();
			}
		}
		else
		{
			pCol = pCol->getNext();
		}
	}
}

UT_uint32 fl_DocSectionLayout::getNumColumns(void) const
{
	return m_iNumColumns;
}

UT_uint32 fl_DocSectionLayout::getColumnGap(void) const
{
	return m_iColumnGap;
}

fl_DocSectionLayout* fl_DocSectionLayout::getNextDocSection(void) const
{
	UT_ASSERT(getType() == FL_SECTION_DOC);

	return (fl_DocSectionLayout*) getNext();
}

fl_DocSectionLayout* fl_DocSectionLayout::getPrevDocSection(void) const
{
	UT_ASSERT(getType() == FL_SECTION_DOC);

	return (fl_DocSectionLayout*) getPrev();
}

UT_Bool fl_DocSectionLayout::doclistener_deleteStrux(const PX_ChangeRecord_Strux * pcrx)
{
	UT_ASSERT(pcrx->getType()==PX_ChangeRecord::PXT_DeleteStrux);
	UT_ASSERT(pcrx->getStruxType()==PTX_Section);

	fl_DocSectionLayout* pPrevSL = getPrevDocSection();
	if (!pPrevSL)
	{
		// TODO shouldn't this just assert?
		UT_DEBUGMSG(("no prior SectionLayout"));
		return UT_FALSE;
	}
	
	// clear all the columns
	fp_Column* pCol = m_pFirstColumn;
	while (pCol)
	{
		pCol->clearScreen();

		pCol = pCol->getNext();
	}

	// remove all the columns from their pages
	pCol = m_pFirstColumn;
	while (pCol)
	{
		if (pCol->getLeader() == pCol)
		{
			pCol->getPage()->removeColumnLeader(pCol);
		}

		pCol = pCol->getNext();
	}

	// get rid of all the layout information for every block
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		pBL->collapse();

		pBL = pBL->getNext();
	}

	// delete all our columns
	pCol = m_pFirstColumn;
	while (pCol)
	{
		fp_Column* pNext = pCol->getNext();

		delete pCol;

		pCol = pNext;
	}

	m_pFirstColumn = NULL;
	m_pLastColumn = NULL;

	while (m_pFirstBlock)
	{
		pBL = m_pFirstBlock;
		removeBlock(pBL);
		pPrevSL->addBlock(pBL);
	}
	
	pPrevSL->m_pNext = m_pNext;
							
	if (m_pNext)
	{
		m_pNext->setPrev(pPrevSL);
	}

	m_pLayout->removeSection(this);

	pPrevSL->format();
	
	FV_View* pView = m_pLayout->getView();
	if (pView)
	{
		pView->_setPoint(pcrx->getPosition());
	}

	delete this;			// TODO whoa!  this construct is VERY dangerous.
	
	return UT_TRUE;
}

void fl_DocSectionLayout::addOwnedPage(fp_Page* pPage)
{
	// TODO do we really need the vecOwnedPages member?
	
	if (m_pHeaderSL)
	{
		m_pHeaderSL->addPage(pPage);
	}

	if (m_pFooterSL)
	{
		m_pFooterSL->addPage(pPage);
	}
}

void fl_DocSectionLayout::deleteOwnedPage(fp_Page* pPage)
{
	// TODO do we really need the vecOwnedPages member?
	
	if (m_pHeaderSL)
	{
		m_pHeaderSL->deletePage(pPage);
	}

	if (m_pFooterSL)
	{
		m_pFooterSL->deletePage(pPage);
	}
}

UT_sint32 fl_DocSectionLayout::breakSection(void)
{
	fl_BlockLayout* pFirstBlock = getFirstBlock();
	if (!pFirstBlock)
	{
		return 0;
	}

	fp_Line* pCurrentLine = pFirstBlock->getFirstLine();
		
	fp_Column* pCurColumn = (fp_Column*) getFirstContainer();
	UT_ASSERT(pCurColumn);

	while (pCurColumn)
	{
		fp_Line* pFirstLineToKeep = pCurrentLine;
		fp_Line* pLastLineToKeep = NULL;
		fp_Line* pOffendingLine = NULL;
		
		UT_sint32 iMaxColHeight = pCurColumn->getMaxHeight();
		UT_sint32 iWorkingColHeight = 0;

		fp_Line* pCurLine = pFirstLineToKeep;
		while (pCurLine)
		{
			UT_sint32 iLineHeight = pCurLine->getHeight();
			UT_sint32 iLineMarginAfter = pCurLine->getMarginAfter();
			UT_sint32 iTotalLineSpace = iLineHeight + iLineMarginAfter;

			if ((iWorkingColHeight + iTotalLineSpace) > iMaxColHeight)
			{
				pOffendingLine = pCurLine;

				/*
				  We have found the offending line (the first one which won't fit in the
				  column) and we now need to decide whether we can break the column
				  just before it.
				*/

				if (pOffendingLine == pFirstLineToKeep)
				{
					/*
					  Wow!  The very first line in this column won't fit.
					  
					  Big line.  (or maybe a small column)
					  
					  TODO what should we do here?  For now, we force it.
					*/
					pLastLineToKeep = pFirstLineToKeep;
				}
				else
				{
					fl_BlockLayout* pBlock = pOffendingLine->getBlock();
					UT_uint32 iWidows = pBlock->getProp_Widows();
					UT_uint32 iOrphans = pBlock->getProp_Orphans();

					UT_uint32 iNumLinesBeforeOffending = 0;
					UT_uint32 iNumLinesAfterOffending = 0;
					UT_Bool bFoundOffending = UT_FALSE;
					
					fp_Line* pFirstLineInBlock = pBlock->getFirstLine();
					pCurLine = pFirstLineInBlock;
					while (pCurLine)
					{
						if (bFoundOffending)
						{
							iNumLinesAfterOffending++;
						}
						else
						{
							if (pCurLine == pOffendingLine)
							{
								iNumLinesAfterOffending = 1;
								bFoundOffending = UT_TRUE;
							}
							else
							{
								iNumLinesBeforeOffending++;
							}
						}
						
						pCurLine = pCurLine->getNext();
					}

					UT_uint32 iNumLinesInBlock = iNumLinesBeforeOffending + iNumLinesAfterOffending;

					UT_uint32 iNumBlockLinesInThisColumn = 0;
					pCurLine = pOffendingLine->getPrev();
					while (pCurLine)
					{
						iNumBlockLinesInThisColumn++;
						if (pCurLine == pFirstLineToKeep)
						{
							break;
						}

						pCurLine = pCurLine->getPrev();
					}

					if (
						pBlock->getProp_KeepTogether()
						&& (iNumLinesBeforeOffending == iNumBlockLinesInThisColumn)
						&& (pBlock->getFirstLine() != pFirstLineToKeep)
						)
					{
						/*
						  This block wants to be kept all in the same column.
						  Bump the whole block to the next column.
						*/

						/*
						  If the block is simply too big to fit in a
						  single column, then we can spawn an infinite
						  loop by continually punting it to the next
						  one.  So, we assume that if the first line
						  in the block is the first line in this
						  column, we just keep it and cope.  This will
						  be slightly incorrect in cases where pushing
						  it to the next column would allow the block
						  to try to live in a larger column, thus
						  staying all together.
						*/
						
						pLastLineToKeep = pFirstLineInBlock->getPrevLineInSection();
					}
					else if (
						(iNumLinesInBlock < (iWidows + iOrphans))
						&& (iNumLinesBeforeOffending == iNumBlockLinesInThisColumn)
						)
					{
						/*
						  There are not enough lines to divide between the
						  two columns while still satisfying both constraints.
						  Bump the whole block to the next column.
						*/
						
						pLastLineToKeep = pFirstLineInBlock->getPrevLineInSection();
					}
					else if (
						(iNumLinesBeforeOffending < iOrphans)
						&& (iNumLinesBeforeOffending == iNumBlockLinesInThisColumn)
						)
					{
						/*
						  We're leaving too few lines in the current column.
						  Bump the whole block.
						*/

						pLastLineToKeep = pFirstLineInBlock->getPrevLineInSection();
					}
					else if (
						(iNumLinesAfterOffending < iWidows)
						&& ((iWidows - iNumLinesAfterOffending) < iNumBlockLinesInThisColumn)
						)
					{
						/*
						  There aren't going to be enough lines in the next
						  column.  Bump just enough.
						*/

						UT_uint32 iNumLinesNeeded = (iWidows - iNumLinesAfterOffending);
						pLastLineToKeep = pOffendingLine->getPrevLineInSection();
						for (UT_uint32 iBump = 0; iBump < iNumLinesNeeded; iBump++)
						{
							pLastLineToKeep = pLastLineToKeep->getPrevLineInSection();
						}
					}
					else
					{
						pLastLineToKeep = pOffendingLine->getPrevLineInSection();
					}
				}
				break;
			}
			else
			{
				iWorkingColHeight += iTotalLineSpace;
				if (
					pCurLine->containsForcedColumnBreak()
					|| pCurLine->containsForcedPageBreak()
					)
				{
					pLastLineToKeep = pCurLine;
					break;
				}
			}

			pCurLine = pCurLine->getNextLineInSection();
		}

		if (pLastLineToKeep)
		{
			pCurrentLine = pLastLineToKeep->getNextLineInSection();
		}
		else
		{
			pCurrentLine = NULL;
		}
		
		pCurLine = pFirstLineToKeep;
		while (pCurLine)
		{
			if (pCurLine->getContainer() != pCurColumn)
			{
				pCurLine->getContainer()->removeLine(pCurLine);
				pCurColumn->addLine(pCurLine);
			}

			if (pCurLine == pLastLineToKeep)
			{
				break;
			}
			else
			{
				pCurLine = pCurLine->getNextLineInSection();
			}
		}

		fp_Column* pNextColumn = NULL;
		
		if (pLastLineToKeep)
		{
			UT_ASSERT(pLastLineToKeep->getContainer() == pCurColumn);
			
			if (pCurColumn->getLastLine() != pLastLineToKeep)
			{
				// make sure there is a next column
				pNextColumn = pCurColumn->getNext();
				if (!pNextColumn)
				{
					pNextColumn = (fp_Column*) getNewContainer();
				}

				pCurColumn->bumpLines(pLastLineToKeep);
			}
		}

		UT_ASSERT((!pLastLineToKeep) || (pCurColumn->getLastLine() == pLastLineToKeep));
			
		pCurColumn->layout();

		pCurColumn = pCurColumn->getNext();
	}

	return 0; // TODO return code
}

fl_HdrFtrSectionLayout::fl_HdrFtrSectionLayout(FL_DocLayout* pLayout, fl_DocSectionLayout* pDocSL, PL_StruxDocHandle sdh, PT_AttrPropIndex indexAP)
	: fl_SectionLayout(pLayout, sdh, indexAP, FL_SECTION_HDRFTR)
{
	m_pDocSL = pDocSL;
}

fl_HdrFtrSectionLayout::~fl_HdrFtrSectionLayout()
{
	// TODO
}

fp_Container* fl_HdrFtrSectionLayout::getFirstContainer()
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return NULL;
}

fp_Container* fl_HdrFtrSectionLayout::getLastContainer()
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return NULL;
}

fp_Container* fl_HdrFtrSectionLayout::getNewContainer(void)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return NULL;
}

struct _PageHdrFtrShadowPair
{
	fp_Page*			pPage;
	fl_HdrFtrShadow*	pShadow;
};

UT_sint32 fl_HdrFtrSectionLayout::_findShadow(fp_Page* pPage)
{
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		if (pPair->pPage == pPage)
		{
			return iCount;
		}
	}

	return -1;
}

void fl_HdrFtrSectionLayout::addPage(fp_Page* pPage)
{
	UT_ASSERT(0 > _findShadow(pPage));

	struct _PageHdrFtrShadowPair* pPair = new _PageHdrFtrShadowPair;
	// TODO outofmem

	pPair->pPage = pPage;
	pPair->pShadow = new fl_HdrFtrShadow(m_pLayout, pPage, this, m_sdh, m_apIndex);
	
	fl_ShadowListener* pShadowListener = new fl_ShadowListener(this, pPair->pShadow);
	m_pDoc->tellListener(pShadowListener);

	m_vecPages.addItem(pPair);
}

void fl_HdrFtrSectionLayout::deletePage(fp_Page* pPage)
{
	UT_sint32 iShadow = _findShadow(pPage);
	UT_ASSERT(iShadow >= 0);

	struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(iShadow);
	UT_ASSERT(pPair);

	UT_ASSERT(pPair->pShadow);
	delete pPair->pShadow;

	m_vecPages.deleteNthItem(iShadow);
}

void fl_HdrFtrSectionLayout::format(void)
{
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		pPair->pShadow->format();
	}
}

void fl_HdrFtrSectionLayout::updateLayout(void)
{
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		pPair->pShadow->updateLayout();
	}
}

UT_Bool fl_HdrFtrSectionLayout::doclistener_changeStrux(const PX_ChangeRecord_StruxChange * pcrxc)
{
	UT_ASSERT(pcrxc->getType()==PX_ChangeRecord::PXT_ChangeStrux);

	setAttrPropIndex(pcrxc->getIndexAP());

	// TODO what happens here?

	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return UT_FALSE;
}

void fl_HdrFtrSectionLayout::_lookupProperties(void)
{
}

UT_Bool fl_HdrFtrSectionLayout::bl_doclistener_populateSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_Span * pcrs, PT_BlockOffset blockOffset, UT_uint32 len)
{
	UT_Bool bResult = UT_TRUE;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		bResult = pPair->pShadow->bl_doclistener_populateSpan(pBL, pcrs, blockOffset, len)
			&& bResult;
	}

	return bResult;
}

UT_Bool fl_HdrFtrSectionLayout::bl_doclistener_populateObject(fl_BlockLayout* pBL, PT_BlockOffset blockOffset, const PX_ChangeRecord_Object * pcro)
{
	UT_Bool bResult = UT_TRUE;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		bResult = pPair->pShadow->bl_doclistener_populateObject(pBL, blockOffset, pcro)
			&& bResult;
	}

	return bResult;
}
	
UT_Bool fl_HdrFtrSectionLayout::bl_doclistener_insertSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_Span * pcrs)
{
	UT_Bool bResult = UT_TRUE;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		bResult = pPair->pShadow->bl_doclistener_insertSpan(pBL, pcrs)
			&& bResult;
	}

	return bResult;
}

UT_Bool fl_HdrFtrSectionLayout::bl_doclistener_deleteSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_Span * pcrs)
{
	UT_Bool bResult = UT_TRUE;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		bResult = pPair->pShadow->bl_doclistener_deleteSpan(pBL, pcrs)
			&& bResult;
	}

	return bResult;
}

UT_Bool fl_HdrFtrSectionLayout::bl_doclistener_changeSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_SpanChange * pcrsc)
{
	UT_Bool bResult = UT_TRUE;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		bResult = pPair->pShadow->bl_doclistener_changeSpan(pBL, pcrsc)
			&& bResult;
	}

	return bResult;
}

UT_Bool fl_HdrFtrSectionLayout::bl_doclistener_deleteStrux(fl_BlockLayout* pBL, const PX_ChangeRecord_Strux * pcrx)
{
	UT_Bool bResult = UT_TRUE;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		bResult = pPair->pShadow->bl_doclistener_deleteStrux(pBL, pcrx)
			&& bResult;
	}

	return bResult;
}

UT_Bool fl_HdrFtrSectionLayout::bl_doclistener_changeStrux(fl_BlockLayout* pBL, const PX_ChangeRecord_StruxChange * pcrxc)
{
	UT_Bool bResult = UT_TRUE;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		bResult = pPair->pShadow->bl_doclistener_changeStrux(pBL, pcrxc)
			&& bResult;
	}

	return bResult;
}

UT_Bool fl_HdrFtrSectionLayout::bl_doclistener_insertBlock(fl_BlockLayout* pBL, const PX_ChangeRecord_Strux * pcrx,
									PL_StruxDocHandle sdh,
									PL_ListenerId lid,
									void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
															PL_ListenerId lid,
															PL_StruxFmtHandle sfhNew))
{
	UT_Bool bResult = UT_TRUE;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		bResult = pPair->pShadow->bl_doclistener_insertBlock(pBL, pcrx, sdh, lid, pfnBindHandles)
			&& bResult;
	}

	return bResult;
}

UT_Bool fl_HdrFtrSectionLayout::bl_doclistener_insertSection(fl_BlockLayout* pBL, const PX_ChangeRecord_Strux * pcrx,
									  PL_StruxDocHandle sdh,
									  PL_ListenerId lid,
									  void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
															  PL_ListenerId lid,
															  PL_StruxFmtHandle sfhNew))
{
	// TODO this should NEVER happen, right?
	
	UT_Bool bResult = UT_TRUE;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		bResult = pPair->pShadow->bl_doclistener_insertSection(pBL, pcrx, sdh, lid, pfnBindHandles)
			&& bResult;
	}

	return bResult;
}

UT_Bool fl_HdrFtrSectionLayout::bl_doclistener_insertObject(fl_BlockLayout* pBL, const PX_ChangeRecord_Object * pcro)
{
	UT_Bool bResult = UT_TRUE;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		bResult = pPair->pShadow->bl_doclistener_insertObject(pBL, pcro)
			&& bResult;
	}

	return bResult;
}

UT_Bool fl_HdrFtrSectionLayout::bl_doclistener_deleteObject(fl_BlockLayout* pBL, const PX_ChangeRecord_Object * pcro)
{
	UT_Bool bResult = UT_TRUE;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		bResult = pPair->pShadow->bl_doclistener_deleteObject(pBL, pcro)
			&& bResult;
	}

	return bResult;
}

UT_Bool fl_HdrFtrSectionLayout::bl_doclistener_changeObject(fl_BlockLayout* pBL, const PX_ChangeRecord_ObjectChange * pcroc)
{
	UT_Bool bResult = UT_TRUE;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		bResult = pPair->pShadow->bl_doclistener_changeObject(pBL, pcroc)
			&& bResult;
	}

	return bResult;
}
	
fl_HdrFtrShadow::fl_HdrFtrShadow(FL_DocLayout* pLayout, fp_Page* pPage, fl_HdrFtrSectionLayout* pHdrFtrSL, PL_StruxDocHandle sdh, PT_AttrPropIndex indexAP)
	: fl_SectionLayout(pLayout, sdh, indexAP, FL_SECTION_SHADOW)
{
	m_pHdrFtrSL = pHdrFtrSL;
	m_pPage = pPage;
}

fl_HdrFtrShadow::~fl_HdrFtrShadow()
{
	// TODO
}

fp_Container* fl_HdrFtrShadow::getFirstContainer()
{
	UT_ASSERT(UT_TODO);

	return NULL;
}

fp_Container* fl_HdrFtrShadow::getLastContainer()
{
	UT_ASSERT(UT_TODO);

	return NULL;
}

fp_Container* fl_HdrFtrShadow::getNewContainer(void)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return NULL;
}

void fl_HdrFtrShadow::format(void)
{
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		pBL->format();
		pBL = pBL->getNext();
	}
}

void fl_HdrFtrShadow::updateLayout(void)
{
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		if (pBL->needsReformat())
		{
			pBL->format();
		}
		
		pBL = pBL->getNext();
	}
}

UT_Bool fl_HdrFtrShadow::doclistener_changeStrux(const PX_ChangeRecord_StruxChange * pcrxc)
{
	UT_ASSERT(pcrxc->getType()==PX_ChangeRecord::PXT_ChangeStrux);

	setAttrPropIndex(pcrxc->getIndexAP());

	// TODO

	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return UT_FALSE;
}

void fl_HdrFtrShadow::_lookupProperties(void)
{
}

fl_ShadowListener::fl_ShadowListener(fl_HdrFtrSectionLayout* pHFSL, fl_HdrFtrShadow* pShadow)
{
	m_pHFSL = pHFSL;
	m_pShadow = pShadow;
}

fl_ShadowListener::~fl_ShadowListener()
{
}

UT_Bool fl_ShadowListener::populate(PL_StruxFmtHandle sfh,
								 const PX_ChangeRecord * pcr)
{
#if 0	
	UT_ASSERT(m_pLayout);
	UT_DEBUGMSG(("fl_ShadowListener::populate\n"));

	UT_Bool bResult = UT_FALSE;

	switch (pcr->getType())
	{
	case PX_ChangeRecord::PXT_InsertSpan:
		{
			const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

			fl_Layout * pL = (fl_Layout *)sfh;
			UT_ASSERT(pL->getType() == PTX_Block);
			fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			PT_BlockOffset blockOffset = pcrs->getBlockOffset();
			UT_uint32 len = pcrs->getLength();
			fl_SectionLayout* pBLSL = pBL->getSectionLayout();
			bResult = pBLSL->bl_doclistener_populateSpan(pBL, pcrs, blockOffset, len);
			goto finish_up;
		}

	case PX_ChangeRecord::PXT_InsertObject:
		{
			const PX_ChangeRecord_Object * pcro = static_cast<const PX_ChangeRecord_Object *>(pcr);

			fl_Layout * pL = (fl_Layout *)sfh;
			UT_ASSERT(pL->getType() == PTX_Block);
			fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			PT_BlockOffset blockOffset = pcro->getBlockOffset();

			fl_SectionLayout* pBLSL = pBL->getSectionLayout();
			bResult = pBLSL->bl_doclistener_populateObject(pBL, blockOffset,pcro);
			goto finish_up;
		}

	default:
		UT_ASSERT(0);
		return UT_FALSE;
	}

finish_up:
	if (0 == m_iGlobCounter)
	{
#ifndef UPDATE_LAYOUT_ON_SIGNAL
		m_pLayout->updateLayout();
#endif
	}
	
	return bResult;
#else
	return UT_FALSE;
#endif
}

UT_Bool fl_ShadowListener::populateStrux(PL_StruxDocHandle sdh,
									  const PX_ChangeRecord * pcr,
									  PL_StruxFmtHandle * psfh)
{
#if 0
	UT_ASSERT(m_pLayout);
	UT_DEBUGMSG(("fl_ShadowListener::populateStrux\n"));

	UT_ASSERT(pcr->getType() == PX_ChangeRecord::PXT_InsertStrux);
	const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);

	switch (pcrx->getStruxType())
	{
	case PTX_Section:
	{
		PT_AttrPropIndex indexAP = pcr->getIndexAP();
		const PP_AttrProp* pAP = NULL;
			
		if (m_pDoc->getAttrProp(indexAP, &pAP) && pAP)
		{
			const XML_Char* pszSectionType = NULL;
			pAP->getAttribute("type", pszSectionType);
			if (
				!pszSectionType
				|| (0 == UT_stricmp(pszSectionType, "doc"))
				)
			{
				// append a SectionLayout to this DocLayout
				fl_DocSectionLayout* pSL = new fl_DocSectionLayout(m_pLayout, sdh, pcr->getIndexAP());
				if (!pSL)
				{
					UT_DEBUGMSG(("no memory for SectionLayout"));
					return UT_FALSE;
				}
			
				m_pLayout->addSection(pSL);

				*psfh = (PL_StruxFmtHandle)pSL;
				
				m_pCurrentSL = pSL;
			}
			else
			{
				if (0 == UT_stricmp(pszSectionType, "header"))
				{
					UT_ASSERT(UT_TODO);
				}
				else if (0 == UT_stricmp(pszSectionType, "footer"))
				{
					const XML_Char* pszID = NULL;
					pAP->getAttribute("id", pszID);

					fl_DocSectionLayout* pDocSL = m_pLayout->findSectionForHdrFtr(pszID);
					UT_ASSERT(pDocSL);
			
					// append a HdrFtrSectionLayout to this DocLayout
					fl_HdrFtrSectionLayout* pSL = new fl_HdrFtrSectionLayout(m_pLayout, pDocSL, sdh, pcr->getIndexAP());
					if (!pSL)
					{
						UT_DEBUGMSG(("no memory for SectionLayout"));
						return UT_FALSE;
					}
			
					pDocSL->setHdrFtr(pSL);

					*psfh = (PL_StruxFmtHandle)pSL;
					
					m_pCurrentSL = pSL;
				}
				else
				{
					return UT_FALSE;
				}
			}
		}
		else
		{
			// TODO fail?
			return UT_FALSE;
		}
	}
	break;

	case PTX_Block:
	{
		UT_ASSERT(m_pCurrentSL);
		
		// append a new BlockLayout to that SectionLayout
		fl_BlockLayout*	pBL = m_pCurrentSL->appendBlock(sdh, pcr->getIndexAP());
		if (!pBL)
		{
			UT_DEBUGMSG(("no memory for BlockLayout"));
			return UT_FALSE;
		}

		// BUGBUG: this is *not* thread-safe, but should work for now
		if (m_bScreen)
			m_pLayout->queueBlockForSpell(pBL);

		*psfh = (PL_StruxFmtHandle)pBL;
	}
	break;
			
	default:
		UT_ASSERT(0);
		return UT_FALSE;
	}

	if (0 == m_iGlobCounter)
	{
#ifndef UPDATE_LAYOUT_ON_SIGNAL
		m_pLayout->updateLayout();
#endif
	}
	
	return UT_TRUE;
#else
	return UT_FALSE;
#endif	
}

UT_Bool fl_ShadowListener::change(PL_StruxFmtHandle sfh,
							   const PX_ChangeRecord * pcr)
{
#if 0	
	UT_DEBUGMSG(("fl_ShadowListener::change\n"));
	UT_Bool bResult = UT_FALSE;

	switch (pcr->getType())
	{
	case PX_ChangeRecord::PXT_GlobMarker:
	{
		UT_ASSERT(sfh == 0);							// globs are not strux-relative
		const PX_ChangeRecord_Glob * pcrg = static_cast<const PX_ChangeRecord_Glob *> (pcr);
		switch (pcrg->getFlags())
		{
		default:
		case PX_ChangeRecord_Glob::PXF_Null:			// not a valid glob type
			UT_ASSERT(0);
			bResult = UT_FALSE;
			goto finish_up;
				
		case PX_ChangeRecord_Glob::PXF_MultiStepStart:
			m_iGlobCounter++;
			bResult = UT_TRUE;
			goto finish_up;
			
		case PX_ChangeRecord_Glob::PXF_MultiStepEnd:
			m_iGlobCounter--;
			bResult = UT_TRUE;
			goto finish_up;
				
		case PX_ChangeRecord_Glob::PXF_UserAtomicStart:	// TODO decide what (if anything) we need
		case PX_ChangeRecord_Glob::PXF_UserAtomicEnd:	// TODO to do here.
			bResult = UT_TRUE;
			goto finish_up;
		}
	}
			
	case PX_ChangeRecord::PXT_InsertSpan:
	{
		const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		bResult = pBLSL->bl_doclistener_insertSpan(pBL, pcrs);
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_DeleteSpan:
	{
		const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		bResult = pBLSL->bl_doclistener_deleteSpan(pBL, pcrs);
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_ChangeSpan:
	{
		const PX_ChangeRecord_SpanChange * pcrsc = static_cast<const PX_ChangeRecord_SpanChange *>(pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		bResult = pBLSL->bl_doclistener_changeSpan(pBL, pcrsc);
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_TempSpanFmt:
	{
		const PX_ChangeRecord_TempSpanFmt * pcrTSF = static_cast<const PX_ChangeRecord_TempSpanFmt *>(pcr);
		if (pcrTSF->getEnabled())
		{
			/*
			  This is just a temporary change at the insertion 
			  point.  It won't take effect unless something's 
			  typed -- but it will cause the toolbars and etc.
			  to be updated.
			*/

			FV_View* pView = m_pLayout->m_pView;
			if (pView)
			{
				UT_ASSERT(pView->isSelectionEmpty());
				pView->_setPoint(pcrTSF->getPosition());
				pView->_setPointAP(pcrTSF->getIndexAP());
#if 0				
				pView->notifyListeners(AV_CHG_TYPING | AV_CHG_FMTCHAR);
#endif				
			}
		}
		else
		{
			// we have been asked to turn off the temporary change at the
			// insertion point.  we need to update any toolbars.

			FV_View* pView = m_pLayout->m_pView;
			if (pView)
			{
				UT_ASSERT(pView->isSelectionEmpty());
				// TODO decide if we need to call "pView->_setPoint(pcrTSF->getPosition());"
				// TODO and if so, add AV_CHG_TYPING to the following notifyListeners().
				pView->_clearPointAP(UT_FALSE);
#if 0				
				pView->notifyListeners(AV_CHG_FMTCHAR);
#endif				
			}
		}

		bResult = UT_TRUE;
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_DeleteStrux:
	{
		const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);

		switch (pcrx->getStruxType())
		{
		case PTX_Section:
		{
			fl_Layout * pL = (fl_Layout *)sfh;
			UT_ASSERT(pL->getType() == PTX_Section);
			fl_DocSectionLayout * pSL = static_cast<fl_DocSectionLayout *>(pL);
			bResult = pSL->doclistener_deleteStrux(pcrx);
			goto finish_up;
		}
		case PTX_Block:
		{
			fl_Layout * pL = (fl_Layout *)sfh;
			UT_ASSERT(pL->getType() == PTX_Block);
			fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			fl_SectionLayout* pBLSL = pBL->getSectionLayout();
			bResult = pBLSL->bl_doclistener_deleteStrux(pBL, pcrx);
			goto finish_up;
		}

		default:
			UT_ASSERT(0);
			bResult = UT_FALSE;
			goto finish_up;
		}
	}
					
	case PX_ChangeRecord::PXT_ChangeStrux:
	{
		const PX_ChangeRecord_StruxChange * pcrxc = static_cast<const PX_ChangeRecord_StruxChange *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;

		// TODO getOldIndexAP() is only intended for use by the document.
		// TODO this assert is probably wrong. --- BUT EVERYTIME IT HAS
		// TODO GONE OFF, I'VE FOUND A BUG, SO MAYBE WE SHOULD KEEP IT :-)
		UT_ASSERT(pL->getAttrPropIndex() == pcrxc->getOldIndexAP());
		UT_ASSERT(pL->getAttrPropIndex() != pcr->getIndexAP());

		switch (pL->getType())
		{
		case PTX_Section:
		{
			fl_DocSectionLayout* pSL = static_cast<fl_DocSectionLayout*>(pL);
			bResult = pSL->doclistener_changeStrux(pcrxc);
			goto finish_up;
		}
		
		case PTX_Block:
		{
			fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			fl_SectionLayout* pBLSL = pBL->getSectionLayout();
			bResult = pBLSL->bl_doclistener_changeStrux(pBL, pcrxc);
			goto finish_up;
		}
					
		default:
			UT_ASSERT(0);
			bResult = UT_FALSE;
			goto finish_up;
		}
	}

	case PX_ChangeRecord::PXT_InsertStrux:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		bResult = UT_FALSE;
		goto finish_up;

	case PX_ChangeRecord::PXT_InsertObject:
	{
		const PX_ChangeRecord_Object * pcro = static_cast<const PX_ChangeRecord_Object *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		bResult = pBLSL->bl_doclistener_insertObject(pBL, pcro);
		goto finish_up;
	}
	case PX_ChangeRecord::PXT_DeleteObject:
	{
		const PX_ChangeRecord_Object * pcro = static_cast<const PX_ChangeRecord_Object *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		bResult = pBLSL->bl_doclistener_deleteObject(pBL, pcro);
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_ChangeObject:
	{
		const PX_ChangeRecord_ObjectChange * pcroc = static_cast<const PX_ChangeRecord_ObjectChange *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		bResult = pBLSL->bl_doclistener_changeObject(pBL, pcroc);
		goto finish_up;
	}
		
	default:
		UT_ASSERT(0);
		bResult = UT_FALSE;
		goto finish_up;
	}

 finish_up:
	if (0 == m_iGlobCounter)
	{
#ifndef UPDATE_LAYOUT_ON_SIGNAL
		m_pLayout->updateLayout();
#endif
	}
	
	return bResult;
#else
	return UT_FALSE;
#endif	
}

UT_Bool fl_ShadowListener::insertStrux(PL_StruxFmtHandle sfh,
									const PX_ChangeRecord * pcr,
									PL_StruxDocHandle sdh,
									PL_ListenerId lid,
									void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
															PL_ListenerId lid,
															PL_StruxFmtHandle sfhNew))
{
#if 0	
	UT_DEBUGMSG(("fl_ShadowListener::insertStrux\n"));

	UT_ASSERT(pcr->getType() == PX_ChangeRecord::PXT_InsertStrux);
	const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);

	fl_Layout * pL = (fl_Layout *)sfh;
	switch (pL->getType())				// see what the immediately prior strux is.
	{
	case PTX_Section:					// the immediately prior strux is a section.

		switch (pcrx->getStruxType())	// see what we are inserting.
		{
		case PTX_Section:				// we are inserting a section.
			// we are inserting a section immediately after a section (with no
			// interviening block).  this is probably a bug, because there should
			// at least be an empty block between them (so that the user can set
			// the cursor there and start typing, if nothing else).
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			return UT_FALSE;
				
		case PTX_Block:					// we are inserting a block.
			// the immediately prior strux is a section.  this probably cannot
			// happen because the insertion point would have been immediately
			// after the existing first block in the section rather than between
			// the section and block.
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			return UT_FALSE;

		default:						// unknown strux.
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			return UT_FALSE;
		}
		
	case PTX_Block:						// the immediately prior strux is a block.

		switch (pcrx->getStruxType())	// see what we are inserting.
		{
		case PTX_Section:				// we are inserting a section.
		{
			// the immediately prior strux is a block.  everything from this point
			// forward (to the next section) needs to be re-parented to this new
			// section.  we also need to verify that there is a block immediately
			// after this new section -- a section must be followed by a block
			// because a section cannot contain content.

			fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			fl_SectionLayout* pBLSL = pBL->getSectionLayout();
			UT_Bool bResult = pBLSL->bl_doclistener_insertSection(pBL, pcrx,sdh,lid,pfnBindHandles);
			if (0 == m_iGlobCounter)
			{
#ifndef UPDATE_LAYOUT_ON_SIGNAL
				m_pLayout->updateLayout();
#endif
			}
	
			return bResult;
		}
		
		case PTX_Block:					// we are inserting a block.
		{
			// the immediately prior strux is also a block.  insert the new
			// block and split the content between the two blocks.
			fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			fl_SectionLayout* pBLSL = pBL->getSectionLayout();
			UT_Bool bResult = pBLSL->bl_doclistener_insertBlock(pBL, pcrx,sdh,lid,pfnBindHandles);
			if (0 == m_iGlobCounter)
			{
#ifndef UPDATE_LAYOUT_ON_SIGNAL
				m_pLayout->updateLayout();
#endif
			}
	
			return bResult;
		}
			
		default:
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			return UT_FALSE;
		}

	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		return UT_FALSE;
	}

	/*NOTREACHED*/
	UT_ASSERT(0);
	return UT_FALSE;
#else
	return UT_FALSE;
#endif	
}

UT_Bool fl_ShadowListener::signal(UT_uint32 iSignal)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return UT_FALSE;
}

