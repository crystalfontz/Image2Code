//============================================================================
// Image2Code - code generator to create an array from an image
// Image2CodeDlg.cpp: Application setup and user interface
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
#include "Image2Code.h"
#include "Image2CodeDlg.h"
#include "gdiplus.h"
#include "ImageArray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    enum { IDD = IDD_ABOUTBOX };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


void CImage2CodeDlg::Reset()
{
    m_filename.LoadString(IDS_IMAGE_NONE);
    m_orientation = 0;
    m_invert = TRUE;
    m_right2left = 0;
    m_bottom2top = 0;
    m_pad_msb = 0;
    m_array_writer = 0;

    delete m_image;
    m_image = NULL;

    m_ctrlg_1bit.Show(this,false);
    m_ctrlg_8bit.Show(this,false);
    m_ctrlg_common.Show(this,false);
}


// CImage2CodeDlg dialog
CImage2CodeDlg::CImage2CodeDlg(CWnd* pParent /*=NULL*/)
: CDialog(CImage2CodeDlg::IDD, pParent), m_image(NULL)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_ctrlg_common.push_back(new Groupable_Ctrl(IDC_INVERT));
    m_ctrlg_common.push_back(new Groupable_Ctrl(ID_CONVERT));
    m_ctrlg_common.push_back(new Groupable_Ctrl(IDC_ARRAY_WRITER));

    m_ctrlg_1bit_vert_up.push_back(new Groupable_Ctrl(IDC_LEFT_TO_RIGHT1));
    m_ctrlg_1bit_vert_up.push_back(new Groupable_Ctrl(IDC_RIGHT_TO_LEFT1));
    m_ctrlg_1bit_vert_up.push_back(new Groupable_Ctrl(IDC_TOP_TO_BOTTOM1));
    m_ctrlg_1bit_vert_up.push_back(new Groupable_Ctrl(IDC_BOTTOM_TO_TOP1));
    m_ctrlg_1bit_vert_up_padding.push_back(new Groupable_Ctrl(IDC_PAD_LSB1));
    m_ctrlg_1bit_vert_up_padding.push_back(new Groupable_Ctrl(IDC_PAD_MSB1));
    m_ctrlg_1bit_vert_up.push_back(&m_ctrlg_1bit_vert_up_padding);

    m_ctrlg_1bit_vert_down.push_back(new Groupable_Ctrl(IDC_LEFT_TO_RIGHT2));
    m_ctrlg_1bit_vert_down.push_back(new Groupable_Ctrl(IDC_RIGHT_TO_LEFT2));
    m_ctrlg_1bit_vert_down.push_back(new Groupable_Ctrl(IDC_TOP_TO_BOTTOM2));
    m_ctrlg_1bit_vert_down.push_back(new Groupable_Ctrl(IDC_BOTTOM_TO_TOP2));
    m_ctrlg_1bit_vert_down_padding.push_back(new Groupable_Ctrl(IDC_PAD_LSB2));
    m_ctrlg_1bit_vert_down_padding.push_back(new Groupable_Ctrl(IDC_PAD_MSB2));
    m_ctrlg_1bit_vert_down.push_back(&m_ctrlg_1bit_vert_down_padding);

    m_ctrlg_1bit_horiz_left.push_back(new Groupable_Ctrl(IDC_LEFT_TO_RIGHT3));
    m_ctrlg_1bit_horiz_left.push_back(new Groupable_Ctrl(IDC_RIGHT_TO_LEFT3));
    m_ctrlg_1bit_horiz_left.push_back(new Groupable_Ctrl(IDC_TOP_TO_BOTTOM3));
    m_ctrlg_1bit_horiz_left.push_back(new Groupable_Ctrl(IDC_BOTTOM_TO_TOP3));
    m_ctrlg_1bit_horiz_left_padding.push_back(new Groupable_Ctrl(IDC_PAD_LSB3));
    m_ctrlg_1bit_horiz_left_padding.push_back(new Groupable_Ctrl(IDC_PAD_MSB3));
    m_ctrlg_1bit_horiz_left.push_back(&m_ctrlg_1bit_horiz_left_padding);

    m_ctrlg_1bit_horiz_right.push_back(new Groupable_Ctrl(IDC_LEFT_TO_RIGHT4));
    m_ctrlg_1bit_horiz_right.push_back(new Groupable_Ctrl(IDC_RIGHT_TO_LEFT4));
    m_ctrlg_1bit_horiz_right.push_back(new Groupable_Ctrl(IDC_TOP_TO_BOTTOM4));
    m_ctrlg_1bit_horiz_right.push_back(new Groupable_Ctrl(IDC_BOTTOM_TO_TOP4));
    m_ctrlg_1bit_horiz_right_padding.push_back(new Groupable_Ctrl(IDC_PAD_LSB4));
    m_ctrlg_1bit_horiz_right_padding.push_back(new Groupable_Ctrl(IDC_PAD_MSB4));
    m_ctrlg_1bit_horiz_right.push_back(&m_ctrlg_1bit_horiz_right_padding);

    m_ctrlg_1bit.push_back(new Groupable_Ctrl(IDC_VERT_UP));
    m_ctrlg_1bit.push_back(&m_ctrlg_1bit_vert_up);
    m_ctrlg_1bit.push_back(new Groupable_Ctrl(IDC_VERT_DOWN));
    m_ctrlg_1bit.push_back(&m_ctrlg_1bit_vert_down);
    m_ctrlg_1bit.push_back(new Groupable_Ctrl(IDC_HORIZ_LEFT));
    m_ctrlg_1bit.push_back(&m_ctrlg_1bit_horiz_left);
    m_ctrlg_1bit.push_back(new Groupable_Ctrl(IDC_HORIZ_RIGHT));
    m_ctrlg_1bit.push_back(&m_ctrlg_1bit_horiz_right);
    m_ctrlg_1bit.push_back(new Groupable_Ctrl(IDC_BITMAP_1BIT));

    m_ctrlg_8bit.push_back(new Groupable_Ctrl(IDC_LEFT_TO_RIGHT5));
    m_ctrlg_8bit.push_back(new Groupable_Ctrl(IDC_RIGHT_TO_LEFT5));
    m_ctrlg_8bit.push_back(new Groupable_Ctrl(IDC_TOP_TO_BOTTOM5));
    m_ctrlg_8bit.push_back(new Groupable_Ctrl(IDC_BOTTOM_TO_TOP5));
    m_ctrlg_8bit.push_back(new Groupable_Ctrl(IDC_BITMAP_8BIT));
}

