-- vertex
#version 330 core 

uniform mat3 normalMatrix;
uniform vec3 lightPosition;
uniform mat4 lightSpaceMatrix;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;


in vec3 vertexPosition;
in vec2 textureCoord;
in vec3 normal;

out VS_OUT {
  vec4 fragPosWorldSpace;
  vec4 fragPosLightSpace;
  vec2 texCoord;
  vec3 n; // Вектора нормали
  vec3 l; // Направления на источники света
} vsOut;

void main() {
  vec4 p = view*model*vec4(vertexPosition, 1.0);

  gl_Position = perspective*p;
  vsOut.texCoord = textureCoord;
  vsOut.fragPosWorldSpace = model*vec4(vertexPosition, 1.0);
  vsOut.fragPosLightSpace = lightSpaceMatrix*vsOut.fragPosWorldSpace;
  vsOut.n = normalize(normalMatrix*normal);
  vsOut.l = normalize(lightPosition - vec3(p));
}
-- fragment
#version 330 core

uniform sampler2D shadowMap;
uniform sampler2D diffuseTexture;


in VS_OUT {
  vec4 fragPosWorldSpace;
  vec4 fragPosLightSpace;
  vec2 texCoord;
  vec3 n; // Вектора нормали
  vec3 l; // Направления на источники света
} fsIn;

// 1 -- полная тень
// 0 -- отсутствие тени
float calculateShadow(vec4 fragPosLightSpace, sampler2D shadowMap) {
  // Перспективно делим
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; 
  // Приводим координаты к интервалу [0, 1]
  projCoords = 0.5 * projCoords + 0.5;

  float minDepth = texture(shadowMap, projCoords.xy).r;
  float curDepth = fragPosLightSpace.z;

  float shadow = curDepth <= minDepth ? 0.0 : 1.0;
  shadow = minDepth;

  return shadow;
}

out vec4 color;

void main() {
  vec4 lightColor = vec4(1, 1, 1, 1);
  float kd = 0.8;
  float ka = 0.2;

  vec4 texColor = texture(diffuseTexture, fsIn.texCoord);
  color = vec4(0);
  vec3 n2 = normalize(fsIn.n); // Нормируем интерполируемую нормаль
  vec3 l2 = normalize(fsIn.l); // И направление на источник света
  float diff = max(dot(n2,l2), 0.0); // диффузионная составляющая света
  float shadow = calculateShadow(fsIn.fragPosLightSpace, shadowMap);
  // color += ((ka + (1 - shadow)*kd*diff)*lightColor * texColor);
  color = vec4(vec3(shadow), 1);
}

