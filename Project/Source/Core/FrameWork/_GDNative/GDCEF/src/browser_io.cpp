//*************************************************************************
// Stigmee: The art to sanctuarize knowledge exchanges.
// Copyright 2021-2022 Alain Duron <duron.alain@gmail.com>
// Copyright 2021-2022 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of Stigmee.
//
// Stigmee is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//*************************************************************************

#include "gdbrowser.hpp"
#include "helper.hpp"

//------------------------------------------------------------------------------
void GDBrowserView::leftClick()
{
    leftMouseDown();
    leftMouseUp();
}

//------------------------------------------------------------------------------
void GDBrowserView::rightClick()
{
    rightMouseDown();
    rightMouseUp();
}

//------------------------------------------------------------------------------
void GDBrowserView::middleClick()
{
    middleMouseDown();
    middleMouseUp();
}

//------------------------------------------------------------------------------
void GDBrowserView::leftMouseDown()
{
    if (!m_browser)
        return;

    CefBrowserHost::MouseButtonType btn = CefBrowserHost::MouseButtonType::MBT_LEFT;
    CefMouseEvent evt;
    evt.x = m_mouse_x;
    evt.y = m_mouse_y;

    m_browser->GetHost()->SendMouseClickEvent(evt, btn, false, 1);
}

//------------------------------------------------------------------------------
void GDBrowserView::rightMouseDown()
{
    if (!m_browser)
        return;

    CefBrowserHost::MouseButtonType btn = CefBrowserHost::MouseButtonType::MBT_RIGHT;
    CefMouseEvent evt;
    evt.x = m_mouse_x;
    evt.y = m_mouse_y;

    m_browser->GetHost()->SendMouseClickEvent(evt, btn, false, 1);
}

//------------------------------------------------------------------------------
void GDBrowserView::leftMouseUp()
{
    if (!m_browser)
        return;

    CefBrowserHost::MouseButtonType btn = CefBrowserHost::MouseButtonType::MBT_LEFT;
    CefMouseEvent evt;
    evt.x = m_mouse_x;
    evt.y = m_mouse_y;

    m_browser->GetHost()->SendMouseClickEvent(evt, btn, true, 1);
}

//------------------------------------------------------------------------------
void GDBrowserView::rightMouseUp()
{
    if (!m_browser)
        return;

    CefBrowserHost::MouseButtonType btn = CefBrowserHost::MouseButtonType::MBT_RIGHT;
    CefMouseEvent evt;
    evt.x = m_mouse_x;
    evt.y = m_mouse_y;

    m_browser->GetHost()->SendMouseClickEvent(evt, btn, true, 1);
}

//------------------------------------------------------------------------------
void GDBrowserView::middleMouseDown()
{
    if (!m_browser)
        return;

    CefBrowserHost::MouseButtonType btn = CefBrowserHost::MouseButtonType::MBT_MIDDLE;
    CefMouseEvent evt;
    evt.x = m_mouse_x;
    evt.y = m_mouse_y;

    m_browser->GetHost()->SendMouseClickEvent(evt, btn, false, 1);
}

//------------------------------------------------------------------------------
void GDBrowserView::middleMouseUp()
{
    if (!m_browser)
        return;

    CefBrowserHost::MouseButtonType btn = CefBrowserHost::MouseButtonType::MBT_MIDDLE;
    CefMouseEvent evt;
    evt.x = m_mouse_x;
    evt.y = m_mouse_y;

    m_browser->GetHost()->SendMouseClickEvent(evt, btn, true, 1);
}

//------------------------------------------------------------------------------
void GDBrowserView::mouseMove(int x, int y)
{
    if (!m_browser)
        return ;

    m_mouse_x = x;
    m_mouse_y = y;

    CefMouseEvent evt;
    evt.x = x;
    evt.y = y;

    bool mouse_leave = false; // TODO
    // AD - Adding focus just like what's done in BLUI
    auto host = m_browser->GetHost();
    host->SetFocus(true);
    host->SendMouseMoveEvent(evt, mouse_leave);
}

//------------------------------------------------------------------------------
void GDBrowserView::mouseWheel(const int wDelta)
{
    if (m_browser == nullptr)
        return ;

    CefMouseEvent evt;
    evt.x = m_mouse_x;
    evt.y = m_mouse_y;

    m_browser->GetHost()->SendMouseWheelEvent(evt, wDelta * 10, wDelta * 10);
}

//------------------------------------------------------------------------------
static uint32_t getKeyboardModifiers(bool shift, bool alt, bool ctrl)
{
    uint32_t modifiers = 0;

    if (shift == true)
        modifiers |= EVENTFLAG_SHIFT_DOWN;
    if (ctrl == true)
        modifiers |= EVENTFLAG_CONTROL_DOWN;
    if (alt == true)
        modifiers |= EVENTFLAG_ALT_DOWN;

    return modifiers;
}

