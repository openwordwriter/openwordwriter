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


// ********************************************************************************
// ********************************************************************************
// *** THIS FILE DEFINES THE BINDINGS TO HANG OFF THE DeadMacron PREFIX KEY IN   ***
// *** THE DEFAULT BINDINGS TABLE.                                              ***
// ********************************************************************************
// ********************************************************************************

#include "ut_assert.h"
#include "ut_types.h"
#include "ev_EditBits.h"
#include "ev_EditBinding.h"
#include "ev_EditMethod.h"
#include "ev_NamedVirtualKey.h"

#define NrElements(a)	((sizeof(a)/sizeof(a[0])))

#define _S		| EV_EMS_SHIFT
#define _C		| EV_EMS_CONTROL
#define _A		| EV_EMS_ALT

/*****************************************************************
******************************************************************
** load bindings for the non-nvk
** (character keys).  note that SHIFT-state is implicit in the
** character value and is not included in the table.  note that
** we do not include the ASCII control characters (\x00 -- \x1f
** and others) since we don't map keystrokes into them.
******************************************************************
*****************************************************************/

struct _iChar
{
	EV_EditBits			m_eb;			// sans ems & shift
	const char *		m_szMethod[EV_COUNT_EMS_NoShift];
};

// TODO finish filling out this table.
// TODO ?? do we ever need _C, _A, _A_C bindings for these
// TODO ?? or can we collapse the width of the table ??

static struct _iChar s_CharTable[] =
{
//	{char, /* desc   */ { none,						_C,		_A,		_A_C	}},
#if 0
	// TODO add these Latin-4 characters when we
	// TODO fix the char widths calculations.
	{0x41, /* A      */ { "insertMacronData",		"",		"",		""		}},
	{0x45, /* E      */ { "insertMacronData",		"",		"",		""		}},
	{0x49, /* I      */ { "insertMacronData",		"",		"",		""		}},
	{0x4f, /* O      */ { "insertMacronData",		"",		"",		""		}},
	{0x55, /* U      */ { "insertMacronData",		"",		"",		""		}},
	{0x61, /* a      */ { "insertMacronData",		"",		"",		""		}},
	{0x65, /* e      */ { "insertMacronData",		"",		"",		""		}},
	{0x69, /* i      */ { "insertMacronData",		"",		"",		""		}},
	{0x6f, /* o      */ { "insertMacronData",		"",		"",		""		}},
	{0x75, /* u      */ { "insertMacronData",		"",		"",		""		}},
#else
	0
#endif
};

static void s_loadChar(EV_EditMethodContainer * /*pemc*/, EV_EditBindingMap * pebm)
{
	int k, m;
	int kLimit = NrElements(s_CharTable);

	for (k=0; k<kLimit; k++)
		for (m=0; m<EV_COUNT_EMS_NoShift; m++)
			if (s_CharTable[k].m_szMethod[m] && *s_CharTable[k].m_szMethod[m])
			{
				EV_EditModifierState ems = EV_EMS_FromNumberNoShift(m);
				pebm->setBinding(EV_EKP_PRESS|s_CharTable[k].m_eb|ems,s_CharTable[k].m_szMethod[m]);
			}
}

/*****************************************************************
******************************************************************
** put it all together and load the default bindings.
******************************************************************
*****************************************************************/

UT_Bool ap_LoadBindings_DeadMacron(EV_EditMethodContainer * pemc,
								   EV_EditBindingMap **ppebm)
{
	UT_ASSERT(pemc);
	UT_ASSERT(ppebm);

	*ppebm = 0;
	EV_EditBindingMap * pNewEBM = new EV_EditBindingMap(pemc);
	if (!pNewEBM)
		return UT_FALSE;

	s_loadChar(pemc,pNewEBM);

	*ppebm = pNewEBM;
	return UT_TRUE;
}
