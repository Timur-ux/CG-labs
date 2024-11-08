#ifndef GLSL_DATA_NAMING_HPP_
#define GLSL_DATA_NAMING_HPP_


namespace uniforms {
  inline const char * texture0 = "main_texture0";
  inline const char * shadowMapTexture = "shadowMap_texture1";

  inline const char * modelMatrix = "model";
  inline const char * viewMatrix = "view";
  inline const char * perspectiveMatrix = "perspective";

  inline const char * lights = "lights";
  inline const char * cameraPos = "cameraPos";

  inline const char * lightSpaceMatrix = "lightSpaceMatrix";
}; // !uniforms

namespace attributes {
  inline const char * vertexPosition = "vertexPosition";
  inline const char * texturePosition = "textureCoord";
  inline const char * normalToBorder = "normal";
}; // !attributes

#endif // !GLSL_DATA_NAMING_HPP_
