// Begin Header Guard
#ifndef prismVolume_h
#define prismVolume_h
#include <string>

class PrismVolume {
private:
  int length;
  int width;
  int height;

public:
  prismVolume(int l, int w, int h);
  double getVolume();
};

#endif
// End Header Guard