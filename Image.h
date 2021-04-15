#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>

  

constexpr int tileSize = 32;

struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

constexpr Pixel backgroundColor{0, 0, 0, 0};
constexpr Pixel backgroundWhite{247, 247, 247, 0};

struct Image
{
  explicit Image(const std::string &a_path);
  Image(int a_width, int a_height, int a_channels);

  int Save(const std::string &a_path);
  //void DrawTile(Image &Tile, Image &screenBuffer, int wi, int he);
  int Width()    const { return width; }
  int Height()   const { return height; }
  int Channels() const { return channels; }
  size_t Size()  const { return size; }
  Pixel* Data()        { return  data; }
  Pixel GetPixel(int x, int y) { return data[width * y + x];}
  void  PutPixel(int x, int y, const Pixel &pix) { data[width* y + x] = pix; }

  Image& operator= (Image& img) {
    width = img.width;
    height = img.height;
    channels = img.channels;
    size = img.size;
    for (int i = 0; i < width*height; i++)
    {
      data[i]=img.data[i];
    }
    self_allocated = img.self_allocated;
    return *this;
  }
  ~Image();

private:
  int width = -1;
  int height = -1;
  int channels = 3;
  size_t size = 0;
  Pixel *data = nullptr;
  bool self_allocated = false;
};



#endif //MAIN_IMAGE_H
