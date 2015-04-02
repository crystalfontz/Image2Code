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
#include "Cross.h"
#include <stdarg.h>

void Portable_String_Class::Format(Portable_String fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    FormatV(fmt,args);
    va_end(args);
}

void Portable_String_Class::FormatV(Portable_String fmt, va_list args)
{
    const int maxtries = 20;
    const unsigned int bufsz = 1024;
    char buf[bufsz];
    char* pbuf = buf;
    int len = 0;
    int attempts = 0;
    bool success = true;
    
    do {
        int maxlen = bufsz * attempts + bufsz - 1;
        len = vsnprintf(pbuf, maxlen, fmt, args);
        attempts++;
        success = (len>=0 && len<maxlen);
        if (!success) {
            if (pbuf != buf)
                delete [] pbuf;
            pbuf = new char[(attempts + 1) * bufsz];
        }
    }
    while (!success && attempts < maxtries);
    
    this->clear();
    if (success) {
        pbuf[len] = '\0';
        this->append(pbuf);
    }
    
    if (pbuf!=buf)
        delete [] pbuf;
}


Bitmap::Bitmap()
    : fWand(NULL)
{
    MagickWandGenesis();
    fWand=NewMagickWand();  
}

Bitmap::~Bitmap()
{
    DestroyMagickWand(fWand);
    MagickWandTerminus();
}

bool Bitmap::Load(const char* name)
{
    MagickBooleanType status  = MagickReadImage(fWand,name);
    return status != MagickFalse;
}

UINT Bitmap::GetWidth()
{
    return MagickGetImageWidth(fWand);
}

UINT Bitmap::GetHeight()
{
    return MagickGetImageHeight(fWand);
}

void Bitmap::GetPixel(INT x, INT y, Color* c)
{
    PixelWand* color = NewPixelWand();
    MagickGetImagePixelColor(fWand, x, y, color);
    *c = Color(
               static_cast<UCHAR>(PixelGetRed(color)*255),
               static_cast<UCHAR>(PixelGetGreen(color)*255),
               static_cast<UCHAR>(PixelGetBlue(color)*255)
               );
    DestroyPixelWand(color);
}

unsigned int Bitmap::GetBPP()
{
    return MagickGetImageDepth(fWand);
}
