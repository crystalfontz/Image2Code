#pragma once
//============================================================================
// Image2Code - code generator to create an array from an image
// ImageArray.h: Get bits in an LCD bitpacking from a Bitmap
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
#include <list>

class ImageArray
{
public:
    class Point
    {
        UINT m_x, m_y;
        UINT m_pack_offset;
    public:
        Point(UINT x, UINT y) : m_x(x), m_y(y), m_pack_offset(0) { }
        Point(UINT x, UINT y, UINT offset) : m_x(x), m_y(y), m_pack_offset(offset) { }
        UINT GetX() { return m_x; }
        UINT GetY() { return m_y; }
        UINT GetPackOffset() { return m_pack_offset; }
    };

    typedef std::list<Point> Points;

    class BitPacker
    {
    protected:
        BOOL m_right2left, m_bottom2top;
        UINT m_img_width, m_img_height;
    public:
        BitPacker(BOOL right2left, BOOL bottom2top) : 
          m_right2left(right2left), m_bottom2top(bottom2top) {}
        virtual ~BitPacker() {}
        void Ready(UINT img_width, UINT img_height) { m_img_width = img_width; m_img_height = img_height; }
        virtual UCHAR GetBPP() = 0;
        virtual UINT GetAdjustedColumns() = 0;
        virtual UINT GetAdjustedRows() = 0;
        virtual void GetPoints(Points& points, UINT col, UINT row) = 0;
        virtual UCHAR Pack(UCHAR gray, UINT i, BOOL invert) = 0;
    };

private:
    Bitmap* m_img;
    BitPacker* m_packer;
    BOOL m_invert;
public:
    ImageArray(Bitmap* img);
    virtual ~ImageArray() {}

    static UCHAR Gray(UCHAR r, UCHAR g, UCHAR b) { return static_cast<UCHAR>(0.3 * r + 0.59 * g + 0.11 * b); }

    void Ready(BitPacker* packer, BOOL invert);
    UINT GetColumns();
    UINT GetRows();
    UINT GetImageHeight();
    UINT GetImageWidth();
    UCHAR GetBPP() { return m_packer->GetBPP(); }
    UCHAR GetUCharAtLCDMem(UINT x, UINT y);

};

class BitPack1Bit : public ImageArray::BitPacker
{
private:
    INT m_orientation;
    BOOL m_pad_msb;
    UINT GetXOffset();
    UINT GetYOffset();
public:
    BitPack1Bit(BOOL right2left, BOOL bottom2top, INT orientation, BOOL pad_msb);
    UCHAR GetBPP() { return 1; }
    UINT GetAdjustedColumns();
    UINT GetAdjustedRows();
    void GetPoints(ImageArray::Points& points, UINT col, UINT row);
    UCHAR Pack(UCHAR gray, UINT i, BOOL invert);
};

class BitPack8Bit : public ImageArray::BitPacker
{
public:
    BitPack8Bit(BOOL right2left, BOOL bottom2top);
    UCHAR GetBPP() { return 8; }
    UINT GetAdjustedColumns() { return m_img_width; }
    UINT GetAdjustedRows() { return m_img_height; }
    void GetPoints(ImageArray::Points& points, UINT col, UINT row);
    UCHAR Pack(UCHAR gray, UINT i, BOOL invert);
};
