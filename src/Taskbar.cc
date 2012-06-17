// Taskbar.cc for Fluxbox Window Manager
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

#include "Application.hh"
#include "Taskbar.hh"
#include "IconButton.hh"
#include "FbTk/MemFun.hh"
#include "FbTk/Image.hh"
#include "WorkspaceCmd.hh"

using std::cout;
using std::endl;
AppIcon::AppIcon(BScreen &scr, unsigned int app_num, FbExt::Application &app):
		Focusable(scr),
		m_app_num(app_num),
		m_app(app),
		m_rc_icontheme(scr.resourceManager(), "", "taskbar.iconTheme", "Taskbar.IconTheme") {
}

void AppIcon::setFocusFlag(bool flag) {
	m_focused = flag;
	notifyFocusChanged();
}

bool AppIcon::focus() {
	JumpToAppCmd(m_app_num).execute();
}

bool AppIcon::window_of(WinClient *client) {
	return(m_app.window_of(client));
}

Taskbar::Taskbar(const FbTk::FbWindow &parent, IconbarTheme &theme,
                         FbTk::ThemeProxy<IconbarTheme> &focused_theme,
                         FbTk::ThemeProxy<IconbarTheme> &unfocused_theme,
                         BScreen &screen, FbTk::Menu &menu, FbExt::ApplicationsList &app_list):
	IconbarTool(parent, theme, focused_theme, unfocused_theme, screen, menu,
			ToolbarItem::FIXED,
			IconbarTool::taskbar),
	m_parent(parent),
	m_applications_list(app_list) {
	FbExt::ApplicationsList::iterator it;
	for(it = m_applications_list.begin(); it != m_applications_list.end(); it++) {
    		// IconButton *button = insertItem(AppIcon(screen, *(*it)), std::distance(m_applications_list.begin(), it));
		unsigned int index = std::distance(m_applications_list.begin(), it);
		AppIcon *app_icon = new AppIcon(screen, index, *(*it));
		m_icons[index] = app_icon;
        	IconButton *button = makeButton(*app_icon);
		cout << "button=" << button << endl;
		m_icon_container.insertItem(button, index);
	}
        m_tracker.join(screen.focusedWindowSig(),
             FbTk::MemFun(*this, &Taskbar::focusedWindowChanged));
}

void Taskbar::focusedWindowChanged(BScreen &screen,
                                         FluxboxWindow *focused_win,
                                         WinClient *client) {
	cout << "focus changed" << endl;
	AppIconMap::iterator it;
	for(it = m_icons.begin(); it != m_icons.end(); it++) {
		AppIcon *app = it->second;
		app->setFocusFlag(app->window_of(client));
	}
}

unsigned int Taskbar::width() const {
	unsigned int w = m_applications_list.size();
	if(!w) w = 3;
	return(m_parent.height() * w);
}

Taskbar::~Taskbar() {
}

void Taskbar::handle_update(UpdateReason reason, Focusable *win) {
    cout << "reason=" << reason << endl;
    if(reason == LIST_ADD || reason == LIST_ORDER) {
    }
}
