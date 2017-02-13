#pragma once

#include <string>

namespace Engine {
  class Bitmap {
    public:
      enum Format {
        Format_Grayscale = 1,
        Format_GrayscaleAlpha = 2,
        Format_RGB = 3,
        Format_RGBA = 4
      };

      Bitmap(unsigned width,
             unsigned height,
             Format format,
             const unsigned char *pixels = NULL);

      ~Bitmap();

      static Bitmap bitmapFromFile(std::string filepath);

      unsigned width() const;
      unsigned height() const;
      Format format() const;
      unsigned char* pixelBuffer() const;
      unsigned char* getPixel(unsigned int column, unsigned int row) const;

      void setPixel(unsigned int column, unsigned int row, const unsigned char* pixel);
      void flipVertically();
      // rotate 90 deg counter-clockwise
      void rotate90CCW();
      void copyRectFromBitmap(const Bitmap &src,
                              unsigned srcCol,
                              unsigned destCol,
                              unsigned srcRow,
                              unsigned destRow,
                              unsigned width,
                              unsigned height);
      // copy constructor
      Bitmap(const Bitmap& other);
      // assignment operator
      Bitmap& operator = (const Bitmap &other);
      
    private:
      Format _format;
      unsigned _width;
      unsigned _height;
      unsigned char *_pixels;
      
      void _set(unsigned width, unsigned height, Format fmt, const unsigned char *pixels);
      //static unsigned getPixelOffset(unsigned col, unsigned row, unsigned width, unsigned height, Format fmt);
  };
}
