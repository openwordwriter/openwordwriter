/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2002 Martin Sevior (msevior@physics.unimelb.edu.au>
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

#include <string.h>
#include <stdlib.h>

#include "ut_types.h"
#include "ut_string.h"
#include "xap_App.h"
#include "ap_Strings.h"
#include "ap_Prefs.h"
#include "fl_SectionLayout.h"
#include "fl_TableLayout.h"
#include "fp_TableContainer.h"
#include "fl_TOCLayout.h"
#include "fl_Layout.h"
#include "fl_DocLayout.h"
#include "fl_BlockLayout.h"
#include "fb_LineBreaker.h"
#include "fp_Page.h"
#include "fp_Line.h"
#include "fp_Column.h"
#include "fp_TOCContainer.h"
#include "fp_ContainerObject.h"
#include "pd_Document.h"
#include "pp_AttrProp.h"
#include "gr_Graphics.h"
#include "pp_Property.h"
#include "px_ChangeRecord.h"
#include "px_CR_Object.h"
#include "px_CR_ObjectChange.h"
#include "px_CR_Span.h"
#include "px_CR_SpanChange.h"
#include "px_CR_Strux.h"
#include "px_CR_StruxChange.h"
#include "px_CR_Glob.h"
#include "fp_Run.h"
#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "ut_units.h"
#include "fv_View.h"
#include "pd_Style.h"

TOCEntry::TOCEntry(fl_BlockLayout * pBlock,
				   UT_sint32 iLevel, 
				   UT_UTF8String & sDispStyle,
				   bool bHaveLabel,
				   FootnoteType iFType,
				   UT_UTF8String & sBefore,
				   UT_UTF8String sAfter, 
				   bool bInherit,
				   UT_sint32 iStartAt):
	m_pBlock(pBlock),
	m_iLevel(iLevel),
	m_sDispStyle(sDispStyle),
	m_bHasLabel(bHaveLabel),
	m_iFType(iFType),
	m_sBefore(sBefore),
	m_sAfter(sAfter),
	m_bInherit(bInherit),
	m_iStartAt(iStartAt)
{
}

TOCEntry::~TOCEntry(void)
{
}

PT_DocPosition TOCEntry::getPositionInDoc(void)
{
	PT_DocPosition pos = m_pBlock->getPosition();
	return pos;
}

void TOCEntry::setPosInList(UT_sint32 posInList)
{
	m_iPosInList = posInList;
}

void TOCEntry::calculateLabel(TOCEntry * pPrevLevel)
{
	UT_String sVal;
	sVal.clear();
	m_pBlock->getView()->getLayout()->getStringFromFootnoteVal(sVal,m_iPosInList,m_iFType);
	if((pPrevLevel == NULL) || !m_bInherit)
	{
		m_sLabel = sVal.c_str();
		return;
	}
	m_sLabel = *(pPrevLevel->getNumLabel());
	m_sLabel += ".";
	m_sLabel += sVal.c_str();
}

UT_UTF8String  TOCEntry::getFullLabel(void)
{
	static UT_UTF8String sFull;
	sFull.clear();
	sFull = m_sBefore;
	sFull += m_sLabel;
	sFull += m_sAfter;
	return sFull;
}

fl_TOCLayout::fl_TOCLayout(FL_DocLayout* pLayout, fl_DocSectionLayout* pDocSL, PL_StruxDocHandle sdh, PT_AttrPropIndex indexAP, fl_ContainerLayout * pMyContainerLayout) 
 	: fl_SectionLayout(pLayout, sdh, indexAP, FL_SECTION_TOC,FL_CONTAINER_TOC,PTX_SectionTOC,pMyContainerLayout),
	  m_bNeedsRebuild(false),
	  m_bNeedsFormat(true),
	  m_bIsOnPage(false),
	  m_pDocSL(pDocSL),
	  m_bHasEndTOC(false),
	  m_iNumType1(FOOTNOTE_TYPE_NUMERIC),
	  m_iNumType2(FOOTNOTE_TYPE_NUMERIC),
	  m_iNumType3(FOOTNOTE_TYPE_NUMERIC),
	  m_iNumType4(FOOTNOTE_TYPE_NUMERIC),
	  m_iTabLeader1(FL_LEADER_DOT),
	  m_iTabLeader2(FL_LEADER_DOT),
	  m_iTabLeader3(FL_LEADER_DOT),
	  m_iTabLeader4(FL_LEADER_DOT),
	  m_iCurrentLevel(0)
{
	UT_ASSERT(m_pDocSL->getContainerType() == FL_CONTAINER_DOCSECTION);
	_createTOCContainer();
	_insertTOCContainer(static_cast<fp_TOCContainer *>(getLastContainer()));
	m_pLayout->addTOC(this);
}

fl_TOCLayout::~fl_TOCLayout()
{
	// NB: be careful about the order of these
	UT_DEBUGMSG(("Deleting TOClayout %x \n",this));
	_purgeLayout();
	fp_TOCContainer * pTC = static_cast<fp_TOCContainer *>(getFirstContainer());
	while(pTC)
	{
		fp_TOCContainer * pNext = static_cast<fp_TOCContainer *>(pTC->getNext());
		if(pTC == static_cast<fp_TOCContainer *>(getLastContainer()))
		{
			pNext = NULL;
		}
		delete pTC;
		pTC = pNext;
	}

	setFirstContainer(NULL);
	setLastContainer(NULL);
	m_pLayout->removeTOC(this);
}
	
/*!
 * Returns the position in the document of the PTX_SectionTOC strux
 * This is very useful for determining the value of the footnote reference
 * and anchor. 
*/
PT_DocPosition fl_TOCLayout::getDocPosition(void) 
{
	PL_StruxDocHandle sdh = getStruxDocHandle();
    return 	m_pLayout->getDocument()->getStruxPosition(sdh);
}

/*!
 * This method returns the length of the footnote. This is such that 
 * getDocPosition() + getLength() is one value beyond the the EndFootnote
 * strux
 */
UT_uint32 fl_TOCLayout::getLength(void)
{
	PT_DocPosition startPos = getDocPosition();
	PL_StruxDocHandle sdhEnd = NULL;
	PL_StruxDocHandle sdhStart = getStruxDocHandle();
	bool bres;
	bres = m_pLayout->getDocument()->getNextStruxOfType(sdhStart,PTX_EndTOC,&sdhEnd);
	UT_ASSERT(bres && sdhEnd);
	PT_DocPosition endPos = m_pLayout->getDocument()->getStruxPosition(sdhEnd);
	UT_uint32 length = static_cast<UT_uint32>(endPos - startPos + 1); 
	return length;
}


bool fl_TOCLayout::bl_doclistener_insertEndTOC(fl_ContainerLayout*,
											  const PX_ChangeRecord_Strux * pcrx,
											  PL_StruxDocHandle sdh,
											  PL_ListenerId lid,
											  void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
																	  PL_ListenerId lid,
																	  PL_StruxFmtHandle sfhNew))
{
	// The endFootnote strux actually needs a format handle to to this Footnote layout.
	// so we bind to this layout.

		
	PL_StruxFmtHandle sfhNew = static_cast<PL_StruxFmtHandle>(this);
	pfnBindHandles(sdh,lid,sfhNew);

//
// increment the insertion point in the view.
//
	FV_View* pView = m_pLayout->getView();
	if (pView && (pView->isActive() || pView->isPreview()))
	{
		pView->setPoint(pcrx->getPosition() +  fl_BLOCK_STRUX_OFFSET);
	}
	else if(pView && pView->getPoint() > pcrx->getPosition())
	{
		pView->setPoint(pView->getPoint() +  fl_BLOCK_STRUX_OFFSET);
	}
	m_bHasEndTOC = true;
	return true;
}


