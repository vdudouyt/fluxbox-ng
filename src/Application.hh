// Application.hh for Fluxbox Window Manager
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

#ifndef __APPLICATION_HH
#define __APPLICATION_HH

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

class WinClient;

namespace FbExt {
class Application  {
	public:
	Application(std::string executable, std::string name, std::string icon);
	~Application();
	bool window_of(WinClient *client);
	void read_desktop_file(std::string fileName);
	std::string get_executable() const { return m_executable; }
	std::string get_name() const { return m_name; }
	std::string get_icon_path() const { return m_icon_path; }
	bool match(Application *app);
	void add_client(WinClient *client);
	void remove_client(WinClient *client); // called in ~WinClient
	WinClient *winClient(); // returns a currently selected client
	void cycleAppWindows(bool reverse);
	void setCurrentClient(WinClient *client);
	// called by AppIcon
	virtual bool focus();
	virtual bool isFocused();

	private:
	std::string m_executable, m_name, m_icon_path;
	std::string m_icon_theme, m_icon_size;
	Display *display;
	typedef std::map<std::string, std::string> Options;
	Options options;
	std::vector<WinClient*> m_client_list;
	unsigned int clientIndex;
};

unsigned long getWindowPid(Display *display, Window w);
std::string getExecutableByPid(unsigned long pid);
std::string getExecutableByWindow(Display *display, Window w);

typedef std::vector<Application*> ApplicationsList;
ApplicationsList readApplicationsList(std::string fileName);
bool isWindowOf(Application *app, const std::string& s);
Application *parseAppSpec(std::string spec);
}

#endif
