//============================================================================
// Image2Code - code generator to create an array from an image
// Image2CodeDlg.h: Application setup and user interface
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
#include "afxwin.h"

#include "GroupableCtrls.h"
#include "ArrayWriter.h"

// Child dialog for selections

// CImage2CodeDlg dialog
class CImage2CodeDlg : public CDialog
{
// Construction
public:
    CImage2CodeDlg(CWnd* pParent = NULL);	// standard constructor
    ~CImage2CodeDlg();

// Dialog Data
    enum { IDD = IDD_IMAGE2CODE_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
    HICON m_hIcon;
    unsigned long m_gdiplusToken;
    Bitmap* m_image;

    void Reset();
    void WriteImageCode();
    void LoadImage(CString& filename);
    void UpdateThumb();
    void Disable1BitAllBut(int orient);

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnDropFiles(HDROP hDropInfo);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedPropertyChange();
    afx_msg void OnBnClickedPropertyChangeL2R();
    afx_msg void OnBnClickedPropertyChangeR2L();
    afx_msg void OnBnClickedPropertyChangeT2B();
    afx_msg void OnBnClickedPropertyChangeB2T();
    afx_msg void OnBnClickedPropertyChangePadLSB();
    afx_msg void OnBnClickedPropertyChangePadMSB();
    afx_msg void OnBnClickedOrientationChange();
    afx_msg void OnBnClickedFileBrowse();
    afx_msg void OnBnClickedConvert();
    afx_msg void OnBnClickedCancel();
protected:

    Groupable_Ctrl_Group m_ctrlg_1bit;
    Groupable_Ctrl_Group m_ctrlg_1bit_vert_up;
    Groupable_Ctrl_Group m_ctrlg_1bit_vert_down;
    Groupable_Ctrl_Group m_ctrlg_1bit_horiz_left;
    Groupable_Ctrl_Group m_ctrlg_1bit_horiz_right;
    Groupable_Ctrl_Group m_ctrlg_1bit_vert_up_padding;
    Groupable_Ctrl_Group m_ctrlg_1bit_vert_down_padding;
    Groupable_Ctrl_Group m_ctrlg_1bit_horiz_left_padding;
    Groupable_Ctrl_Group m_ctrlg_1bit_horiz_right_padding;
    Groupable_Ctrl_Group m_ctrlg_8bit;
    Groupable_Ctrl_Group m_ctrlg_none;
    Groupable_Ctrl_Group m_ctrlg_common;

    CString m_filename;
    BOOL m_invert;
    BOOL m_right2left;
    BOOL m_bottom2top;
    BOOL m_pad_msb;
    int m_orientation;
    CStatic m_view;
    int m_array_writer;
    ArrayWriters m_writers;
    char m_bpp;
};
