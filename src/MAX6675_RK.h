#ifndef __MAX6675_RK_H
#define __MAX6675_RK_H

#include "Particle.h"

// Repository: https://github.com/rickkas7/MAX6675_RK
// License: MIT

/**
 * This class is a singleton; you do not create one as a global, on the stack, or with new.
 * 
 * From global application setup you must call:
 * MAX6675_RK::instance().setup();
 */
class MAX6675_RK {
public:
    /**
     * @brief Container for information about a sensor including its SPI interface and CS pin
     */
    class Sensor {
    public:
        SPIClass *spi = &SPI;
        pin_t csPin = SS;
        bool openDetection = true;

        /**
         * @brief Default constructor using primary SPI (SPI) and default CS pin (SS)
         */
        Sensor() {};

        /**
         * @brief Constructor using primary SPI (SPI) and a user-specified CS pin
         * 
         * @param csPin Pin to use for chip select
         */
        Sensor(pin_t csPin) { this->csPin = csPin; };

        /**
         * @brief Constructor using a specific SPI interface and a user-specified CS pin
         * 
         * @param spi SPI interface to use, such as &SPI, &SPI1, ...
         * @param csPin Pin to use for chip select
         */
        Sensor(SPIClass *spi, pin_t csPin) { this->spi = spi; this->csPin = csPin; };

        /**
         * @brief Copy constructor
         * 
         * @param other 
         */
        Sensor(const Sensor &other) { spi = other.spi; csPin = other.csPin; };

        /**
         * @brief Assignment operator
         * 
         * @param other 
         * @return Sensor& 
         */
        Sensor &operator=(const Sensor &other) { spi = other.spi; csPin = other.csPin; return *this; };

        /**
         * @brief Whether to use open sensor detection when using readValue (default: yes/true)
         * 
         * @param value 
         * @return Sensor& 
         * 
         * The MAX6675 has open sensor detection if the T- pin is connected to GND. If there is no
         * thermocouple, bit D2 will be high, indicating the sensor is open or missing.
         * 
         * The readValue() detects this an returns NaN if the sensor is missing.
         * 
         * If you have T- and GND isolated from each other, you must turn open detection off!
         */
        Sensor &withOpenDetection(bool value = true) { openDetection = value; return *this; };

        /**
         * @brief Read the raw 16-bit value from the sensor
         * 
         * @return uint16_t 
         */
        uint16_t readRaw();

        /**
         * @brief Return a value in degrees C
         * 
         * @return float 
         * 
         * Returns NaN if the sensor is not present. See withOpenDetection.
         */
        float readValue();

        /**
         * @brief SPI settings, You won't generally have to change these settings; they're based on what the MAX6675 requires.
         */
        SPISettings spiSettings = SPISettings(4 * MHZ, MSBFIRST, SPI_MODE3);

    };

    /**
     * @brief Gets the singleton instance of this class, allocating it if necessary
     * 
     * Use MAX6675_RK::instance() to instantiate the singleton.
     */
    static MAX6675_RK &instance();

    /**
     * @brief Add a sensor using a Sensor object
     * 
     * @param sensor 
     * @return MAX6675_RK& 
     */
    MAX6675_RK &withSensor(Sensor sensor) { sensors.append(sensor); return *this; };


    /**
     * @brief Add a sensor with the specified CS pin on primary SPU
     * 
     * @param sensor 
     * @return MAX6675_RK& 
     * 
     * For more complex scenarios, use a Sensor object.
     */
    MAX6675_RK &withSensor(pin_t csPin) { sensors.append(Sensor(csPin)); return *this; };

    /**
     * @brief Perform setup operations; call this from global application setup()
     * 
     * You typically use MAX6675_RK::instance().setup();
     */
    void setup();

    void runForAllSensors(std::function<void(Sensor &sensor)> fn);

    /**
     * @brief Locks the mutex that protects shared resources
     * 
     * This is compatible with `WITH_LOCK(*this)`.
     */
    void lock() { os_mutex_recursive_lock(mutex); };

    /**
     * @brief Attempts to lock the mutex that protects shared resources
     * 
     * @return true if the mutex was locked or false if it was busy already.
     */
    bool tryLock()  { return os_mutex_recursive_trylock(mutex); };

    /**
     * @brief Unlocks the mutex that protects shared resources
     */
    void unlock()  { os_mutex_recursive_unlock(mutex); };

    /**
     * @brief Locks the mutex that protects shared resources (const)
     * 
     * This is compatible with `WITH_LOCK(*this)`.
     */
    void lock() const{ os_mutex_recursive_lock(mutex); };

    /**
     * @brief Attempts to lock the mutex that protects shared resources (const)
     * 
     * @return true if the mutex was locked or false if it was busy already.
     */
    bool tryLock() const { return os_mutex_recursive_trylock(mutex); };

    /**
     * @brief Unlocks the mutex that protects shared resources (const)
     */
    void unlock() const { os_mutex_recursive_unlock(mutex); };


protected:
    /**
     * @brief The constructor is protected because the class is a singleton
     * 
     * Use MAX6675_RK::instance() to instantiate the singleton.
     */
    MAX6675_RK();

    /**
     * @brief The destructor is protected because the class is a singleton and cannot be deleted
     */
    virtual ~MAX6675_RK();

    /**
     * This class is a singleton and cannot be copied
     */
    MAX6675_RK(const MAX6675_RK&) = delete;

    /**
     * This class is a singleton and cannot be copied
     */
    MAX6675_RK& operator=(const MAX6675_RK&) = delete;

    /**
     * @brief Worker thread function
     * 
     * This method is called to perform operations in the worker thread.
     * 
     * You generally will not return from this method.
     */
    os_thread_return_t threadFunction(void);

    /**
     * @brief Mutex to protect shared resources
     * 
     * This is initialized in setup() so make sure you call the setup() method from the global application setup.
     */
    mutable os_mutex_t mutex = 0;

    /**
     * @brief Worker thread instance class
     * 
     * This is initialized in setup() so make sure you call the setup() method from the global application setup.
     */
    Thread *thread = 0;

    /**
     * @brief Vector of Sensor objects
     */
    Vector<Sensor> sensors;

    /**
     * @brief Singleton instance of this class
     * 
     * The object pointer to this class is stored here. It's NULL at system boot.
     */
    static MAX6675_RK *_instance;

};
#endif  /* __MAX6675_RK_H */
