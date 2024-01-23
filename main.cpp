#include <csignal>
#include <iostream>
#include <thread>

#include "Timer.hpp"

static bool isRunning = true;
static bool displayTime = true;
static bool isPaused = false;
static bool resetDisplay = false;

std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    std::tm *ltm = std::localtime(&now);

    std::string currentTime{""};
    if (ltm->tm_hour < 10) currentTime += "0";
    currentTime += std::to_string(ltm->tm_hour) + ":";
    if (ltm->tm_min < 10) currentTime += "0";
    currentTime += std::to_string(ltm->tm_min) + ":";
    if (ltm->tm_sec < 10) currentTime += "0";
    currentTime += std::to_string(ltm->tm_sec);

    return currentTime;
}

// Handle interrupt signal
void sigintHandler(int sig_num) {
    std::cout << "\nCtrl-C received. Exiting...\n";
    isRunning = false;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::signal(SIGINT, SIG_DFL);
}

void sigcontHandler(int sig_num);

// Handle suspend signal
void sigtstpHandler(int sig_num) {
    isPaused = true;
    std::cout << "\nCtrl-Z received. Pausing...\n";

    // Pause
    std::cout << "Paused at: " << getCurrentTime() << "\n\n";

    // Register signal SIGCONT and signal handler
    std::signal(SIGCONT, sigcontHandler);

    // Restore and execute the default signal handler for SIGTSTP
    std::signal(SIGTSTP, SIG_DFL);
    std::raise(SIGTSTP);
}

// Handle foreground signal
void sigcontHandler(int sig_num) {
    std::cout << "\nResuming timer...\n";

    // Register signal SIGTSTP and signal handler
    std::signal(SIGTSTP, sigtstpHandler);

    // Restore the default signal handler for SIGCONT
    std::signal(SIGCONT, SIG_DFL);

    // Resume
    std::cout << "Resumed at: " << getCurrentTime() << "\n\n";

    std::this_thread::sleep_for(std::chrono::seconds(1));

    resetDisplay = true;
    isPaused = false;
}

void timerLoop(wl::Timer *t) {
    while (isRunning) {
        while (isPaused) std::this_thread::sleep_for(std::chrono::seconds(1));

        if (resetDisplay) {
            t->firstDisplay = true;
            resetDisplay = false;
        }

        ++(*t);
        if (displayTime) t->display();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    wl::Timer *t{new wl::Timer()};

    // Register signal SIGINT and signal handler
    std::signal(SIGINT, sigintHandler);

    // Register signal SIGTSTP and signal handler
    std::signal(SIGTSTP, sigtstpHandler);

    std::thread timerThread(timerLoop, t);
    timerThread.join();

    std::cout << "Timer stopped at: " << getCurrentTime() << "\n";
    std::cout << std::endl;

    delete t;

    return 0;
}
