#ifndef EVENTS_HPP_
#define EVENTS_HPP_

#include <iterator>
#include <list>
template <typename ...Args>
class IEventHandler {
  public:
    virtual void call(Args...args) = 0;
    virtual bool operator!=(const IEventHandler<Args...> & other) {
      return this != &other;
    }
  protected:
    IEventHandler() {}
};


template <typename ...Args>
class IEvent {
  public:
    virtual void operator+=(IEventHandler<Args...> &handler) = 0;
    virtual void operator-=(IEventHandler<Args...> &handler) = 0;
};

template <typename ...Args>
class Event : public IEvent<Args...> {
  using THandler = IEventHandler<Args...>;
  std::list<THandler*> handlers;
  public:
  void operator+=(IEventHandler<Args...> & handler) override final {
    handlers.insert(std::begin(handlers), &handler);
  }

  void operator-=(IEventHandler<Args...> & handler) override final {
    auto it = std::begin(handlers);
    while(**it != handler) ++it;

    if(it == std::end(handlers)) return;
    handlers.erase(it);
  }

  virtual void invoke(Args... args) {
    for(auto & handler : handlers) 
      handler->call(args...);
  }
};

#endif // !EVENTS_HPP_