/*!
 * This signals an incomplete footnote section.
 */
bool fl_TOCLayout::doclistener_deleteEndTOC( const PX_ChangeRecord_Strux * pcrx)
{
	m_bHasEndTOC = false;
	return true;
}


fl_SectionLayout * fl_TOCLayout::getSectionLayout(void) const
{
	fl_ContainerLayout * pDSL = myContainingLayout();
	while(pDSL)
	{
		if(pDSL->getContainerType() == FL_CONTAINER_DOCSECTION)
		{
			return static_cast<fl_SectionLayout *>(pDSL);
		}
		pDSL = pDSL->myContainingLayout();
	}
	return NULL;
}

FootnoteType fl_TOCLayout::getNumType(UT_sint32 iLevel)
{
	if(iLevel == 1)
	{
		return m_iNumType1;
	}
	else if(iLevel == 2)
	{
		return m_iNumType2;
	}
	else if(iLevel == 3)
	{
		return m_iNumType3;
	}
	else if(iLevel == 4)
	{
		return m_iNumType4;
	}
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	return static_cast<FootnoteType>(0);
}


eTabLeader fl_TOCLayout::getTabLeader(UT_sint32 iLevel)
{
	if(iLevel == 1)
	{
		return m_iTabLeader1;
	}
	else if(iLevel == 2)
	{
		return m_iTabLeader2;
	}
	else if(iLevel == 3)
	{
		return m_iTabLeader3;
	}
	else if(iLevel == 4)
	{
		return m_iTabLeader4;
	}
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	return static_cast<eTabLeader>(0);
}

UT_sint32 fl_TOCLayout::getTabPosition(UT_sint32 iLevel, fl_BlockLayout * pBlock)
{
	fp_TOCContainer * pTOCC = static_cast<fp_TOCContainer *>(getFirstContainer());
	if(pTOCC == NULL)
	{
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		return 0;
	}
	UT_sint32 iWidth = pTOCC->getWidth() -pBlock->getLeftMargin();
	UT_UTF8String sStr("");
	if(iLevel == 1)
	{
		sStr = m_sNumOff1;
	}
	else if(iLevel == 2)
	{
		sStr = m_sNumOff2;
	}
	else if(iLevel == 3)
	{
		sStr = m_sNumOff3;
	}
	else if(iLevel == 4)
	{
		sStr = m_sNumOff4;
	}
	iWidth -= UT_convertToLogicalUnits(sStr.utf8_str());
	return iWidth;
}

bool fl_TOCLayout::addBlock(fl_BlockLayout * pBlock)
{
	UT_UTF8String sStyle;
	pBlock->getStyle(sStyle);
	if(_isStyleInTOC(sStyle,m_sSourceStyle1))
	{
		m_iCurrentLevel = 1;
		_addBlockInVec(pBlock,m_sDestStyle1);
		return true;
	}
	if(_isStyleInTOC(sStyle,m_sSourceStyle2))
	{
		m_iCurrentLevel = 2;
		_addBlockInVec(pBlock,m_sDestStyle2);
		return true;
	}
	if(_isStyleInTOC(sStyle,m_sSourceStyle3))
	{
		m_iCurrentLevel = 3;
		_addBlockInVec(pBlock,m_sDestStyle3);
		return true;
	}
	if(_isStyleInTOC(sStyle,m_sSourceStyle4))
	{
		m_iCurrentLevel = 4;
		_addBlockInVec(pBlock,m_sDestStyle4);
		return true;
	}
	return false;
}

void fl_TOCLayout::_addBlockInVec(fl_BlockLayout * pBlock, UT_UTF8String & sStyle)
{
// First find where to put the block.
	markAllRunsDirty();
	setNeedsReformat(0);
	setNeedsRedraw();
	PT_DocPosition posNew = pBlock->getPosition();
	TOCEntry * pEntry = NULL;
	fl_BlockLayout * pPrevBL = NULL;
	UT_sint32 i = 0;
	bool bFound = false;
	for(i=0; i< static_cast<UT_sint32>(m_vecEntries.getItemCount()); i++)
	{
		pEntry = static_cast<TOCEntry *>(m_vecEntries.getNthItem(i));
		pPrevBL = pEntry->getBlock();
		UT_DEBUGMSG(("Looking at Block %x pos %d \n",pPrevBL,pPrevBL->getPosition()));
		if(pPrevBL->getPosition() > posNew)
		{
			bFound = true;
			break;
		}
	}

	UT_sint32 iAllBlocks = 0;
	if(bFound)
	{
		if(i > 0)
		{
			pEntry =  static_cast<TOCEntry *>(m_vecEntries.getNthItem(i-1));
			pPrevBL =  pEntry->getBlock();
		}
		else
		{
			pPrevBL = NULL;
		}
	}
	iAllBlocks = i;
	PD_Style * pStyle = NULL;
	if(pPrevBL)
	{
		UT_DEBUGMSG(("My block at pos %d Inserted after Block at pos %d \n",posNew,pPrevBL->getPosition()));
	}
	else
	{
		UT_DEBUGMSG(("New block at pos append to end \n",posNew));

	}
	m_pDoc->getStyle(sStyle.utf8_str(),&pStyle);
	fl_TOCListener * pListen = new fl_TOCListener(this,pPrevBL,pStyle);
	PT_DocPosition posStart,posEnd;
	posStart = pBlock->getPosition(true);
	posEnd = posStart + static_cast<PT_DocPosition>(pBlock->getLength());
	PD_DocumentRange * docRange = new PD_DocumentRange(m_pDoc,posStart,posEnd);
	m_pDoc->tellListenerSubset(pListen, docRange);
	delete docRange;
	delete pListen;
	fl_BlockLayout * pNewBlock;
	if(pPrevBL)
	{
		pNewBlock = static_cast<fl_BlockLayout *>(pPrevBL->getNext());
	}
	else
	{
		pNewBlock = static_cast<fl_BlockLayout *>(getFirstLayout());
	}
	UT_DEBUGMSG(("New TOC block in TOCLayout %x \n",pNewBlock));
//
// OK Now add the block to out vectors.
//
	TOCEntry *pNewEntry = createNewEntry(pNewBlock);
	if(iAllBlocks == 0)
	{
		m_vecEntries.insertItemAt(static_cast<void *>(pNewEntry),0);
	}
	else if (iAllBlocks < static_cast<UT_sint32>(m_vecEntries.getItemCount()-1))
	{
		m_vecEntries.insertItemAt(static_cast<void *>(pNewEntry),iAllBlocks);
	}
	else
	{
		m_vecEntries.addItem(static_cast<void *>(pNewEntry));
	}
	_calculateLabels();
//
// Tell the block it's shadowed in a TOC
//
	pBlock->setStyleInTOC(true);
//
// Now append the tab and Field's to end of the new Block.
//
	PT_DocPosition iLen = static_cast<PT_DocPosition>(pBlock->getLength());
	iLen -=1; // subtract 1 for the inital strux
	pNewBlock->_doInsertTOCTabRun(iLen);
	iLen++;
	pNewBlock->_doInsertFieldTOCRun(iLen);
//
// Now Insert the TAB and TOCListLabel runs if requested.
//
	if(pNewEntry->hasLabel())
	{
		pNewBlock->_doInsertTOCListTabRun(0);
		pNewBlock->_doInsertTOCListLabelRun(0);
	}
	
}

