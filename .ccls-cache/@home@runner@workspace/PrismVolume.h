// Begin Header Guard
#ifndef PRISM_VOLUME_H
#define PRISM_VOLUME_H
#include <string>

class PrismVolume {
private:
  int length;
  int width;
  int height;

public:
  PrismVolume(int l, int w, int h);
  double getVolume() const;
};

#endif
// End Header Guard