#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include "Framebuffer.hpp"
#include "objects/Object.hpp"
#include <list>

// Для корректной работы структура LightData должна совпадать с оной в шейдере
struct LightData {
  glm::mat3 normalMatrix;  // Матрица нормализации нормалей
  glm::vec3 lightPosition; // Позиция света
  glm::vec4 color;         // Цвет источника

  float kDiffuse; // Коэффициент диффузионной составляющей света
  float kAmbient; // Коэффициент фонового света
  float kGlare;   // Коэффициент блика
  glm::mat4 lightSpaceMatrix;
  Texture2D *shadowMap_texture1;

  LightData(glm::mat3 theNormalMatrix, glm::vec3 theLightPosition,
            glm::vec4 theColor, float theKDiffuse, float theKAmbient,
            float theKGlare, glm::mat4 theLightSpaceMatrix,
            Texture2D *theShadowMap)
      : normalMatrix(theNormalMatrix), lightPosition(theLightPosition),
        color(theColor), kDiffuse(theKDiffuse), kAmbient(theKAmbient),
        kGlare(theKGlare), lightSpaceMatrix(theLightSpaceMatrix),
        shadowMap_texture1(theShadowMap) {}

  /**
   * @brief set fields as uniforms in program
   *
   * @param program -- required program
   * @param i -- index in array of structs
   * @param startLoc -- location used in shader to array of structs
   */
  virtual bool setAsUniform(Program &program, size_t i, size_t startLoc,
                            size_t shaderMapTextureId = 1);
  static std::string firstField() { return "normalMatrix"; }
};

class ILight {
public:
  virtual void attachTo(IMoveable *newHost) = 0;
  virtual LightData getData(const Object &object) = 0;
  virtual void renderToShadowMap(const std::list<Object *> &objects) = 0;
  virtual ~ILight() {}
};

#endif // !LIGHT_HPP_