CImage2CodeDlg::~CImage2CodeDlg()
{
}

void CImage2CodeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_FILENAME, m_filename);
    DDX_Check(pDX, IDC_INVERT, m_invert);
    DDX_Control(pDX, IDC_VIEW, m_view);
    DDX_Radio(pDX, IDC_LEFT_TO_RIGHT1, m_right2left);
    DDX_Radio(pDX, IDC_LEFT_TO_RIGHT2, m_right2left);
    DDX_Radio(pDX, IDC_LEFT_TO_RIGHT3, m_right2left);
    DDX_Radio(pDX, IDC_LEFT_TO_RIGHT4, m_right2left);
    DDX_Radio(pDX, IDC_LEFT_TO_RIGHT5, m_right2left);
    DDX_Radio(pDX, IDC_TOP_TO_BOTTOM1, m_bottom2top);
    DDX_Radio(pDX, IDC_TOP_TO_BOTTOM2, m_bottom2top);
    DDX_Radio(pDX, IDC_TOP_TO_BOTTOM3, m_bottom2top);
    DDX_Radio(pDX, IDC_TOP_TO_BOTTOM4, m_bottom2top);
    DDX_Radio(pDX, IDC_TOP_TO_BOTTOM5, m_bottom2top);
    DDX_Radio(pDX, IDC_PAD_LSB1, m_pad_msb);
    DDX_Radio(pDX, IDC_PAD_LSB2, m_pad_msb);
    DDX_Radio(pDX, IDC_PAD_LSB3, m_pad_msb);
    DDX_Radio(pDX, IDC_PAD_LSB4, m_pad_msb);
    DDX_Radio(pDX, IDC_VERT_UP, m_orientation);
    DDV_MinMaxInt(pDX, m_orientation, 0, 3);
    DDX_CBIndex(pDX, IDC_ARRAY_WRITER, m_array_writer);
}

