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

    runForAllSensors([this](Sensor &sensor) {
        sensor.spi->begin(sensor.csPin);
    });

    thread = new Thread("MAX6675_RK", [this]() { return threadFunction(); }, OS_THREAD_PRIORITY_DEFAULT, 3072);
}

void MAX6675_RK::runForAllSensors(std::function<void(Sensor &sensor)> fn) {
    for(auto it = sensors.begin(); it != sensors.end(); it++) {
        fn(*it);
    }
}


os_thread_return_t MAX6675_RK::threadFunction(void) {
    while(true) {
        // Put your code to run in the worker thread here
        delay(1);
    }
}

uint16_t MAX6675_RK::Sensor::readRaw() {
    uint16_t result;

    spi->beginTransaction(spiSettings);
    digitalWrite(csPin, LOW);

    uint8_t b[2];
    b[0] = spi->transfer(0x00);
    b[1] = spi->transfer(0x00);

    result = (b[0] << 8) | b[1];

    digitalWrite(csPin, HIGH);
    spi->endTransaction();

    return result;
}

float MAX6675_RK::Sensor::readValue() {
    uint16_t raw = readRaw();

    if (openDetection) {
        if ((raw & 0b100) != 0) {
            // Open or missing sensor
            return nanf("");
        }
    }

    // Remove low 3 bits
    raw >>= 3;

    // Result is in quarter degree increments
    return ((float)raw) / 4.0;
}
