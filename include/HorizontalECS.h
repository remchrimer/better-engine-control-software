//
// Created by kevin on 5/23/2022.
//

#ifndef BETTER_ENGINE_CONTROL_SOFTWARE_HORIZONTALECS_H
#define BETTER_ENGINE_CONTROL_SOFTWARE_HORIZONTALECS_H
#include "generic/IECS.h"
#include "generic/ICommBoundary.h"
#include <chrono>
#include <queue>

class HorizontalECS: public IECS{
public:
    HorizontalECS(ICommBoundary* net, IPhysicalBoundary* bound, std::queue<CommandData*> comQueue,
                  WatchDog* wDog, Sequencer* seq, bool autoControlled);
    void reportToBoundary() override;
    void stepECS() override;
    void acceptCommand(CommandData* commands) override;

protected:
    uint64_t getTimeStamp();

    ICommBoundary* networker;
    IPhysicalBoundary* boundary;
    std::queue<CommandData*> commandQueue;

    WatchDog* watchDog;
    Sequencer* sequencer;

    bool autoControlled;
};
#endif //BETTER_ENGINE_CONTROL_SOFTWARE_HORIZONTALECS_H