BEGIN_MESSAGE_MAP(CImage2CodeDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DROPFILES()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_FILEBROWSE, &CImage2CodeDlg::OnBnClickedFileBrowse)
    ON_BN_CLICKED(ID_CONVERT, &CImage2CodeDlg::OnBnClickedConvert)
    ON_BN_CLICKED(IDC_INVERT, &CImage2CodeDlg::OnBnClickedPropertyChange)
    ON_CBN_SELCHANGE(IDC_ARRAY_WRITER, &CImage2CodeDlg::OnBnClickedPropertyChange)
    ON_BN_CLICKED(IDC_LEFT_TO_RIGHT1, &CImage2CodeDlg::OnBnClickedPropertyChangeL2R)
    ON_BN_CLICKED(IDC_LEFT_TO_RIGHT2, &CImage2CodeDlg::OnBnClickedPropertyChangeL2R)
    ON_BN_CLICKED(IDC_LEFT_TO_RIGHT3, &CImage2CodeDlg::OnBnClickedPropertyChangeL2R)
    ON_BN_CLICKED(IDC_LEFT_TO_RIGHT4, &CImage2CodeDlg::OnBnClickedPropertyChangeL2R)
    ON_BN_CLICKED(IDC_LEFT_TO_RIGHT5, &CImage2CodeDlg::OnBnClickedPropertyChangeL2R)
    ON_BN_CLICKED(IDC_RIGHT_TO_LEFT1, &CImage2CodeDlg::OnBnClickedPropertyChangeR2L)
    ON_BN_CLICKED(IDC_RIGHT_TO_LEFT2, &CImage2CodeDlg::OnBnClickedPropertyChangeR2L)
    ON_BN_CLICKED(IDC_RIGHT_TO_LEFT3, &CImage2CodeDlg::OnBnClickedPropertyChangeR2L)
    ON_BN_CLICKED(IDC_RIGHT_TO_LEFT4, &CImage2CodeDlg::OnBnClickedPropertyChangeR2L)
    ON_BN_CLICKED(IDC_RIGHT_TO_LEFT5, &CImage2CodeDlg::OnBnClickedPropertyChangeR2L)
    ON_BN_CLICKED(IDC_TOP_TO_BOTTOM1, &CImage2CodeDlg::OnBnClickedPropertyChangeT2B)
    ON_BN_CLICKED(IDC_TOP_TO_BOTTOM2, &CImage2CodeDlg::OnBnClickedPropertyChangeT2B)
    ON_BN_CLICKED(IDC_TOP_TO_BOTTOM3, &CImage2CodeDlg::OnBnClickedPropertyChangeT2B)
    ON_BN_CLICKED(IDC_TOP_TO_BOTTOM4, &CImage2CodeDlg::OnBnClickedPropertyChangeT2B)
    ON_BN_CLICKED(IDC_TOP_TO_BOTTOM5, &CImage2CodeDlg::OnBnClickedPropertyChangeT2B)
    ON_BN_CLICKED(IDC_BOTTOM_TO_TOP1, &CImage2CodeDlg::OnBnClickedPropertyChangeB2T)
    ON_BN_CLICKED(IDC_BOTTOM_TO_TOP2, &CImage2CodeDlg::OnBnClickedPropertyChangeB2T)
    ON_BN_CLICKED(IDC_BOTTOM_TO_TOP3, &CImage2CodeDlg::OnBnClickedPropertyChangeB2T)
    ON_BN_CLICKED(IDC_BOTTOM_TO_TOP4, &CImage2CodeDlg::OnBnClickedPropertyChangeB2T)
    ON_BN_CLICKED(IDC_BOTTOM_TO_TOP5, &CImage2CodeDlg::OnBnClickedPropertyChangeB2T)
    ON_BN_CLICKED(IDC_PAD_LSB1, &CImage2CodeDlg::OnBnClickedPropertyChangePadLSB)
    ON_BN_CLICKED(IDC_PAD_LSB2, &CImage2CodeDlg::OnBnClickedPropertyChangePadLSB)
    ON_BN_CLICKED(IDC_PAD_LSB3, &CImage2CodeDlg::OnBnClickedPropertyChangePadLSB)
    ON_BN_CLICKED(IDC_PAD_LSB4, &CImage2CodeDlg::OnBnClickedPropertyChangePadLSB)
    ON_BN_CLICKED(IDC_PAD_MSB1, &CImage2CodeDlg::OnBnClickedPropertyChangePadMSB)
    ON_BN_CLICKED(IDC_PAD_MSB2, &CImage2CodeDlg::OnBnClickedPropertyChangePadMSB)
    ON_BN_CLICKED(IDC_PAD_MSB3, &CImage2CodeDlg::OnBnClickedPropertyChangePadMSB)
    ON_BN_CLICKED(IDC_PAD_MSB4, &CImage2CodeDlg::OnBnClickedPropertyChangePadMSB)
    ON_BN_CLICKED(IDC_VERT_UP, &CImage2CodeDlg::OnBnClickedOrientationChange)
    ON_BN_CLICKED(IDC_VERT_DOWN, &CImage2CodeDlg::OnBnClickedOrientationChange)
    ON_BN_CLICKED(IDC_HORIZ_LEFT, &CImage2CodeDlg::OnBnClickedOrientationChange)
    ON_BN_CLICKED(IDC_HORIZ_RIGHT, &CImage2CodeDlg::OnBnClickedOrientationChange)