UT_sint32 fl_TOCLayout::isInVector(fl_BlockLayout * pBlock, 
								   UT_Vector * pVecEntries)
{
	TOCEntry * pThisEntry = NULL;
	fl_BlockLayout * pThisBL = NULL;
	UT_sint32 i = 0;
	for(i=0; i< static_cast<UT_sint32>(pVecEntries->getItemCount()); i++)
	{

		pThisEntry = static_cast<TOCEntry *>(pVecEntries->getNthItem(i));
		pThisBL = pThisEntry->getBlock();
		if(pThisBL->getStruxDocHandle() == pBlock->getStruxDocHandle())
		{
			return i;
		}
	}
	return -1;
}

bool fl_TOCLayout::removeBlock(fl_BlockLayout * pBlock)
{
	if(isInVector(pBlock,&m_vecEntries) >= 0)
	{
		_removeBlockInVec(pBlock);
		_calculateLabels();
		return true;
	}
	return false;
}

fl_BlockLayout * fl_TOCLayout::findMatchingBlock(fl_BlockLayout * pBlock)
{
	TOCEntry * pThisEntry = NULL;
	fl_BlockLayout * pThisBL = NULL;
	UT_sint32 i = 0;
	bool bFound = false;
	for(i=0; i< static_cast<UT_sint32>(m_vecEntries.getItemCount()); i++)
	{
		pThisEntry = static_cast<TOCEntry *>(m_vecEntries.getNthItem(i));
		pThisBL = pThisEntry->getBlock();
		if(pThisBL->getStruxDocHandle() == pBlock->getStruxDocHandle())
		{
			bFound = true;
			break;
		}
	}
	if(bFound)
	{
		return pThisBL;
	}
	return NULL;
}

void fl_TOCLayout::_removeBlockInVec(fl_BlockLayout * pBlock)
{
	TOCEntry * pThisEntry = NULL;
	fl_BlockLayout * pThisBL = NULL;
	UT_sint32 i = 0;
	bool bFound = false;
	for(i=0; i< static_cast<UT_sint32>(m_vecEntries.getItemCount()); i++)
	{
		pThisEntry = static_cast<TOCEntry *>(m_vecEntries.getNthItem(i));
		pThisBL = pThisEntry->getBlock();
		if(pThisBL->getStruxDocHandle() == pBlock->getStruxDocHandle())
		{
			bFound = true;
			break;
		}
	}
	UT_sint32 iAllVec =i;
	if(!bFound)
	{
		return;
	}
//
// unlink it from the TOCLayout
//
	if(static_cast<fl_BlockLayout *>(getFirstLayout()) == pThisBL)
	{
		setFirstLayout(pThisBL->getNext());
	}
	if(static_cast<fl_BlockLayout *>(getLastLayout()) == pThisBL)
	{
		setLastLayout(pThisBL->getPrev());
	}
	if(pThisBL->getPrev())
	{
		pThisBL->getPrev()->setNext(pThisBL->getNext());
	}
	if(pThisBL->getNext())
	{
		pThisBL->getNext()->setPrev(pThisBL->getPrev());
	}
	delete pThisBL;
	delete pThisEntry;
	m_vecEntries.deleteNthItem(iAllVec);
	markAllRunsDirty();
	setNeedsReformat(0);
	setNeedsRedraw();

}

UT_sint32 fl_TOCLayout::_getStartValue(TOCEntry * pEntry)
{
	if(pEntry->getLevel() == 1)
	{
		return m_iStartAt1;
	}
	else if(pEntry->getLevel() == 2)
	{
		return m_iStartAt2;
	}
	else if(pEntry->getLevel() == 3)
	{
		return m_iStartAt3;
	}
	else
	{
		return m_iStartAt4;
	}
}

