#include "PrismVolume.h"

PrismVolume::PrismVolume(int l, int w, int h)
    : length(l), width(w), height(h) {}

double PrismVolume::getVolume() const { return length * width * height; }