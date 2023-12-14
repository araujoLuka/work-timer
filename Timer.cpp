#include "Timer.hpp"

#include <algorithm>
#include <iostream>

namespace wl {

Timer::Timer() : firstDisplay{true}, hours{0}, minutes{0}, seconds{0} {}

int8_t Timer::getHours() const { return this->hours; }
int8_t Timer::getMinutes() const { return this->minutes; }
int8_t Timer::getSeconds() const { return this->seconds; }

// Format: hh:mm:ss
std::string Timer::toString() const {
    std::string result{""};

    if (this->hours < 10) result += "0";
    result += std::to_string(this->hours) + ":";

    if (this->minutes < 10) result += "0";
    result += std::to_string(this->minutes) + ":";

    if (this->seconds < 10) result += "0";
    result += std::to_string(this->seconds);

    return result;
}

void Timer::display() {
    std::string displayMessage{
        "==============================\n"
        "   WorkTimer - Counter\n"
        "==============================\n"
        " - Elapsed Time: " +
        toString() + '\n' +
        "==============================\n"
        "\n Press Ctrl+C to exit"
        "\n Press Ctrl+Z to pause\n\n"};

    // Calculate the number of lines in the message
    int messageLines = std::count(displayMessage.begin(), displayMessage.end(), '\n');

    // Return to the beginning of the line using \033[F
    std::string returnToStart = "\033[" + std::to_string(messageLines) + "F";

    if (!this->firstDisplay) std::cout << returnToStart;

    std::cout << displayMessage;
    firstDisplay = false;
}

void Timer::tick() {
    this->seconds++;

    if (this->seconds == 60) {
        this->seconds = 0;
        this->minutes++;
    }

    if (this->minutes == 60) {
        this->minutes = 0;
        this->hours++;
    }
}

Timer &Timer::operator++() {
    this->tick();
    return *this;
}

Timer &Timer::operator+(Timer const &rhs) {
    this->hours += rhs.getHours();
    this->minutes += rhs.getMinutes();
    this->seconds += rhs.getSeconds();

    if (this->seconds >= 60) {
        this->seconds -= 60;
        this->minutes++;
    }

    if (this->minutes >= 60) {
        this->minutes -= 60;
        this->hours++;
    }

    return *this;
}

Timer &Timer::operator-(Timer const &rhs) {
    this->hours -= rhs.getHours();
    this->minutes -= rhs.getMinutes();
    this->seconds -= rhs.getSeconds();

    if (this->seconds < 0) {
        this->seconds += 60;
        this->minutes--;
    }

    if (this->minutes < 0) {
        this->minutes += 60;
        this->hours--;
    }

    if (this->hours < 0) {
        this->hours = 0;
        this->minutes = 0;
        this->seconds = 0;
    }

    return *this;
}

}  // namespace wl