void fl_TOCLayout::_calculateLabels(void)
{
	UT_sint32 i = 0;
	TOCEntry * pThisEntry = NULL;
	TOCEntry * pPrevEntry = NULL;
	UT_Stack stEntry;
	pThisEntry = static_cast<TOCEntry *>(m_vecEntries.getNthItem(0));
	stEntry.push(static_cast<void *>(pThisEntry));
	for(i=1; i<	static_cast<UT_sint32>(m_vecEntries.getItemCount()); i++)
	{
		if(pPrevEntry == NULL)
		{
			pThisEntry->setPosInList(_getStartValue(pThisEntry));
			pThisEntry->calculateLabel(pPrevEntry);
			pPrevEntry = pThisEntry;
		}
		pThisEntry = static_cast<TOCEntry *>(m_vecEntries.getNthItem(i));
		if(pThisEntry->getLevel() == pPrevEntry->getLevel())
		{
			pThisEntry->setPosInList(pPrevEntry->getPosInList()+1);
			void * pTmp;
			stEntry.viewTop(&pTmp);
			TOCEntry * pPrevLevel = static_cast<TOCEntry *>(pTmp);
			if(pPrevLevel && pPrevLevel->getLevel() < pThisEntry->getLevel())
			{
				pThisEntry->calculateLabel(pPrevLevel);
			}
			else
			{
				pThisEntry->calculateLabel(NULL);
			}
			pPrevEntry = pThisEntry;
		}
		else if(pThisEntry->getLevel() > pPrevEntry->getLevel())
		{
			stEntry.push(pPrevEntry);
			pThisEntry->setPosInList(_getStartValue(pThisEntry));
			pThisEntry->calculateLabel(pPrevEntry);
			pPrevEntry = pThisEntry;
		}
		else
		{
			bool bStop = false;
			while((stEntry.getDepth()>0) && !bStop)
			{
				void * pTmp;
				stEntry.pop(&pTmp);
				pPrevEntry = static_cast<TOCEntry *>(pTmp);
				if(pPrevEntry->getLevel() == pThisEntry->getLevel())
				{
					bStop = true;
				}
			}
			if(bStop)
			{
				pThisEntry->setPosInList(pPrevEntry->getPosInList()+1);
				void * pTmp;
				stEntry.viewTop(&pTmp);
				TOCEntry * pPrevLevel = static_cast<TOCEntry *>(pTmp);
				if(pPrevLevel && pPrevLevel->getLevel() < pThisEntry->getLevel())
				{
					pThisEntry->calculateLabel(pPrevLevel);
				}
				else
				{
					pThisEntry->calculateLabel(NULL);
				}
				pPrevEntry = pThisEntry;
			}
			else
			{
				pThisEntry->setPosInList(_getStartValue(pThisEntry));
				pPrevEntry = pThisEntry;
				pThisEntry->calculateLabel(NULL);
			}
		}
	}
}

bool fl_TOCLayout::isStyleInTOC(UT_UTF8String & sStyle)
{
	if(_isStyleInTOC(sStyle,m_sSourceStyle1))
	{
		return true;
	}
	if(_isStyleInTOC(sStyle,m_sSourceStyle2))
	{
		return true;
	}
	if(_isStyleInTOC(sStyle,m_sSourceStyle3))
	{
		return true;
	}
	if(_isStyleInTOC(sStyle,m_sSourceStyle4))
	{
		return true;
	}
	return false;
}

/*!
 * Does a case insensitive search of the basedon heirachy for a match.
 */
bool fl_TOCLayout::_isStyleInTOC(UT_UTF8String & sStyle, UT_UTF8String & sTOCStyle)
{
	UT_UTF8String sTmpStyle = sStyle;
	const char * sLStyle = sTOCStyle.utf8_str();
	UT_DEBUGMSG(("Looking at Style %s \n",sLStyle));
	UT_DEBUGMSG(("Base input style is %s \n",sTmpStyle.utf8_str()));
	if(UT_stricmp(sLStyle,sTmpStyle.utf8_str()) == 0)
	{
		UT_DEBUGMSG(("Found initial match \n"));
		return true;
	}
	PD_Style * pStyle = NULL;
	m_pDoc->getStyle(sTmpStyle.utf8_str(), &pStyle);
	if(pStyle != NULL)
	{
		UT_sint32 iLoop = 0;
		while((pStyle->getBasedOn()) != NULL && (iLoop < 10))
		{
			pStyle = pStyle->getBasedOn();
			iLoop++;
			sTmpStyle = pStyle->getName();
			UT_DEBUGMSG(("Level %d style is %s \n",iLoop,sTmpStyle.utf8_str()));
			if(UT_stricmp(sLStyle,sTmpStyle.utf8_str()) == 0)
			{
				UT_DEBUGMSG(("Found match  at Level %d \n",iLoop));
				return true;
			}
		}
	}
	UT_DEBUGMSG(("No match Found \n"));
	return false;
}


bool fl_TOCLayout::isBlockInTOC(fl_BlockLayout * pBlock)
{
	TOCEntry * pEntry = NULL;
	PL_StruxDocHandle sdh = pBlock->getStruxDocHandle();
	UT_sint32 i = 0;
	for(i=0; i< static_cast<UT_sint32>(m_vecEntries.getItemCount()); i++)
	{

		pEntry = static_cast<TOCEntry *>(m_vecEntries.getNthItem(i));
		fl_BlockLayout *pBL = pEntry->getBlock();
		if(pBL->getStruxDocHandle() == sdh)
		{
			return true;
		}
	}
	return false;
}


UT_UTF8String * fl_TOCLayout::getTOCListLabel(fl_BlockLayout * pBlock)
{
	static UT_UTF8String str;
	str.clear();
	TOCEntry * pEntry = NULL;
	PL_StruxDocHandle sdh = pBlock->getStruxDocHandle();
	UT_sint32 i = 0;
	bool bFound = false;
	for(i=0; i< static_cast<UT_sint32>(m_vecEntries.getItemCount()); i++)
	{

		pEntry = static_cast<TOCEntry *>(m_vecEntries.getNthItem(i));
		fl_BlockLayout *pBL = pEntry->getBlock();
		if(pBL->getStruxDocHandle() == sdh)
		{
			bFound = true;
			break;
		}
	}
	if(!bFound)
	{
		return &str;
	}
	str = pEntry->getFullLabel();
	return &str;
}

fl_BlockLayout * fl_TOCLayout::getMatchingBlock(fl_BlockLayout * pBlock)
{
	return findMatchingBlock(pBlock);
}

bool fl_TOCLayout::doclistener_changeStrux(const PX_ChangeRecord_StruxChange * pcrxc)
{
	UT_ASSERT(pcrxc->getType()==PX_ChangeRecord::PXT_ChangeStrux);


	setAttrPropIndex(pcrxc->getIndexAP());
	collapse();
	_lookupProperties();
	_createTOCContainer();
	_insertTOCContainer(static_cast<fp_TOCContainer *>(getLastContainer()));
	return true;
}


bool fl_TOCLayout::recalculateFields(UT_uint32 iUpdateCount)
{

	bool bResult = false;
	fl_ContainerLayout*	pBL = getFirstLayout();
	while (pBL)
	{
		bResult = pBL->recalculateFields(iUpdateCount) || bResult;
		pBL = pBL->getNext();
	}
	return bResult;
}


void fl_TOCLayout::markAllRunsDirty(void)
{
	fl_ContainerLayout*	pCL = getFirstLayout();
	while (pCL)
	{
		pCL->markAllRunsDirty();
		pCL = pCL->getNext();
	}
}

void fl_TOCLayout::updateLayout(void)
{
	if(needsReformat())
	{
		format();
	}
	fl_ContainerLayout*	pBL = getFirstLayout();
	while (pBL)
	{
		if (pBL->needsReformat())
		{
			pBL->format();
		}

		pBL = pBL->getNext();
	}
}

