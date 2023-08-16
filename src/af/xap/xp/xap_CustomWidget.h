/* AbiSource Application Framework
 * Copyright (C) 2010 Patrik Fimml
 * Copyright (C) 2021-2022 Hubert Figuière
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

#pragma once

#include <optional>
#include <queue>

#include "xap_Drawable.h"

/* utility class for widgets drawing in layout units */
class ABI_EXPORT XAP_CustomWidget
    : public XAP_Drawable
{
public:
    virtual void queueDrawLU(const UT_Rect* clip);
    virtual void drawImmediate(const UT_Rect* clip) override;
    virtual void queueDraw(const UT_Rect *clip = nullptr) override;

    virtual void drawImmediateLU(const UT_Rect* clip) = 0;
private:
    std::queue<std::optional<UT_Rect>> m_drawQueue;
};