END_MESSAGE_MAP()


// CImage2CodeDlg message handlers

BOOL CImage2CodeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    CComboBox* ar = reinterpret_cast<CComboBox*>(GetDlgItem(IDC_ARRAY_WRITER));
    if (ar) {
        for(UINT i = 0; i < m_writers.size(); i++)
            ar->AddString(m_writers[i]->GetName());
    }

    Reset();

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImage2CodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}


void CImage2CodeDlg::OnDestroy()
{
    Reset();
    GdiplusShutdown(m_gdiplusToken);
}


// If you add a minimize button to your dialog, you will need the code below
//  this is automatically done for you by the framework.

void CImage2CodeDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImage2CodeDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CImage2CodeDlg::OnDropFiles(HDROP hDropInfo)
{
    CString filename;
    DragQueryFile(hDropInfo, 0, filename.GetBuffer(MAX_PATH), MAX_PATH);
    LoadImage(filename);
    DragFinish(hDropInfo);
}


void CImage2CodeDlg::OnBnClickedFileBrowse()
{
    CFileDialog dlgFile(TRUE);

    dlgFile.DoModal();

    LoadImage(dlgFile.GetPathName());
}


void CImage2CodeDlg::LoadImage(CString& filename)
{
    Reset();

    m_image = Bitmap::FromFile(filename);

    if (m_image != NULL && m_image->GetLastStatus() != Ok) {
        AfxMessageBox(IDS_IMAGE_LOADERROR, MB_OK|MB_ICONSTOP);
        Reset();
    }

    if (m_image) {
        if (m_image->GetPixelFormat() == PixelFormat1bppIndexed) {
            m_ctrlg_1bit.Show(this,true);
            Disable1BitAllBut(m_orientation + 1);
            m_bpp = 1;
        }
        else {
            m_ctrlg_8bit.Show(this,true);
            m_bpp = 8;
        }
        m_ctrlg_common.Show(this,true);

        m_filename.Format(L"File: %s",filename);
        m_filename.AppendFormat(L"\nWill be written as %u bits/pixel gray",m_bpp);
        m_filename.AppendFormat(L"\nWidth: %u, Height: %u", m_image->GetWidth(), m_image->GetHeight());

    }

    UpdateThumb();
    UpdateData(FALSE);

}

