//============================================================================
// Image2Code - code generator to create an array from an image
// ImageArray.cpp: Get bits in an LCD bitpacking from a Bitmap
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
#include "ImageArray.h"

ImageArray::ImageArray(Bitmap* img)
: m_img(img)
{

}

UINT ImageArray::GetColumns()
{
    return  m_packer->GetAdjustedColumns();
}

UINT ImageArray::GetRows()
{
    return  m_packer->GetAdjustedRows();
}

UINT ImageArray::GetImageWidth()
{
    return m_img->GetWidth();
}

UINT ImageArray::GetImageHeight()
{
    return  m_img->GetHeight();
}

void ImageArray::Ready(BitPacker* packer, BOOL invert)
{
    m_packer = packer;
    m_invert = invert;
    m_packer->Ready(m_img->GetWidth(), m_img->GetHeight());
}

UCHAR ImageArray::GetUCharAtLCDMem(UINT x, UINT y)
{
    UCHAR val = 0;

    if (x < GetColumns() && y < GetRows()) {
        Points points;
        m_packer->GetPoints(points, x, y);
        Points::iterator point = points.begin();
        while (point != points.end()) {
            Color c;
            m_img->GetPixel((*point).GetX(), (*point).GetY(), &c);
            UCHAR gray = Gray(c.GetR(), c.GetG(), c.GetB());
            val |= (m_packer->Pack(gray, (*point).GetPackOffset(), m_invert));
            point++;
        }
    }
    return val;
}

BitPack8Bit::BitPack8Bit(BOOL right2left, BOOL bottom2top)
: BitPacker(right2left,bottom2top)
{

}

void BitPack8Bit::GetPoints(ImageArray::Points& points, UINT col, UINT row)
{
    points.push_back(ImageArray::Point(m_right2left?m_img_width-col-1:col,m_bottom2top?m_img_height-row-1:row));	
}

UCHAR BitPack8Bit::Pack(UCHAR gray, UINT i, BOOL invert)
{
    return invert?~gray:gray;
}


BitPack1Bit::BitPack1Bit(BOOL right2left, BOOL bottom2top, INT orientation, BOOL pad_msb)
: BitPacker(right2left,bottom2top), m_orientation(orientation), m_pad_msb(pad_msb)
{

}

UINT BitPack1Bit::GetXOffset()
{
    return m_img_width&0x07;
}

UINT BitPack1Bit::GetYOffset()
{
    return m_img_height&0x07;
}

UINT BitPack1Bit::GetAdjustedColumns()
{
    if (m_orientation == 0 || m_orientation == 1) {
        return m_img_width;
    }
    else {
        return (m_img_width >> 3) + (GetXOffset()?1:0);		
    }
}

UINT BitPack1Bit::GetAdjustedRows()
{
    if (m_orientation == 0 || m_orientation == 1) {
        return (m_img_height >> 3) + (GetYOffset()?1:0);		
    }
    else {
        return m_img_height;
    }
}

void BitPack1Bit::GetPoints(ImageArray::Points& points, UINT col, UINT row)
{
    UINT x,y;
    BOOL pad_lead;

    col = m_right2left?GetAdjustedColumns()-col-1:col;
    row = m_bottom2top?GetAdjustedRows()-row-1:row;
    for (int i = 0; i < 8; i++) {
        if (m_orientation == 0 || m_orientation == 1) {
            y = row * 8 + i;
            pad_lead = (m_orientation == 0 && m_pad_msb) || (m_orientation == 1 && !m_pad_msb);
            if (pad_lead && GetYOffset() > 0)
                if (y < 8 - GetYOffset())
                    continue;
                else
                    y -= 8 - GetYOffset();
            else
                if (y > m_img_height - 1)
                    continue;
            points.push_back(ImageArray::Point(col,y,i));	
        }
        else {
            x = col * 8 + i;
            pad_lead = (m_orientation == 2 && m_pad_msb) || (m_orientation == 3 && !m_pad_msb);
            if (pad_lead && GetXOffset() > 0)
                if (x < 8 - GetXOffset())
                    continue;
                else
                    x -= 8 - GetXOffset();
            else
                if (x > m_img_width - 1)
                    continue;
            points.push_back(ImageArray::Point(x,row,i));
        }
    }
}

UCHAR BitPack1Bit::Pack(UCHAR gray, UINT i, BOOL invert)
{
    if ((!invert && gray != 0) || (invert && gray == 0)) {
        if (m_orientation == 0 || m_orientation == 2)
            return 0x80>>i;
        else
            return 0x01<<i;
    }
    else
        return 0;
}
