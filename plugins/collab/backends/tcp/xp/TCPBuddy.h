/* Copyright (C) 2006 by Marc Maurer <uwog@uwog.net>
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

#ifndef __TCPBUDDY_H__
#define __TCPBUDDY_H__

#include <map>
#include <string>
#include <memory>
#include "ut_string_class.h"
#include <core/account/xp/Buddy.h>
#include <core/account/xp/DocTreeItem.h>
#include <core/account/xp/AccountHandler.h>

class DocHandle;

class TCPBuddy : public Buddy
{
public:
	TCPBuddy(AccountHandler* handler, const std::string& address, const std::string& port)
		: Buddy(handler),
		m_address(address),
		m_port(port)
	{
		setVolatile(true);
	}

	virtual UT_UTF8String getDescriptor(bool /*include_session_info = false*/) const override
	{
		return UT_UTF8String("tcp://") + m_address.c_str() + UT_UTF8String(":") + m_port.c_str();
	}

	virtual UT_UTF8String getDescription() const override
	{
		return UT_UTF8String(m_address.c_str()) + UT_UTF8String(":") + m_port.c_str();
	}

	const std::string& getAddress() const
		{ return m_address; }

	const std::string& getPort() const
		{ return m_port; }

	virtual const DocTreeItem* getDocTreeItems() const override
	{
		const std::vector<DocHandle*>& docHandles = getDocHandles();
		DocTreeItem* first = nullptr;
		DocTreeItem* prev = nullptr;
		for (std::vector<DocHandle*>::const_iterator pos = docHandles.begin(); pos != docHandles.end(); pos++)
		{
			DocTreeItem* item = new DocTreeItem();
			item->m_type = DOCTREEITEM_TYPE_DOCUMENT;
			item->m_docHandle = *pos;
			item->m_child = nullptr;
			item->m_next = nullptr;

			if (!first)
				first = item;
			if (prev)
				prev->m_next = item;
			prev = item;
		}
		return first;
	}

private:
	std::string m_address;
	std::string m_port;
};

typedef std::shared_ptr<TCPBuddy> TCPBuddyPtr;

#endif /* TCPBUDDY_H */
