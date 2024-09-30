#ifndef SCENE_HPP_
#define SCENE_HPP_
// Definition of scene -- container of objects

#include "Camera.hpp"
#include "IUpdateable.hpp"
#include "Object.hpp"
#include <memory>


class Scene : public IUpdateable {
  std::map<Id, ptr<Object>> objects_;
  std::map<Id, ptr<Camera>> cameras_;

  public:
  Scene() = default;
  void addObject(ptr<Object> object);
  void addCamera(ptr<Camera> camera);

  void update() override;
};
#endif // !SCENE_HPP_
