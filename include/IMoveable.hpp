#ifndef IMOVABLE_HPP_
#define IMOVABLE_HPP_

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

  virtual ~IMoveable() {}
};

class MoveableBase : public IMoveable {
protected:
  glm::mat4 model_;
  glm::vec3 position_;
  glm::vec3 forward_;
  glm::vec3 up_ = glm::vec3(0, 1, 0);

public:
  MoveableBase(glm::vec3 position, glm::vec3 target);

  const glm::vec3 &position() const override { return position_; }
  const glm::vec3 &forward() const override { return forward_; }
  const glm::vec3 &up() const override { return up_; }

  const glm::mat4 &model() const override { return model_; }

  virtual void shiftBy(glm::vec3 dp) override;
  virtual void moveTo(glm::vec3 newPosition) override;
  virtual void rotateAround(glm::vec3 v, float rads) override;

  virtual ~MoveableBase() {}
};

#endif // !IMOVABLE_HPP_
