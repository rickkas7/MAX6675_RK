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
     * @brief Gets the singleton instance of this class, allocating it if necessary
     * 
     * Use MAX6675_RK::instance() to instantiate the singleton.
     */
    static MAX6675_RK &instance();

    /**
     * @brief Perform setup operations; call this from global application setup()
     * 
     * You typically use MAX6675_RK::instance().setup();
     */
    void setup();

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
     * @brief Singleton instance of this class
     * 
     * The object pointer to this class is stored here. It's NULL at system boot.
     */
    static MAX6675_RK *_instance;

};
#endif  /* __MAX6675_RK_H */
