// Application.cc for Fluxbox Window Manager
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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <algorithm>
#include "Application.hh"
#include "WinClient.hh"
#include "FbTk/StringUtil.hh"

namespace FbExt {

using std::cout;
using std::endl;

Application::~Application() {
}

Application::Application(std::string executable="", std::string name="", std::string icon_path=""):
	m_executable(executable),
	m_name(name),
	m_icon_path(icon_path),
	clientIndex(0) {
}

Application *parseAppSpec(std::string spec) {
	std::cout << "spec=" << spec << std::endl;
	int pos = 0;
	std::string pair, name, exec, icon;
	while((pos = FbTk::StringUtil::getStringBetween(pair, spec.substr(pos).c_str(), '(', ')')) > 0) {
		size_t separator = pair.find('=');
		if(separator != std::string::npos) {
			std::string key = pair.substr(0, separator);
			std::string value = pair.substr(separator+1);
			if(key == "name")
				name = value;
			else if(key == "exec")
				exec = value;
			else if(key == "icon")
				icon = value;
		}
	}
	if(exec.empty()) exec = name;
	return(new Application(exec, name, icon));
}

bool Application::match(Application *app) {
	return(this->get_executable() == app->get_executable());
}

void Application::cycleAppWindows(bool reverse) {
	cout<<"cycleAppWindows"<<endl;
	if(!m_client_list.size()) return;
	clientIndex = (clientIndex+1) % m_client_list.size();
	WinClient *client = m_client_list[clientIndex];
	client->focus();
	if(client->fbwindow())
			client->fbwindow()->raise();
}

void Application::setCurrentClient(WinClient *client) {
	std::vector<WinClient*>::iterator it = find(m_client_list.begin(), m_client_list.end(), client);
	if(it == m_client_list.end())
		return;
	clientIndex = std::distance(m_client_list.begin(), it);
}

bool Application::focus() {
	//cout << "executable=" << executable << endl;
}

bool Application::isFocused() {
	return(true);
}

unsigned long getWindowPid(Display *display, Window w) {
	static Atom atomPid = XInternAtom(display, "_NET_WM_PID", True);
	Atom type;
	int format;
	unsigned long n_items;
	unsigned long bytes_after;
	unsigned char *data;
	XGetWindowProperty(display, w, atomPid, 0, 1, False,
				XA_CARDINAL,
				&type,
				&format,
				&n_items,
				&bytes_after,
				&data);
	if(!data)
		return(0);
	return(*((unsigned long*)data));
}

std::string getExecutableByPid(unsigned long pid) {
	std::string str;
	char filename[64];
	sprintf(filename, "/proc/%ld/comm", pid);
	std::ifstream file(filename, std::ios::in);
	std::getline(file, str);
	return(str);
}

std::string getExecutableByWindow(Display *display, Window w) {
	unsigned long pid = getWindowPid(display, w);
	std::cout << "pid: " << pid << std::endl;
	return(getExecutableByPid(pid));
}

void Application::add_client(WinClient *client) { 
	m_client_list.push_back(client);
	setCurrentClient(client);
}
void Application::remove_client(WinClient *client) {
	std::vector<WinClient*>::iterator it = std::find(m_client_list.begin(), m_client_list.end(), client);
	if(clientIndex && (clientIndex >= std::distance(m_client_list.begin(), it)))
		clientIndex--;
	if(it != m_client_list.end())
		m_client_list.erase(it);
}

WinClient *Application::winClient() {
	return (m_client_list.size() > clientIndex ? m_client_list[clientIndex] : NULL);
}

bool Application::window_of(WinClient *client) {
	std::vector<WinClient*>::iterator it = std::find(m_client_list.begin(), m_client_list.end(), client);
	return(it != m_client_list.end());
}

bool isWindowOf(Application *app, const std::string &s) {
	return(app->get_name() == s);
}
}
