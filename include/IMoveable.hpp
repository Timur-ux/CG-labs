#ifndef IMOVABLE_HPP_
#define IMOVABLE_HPP_

#include "events.hpp"
#include <glm/glm.hpp>

class IMoveable {
public:
  virtual const glm::vec3 &position() const = 0;
  virtual const glm::vec3 &forward() const = 0;
  virtual const glm::vec3 &up() const = 0;

  virtual const glm::mat4 &model() const = 0;

  virtual void shiftBy(glm::vec3 dp) = 0;
  virtual void moveTo(glm::vec3 newPosition) = 0;
  virtual void rotateAround(glm::vec3 v, float rads) = 0;
  virtual void lookInto(glm::vec3 direction) = 0;
  virtual void lookAt(glm::vec3 position) = 0;
  virtual void scaleBy(glm::vec3 scale) = 0;
  virtual void freezeUp() = 0;

  virtual ~IMoveable() {}
};

class MoveableBase : public IMoveable, public IEventHandler<> {
protected:
  glm::mat4 translateModel_;
  glm::mat4 rotateModel_;
  glm::mat4 model_;

  glm::vec3 position_;
  glm::vec3 forward_;
  glm::vec3 up_ = glm::vec3(0, 1, 0);

  Event<> onMove_;

  MoveableBase *following_ = nullptr;
  glm::vec3 followOffset_{0};

  bool upFreezed_ = false;

public:
  MoveableBase(glm::vec3 position = glm::vec3(0), glm::vec3 forward = glm::vec3(0, 0, 1));

  IEvent<> & onMove;

  const glm::vec3 &position() const override { return position_; }
  const glm::vec3 &forward() const override { return forward_; }
  const glm::vec3 &up() const override { return up_; }

  const glm::mat4 &model() const override { return model_; }

  virtual void shiftBy(glm::vec3 dp) override;
  virtual void moveTo(glm::vec3 newPosition) override;
  virtual void rotateAround(glm::vec3 v, float rads) override;
  virtual void lookInto(glm::vec3 direction) override;
  virtual void lookAt(glm::vec3 position) override;
  virtual void scaleBy(glm::vec3 scale) override;
  virtual void freezeUp() override {upFreezed_ = !upFreezed_;};

  void follow(MoveableBase *objToFollow, glm::vec3 followOffset);

  virtual void call() override;

  virtual ~MoveableBase() {}
};

#endif // !IMOVABLE_HPP_