void fl_TOCLayout::redrawUpdate(void)
{
	fl_ContainerLayout*	pBL = getFirstLayout();
	while (pBL)
	{
		if (pBL->needsRedraw())
		{
			pBL->redrawUpdate();
		}

		pBL = pBL->getNext();
	}
}


bool fl_TOCLayout::doclistener_deleteStrux(const PX_ChangeRecord_Strux * pcrx)
{
	UT_ASSERT(pcrx->getType()==PX_ChangeRecord::PXT_DeleteStrux);
//
// Remove all remaining structures
//
	collapse();
//	UT_ASSERT(pcrx->getStruxType()== PTX_SectionTOC);
//

	fl_ContainerLayout * pPrev = getPrev();
	fl_ContainerLayout * pNext = getNext();

	if(pPrev != NULL)
	{
		pPrev->setNext(pNext);
	}
	else
	{
		myContainingLayout()->setFirstLayout(pNext);
	}
	if(pNext != NULL)
	{
		pNext->setPrev(pPrev);
	}
	else
	{
		myContainingLayout()->setLastLayout(pPrev);
	}
	delete this;			// TODO whoa!  this construct is VERY dangerous.

	return true;
}

TOCEntry * fl_TOCLayout::createNewEntry(fl_BlockLayout * pNewBL)
{
	UT_UTF8String sDispStyle("");
	bool bHaveLabel = true;
	FootnoteType iFType; 
	UT_UTF8String sBefore;
	UT_UTF8String sAfter; 
	bool bInherit; 	
	UT_sint32 iStartAt;
	if(m_iCurrentLevel == 1)
	{
		sDispStyle = m_sDestStyle1;
		bHaveLabel = m_bHasLabel1;
		iFType = m_iLabType1;
		sBefore = m_sLabBefore1;
		sAfter = m_sLabAfter1;
		bInherit = m_bInherit1;
		iStartAt = m_iStartAt1;
	}
	else if( m_iCurrentLevel == 2)
	{
		sDispStyle = m_sDestStyle2;
		bHaveLabel = m_bHasLabel2;
		iFType = m_iLabType2;
		sBefore = m_sLabBefore2;
		sAfter = m_sLabAfter2;
		bInherit = m_bInherit2;
		iStartAt = m_iStartAt2;
	}
	else if( m_iCurrentLevel == 3)
	{
		sDispStyle = m_sDestStyle3;
		bHaveLabel = m_bHasLabel3;
		iFType = m_iLabType3;
		sBefore = m_sLabBefore3;
		sAfter = m_sLabAfter3;
		bInherit = m_bInherit3;
		iStartAt = m_iStartAt3;
	}
	else if( m_iCurrentLevel == 4)
	{
		sDispStyle = m_sDestStyle4;
		bHaveLabel = m_bHasLabel4;
		iFType = m_iLabType4;
		sBefore = m_sLabBefore4;
		sAfter = m_sLabAfter4;
		bInherit = m_bInherit4;
		iStartAt = m_iStartAt4;
	}
	TOCEntry * pNew = new TOCEntry(pNewBL,m_iCurrentLevel,
								   sDispStyle,
								   bHaveLabel,
								   iFType,
								   sBefore,
								   sAfter,
								   bInherit,
								   iStartAt);
	return pNew;
}

/*!
 * This method removes all layout structures contained by this layout
 * structure.
 */
void fl_TOCLayout::_purgeLayout(void)
{
	UT_DEBUGMSG(("TOCLayout: purge \n"));
	fl_ContainerLayout * pCL = getFirstLayout();
	while(pCL)
	{
		fl_ContainerLayout * pNext = pCL->getNext();
		delete pCL;
		pCL = pNext;
	}
	UT_VECTOR_PURGEALL(TOCEntry *, m_vecEntries);
	m_vecEntries.clear();
	setFirstLayout(NULL);
	setLastLayout(NULL);
}


/*!
 * This method creates a new TOC.
 */
void fl_TOCLayout::_createTOCContainer(void)
{
	_lookupProperties();
	UT_ASSERT(getFirstLayout() == NULL);
	fp_TOCContainer * pTOCContainer = new fp_TOCContainer(static_cast<fl_SectionLayout *>(this));
	setFirstContainer(pTOCContainer);
	setLastContainer(pTOCContainer);
	fl_ContainerLayout * pCL = myContainingLayout();
	while(pCL!= NULL && pCL->getContainerType() != FL_CONTAINER_DOCSECTION)
	{
		pCL = pCL->myContainingLayout();
	}
	fl_DocSectionLayout * pDSL = static_cast<fl_DocSectionLayout *>(pCL);
	UT_ASSERT(pDSL != NULL);

	fp_Container * pCon = pCL->getLastContainer();
	UT_ASSERT(pCon);
	UT_sint32 iWidth = pCon->getWidth();
	pTOCContainer->setWidth(iWidth);
	m_pLayout->fillTOC(this);
	if(m_bTOCHeading)
	{
		fl_BlockLayout * pBlock = m_pLayout->findBlockAtPosition(getPosition()-1);
		UT_ASSERT(pBlock);
		if(pBlock)
		{
			PD_Style * pStyle = NULL;
			m_pDoc->getStyle(m_sTOCHeadingStyle.utf8_str(), &pStyle);
			if(pStyle == NULL)
			{
				m_pDoc->getStyle("Heading 1", &pStyle);
			}
			PT_AttrPropIndex indexAP = pStyle->getIndexAP();

			fl_BlockLayout * pNewBlock = static_cast<fl_BlockLayout *>(insert(pBlock->getStruxDocHandle(),NULL,indexAP,FL_CONTAINER_BLOCK));
			pNewBlock->_doInsertTOCHeadingRun(0);
		}
	}
}

/*!
  Create a new TOC container.
  \return The newly created TOC container
*/
fp_Container* fl_TOCLayout::getNewContainer(fp_Container *)
{
	UT_DEBUGMSG(("creating new TOC Physical container\n"));
	_createTOCContainer();
	_insertTOCContainer(static_cast<fp_TOCContainer *>(getLastContainer()));
	return static_cast<fp_Container *>(getLastContainer());
}


/*!
 * This method inserts the given TOCContainer into its correct place in the
 * Vertical container.
 */
