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

ArrayWriters::ArrayWriters()
{
    push_back(new CArrayWriter());
    push_back(new AsmArrayWriter());
    push_back(new CF0Writer());
}

ArrayWriters::~ArrayWriters()
{
    for(UINT i = 0; i < size(); i++)
        delete (*this)[i];
}


void CArrayWriter::IWriter(StdFileIO* file, ImageArray* img)
{
    UINT x, y;
    Portable_String_Class value;
    for(y = 0; y < img->GetRows(); y++)
    {
        if (y != 0) {
            file->WriteString(Portable_String_Modifier(",\r\n"));
        }
        file->WriteString(Portable_String_Modifier("{"));
        for(x = 0; x < img->GetColumns(); x++)
        {
            if (x != 0) {
                file->WriteString(Portable_String_Modifier(","));
            }

            value.Format(Portable_String_Modifier("0x%02X"),img->GetUCharAtLCDMem(x,y));
            file->WriteString(value);
        }
        file->WriteString(Portable_String_Modifier("}"));
    }
    file->WriteString(Portable_String_Modifier(",\r\n"));
}

void AsmArrayWriter::IWriter(StdFileIO* file, ImageArray* img)
{
    UINT x, y;
    Portable_String_Class value;
    for(y = 0; y < img->GetRows(); y++)
    {
        if (y != 0) {
            file->WriteString(Portable_String_Modifier("\r\n"));
        }
        file->WriteString(Portable_String_Modifier("\tdb\t"));
        for(x = 0; x < img->GetColumns(); x++)
        {
            if (x != 0) {
                file->WriteString(Portable_String_Modifier(","));
            }

            value.Format(Portable_String_Modifier("%02Xh"),img->GetUCharAtLCDMem(x,y));
            file->WriteString(value);
        }
    }
    file->WriteString(Portable_String_Modifier(",\r\n"));
}

void CF0Writer::IWriter(StdFileIO* file, ImageArray* img)
{
    UINT x, y;
    UCHAR value;
    UCHAR height, width;

    width = img->GetImageWidth();
    height = img->GetImageHeight();
    file->Write(&width,sizeof(width));
    file->Write(&height,sizeof(height));

    for(y = 0; y < img->GetRows(); y++)
    {
        for(x = 0; x < img->GetColumns(); x++)
        {
            value = img->GetUCharAtLCDMem(x,y);
            file->Write(&value,sizeof(value));
        }
    }
}
