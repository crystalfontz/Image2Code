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
#include "Cross.h"
#include "ArrayWriter.h"
#include "ImageArray.h"

extern StdFileIO* outputPtr;

void ArrayWriterToBinaryFile::Writer(ImageArray* img)
{
    IWriter(outputPtr,img);
}