void CImage2CodeDlg::UpdateThumb()
{
    if (m_image) {
        RECT rect;
        HBITMAP hbitmap;
        m_view.GetWindowRect(&rect);
        UINT width = rect.right - rect.left;
        UINT height = rect.bottom - rect.top;
        if (m_image->GetHeight() > m_image->GetWidth()) {
            width = static_cast<UINT>(static_cast<FLOAT>(width) * m_image->GetWidth() / m_image->GetHeight());
        }
        else {
            height = static_cast<UINT>(static_cast<FLOAT>(height) * m_image->GetHeight() / m_image->GetWidth());
        }
        Bitmap* pNewBitmap = (Bitmap*)m_image->GetThumbnailImage(width,height);
        pNewBitmap->GetHBITMAP(Color::Red,&hbitmap);
        HBITMAP old = m_view.SetBitmap(hbitmap);
        DeleteObject(old);
        DeleteObject(hbitmap);
        delete pNewBitmap;
    }
    else {
        DeleteObject(m_view.SetBitmap(NULL));
    }
}

void CImage2CodeDlg::OnBnClickedConvert()
{
    if (m_image) {
        WriteImageCode();
    }
    else {
        AfxMessageBox(IDS_IMAGE_NONE, MB_OK|MB_ICONSTOP);
    }
}

void CImage2CodeDlg::WriteImageCode()
{
    ImageArray img(m_image);
    ImageArray::BitPacker* packer = NULL;

    switch (m_bpp) {
        case 1:
            packer = new BitPack1Bit(m_right2left,m_bottom2top,m_orientation,m_pad_msb);
            break;
        case 8:
            packer = new BitPack8Bit(m_right2left,m_bottom2top);
            break;
        default:
            AfxMessageBox(IDS_ARRAY_WRITER_INVALID_BPP, MB_OK|MB_ICONSTOP);
            break;
    }

    if (packer) {
        img.Ready(packer,m_invert);

        if (m_array_writer >= 0 && static_cast<unsigned int>(m_array_writer) < m_writers.size())
            m_writers[m_array_writer]->Writer(&img);
        else
            AfxMessageBox(IDS_ARRAY_WRITER_NONE, MB_OK|MB_ICONSTOP);

        delete packer;
    }
}


void CImage2CodeDlg::OnBnClickedPropertyChange()
{
    UpdateData(TRUE);

}

void CImage2CodeDlg::OnBnClickedOrientationChange()
{
    UpdateData(TRUE);
    Disable1BitAllBut(m_orientation + 1);
    UpdateData(FALSE);
}

void CImage2CodeDlg::Disable1BitAllBut(int orient)
{
    int i;
    bool state;
    for (i = 1; i <=4 ; i++) {
        state = i == orient;
        switch (i) {
            case 1:
                m_ctrlg_1bit_vert_up.Enable(this,state);
                if (m_image->GetHeight()%8 == 0)
                    m_ctrlg_1bit_vert_up_padding.Enable(this,FALSE);
                break;
            case 2:
                m_ctrlg_1bit_vert_down.Enable(this,state);
                if (m_image->GetHeight()%8 == 0)
                    m_ctrlg_1bit_vert_down_padding.Enable(this,FALSE);
                break;
            case 3:
                m_ctrlg_1bit_horiz_left.Enable(this,state);
                if (m_image->GetWidth()%8 == 0)
                    m_ctrlg_1bit_horiz_left_padding.Enable(this,FALSE);
                break;
            case 4:
                m_ctrlg_1bit_horiz_right.Enable(this,state);
                if (m_image->GetWidth()%8 == 0)
                    m_ctrlg_1bit_horiz_right_padding.Enable(this,FALSE);
                break;
            default:
                break;
        }
    }
}

void CImage2CodeDlg::OnBnClickedPropertyChangeL2R()
{
    m_right2left = 0;
    UpdateData(FALSE);
}

void CImage2CodeDlg::OnBnClickedPropertyChangeR2L()
{
    m_right2left = 1;
    UpdateData(FALSE);
}

void CImage2CodeDlg::OnBnClickedPropertyChangeT2B()
{
    m_bottom2top = 0;
    UpdateData(FALSE);
}

void CImage2CodeDlg::OnBnClickedPropertyChangeB2T()
{
    m_bottom2top = 1;
    UpdateData(FALSE);
}

void CImage2CodeDlg::OnBnClickedPropertyChangePadLSB()
{
    m_pad_msb = 0;
    UpdateData(FALSE);
}

void CImage2CodeDlg::OnBnClickedPropertyChangePadMSB()
{
    m_pad_msb = 1;
    UpdateData(FALSE);
}