void fl_TOCLayout::_insertTOCContainer( fp_TOCContainer * pNewTOC)
{
	fl_ContainerLayout * pUPCL = myContainingLayout();
	fl_ContainerLayout * pPrevL = static_cast<fl_ContainerLayout *>(getPrev());
	fp_Container * pPrevCon = NULL;
	fp_Container * pUpCon = NULL;
	if(pPrevL != NULL)
	{
		while(pPrevL && ((pPrevL->getContainerType() == FL_CONTAINER_FOOTNOTE) || pPrevL->getContainerType() == FL_CONTAINER_ENDNOTE))
		{
			pPrevL = pPrevL->getPrev();
		}
		if(pPrevL)
		{
			if(pPrevL->getContainerType() == FL_CONTAINER_TABLE)
			{
//
// Handle if prev container is table that is broken across a page
//
				fl_TableLayout * pTL = static_cast<fl_TableLayout *>(pPrevL);
				fp_TableContainer * pTC = static_cast<fp_TableContainer *>(pTL->getFirstContainer());
				fp_TableContainer * pFirst = pTC->getFirstBrokenTable();
				fp_TableContainer * pLast = pTC->getLastBrokenTable();
				if((pLast != NULL) && pLast != pFirst)
				{
					pPrevCon = static_cast<fp_Container *>(pLast);
					pUpCon = pLast->getContainer();
				}
				else
				{
					pPrevCon = pPrevL->getLastContainer();
					pUpCon = pPrevCon->getContainer();
				}
			}
			else
			{
				pPrevCon = pPrevL->getLastContainer();
				pUpCon = pPrevCon->getContainer();
			}
		}
		else
		{
			pUpCon = pUPCL->getLastContainer();
		}
		UT_ASSERT(pUpCon);
	}
	else
	{
		pUpCon = pUPCL->getLastContainer();
		UT_ASSERT(pUpCon);
	}
	if(pPrevL == NULL)
	{
		xxx_UT_DEBUGMSG(("SEVIOR!!!!!!!!!! New TOC %x added into %x \n",pNewTOC,pUpCon));
		pUpCon->addCon(pNewTOC);
		pNewTOC->setContainer(pUpCon);
;
	}
	else
	{
		UT_sint32 i = pUpCon->findCon(pPrevCon);
		xxx_UT_DEBUGMSG(("SEVIOR!!!!!!!!!! New TOC %x inserted into %x \n",pNewTOC,pUpCon));
		if(i >= 0 && (i+1) < static_cast<UT_sint32>(pUpCon->countCons()))
		{
			pUpCon->insertConAt(pNewTOC,i+1);
			pNewTOC->setContainer(pUpCon);
		}
		else if( i >=0 &&  (i+ 1) == static_cast<UT_sint32>(pUpCon->countCons()))
		{
			pUpCon->addCon(pNewTOC);
			pNewTOC->setContainer(pUpCon);
		}
		else
		{
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		}
	}
}


void fl_TOCLayout::format(void)
{
	xxx_UT_DEBUGMSG(("SEVIOR: Formatting TOC container is %x \n",getFirstContainer()));
	if(getFirstContainer() == NULL)
	{
		getNewContainer();
	}
	fl_ContainerLayout*	pBL = getFirstLayout();
	
	while (pBL)
	{
		pBL->format();
		UT_sint32 count = 0;
		while(pBL->getLastContainer() == NULL || pBL->getFirstContainer()==NULL)
		{
			UT_DEBUGMSG(("Error formatting a block try again \n"));
			count = count + 1;
			pBL->format();
			if(count > 3)
			{
				UT_DEBUGMSG(("Give up trying to format. Hope for the best :-( \n"));
				break;
			}
		}
		pBL = pBL->getNext();
	}
	static_cast<fp_TOCContainer *>(getFirstContainer())->layout();
	m_bNeedsFormat = false;
	m_bNeedsReformat = false;
}

