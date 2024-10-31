#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include "objects/Object.hpp"

struct LightData {
  glm::mat3 normalMatrix; // Матрица нормализации нормалей
  glm::vec3 lightPosition; // Позиция света
  glm::vec4 color; // Цвет источника

  float kDiffuse; // Коэффициент диффузионной составляющей света
  float kAmbient; // Коэффициент фонового света
  float kGlare; // Коэффициент блика 
                
  LightData(
 glm::mat3 theNormalMatrix,
 glm::vec3 theLightPosition,
 glm::vec4 theColor,
 float theKDiffuse,
 float theKAmbient,
 float theKGlare
      )
    : normalMatrix(theNormalMatrix)
    , lightPosition(theLightPosition)
    , color(theColor)
    , kDiffuse(theKDiffuse)
    , kAmbient(theKAmbient)
    , kGlare(theKGlare) {}



  /**
   * @brief set fields as uniforms in program
   *
   * @param program -- required program
   * @param i -- index in array of structs
   * @param startLoc -- location used in shader to array of structs
   */
  virtual bool setAsUniform(Program & program, size_t i, size_t startLoc);
  static std::string firstField() {return "normalMatrix";}
};

class ILight {
  public:
    virtual void attachTo(IMoveable *newHost) = 0; 
    virtual LightData getData(const Object & object) const = 0;
};


#endif // !LIGHT_HPP_
