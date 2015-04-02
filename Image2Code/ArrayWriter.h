//============================================================================
// Image2Code - code generator to create an array from an image
// ArrayWriter.h: Write out the code for a multidimensional array in several languages
// Copyright 2008, Crystalfontz America, Inc. Written by Rob Emanuele
// http://www.crystalfontz.com, rje@crystalfontz.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//============================================================================

#pragma once
#include <vector>
#include "Cross.h"
class ImageArray;

class ArrayWriter
{
public:
    virtual ~ArrayWriter() {}
    virtual Portable_String GetName() = 0;
    virtual Portable_String IGetDefExt() = 0;
    virtual void Writer(ImageArray* img) = 0;
};

class ArrayWriterToBinaryFile: public ArrayWriter
{
protected:
     virtual void IWriter(StdFileIO* file, ImageArray* img) = 0;
public:
    void Writer(ImageArray* img);
};

#ifdef TMP_FILE_WRITER_AVAILABLE
class ArrayWriterToTmpFile: public ArrayWriter
{
protected:
    virtual void IWriter(StdFileIO* file, ImageArray* img) = 0;
public:
    void Writer(ImageArray* img);
};
#else
#define ArrayWriterToTmpFile  ArrayWriterToBinaryFile
#endif

class CArrayWriter: public ArrayWriterToTmpFile
{
protected:
    void IWriter(StdFileIO* file, ImageArray* img);
public:
    Portable_String IGetDefExt() { return Portable_String_Modifier("inc"); }
    Portable_String GetName() { return Portable_String_Modifier("C Array Writer"); }
};

class AsmArrayWriter: public ArrayWriterToTmpFile
{
protected:
    void IWriter(StdFileIO* file, ImageArray* img);
public:
    Portable_String IGetDefExt() { return Portable_String_Modifier("asm"); }
    Portable_String GetName() { return Portable_String_Modifier("ASM Array Writer"); }
};

class CF0Writer: public ArrayWriterToBinaryFile
{
protected:
    Portable_String IGetDefExt() { return Portable_String_Modifier("cf0"); }
    void IWriter(StdFileIO* file, ImageArray* img);
public:
    Portable_String GetName() { return Portable_String_Modifier("CF KS0107/0108 Demo Board Format"); }
};

class ArrayWriters: public std::vector<ArrayWriter*>
{
public:
    ArrayWriters();
    ~ArrayWriters();
};
