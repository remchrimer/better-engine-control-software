//
// Created by Cameron on 5/13/21.
//

#ifndef ENGINECONTROLSYSTEM_PIUTILS_H
#define ENGINECONTROLSYSTEM_PIUTILS_H

#include <cstdint>

// TODO: we have these structs duplicated in the separate ecs-utils repo. maybe merge the two together, so they can
// share the same struct definition?
/**
 * Packed struct for data Teensy Arduino sends over serial
 */
#pragma pack(push, 1)
typedef struct TeensyData {
    int16_t adc0, adc1, adc2, adc3, adc4, adc5, adc6, adc7, adc8, adc9, adc10, adc11;
    int32_t loadCell0;
    double tc0, tc1, tc2, tc3, tc4, tc5, tc6, tc7;
} TeensyData;
#pragma pack(pop)


/**
 * Packed struct containing teensy data as well as a verification number
 * to check for data correctness
 */
#pragma pack(push, 1)
typedef struct WrappedPacket {
    TeensyData dataPacket;
    uint16_t packetCRC;
} WrappedPacket;
#pragma pack(pop)

/**
 * Packed struct containing data from ADCS as well as a verification number
 * to check for data correctness
 */
#pragma pack(push, 1)
typedef struct AdcBreakoutSensorData {
    int16_t adc0, adc1, adc2, adc3, adc4, adc5, adc6, adc7, adc8, adc9, adc10, adc11;
    uint16_t packetCRC;
} AdcBreakoutSensorData;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct PropBoardSensorData {
    uint32_t timestamp;
    int32_t adc0, adc1, adc2, adc3, adc4, adc5, adc6, adc7, adc8, adc9, adc10, adc11, adc12, adc13, adc14;
    int32_t loadCellRaw;
    float tcInternalTemp;
    float tcTemp1, tcTemp2, tcTemp3, tcTemp4;
    // fault flags is a bitfield [fault, open, gnd, vcc] from LSB to MSB for each tc reader, 1 meaning that fault is active
    uint8_t tcFaultFlags[4];
    uint16_t packetCRC;
} PropBoardSensorData;
#pragma pack(pop)


static_assert(sizeof(PropBoardSensorData) == 94, "Packed struct size check");



/*
 * The below is a interesting hack to print the size of a struct at compile time
 * This is useful to discover the size of a certain packed struct without needing to run the exe on a pi
 *
 * invoke the function with something like
 *  print_size_as_warning<sizeof(int)>()();
 *  notice the sizeof(int)!
 *
 * on msvc, i get compiler warnings like
 *  note: while compiling class template member function 'char print_size_as_warning<4>::operator ()(void)'
 * the 4 is the size of int
 *
 * we should probably only sparingly use it if the struct changes if we want to go for -Werror *
 */
template<int N>
struct print_size_as_warning
{
    char operator()() { return N + 256; } //deliberately causing overflow
};

#endif //ENGINECONTROLSYSTEM_PIUTILS_H