void fl_TOCLayout::_lookupProperties(void)
{
 	const PP_AttrProp* pSectionAP = NULL;

	m_pLayout->getDocument()->getAttrProp(m_apIndex, &pSectionAP);
	// I can't think of any properties we need for now.
	// If we need any later, we'll add them. -PL
	const XML_Char *pszTOCPID = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-id",pszTOCPID))
	{
		m_iTOCPID = 0;
	}
	else
	{
		m_iTOCPID = atoi(pszTOCPID);
	}

	m_sNumOff1 = "0.5in";
	m_sNumOff2 = "0.5in";
	m_sNumOff3 = "0.5in";
	m_sNumOff4 = "0.5in";
	const XML_Char *pszTOCSRC = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-source-style-1",pszTOCSRC))
	{
		m_sSourceStyle1 = "Heading 1";
	}
	else
	{
		m_sSourceStyle1 = pszTOCSRC;
	}
	pszTOCSRC = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-source-style-2",pszTOCSRC))
	{
		m_sSourceStyle2 = "Heading 2";
	}
	else
	{
		m_sSourceStyle2 = pszTOCSRC;
	}
	pszTOCSRC = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-source-style-3",pszTOCSRC))
	{
		m_sSourceStyle3 = "Heading 3";
	}
	else
	{
		m_sSourceStyle3 = pszTOCSRC;
	}
	pszTOCSRC = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-source-style-4",pszTOCSRC))
	{
		m_sSourceStyle4 = "Heading 4";
	}
	else
	{
		m_sSourceStyle4 = pszTOCSRC;
	}
	const XML_Char * pszTOCDEST = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-dest-style-1",pszTOCDEST))
	{
		m_sDestStyle1 = "Contents 1";
	}
	else
	{
		m_sDestStyle1 = pszTOCDEST;
	}
	pszTOCDEST = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-dest-style-2",pszTOCDEST))
	{
		m_sDestStyle2 = "Contents 2";
	}
	else
	{
		m_sDestStyle2 = pszTOCDEST;
	}
	pszTOCDEST = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-dest-style-3",pszTOCDEST))
	{
		m_sDestStyle3 = "Contents 3";
	}
	else
	{
		m_sDestStyle3 = pszTOCDEST;
	}
	pszTOCDEST = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-dest-style-4",pszTOCDEST))
	{
		m_sDestStyle4 = "Contents 4";
	}
	else
	{
		m_sDestStyle4 = pszTOCDEST;
	}
	const XAP_StringSet * pSS = XAP_App::getApp()->getStringSet();
	const XML_Char * pszTOCHEADING = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-heading",pszTOCHEADING))
	{
		m_sTOCHeading = pSS->getValueUTF8(AP_STRING_ID_TOC_TocHeading);
	}
	else
	{
		m_sTOCHeading = pszTOCHEADING;
	}

	const XML_Char * pszTOCHEADINGStyle = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-heading-style",pszTOCHEADINGStyle))
	{
		m_sTOCHeadingStyle = "Contents Header";
	}
	else
	{
		m_sTOCHeadingStyle = pszTOCHEADINGStyle;
	}


	const XML_Char * pszTOCHASHEADING = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-has-heading",pszTOCHASHEADING))
	{
		m_bTOCHeading = true;
	}
	else
	{
		if(UT_stricmp(pszTOCHASHEADING,"1") == 0)
		{
			m_bTOCHeading = true;
		}
		else
		{
			m_bTOCHeading = true;
		}
	}
//
// TOC Label
//
	const XML_Char * pszTOCLABEL = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-has-label1",pszTOCLABEL))
	{
		m_bHasLabel1 = true;
	}
	else
	{
		if(UT_stricmp(pszTOCLABEL,"1") == 0)
		{
			m_bHasLabel1 = true;
		}
		else
		{
			m_bHasLabel1 = false;
		}
	}
	pszTOCLABEL = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-has-label2",pszTOCLABEL))
	{
		m_bHasLabel2 = true;
	}
	else
	{
		if(UT_stricmp(pszTOCLABEL,"1") == 0)
		{
			m_bHasLabel2 = true;
		}
		else
		{
			m_bHasLabel2 = false;
		}
	}
	pszTOCLABEL = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-has-label3",pszTOCLABEL))
	{
		m_bHasLabel3 = true;
	}
	else
	{
		if(UT_stricmp(pszTOCLABEL,"1") == 0)
		{
			m_bHasLabel3 = true;
		}
		else
		{
			m_bHasLabel3 = false;
		}
	}
	pszTOCLABEL = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-has-label4",pszTOCLABEL))
	{
		m_bHasLabel4 = true;
	}
	else
	{
		if(UT_stricmp(pszTOCLABEL,"1") == 0)
		{
			m_bHasLabel4 = true;
		}
		else
		{
			m_bHasLabel4 = false;
		}
	}
//
// TOC Label Inherits
//
	const XML_Char * pszTOCLABELINHERITS = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-inherits1",pszTOCLABELINHERITS))
	{
		m_bInherit1 = true;
	}
	else
	{
		if(UT_stricmp(pszTOCLABELINHERITS,"1") == 0)
		{
			m_bInherit1 = true;
		}
		else
		{
			m_bInherit1 = false;;
		}
	}
	pszTOCLABELINHERITS = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-inherits2",pszTOCLABELINHERITS))
	{
		m_bInherit2 = true;
	}
	else
	{
		if(UT_stricmp(pszTOCLABELINHERITS,"1") == 0)
		{
			m_bInherit2 = true;
		}
		else
		{
			m_bInherit2 = false;;
		}
	}
	pszTOCLABELINHERITS = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-inherits3",pszTOCLABELINHERITS))
	{
		m_bInherit3 = true;
	}
	else
	{
		if(UT_stricmp(pszTOCLABELINHERITS,"1") == 0)
		{
			m_bInherit3 = true;
		}
		else
		{
			m_bInherit3 = false;;
		}
	}
	pszTOCLABELINHERITS = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-inherits4",pszTOCLABELINHERITS))
	{
		m_bInherit4 = true;
	}
	else
	{
		if(UT_stricmp(pszTOCLABELINHERITS,"1") == 0)
		{
			m_bInherit4 = true;
		}
		else
		{
			m_bInherit4 = false;;
		}
	}
//
// TOC Label Type
//
	const XML_Char * pszTOCLABELTYPE = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-type1",pszTOCLABELTYPE))
	{
		m_iLabType1 = FOOTNOTE_TYPE_NUMERIC;
	}
	else
	{
		m_iLabType1 = m_pLayout->FootnoteTypeFromString(pszTOCLABELTYPE);
	}
	pszTOCLABELTYPE = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-type2",pszTOCLABELTYPE))
	{
		m_iLabType2 = FOOTNOTE_TYPE_NUMERIC;
	}
	else
	{
		m_iLabType2 = m_pLayout->FootnoteTypeFromString(pszTOCLABELTYPE);
	}
	pszTOCLABELTYPE = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-type3",pszTOCLABELTYPE))
	{
		m_iLabType3 = FOOTNOTE_TYPE_NUMERIC;
	}
	else
	{
		m_iLabType3 = m_pLayout->FootnoteTypeFromString(pszTOCLABELTYPE);
	}
	pszTOCLABELTYPE = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-type4",pszTOCLABELTYPE))
	{
		m_iLabType4 = FOOTNOTE_TYPE_NUMERIC;
	}
	else
	{
		m_iLabType4 = m_pLayout->FootnoteTypeFromString(pszTOCLABELTYPE);
	}
//
// TOC Label Before Text
//
	const XML_Char * pszTOCSTRBEFORE = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-before1",pszTOCSTRBEFORE))
	{
		m_sLabBefore1 = "";
	}
	else
	{
		m_sLabBefore1 = pszTOCSTRBEFORE;
	}
	pszTOCSTRBEFORE = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-before2",pszTOCSTRBEFORE))
	{
		m_sLabBefore2 = "";
	}
	else
	{
		m_sLabBefore2 = pszTOCSTRBEFORE;
	}
	pszTOCSTRBEFORE = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-before3",pszTOCSTRBEFORE))
	{
		m_sLabBefore3 = "";
	}
	else
	{
		m_sLabBefore3 = pszTOCSTRBEFORE;
	}
	pszTOCSTRBEFORE = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-before4",pszTOCSTRBEFORE))
	{
		m_sLabBefore4 = "";
	}
	else
	{
		m_sLabBefore4 = pszTOCSTRBEFORE;
	}
//
// TOC Label After Text
//
	const XML_Char * pszTOCSTRAFTER = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-after1",pszTOCSTRAFTER))
	{
		m_sLabAfter1 = "";
	}
	else
	{
		m_sLabAfter1 = pszTOCSTRAFTER;
	}
	pszTOCSTRAFTER = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-after2",pszTOCSTRAFTER))
	{
		m_sLabAfter2 = "";
	}
	else
	{
		m_sLabAfter2 = pszTOCSTRAFTER;
	}
	pszTOCSTRAFTER = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-after2",pszTOCSTRAFTER))
	{
		m_sLabAfter2 = "";
	}
	else
	{
		m_sLabAfter3 = pszTOCSTRAFTER;
	}
	pszTOCSTRAFTER = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-after4",pszTOCSTRAFTER))
	{
		m_sLabAfter4 = "";
	}
	else
	{
		m_sLabAfter4 = pszTOCSTRAFTER;
	}
//
// TOC Label Initial Value
//
	const XML_Char * pszTOCLABELSTART = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-start1",pszTOCLABELSTART))
	{
		m_iStartAt1 = 1;
	}
	else
	{
		m_iStartAt1 = atoi(pszTOCLABELSTART);
	}
	pszTOCLABELSTART = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-start2",pszTOCLABELSTART))
	{
		m_iStartAt2 = 1;
	}
	else
	{
		m_iStartAt2 = atoi(pszTOCLABELSTART);
	}
	pszTOCLABELSTART = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-start3",pszTOCLABELSTART))
	{
		m_iStartAt3 = 1;
	}
	else
	{
		m_iStartAt3 = atoi(pszTOCLABELSTART);
	}
	pszTOCLABELSTART = NULL;
	if(!pSectionAP || !pSectionAP->getProperty("toc-label-start4",pszTOCLABELSTART))
	{
		m_iStartAt4 = 1;
	}
	else
	{
		m_iStartAt4 = atoi(pszTOCLABELSTART);
	}
}

