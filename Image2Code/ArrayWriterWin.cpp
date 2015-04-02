//============================================================================
// Image2Code - code generator to create an array from an image
// ArrayWriter.cpp: Write out the code for a multidimensional array in several languages
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
#include "Cross.h"
#include "ArrayWriter.h"
#include "ImageArray.h"
#include "Resource.h"

CString GetTempFilePath(LPCTSTR strPattern)
{
    CString csPath;
    if(GetTempPath(_MAX_PATH,csPath.GetBuffer(_MAX_PATH+1)) != 0) {
        csPath.ReleaseBuffer();
        CString csTempFile;
        if(GetTempFileName(csPath,strPattern,0,csTempFile.GetBuffer(_MAX_PATH+1))
            != 0) {
                csTempFile.ReleaseBuffer();
                return csTempFile;
        }
    }
    return CString();
}

void ArrayWriterToTmpFile::Writer(ImageArray* img)
{
    CString tmpfilename = GetTempFilePath(L"Image2Code");

    CStdioFile tmp;
    if (tmp.Open(tmpfilename,CFile::modeCreate|CFile::modeWrite|CFile::typeText)) {

        IWriter(&tmp,img);

        tmp.Close();

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ZeroMemory( &pi, sizeof(pi) );

        CString command(L"notepad");
        command += L" ";
        command += tmpfilename;

        BOOL run = CreateProcess( NULL,
            T2W(command.GetBuffer()),    // Command line
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory 
            &si,            // Pointer to STARTUPINFO structure
            &pi );          // Pointer to PROCESS_INFORMATION structure

        if (run) {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else {
            AfxMessageBox(IDS_IMAGE_NONOTEPAD, MB_OK|MB_ICONSTOP);
        }

    }
    else {
        AfxMessageBox(IDS_IMAGE_TMPFILEFAIL, MB_OK|MB_ICONSTOP);
    }

}

void ArrayWriterToBinaryFile::Writer(ImageArray* img)
{
    CFileDialog dlgFile(FALSE, IGetDefExt(), L"imagefile", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);

    if (dlgFile.DoModal() == IDOK) {
        CStdioFile tmp;
        if (tmp.Open(dlgFile.GetPathName(),CFile::modeCreate|CFile::modeWrite|CFile::typeBinary)) {

            IWriter(&tmp,img);

            tmp.Close();
        }
        else {
            AfxMessageBox(IDS_IMAGE_SAVEASFAIL, MB_OK|MB_ICONSTOP);
        }
    }
}

