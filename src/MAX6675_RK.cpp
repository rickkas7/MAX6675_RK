#include "MAX6675_RK.h"

MAX6675_RK *MAX6675_RK::_instance;

// [static]
MAX6675_RK &MAX6675_RK::instance() {
    if (!_instance) {
        _instance = new MAX6675_RK();
    }
    return *_instance;
}

MAX6675_RK::MAX6675_RK() {
}

MAX6675_RK::~MAX6675_RK() {
}

void MAX6675_RK::setup() {
    os_mutex_recursive_create(&mutex);

    thread = new Thread("MAX6675_RK", [this]() { return threadFunction(); }, OS_THREAD_PRIORITY_DEFAULT, 3072);
}

os_thread_return_t MAX6675_RK::threadFunction(void) {
    while(true) {
        // Put your code to run in the worker thread here
        delay(1);
    }
}

