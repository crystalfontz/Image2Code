//============================================================================
// Image2Code - code generator to create an array from an image
// GroupableCtrls.h: Encapsulate dialog controls into groups that can include each other
// Copyright 2008, Crystalfontz America, Inc. Written by Rob Emanuele
// http://www.crystalfontz.com, rje@crystalfontz.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//============================================================================

#pragma once
#include <list>

class Groupable_Ctrl_Base
{
public:
    virtual ~Groupable_Ctrl_Base() {}
    static void ID_Show(CWnd* wnd, DWORD id, bool show);
    static void ID_Enable(CWnd* wnd, DWORD id, bool enable);
    virtual void Show(CWnd* wnd, bool show) = 0;
    virtual void Enable(CWnd* wnd, bool enable) = 0;
};

typedef std::list<Groupable_Ctrl_Base*> Groupable_Ctrl_List_t;

class Groupable_Ctrl : public Groupable_Ctrl_Base
{
private:
    DWORD m_id;
public:
    Groupable_Ctrl(DWORD id) : m_id(id) {}
    void Show(CWnd* wnd, bool show) { ID_Show(wnd,m_id,show); }
    void Enable(CWnd* wnd, bool enable) { ID_Enable(wnd,m_id,enable); }
};

class Groupable_Ctrl_Group : public Groupable_Ctrl_Base, public std::list<Groupable_Ctrl_Base*>
{
public:
    Groupable_Ctrl_Group() {}
    void Show(CWnd* wnd, bool show);
    void Enable(CWnd* wnd, bool enable);
};