void fl_TOCLayout::_localCollapse(void)
{
	// ClearScreen on our Cell. One Cell per layout.
	fp_TOCContainer *pTC = static_cast<fp_TOCContainer *>(getFirstContainer());
	if (pTC)
	{
		pTC->clearScreen();
	}

	// get rid of all the layout information for every containerLayout
	fl_ContainerLayout*	pCL = getFirstLayout();
	while (pCL)
	{
		pCL->collapse();
		pCL = pCL->getNext();
	}
	m_bNeedsReformat = true;
}

void fl_TOCLayout::collapse(void)
{
	_localCollapse();
	fp_TOCContainer *pTC = static_cast<fp_TOCContainer *>(getFirstContainer());
	if (pTC)
	{
//
// remove it from the linked list.
//
		fp_Container * pPrev = static_cast<fp_Container *>(pTC->getPrev());
		if(pPrev)
		{
			pPrev->setNext(pTC->getNext());
		}
		if(pTC->getNext())
		{
			pTC->getNext()->setPrev(pPrev);
		}
//
// Remove it from the vertical container that contains it.
//
		static_cast<fp_VerticalContainer *>(pTC->getContainer())->removeContainer(pTC);
		pTC->setContainer(NULL);
		delete pTC;
	}
	setFirstContainer(NULL);
	setLastContainer(NULL);
	_purgeLayout();
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

fl_TOCListener::fl_TOCListener(fl_TOCLayout* pTOCL, fl_BlockLayout* pPrevBL, PD_Style * pStyle)
{
	UT_ASSERT(pTOCL);

	m_pDoc = pTOCL->getDocLayout()->getDocument();
	m_pTOCL = pTOCL;
	m_pPrevBL = pPrevBL;
	m_bListening = false;
	m_pCurrentBL = NULL;
	m_pStyle = pStyle;
}

fl_TOCListener::~fl_TOCListener()
{
}

bool fl_TOCListener::populate(PL_StruxFmtHandle sfh,
								 const PX_ChangeRecord * pcr)
{
	if (!m_bListening)
	{
		return true;
	}

	UT_ASSERT(m_pTOCL);
	UT_DEBUGMSG(("fl_TOCListener::populate block %x \n",m_pCurrentBL));

	bool bResult = false;
	FV_View* pView = m_pTOCL->getDocLayout()->getView();
	PT_DocPosition oldPos = 0;
	//
	// We're not printing
	//
	if(pView != NULL)
	{
		oldPos = pView->getPoint();
	}
	switch (pcr->getType())
	{
	case PX_ChangeRecord::PXT_InsertSpan:
	{
		const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

		{
			const fl_Layout * pL = static_cast<const fl_Layout *>(sfh);
			UT_ASSERT(pL->getType() == PTX_Block);
			UT_ASSERT(m_pCurrentBL == (static_cast<const fl_ContainerLayout *>(pL)));
		}
		PT_BlockOffset blockOffset = pcrs->getBlockOffset();
		UT_uint32 len = pcrs->getLength();


		bResult = static_cast<fl_BlockLayout *>(m_pCurrentBL)->doclistener_populateSpan(pcrs, blockOffset, len);
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_InsertObject:
	{
		const PX_ChangeRecord_Object * pcro = static_cast<const PX_ChangeRecord_Object *>(pcr);

		{
			const fl_Layout * pL = static_cast<const fl_Layout *>(sfh);
			UT_ASSERT(pL->getType() == PTX_Block);
			UT_ASSERT(m_pCurrentBL == (static_cast<const fl_ContainerLayout *>(pL)));
		}
		PT_BlockOffset blockOffset = pcro->getBlockOffset();

// sterwill -- is this call to getSectionLayout() needed?  pBLSL is not used.

//			fl_SectionLayout* pBLSL = m_pCurrentBL->getSectionLayout();
		bResult = static_cast<fl_BlockLayout *>(m_pCurrentBL)->doclistener_populateObject(blockOffset,pcro);
		goto finish_up;
	}
	default:
		UT_DEBUGMSG(("Unknown Change record = %d \n",pcr->getType()));
		//
		// We're not printing
		//
		if(pView != NULL)
		{
			pView->setPoint(oldPos);
		}
		return true;
	}

 finish_up:
	//
	// We're not printing
	//
	if(pView != NULL)
	{
		pView->setPoint(oldPos);
	}
	return bResult;
}

bool fl_TOCListener::populateStrux(PL_StruxDocHandle sdh,
									  const PX_ChangeRecord * pcr,
									  PL_StruxFmtHandle * psfh)
{
	UT_ASSERT(m_pTOCL);
	UT_DEBUGMSG(("fl_TOCListener::populateStrux\n"));

	UT_ASSERT(pcr->getType() == PX_ChangeRecord::PXT_InsertStrux);
	PT_AttrPropIndex iTOC = m_pStyle->getIndexAP();
	const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);
	m_bListening = true;
	switch (pcrx->getStruxType())
	{
	case PTX_Block:
	{
		if (m_bListening)
		{
			// append a new BlockLayout to that SectionLayout
			fl_ContainerLayout*	pBL = m_pTOCL->insert(sdh,m_pPrevBL, iTOC,FL_CONTAINER_BLOCK);
			UT_DEBUGMSG(("New TOC block %x created and set as current \n",pBL));
			if (!pBL)
			{
				UT_DEBUGMSG(("no memory for BlockLayout"));
				return false;
			}
			m_pCurrentBL = pBL;
			*psfh = static_cast<PL_StruxFmtHandle>(pBL);
		}

	}
	break;

	default:
		UT_ASSERT(0);
		return false;
	}
	//
	// We're not printing
	//
	return true;
}

bool fl_TOCListener::change(PL_StruxFmtHandle /*sfh*/,
							   const PX_ChangeRecord * /*pcr*/)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return false;
}

bool fl_TOCListener::insertStrux(PL_StruxFmtHandle /*sfh*/,
									const PX_ChangeRecord * /*pcr*/,
									PL_StruxDocHandle /*sdh*/,
									PL_ListenerId /*lid*/,
									void (* /*pfnBindHandles*/)(PL_StruxDocHandle sdhNew,
																PL_ListenerId lid,
																PL_StruxFmtHandle sfhNew))
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return false;
}

bool fl_TOCListener::signal(UT_uint32 /*iSignal*/)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return false;
}


