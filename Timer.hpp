#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdint>
#include <string>

namespace wl {

class Timer {
   public:
    Timer();
    virtual ~Timer() = default;

    int8_t getHours() const;
    int8_t getMinutes() const;
    int8_t getSeconds() const;

    std::string toString() const;
    void display() const;
    void tick();

    Timer &operator++();

    Timer &operator+(Timer const &rhs);
    Timer &operator-(Timer const &rhs);

   private:
    int16_t hours;
    int8_t minutes;
    int8_t seconds;
};

}  // namespace wl

#endif
