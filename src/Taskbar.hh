// Taskbar.hh for Fluxbox Window Manager
// Copyright (c) 2012 Valentin Dudouyt (valentin.dudouyt at gmail dot com)
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef TASKBAR_HH
#define TASKBAR_HH

#include "ToolbarItem.hh"
#include "FbMenu.hh"

#include "FbTk/Container.hh"
#include "FbTk/CachedPixmap.hh"
#include "FbTk/Resource.hh"
#include "FbTk/Button.hh"
#include "IconbarTool.hh"
#include "Focusable.hh"
#include "Toolbar.hh"
#include "Application.hh"

class AppIcon: public Focusable {
public:
	AppIcon(BScreen &scr, unsigned int app_num, FbExt::Application &app);
	~AppIcon() {}
	virtual bool focus();
	void setFocusFlag(bool flag);
	bool window_of(WinClient *client);
	unsigned int m_app_num;
private:
	std::string resolve_icon_path(std::string icon_theme, std::string icon_name);
	FbExt::Application &m_app;
        FbTk::Resource<std::string> m_rc_icontheme;
};

class Taskbar: public IconbarTool {
public:
    Taskbar(const FbTk::FbWindow &parent, IconbarTheme &theme,
                FbTk::ThemeProxy<IconbarTheme> &focused_theme,
                FbTk::ThemeProxy<IconbarTheme> &unfocused_theme,
                BScreen &screen, FbTk::Menu &menu, FbExt::ApplicationsList &app_list);
    ~Taskbar();
    unsigned int width() const;
private:
	typedef std::map<unsigned int, AppIcon*> AppIconMap;
	const FbTk::FbWindow &m_parent;
	FbExt::ApplicationsList &m_applications_list;
    	virtual void handle_update(UpdateReason reason, Focusable *win);
	void focusedWindowChanged(BScreen &screen, FluxboxWindow *focused_win, WinClient *client);
	FbTk::SignalTracker m_tracker;
	AppIconMap m_icons;
};
#endif
