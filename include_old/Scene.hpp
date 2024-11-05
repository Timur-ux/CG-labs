#ifndef SCENE_HPP_
#define SCENE_HPP_
// Definition of scene -- container of objects

#include "Camera.hpp"
#include "IUpdateable.hpp"
#include "Object.hpp"
#include "events.hpp"

class UpdateEventHandler;
class Scene : public IUpdateable {
  std::map<Id, Object*> objects_;
  std::map<Id, Camera*> cameras_;
  ptr<UpdateEventHandler> objectUpdateEventHandler_;
  Event<const double &, const double &> timeUpdateEvent_;

  public:
  Scene();
  IEvent<const double &, const double &> &updateEvent;
  void addObject(Object* object);
  void addCamera(Camera* camera);

  void update(const double & time, const double & dt) override;
  void initialUpdate(const double & dt); // Call before start application loop
};

class UpdateEventHandler : public IEventHandler<Object*> {
  std::vector<Object*> movedObjects;
  public:
  void call(Object*) override;
  std::vector<Object*>&& getAndReset();
};
#endif // !SCENE_HPP_