//------------------------------------------------------------------------------
void GDBrowserView::keyPress(int key, bool pressed, bool shift, bool alt, bool ctrl)
{
    if (!m_browser)
        return;

    CefKeyEvent event;
    char16 key16b = char16(key);
    if (pressed == true)
    {
        // set the event modifier if they are activated
        event.modifiers = getKeyboardModifiers(shift, alt, ctrl);

        if ((key >= 32) && (key <= 126)) // ASCII
        {
            BROWSER_DEBUG_VAL("ASCII CODE");
            event.windows_key_code = key;
            event.character = key16b;
            event.unmodified_character = key16b;
            event.type = KEYEVENT_CHAR;
            m_browser->GetHost()->SendKeyEvent(event);
        }
        else if (key == godot::GlobalConstants::KEY_SPACE ||
                 key == godot::GlobalConstants::KEY_TAB)
        {
            BROWSER_DEBUG_VAL("KEY_SPACE / KEY_TAB");
            event.windows_key_code = key;
            event.character = key16b;
            event.native_key_code = key;
            event.type = KEYEVENT_RAWKEYDOWN;
            m_browser->GetHost()->SendKeyEvent(event);
            event.type = KEYEVENT_CHAR;
            m_browser->GetHost()->SendKeyEvent(event);
        }
        else if (key == godot::GlobalConstants::KEY_BACKSPACE ||
                 key == godot::GlobalConstants::KEY_ENTER ||
                 key == godot::GlobalConstants::KEY_KP_ENTER )
        {
            if (key == godot::GlobalConstants::KEY_BACKSPACE) {
                BROWSER_DEBUG_VAL("KEY_BACKSPACE");
                event.windows_key_code = 8;
                event.character = 8;
                event.unmodified_character = 8;
            }
            else if (key == godot::GlobalConstants::KEY_ENTER) {
                BROWSER_DEBUG_VAL("KEY_ENTER");
                event.windows_key_code = 13;
                event.character = 13;
                event.unmodified_character = 13;
            }
            else if (key == godot::GlobalConstants::KEY_KP_ENTER) {
                BROWSER_DEBUG_VAL("KEY_KP_ENTER");
                event.windows_key_code = 13;
                event.character = 13;
                event.unmodified_character = 13;
            }

            event.character = char16(event.windows_key_code);
            event.native_key_code = event.windows_key_code;
            event.type = KEYEVENT_KEYDOWN;
            m_browser->GetHost()->SendKeyEvent(event);
            event.type = KEYEVENT_CHAR;
            m_browser->GetHost()->SendKeyEvent(event);
        }
        else if (key >= 320 && key <= 329) // NUMBERS & NUMPAD
        {
            BROWSER_DEBUG_VAL("NUMBERS and NUMPAD");
            event.windows_key_code = key;
            event.character = key16b;
            event.native_key_code = key;

            event.type = KEYEVENT_KEYDOWN;
            m_browser->GetHost()->SendKeyEvent(event);
            event.type = KEYEVENT_CHAR;
            m_browser->GetHost()->SendKeyEvent(event);
        }
        else if (key == godot::GlobalConstants::KEY_RIGHT ||
                 key == godot::GlobalConstants::KEY_LEFT ||
                 key == godot::GlobalConstants::KEY_UP ||
                 key == godot::GlobalConstants::KEY_DOWN ||
                 key == godot::GlobalConstants::KEY_PAGEUP ||
                 key == godot::GlobalConstants::KEY_PAGEDOWN ||
                 key == godot::GlobalConstants::KEY_HOME ||
                 key == godot::GlobalConstants::KEY_END ||
                 key == godot::GlobalConstants::KEY_INSERT ||
                 key == godot::GlobalConstants::KEY_DELETE) // ARROWS
        {
            // https://keycode.info/

            if (key == godot::GlobalConstants::KEY_RIGHT) {
                BROWSER_DEBUG_VAL("KEY_RIGHT");
                event.windows_key_code = 39;
            }
            else if (key == godot::GlobalConstants::KEY_LEFT) {
                BROWSER_DEBUG_VAL("KEY_LEFT");
                event.windows_key_code = 37;
            }
            else if (key == godot::GlobalConstants::KEY_UP) {
                BROWSER_DEBUG_VAL("KEY_UP");
                event.windows_key_code = 38;
            }
            else if (key == godot::GlobalConstants::KEY_DOWN) {
                BROWSER_DEBUG_VAL("KEY_DOWN");
                event.windows_key_code = 40;
            }
            else if (key == godot::GlobalConstants::KEY_PAGEUP) {
                BROWSER_DEBUG_VAL("KEY_PAGEUP");
                event.windows_key_code = 33;
            }
            else if (key == godot::GlobalConstants::KEY_PAGEDOWN) {
                BROWSER_DEBUG_VAL("KEY_PAGEDOWN");
                event.windows_key_code = 34;
            }
            else if (key == godot::GlobalConstants::KEY_HOME) {
                BROWSER_DEBUG_VAL("KEY_HOME");
                event.windows_key_code = 36; // Debut
            }
            else if (key == godot::GlobalConstants::KEY_END) {
                BROWSER_DEBUG_VAL("KEY_END");
                event.windows_key_code = 35; // Fin
            }
            else if (key == godot::GlobalConstants::KEY_INSERT) {
                BROWSER_DEBUG_VAL("KEY_INSERT");
                event.windows_key_code = 45; // Insert
            }
            else if (key == godot::GlobalConstants::KEY_DELETE) {
                BROWSER_DEBUG_VAL("KEY_DELETE");
                event.windows_key_code = 46; // Del (not dot when no char event)
            }

            event.type = KEYEVENT_KEYDOWN;
            event.character = char16(event.windows_key_code);
            event.native_key_code = event.windows_key_code;
            m_browser->GetHost()->SendKeyEvent(event);
        }
        else
        {
            BROWSER_DEBUG_VAL("Any Char");
            event.windows_key_code = key;
            event.character = key16b;
            event.native_key_code = key;
            event.unmodified_character = key16b;

            event.type = KEYEVENT_KEYDOWN;
            m_browser->GetHost()->SendKeyEvent(event);
            event.type = pressed ? KEYEVENT_CHAR : KEYEVENT_KEYUP;
            m_browser->GetHost()->SendKeyEvent(event);
        }
    }
    else
    {
        BROWSER_DEBUG_VAL("PRESSED FALSE");
        event.native_key_code |= int(0xC0000000);
        event.type = KEYEVENT_KEYUP;
        m_browser->GetHost()->SendKeyEvent(event);
    }
}
