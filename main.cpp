#include <pthread.h>

#include <csignal>
#include <iostream>
#include <thread>

#include "Timer.hpp"

static bool isRunning = true;
static bool displayTime = true;

// Handle interrupt signal
void sigintHandler(int sig_num) {
    // Restore the cursor position
    std::cout << "\nCtrl-C received. Exiting...\n";
    isRunning = false;
}

void *timerLoop(void *arg) {
    wl::Timer *t{static_cast<wl::Timer *>(arg)};

    // Register signal SIGINT and signal handler
    std::signal(SIGINT, sigintHandler);

    while (isRunning) {
        ++(*t);
        if (displayTime) t->display();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    pthread_exit(NULL);
}

int main() {
    wl::Timer *t{new wl::Timer()};
    std::cout << "==============================\n"
              << "    WorkTimer - Counter\n\n";

    pthread_t timerThread;
    pthread_create(&timerThread, NULL, timerLoop, t);

    pthread_join(timerThread, NULL);

    std::cout << "Timer stopped.\n";
    std::cout << std::endl;

    delete t;

    return 0;
}
