#include "../include/Bitmap.h"
#include <stdexcept>
#include <cstdlib>

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include "../include/common/stb_image.h"

using namespace Engine;

inline unsigned char AverageRGB(unsigned char rgb[3])
{
  return (unsigned char)(((double)rgb[0] + (double)rgb[1] + (double)rgb[2]) / 3.0);
}

static void GrayscaleToGrayscaleAlpha(unsigned char *src, unsigned char *dest)
{
  dest[0] = src[0];
  dest[1] = 255;
}

static void GrayscaleToRGB(unsigned char *src, unsigned char *dest)
{
  dest[0] = src[0];
  dest[1] = src[0];
  dest[2] = src[0];
}

static void GrayscaleToRGBA(unsigned char *src, unsigned char *dest)
{
  dest[0] = src[0];
  dest[1] = src[0];
  dest[2] = src[0];
  dest[3] = 255;
}

static void GrayscaleAlphaToGrayscale(unsigned char *src, unsigned char *dest)
{
  dest[0] = src[0];  
}

static void GrayscaleAlphaToRGB(unsigned char *src, unsigned char *dest)
{
  dest[0] = src[0];
  dest[1] = src[0];
  dest[2] = src[0];
}

static void GrayscaleAlphaToRGBA(unsigned char *src, unsigned char *dest)
{
  dest[0] = src[0];
  dest[1] = src[0];
  dest[2] = src[0];
  dest[3] = src[1];
}

static void RGBToGrayscale(unsigned char *src, unsigned char *dest)
{
  dest[0] = AverageRGB(src);
}

static void RGBToGrayscaleAlpha(unsigned char *src, unsigned char *dest)
{
  dest[0] = AverageRGB(src);
  dest[1] = 255;
}

static void RGBToRGBA(unsigned char *src, unsigned char *dest)
{
  dest[0] = src[0];
  dest[1] = src[1];
  dest[2] = src[2];
  dest[3] = 255;
}
static void RGBAToGrayscale(unsigned char *src, unsigned char *dest)
{
  dest[0] = AverageRGB(src);
}
static void RGBAToGrayscaleAlpha(unsigned char *src, unsigned char *dest)
{
  dest[0] = AverageRGB(src);
  dest[1] = src[3];
}
static void RGBAToRGB(unsigned char *src, unsigned char *dest)
{
  dest[0] = src[0];
  dest[1] = src[1];
  dest[2] = src[2];
}

typedef void(*FormatConverterFunc)(unsigned char*, unsigned char*);

static FormatConverterFunc ConverterFuncForFormats(Bitmap::Format srcFormat, Bitmap::Format destFormat) 
{
  if (srcFormat == destFormat)
    throw std::runtime_error("just use memcpy if the pixel formats are the same, ya dingus.");  

  switch(srcFormat) {
    case Bitmap::Format_Grayscale:
      switch(destFormat) {
        case Bitmap::Format_GrayscaleAlpha: return GrayscaleToGrayscaleAlpha;
        case Bitmap::Format_RGB: return GrayscaleToRGB;
        case Bitmap::Format_RGBA: return GrayscaleToRGBA;
        default: throw std::runtime_error("unhandled bitmap format.");
      }
      break;
    case Bitmap::Format_GrayscaleAlpha:
      switch(destFormat) {
        case Bitmap::Format_Grayscale: return GrayscaleAlphaToGrayscale;
        case Bitmap::Format_RGB: return GrayscaleAlphaToRGB;
        case Bitmap::Format_RGBA: return GrayscaleAlphaToRGBA;
        default: throw std::runtime_error("unhandled bitmap format.");
      }
      break;
    case Bitmap::Format_RGB:
      switch(destFormat) {
        case Bitmap::Format_GrayscaleAlpha: return RGBToGrayscaleAlpha;
        case Bitmap::Format_Grayscale: return RGBToGrayscale;
        case Bitmap::Format_RGBA: return RGBToRGBA;
        default: throw std::runtime_error("unhandled bitmap format.");
      }
      break;
    case Bitmap::Format_RGBA:
      switch(destFormat) {
        case Bitmap::Format_GrayscaleAlpha: return RGBAToGrayscaleAlpha;
        case Bitmap::Format_Grayscale: return RGBAToGrayscale;
        case Bitmap::Format_RGB: return RGBAToRGB;
        default: throw std::runtime_error("unhandled bitmap format.");
      }
      break;
    default:
      throw std::runtime_error("unhandled bitmap format.");
  }
}

inline static unsigned getPixelOffset(unsigned col, unsigned row, unsigned width, unsigned height, Bitmap::Format fmt) 
{
  return (row*width + col)*fmt;
}

inline bool rectsOverlap(unsigned srcCol, unsigned srcRow, unsigned destCol, unsigned destRow, unsigned width, unsigned height) {
  unsigned colDiff = srcCol > destCol ? srcCol - destCol : destCol - srcCol;
  if (colDiff < width)
    return true;
  unsigned rowDiff = srcRow > destRow ? srcRow - destRow : destRow - srcRow;
  if (rowDiff < height)
    return true;
  return false;
}

// class stuff

Bitmap::Bitmap(unsigned width, unsigned height, Format fmt, const unsigned char *pixels) :
  _pixels(NULL)
{
  _set(width, height, fmt, pixels);
}

