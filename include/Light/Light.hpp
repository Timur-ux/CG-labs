#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include "CameraMVP.hpp"
#include "DepthFramebuffer.hpp"
#include "IMoveable.hpp"
#include "meshes/Mesh.hpp"

// Структура для передачи данных о свете внутрь программы шейдера
// Для корректной работы структура LightData должна совпадать с оной в шейдере
struct LightData {
  static const int MAXIMUM_TEXTURE_BLOCKS = 8;
  static const int DEPTH_MAP_OFFSET =
      1; // До какого блока у нас лежат обычные текстуры и с какого блока у нас
         // начинаются карты глубин
  glm::mat3 normalMatrix;  // Матрица нормализации нормалей
  glm::vec3 lightPosition; // Позиция света
  glm::vec4 color;         // Цвет источника

  float kDiffuse; // Коэффициент диффузионной составляющей света
  float kAmbient; // Коэффициент фонового света
  float kGlare;   // Коэффициент блика
  glm::mat4
      lightSpaceMatrix; // Проективно-видовая матрица позиции источника света
  DepthFramebuffer &depthFramebuffer; // Отсюда будет вытаскиваться карта глубин
  int enabled; // Включен ли данный источник света

  LightData(glm::mat3 theNormalMatrix, glm::vec3 theLightPosition,
            glm::vec4 theColor, float theKDiffuse, float theKAmbient,
            float theKGlare, glm::mat4 theLightSpaceMatrix,
            DepthFramebuffer &theDepthFramebuffer, int theEnabled)
      : normalMatrix(theNormalMatrix), lightPosition(theLightPosition),
        color(theColor), kDiffuse(theKDiffuse), kAmbient(theKAmbient),
        kGlare(theKGlare), lightSpaceMatrix(theLightSpaceMatrix),
        depthFramebuffer(theDepthFramebuffer), enabled(theEnabled) {}

  /**
   * @brief set fields as uniforms in program
   *
   * @param program -- required program
   * @param i -- index in array of structs
   * @param startLoc -- location used in shader to array of structs
   */
  virtual bool setAsUniform(Program &program, size_t i, size_t startLoc);

  /**
   * @brief Используется для определения позиции массива с данными о свете
   *
   * @return строка вида <имя массива>[0].<имя первого поля>
   */
  static std::string firstField();
};

class ILight {
  virtual LightData getData(const Mesh &object) = 0;

public:
  virtual void renderToShadowMap(const std::vector<Mesh *> &objects) = 0;
  virtual void bindDepthMapTo(int block) = 0;
  virtual bool setLightParamsFor(const Mesh &object, size_t i) = 0;
  virtual void swapState() = 0;
  virtual ~ILight() {}
};

class LightBase : public ILight, public Transform {
protected:
  CameraMVP &cameraData_;
  Program &program_;
  Program &shadowMapProgram_;

  glm::vec4 color_;
  GLfloat kDiffuse_;
  GLfloat kAmbient_;
  GLfloat kGlare_ = 0;

  glm::mat4 lightProjection_;
  glm::mat4 lightSpaceMatrix_; //  lightProjection*view

  GLfloat n_ = 1.0f, f_ = 100.0f;
  DepthFramebuffer depthFramebuffer_;

  LightData getData(const Mesh &object) override;
  bool enabled_ = true;
public:
  LightBase(glm::vec3 position, glm::vec3 target, CameraMVP &cameraData,
            Program &program, DepthFramebuffer &&framebuffer,
            glm::vec4 color = {1, 1, 1, 1}, GLfloat kDiffuse = 0.6,
            GLfloat kAmbient = 0.2, GLfloat kGlare = 0.2);

  virtual void renderToShadowMap(const std::vector<Mesh *> &objects) override;
  virtual void bindDepthMapTo(int block) override;
  virtual bool setLightParamsFor(const Mesh &object, size_t i) override;
  virtual void swapState() override {enabled_ = !enabled_;}
};

#endif // !LIGHT_HPP_
