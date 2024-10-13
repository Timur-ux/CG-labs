#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include "Square2d.hpp"
#include "events.hpp"
#include <cmath>
class Button : public Square2d, public IEventHandler<GLfloat, GLfloat> {
  Event<> onClick_;
  IEvent<GLfloat, GLfloat> &clickOnWindowEvent_;

  public:
    Button(GLfloat center[2], GLfloat width, GLfloat height, Program program, IEvent<GLfloat, GLfloat> &clickOnWindowEvent) : Square2d(center, width, height, program), onClick(onClick_), clickOnWindowEvent_(clickOnWindowEvent)
  {
    clickOnWindowEvent_ += *this; 
  };

  void call(GLfloat  x, GLfloat y) override {
    if(std::abs(x - x_) <= width_ && std::abs(y - y_) <= height_) 
      onClick_.invoke(); // В этом случае на нас тыкнули, значит
                         // мы должны тыкнуть все подписчеков
  };
  IEvent<> &onClick;

  ~Button() {
    clickOnWindowEvent_ -= *this;
  }
};

#endif // !BUTTON_HPP_
