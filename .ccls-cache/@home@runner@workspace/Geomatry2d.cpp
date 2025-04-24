#include "Geomatry2D.h"

namespace Geomatry2D{
  double calculateArea(double radius){
    return 3.1415 * (radius * radius);
  } 
  double calculateArea(double height, double width){
    return height * width;
  }
}
