/*
  XMEM LCD Library for the Arduino

  Copyright 2012,2013 Andrew Brown

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This notice may not be removed or altered from any source distribution.
*/

/*
 * Implementation for Adafruit TFT LCD copyright (c) 2013 Colin Irwin, see
 * http://aethersdr.org/ for details. Provided under original terms and
 * conditions.
 */

/**
 * @file ILI9325Colour16Specialisation.h
 * @brief Specialisation of ILI9325Colour for 16-bit colours.
 * @ingroup ILI9325
 */

#pragma once

namespace lcd {

  /**
   * @brief Template specialisation of ILI9325Colour for 16-bit colours
   * @tparam TAccessMode The access mode in use.
   * @ingroup ILI9325
   */

  template<class TAccessMode>
  class ILI9325Colour<COLOURS_16BIT,TAccessMode> {

    protected:
      uint16_t getInterfacePixelFormat() const;

      typedef uint32_t TColour;

      struct UnpackedColour {
        uint8_t first,second;
      };

    public:
      void unpackColour(TColour src,UnpackedColour& dest) const;
      void unpackColour(uint8_t red,uint8_t green,uint8_t blue,UnpackedColour& dest) const;
      void writePixel(const UnpackedColour& cr) const;
      void writePixelAgain(const UnpackedColour& cr) const;
      void fillPixels(uint32_t numPixels,const UnpackedColour& cr) const;
      void allocatePixelBuffer(uint32_t numPixels,uint8_t*& buffer,uint32_t& bytesPerPixel) const;
      uint8_t getBytesPerPixel() const;
  };


  /**
   * Get the register setting for 16-bit colours
   * @return 16-bit interface pixel format register setting
   */

  template<class TAccessMode>
  inline uint16_t ILI9325Colour<COLOURS_16BIT,TAccessMode>::getInterfacePixelFormat() const {
    return 0x0000;
  }


  /**
   * Unpack the colour from rrggbb to the internal 5-6-5 format
   *
   * 00000000RRRRRRRRGGGGGGGGBBBBBBBB ->
   * 0000000000000000RRRRRGGGGGGBBBBB
   *
   * @param src rrggbb
   * @param dest The unpacked colour structure
   */

  template<class TAccessMode>
  inline void ILI9325Colour<COLOURS_16BIT,TAccessMode>::unpackColour(TColour src,UnpackedColour& dest) const {
    uint8_t r,g,b;

    r=src >> 16;
    g=src >> 8;
    b=src;

    r&=0xf8;
    g&=0xfc;
    b&=0xf8;

    dest.first=r | (g >> 5);
    dest.second=(g << 3) | (b >> 3);
  }


  /**
   * Unpack the colour from components to the internal format
   * @param red
   * @param green
   * @param blue
   * @param dest
   */

  template<class TAccessMode>
  inline void ILI9325Colour<COLOURS_16BIT,TAccessMode>::unpackColour(uint8_t red,uint8_t green,uint8_t blue,UnpackedColour& dest) const {
    red&=0xf8;
    green&=0xfc;
    blue&=0xf8;

    dest.first=red | (green >> 5);
    dest.second=(green << 3) | (blue >> 3);
  }


  /**
   * Write a single pixel to the current output position.
   * Assumes that the caller has already issued the beginWriting() command.
   * @param cr The pixel to write
   */

  template<class TAccessMode>
  inline void ILI9325Colour<COLOURS_16BIT,TAccessMode>::writePixel(const UnpackedColour& cr) const {
    TAccessMode::writeData(cr.second,cr.first);
  }


  /**
   * Write the same colour pixel that we last wrote. This gives the access mode a chance to
   * optimise sequential pixel writes. The colour is provided for drivers that cannot optimise
   * and must fall back to a full write.
   * @param cr The pixel to write
   */

  template<class TAccessMode>
  inline void ILI9325Colour<COLOURS_16BIT,TAccessMode>::writePixelAgain(const UnpackedColour& cr) const {
    TAccessMode::writeDataAgain(cr.second,cr.first);
  }


  /**
   * Fill a block of pixels with the same colour. This operation will issue the
   * beginWriting() command for you.
   * @param numPixels how many
   * @param cr The unpacked colour to write
   */

  template<class TAccessMode>
  inline void ILI9325Colour<COLOURS_16BIT,TAccessMode>::fillPixels(uint32_t numPixels,const UnpackedColour& cr) const {

    uint8_t lo8,hi8;

    lo8=cr.second;
    hi8=cr.first;

    TAccessMode::writeCommand(ili9325::ILI932X_RW_GRAM);
    TAccessMode::writeMultiData(numPixels,lo8,hi8);
  }


  /**
   * Allocate a buffer for pixel data. You supply the number of pixels and this allocates the buffer as a uint8_t[].
   * Allocated buffers should be freed with free()
   *
   * @param numPixels The number of pixels to allocate
   * @param buffer The output buffer
   * @param bytesPerPixel Output the number of bytes per pixel
   */

  template<class TAccessMode>
  inline void ILI9325Colour<COLOURS_16BIT,TAccessMode>::allocatePixelBuffer(uint32_t numPixels,uint8_t*& buffer,uint32_t& bytesPerPixel) const {
    buffer=(uint8_t *)malloc(numPixels * 2);
    bytesPerPixel=2;
  }

  /**
   * Get the number of bytes per pixel
   * @return 2
   */

  template<class TAccessMode>
  inline uint8_t ILI9325Colour<COLOURS_16BIT,TAccessMode>::getBytesPerPixel() const {
    return 2;
  }
}