Bitmap::~Bitmap() {
  if (_pixels) free(_pixels);
}

Bitmap Bitmap::bitmapFromFile(std::string filePath) {
  int width, height, channels;
  unsigned char *pixels = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
  if (!pixels) throw std::runtime_error(stbi_failure_reason());
  Bitmap bmp(width, height, (Format)channels, pixels);
  stbi_image_free(pixels);
  return bmp;
}

Bitmap::Bitmap(const Bitmap& other) :
  _pixels(NULL)
{
  _set(other._width, other._height, other._format, other._pixels);
}

Bitmap& Bitmap::operator = (const Bitmap& other) {
  _set(other._width, other._height, other._format, other._pixels);
  return *this;
}

// getters / setters

unsigned int Bitmap::width() const {
  return _width;
}
unsigned int Bitmap::height() const {
  return _height;
}
Bitmap::Format Bitmap::format() const {
  return _format; 
}
unsigned char* Bitmap::pixelBuffer() const {
  return _pixels;
}

unsigned char* Bitmap::getPixel(unsigned int col, unsigned int row) const {
  if (col >= _width || row >= _height)
    throw std::runtime_error("pixel coord out of bounds.");

  return _pixels + getPixelOffset(col, row, _width, _height, _format);
}

void Bitmap::setPixel(unsigned int col, unsigned int row, const unsigned char* pixel) {
  unsigned char* myPixel = getPixel(col, row);
  memcpy(myPixel, pixel, _format); 
}

void Bitmap::flipVertically() {
  unsigned long rowSize = _format * _width;
  unsigned char* rowBuffer = new unsigned char[rowSize];
  unsigned halfRows = _height / 2;
 
  // iterate over half the image. 
  for (unsigned rowIdx = 0; rowIdx < halfRows; ++rowIdx) {
    unsigned char* row = _pixels + getPixelOffset(0, rowIdx, _width, _height, _format);
    unsigned char *oppositeRow = _pixels + getPixelOffset(0, _height - rowIdx - 1, _width, _height, _format);

    // copy row into rowBuffer
    memcpy(rowBuffer, row, rowSize);
    // copy oppositeRow into row
    memcpy(row, oppositeRow, rowSize);
    // coppy oppositeRow into rowBuffer
    memcpy(oppositeRow, rowBuffer, rowSize);
  }
}

void Bitmap::rotate90CCW() {
  unsigned char* newPixels = (unsigned char*) malloc(_format*_width*_height);
  
  for (unsigned row = 0; row < _height; ++row) {
    for (unsigned col = 0; col < _width; ++col) {
      unsigned srcOffset = getPixelOffset(col, row, _width, _height, _format);
      unsigned destOffset = getPixelOffset(row, _width - col - 1, _height, _width, _format);
      memcpy(newPixels + destOffset, _pixels + srcOffset, _format);
    }
  }
  
  free(_pixels);
  _pixels = newPixels;
  
  unsigned swapTmp = _height;
  _height = _width;
  _width = swapTmp;
}

void Bitmap::copyRectFromBitmap(const Bitmap& src,
                                unsigned srcCol,
                                unsigned srcRow,
                                unsigned destCol,
                                unsigned destRow,
                                unsigned width,
                                unsigned height)
{
  if (srcCol == 0 && srcRow == 0 && width == 0 && height == 0) {
    width = src.width();
    width = src.height();
  }

  if (width == 0 || height == 0)
    throw std::runtime_error("can't copy zero height/width rect.");
  
  if (srcCol + width >= src.width() || srcRow + height >= src.height())
    throw std::runtime_error("rect doesn't fit in destination bmp.");
  if (_pixels == src._pixels && rectsOverlap(srcCol, srcRow, destCol, destRow, width, height))
    throw std::runtime_error("source and destination are the same bitmap, and rects overlap.  not allowed!  fuck you!");

  FormatConverterFunc converter = NULL;
  if (_format != src._format)
    converter = ConverterFuncForFormats(_format, src._format);

  for (unsigned row = 0; row < height; ++row) {
    for (unsigned col = 0; col < width ; ++col) {
      unsigned char* srcPixel = src._pixels + getPixelOffset(srcCol + col, srcRow + row, src._width, src._height, src._format);
      unsigned char* destPixel = _pixels + getPixelOffset(destCol + col, destRow + row, _width, _height, _format);

      if (converter) {
        converter(srcPixel, destPixel);
      } else {
        memcpy(destPixel, srcPixel, _format);
      } 
    }
  }
}

void Bitmap::_set(unsigned width,
                  unsigned height,
                  Format format,
                  const unsigned char* pixels)
{
  if (width == 0) throw std::runtime_error("zero width bitmap.");
  if (height == 0) throw std::runtime_error("zero height bitmap.");
  if (format <= 0 || format > 4) throw std::runtime_error("invalid bitmap format.");

  _width = width;
  _height = height;
  _format = format;

  size_t newSize = _width * _height * _format;

  if (_pixels) {
    _pixels = (unsigned char*)realloc(_pixels, newSize);
  } else {
    _pixels = (unsigned char*)malloc(newSize);
  }

  if (pixels)
    memcpy(_pixels, pixels, newSize);
}
