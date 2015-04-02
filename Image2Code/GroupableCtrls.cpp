//============================================================================
// Image2Code - code generator to create an array from an image
// GroupableCtrls.cpp: Encapsulate dialog controls into groups that can include each other
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

#include "stdafx.h"
#include "GroupableCtrls.h"

void Groupable_Ctrl_Base::ID_Show(CWnd* wnd, DWORD id, bool show)
{
    wnd->GetDlgItem(id)->ShowWindow(show?SW_SHOW:SW_HIDE);
}

void Groupable_Ctrl_Base::ID_Enable(CWnd* wnd, DWORD id, bool enable)
{
    wnd->GetDlgItem(id)->EnableWindow(enable);
}

void Groupable_Ctrl_Group::Show(CWnd* wnd, bool show)
{
    Groupable_Ctrl_List_t::iterator it = begin();
    while (it != end()) {
        (*it)->Show(wnd,show);
        it++;
    }
}

void Groupable_Ctrl_Group::Enable(CWnd* wnd, bool enable)
{
    Groupable_Ctrl_List_t::iterator it = begin();
    while (it != end()) {
        (*it)->Enable(wnd,enable);
        it++;
    }
}
