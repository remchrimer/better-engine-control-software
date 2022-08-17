//
// Created by kevin on 6/17/2022.
//

#ifndef BETTER_ENGINE_CONTROL_SOFTWARE_TEENSYBOUNDARY_H
#define BETTER_ENGINE_CONTROL_SOFTWARE_TEENSYBOUNDARY_H

#include "IPhysicalBoundary.h"

#include "phys-boundary/valves/IECSValve.h"
#include "PiUtils.h"

#include <mutex>
#include <thread>
#include <string>
#include <libserial/SerialPort.h>

/**
 * Implementation of IPhysicalBoundary for getting and sending data to horizontal
 * test stand.
 *
 * Sensor readings are passed to this CommBoundary by the Teensy Arduino, requiring
 * us to read data from a serial port. Effector readings and commands are handled
 * directly in this CommBoundary.
 */
class TeensyBoundary: public IPhysicalBoundary{
public:
    TeensyBoundary(std::string adcboardPortLoc, std::string teensyPortLoc);

    /*
     * std::mutex is not copyable or movable
     *
     * Because of this, it's probably better to explicitly delete
     * the copy/move constructor, as well as the general assignment
     * constructor
     */
    TeensyBoundary(const TeensyBoundary& other) = delete;
    TeensyBoundary(TeensyBoundary&& other) = delete;

    TeensyBoundary& operator=(TeensyBoundary other) = delete;

    /**
     * Destructor for boundary
     *
     * Not subtle why the default constructor is good enough
     *
     * The worker thread has to be destroyed when the object is
     * destroyed, otherwise bad times will happen.
     *
     * workerThread is a std::jthread, not a std::thread. jthreads allow us to request
     * for the thread to stop (this stop token can be seen in the constructor),
     * through the request_stop() method. The default destructor for std::jthread
     * will call request_stop() itself if the thread isn't detached, so the cleanup
     * we need is already default
     *
     * Note this means that the choice to use std::jthread over std::thread is critical
     *
     */
    ~TeensyBoundary() = default;


    /**
     * Returns the latest stored sensor data.
     * Locks the sensor data mutex to avoid data races.
     * @return a SensorData object
     */
    SensorData readFromBoundary() override;
    void writeToBoundary(CommandData& data) override;

private:
    /**
     * Reads sensor data from Teensy and ADC's serial ports, as well
     * as raw from the actual valves and shit. To be ran in a separate thread.
     */
    void readPackets();

    /**
     * Updates the storedState field for load cell and TC data packet
     * Locks the sensor data mutex to avoid data races
     * @param wrappedPacket reference to data packet from serial packet
     */
    void readFromTeensy(WrappedPacket& wrappedPacket);

    /**
     * Updates the storedState field to pressurant data from packet
     * Locks the sensor data mutex to avoid data races
     * @param adcPacket reference to data packet from serial packet
     */
    void readFromADCBoard(AdcBreakoutSensorData& adcPacket);

    /**
     * Updates the storedState field with newest data from effectors
     * Effectors are read directly, not through data packet transmission
     * Locks the sensor data mutex to avoid data races
     */
    void readFromEffectors();

    IECSValve* loxPressurant;
    IECSValve* kerPressurant;
    IECSValve* loxPurge;
    IECSValve* kerPurge;
    IECSValve* loxVent;
    IECSValve* kerVent;
    IECSValve* loxFlow;
    IECSValve* kerFlow;
    IECSValve* loxDrip;
    IECSValve* kerDrip;


    /**
     * The sensorDataWriteMutex MUST be locked before trying to read or write to the storedData field for
     * thread safety. Otherwise a data race could occur
     */
    SensorData storedData;
    std::mutex sensorDataWriteMutex;

    /**
     * Order of fields is important here, we want the
     * thread to be initialized AFTER the serial ports
     */
    LibSerial::SerialPort adcboardPort;
    LibSerial::SerialPort teensyPort;

    /**
     * DO NOT detach this thread
     */
    std::jthread workerThread;
};
#endif //BETTER_ENGINE_CONTROL_SOFTWARE_TEENSYBOUNDARY_H
