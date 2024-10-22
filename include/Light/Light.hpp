#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include "objects/Object.hpp"

class ILight {
  public:
    virtual void setupLightFor(Object & object) = 0;
};


#endif // !LIGHT_HPP_
