#ifndef IUPDATEABLE_HPP_
#define IUPDATEABLE_HPP_

class IUpdateable {
  public:
    virtual void update(const double & time, const double & dt) = 0;
};

#endif // !IUPDATEABLE_HPP_
