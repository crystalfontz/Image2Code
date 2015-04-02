//============================================================================
// Image2Code - code generator to create an array from an image
// Image2Code.cpp: Application file
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

#if defined(_WIN32)
#include "stdafx.h"

typedef LPCTSTR Portable_String;
#define Portable_String_Modifier(s) L##s

typedef CString Portable_String_Class;
typedef CStdioFile StdFileIO;

#define TMP_FILE_WRITER_AVAILABLE

#elif defined(__linux__)

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <string>
#include <wand/MagickWand.h>

typedef bool BOOL;
typedef unsigned int UINT;
typedef int INT;
typedef unsigned char UCHAR;

typedef const char*  Portable_String;
#define Portable_String_Modifier(s) s


class StdFileIO
{
private:
    int fFd;
public:
    StdFileIO(int fd) 
        : fFd(fd) {}
    ~StdFileIO() {}
    void WriteString(Portable_String s) { write(fFd,s,strlen(s)); }
    void Write(const void* buf, unsigned int sz) { write(fFd,buf,sz); }
};

class Portable_String_Class : public std::string
{
 public:
    operator Portable_String () { return c_str(); }
    void Format(Portable_String fmt, ...);
    void FormatV(Portable_String fmt, va_list args);
};

class Color
{
 private:
    UCHAR fR,fG,fB;
 public:
    Color()
     : fR(0), fG(0), fB(0) {}
    Color(UCHAR r, UCHAR g, UCHAR b)
     : fR(r), fG(g), fB(b) {}
    UCHAR GetR() { return fR; }
    UCHAR GetG() { return fG; }
    UCHAR GetB() { return fB; }  
};

class Bitmap
{
 private:
    MagickWand *fWand;
 public:
    Bitmap();
    ~Bitmap();

    bool Load(const char* name);
    UINT GetWidth();
    UINT GetHeight();
    
    void GetPixel(INT x, INT y, Color* c);

    unsigned int GetBPP();
};

#else
#error "Unsupported OS"
#endif
