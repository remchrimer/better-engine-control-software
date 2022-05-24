//
// Created by Neel Sortur on 4/25/22.
//

#ifndef ENGINECONTROLSYSTEM_SEQUENCER_H
#define ENGINECONTROLSYSTEM_SEQUENCER_H

#include "generic/ISequence.h"

class Sequencer {
public:
    explicit Sequencer();

    /**
     * Starts a sequence given the starting time.
     * @param startTime The time value that indicates when the sequence should start
     * @param seq The sequence to be started by the Sequencer
     * @return
     */
    virtual void startSequence(uint64_t startTime, ISequence *seq);

//    /**
//     * Aborts the current sequence that is being run by stopping at the current state and
//     * abandoning the subsequent states
//     * @return
//     */
//    virtual void abortSequence();
//
    /**
     * Checks if a sequence has been started and is not over
     * @return if a sequence is currently running
     */
    virtual bool sequenceRunning();

    /**
     * Steps through the sequence given the current time, either remaining at the current
     * state or moving on to the next in the sequence.
     * @param currTime The current time in the program we are running, the ECS time
     * @return
     */
    virtual ECSState stepSequence(uint64_t currTime);

//    /**
//     * Converts the current sequence to a string representation
//     * @return the current sequence as a string
//     */
//    virtual std::string seqToString();
//
//    virtual std::string getLastAbort();


protected:
    int index;
    uint64_t startOfCurrStateTime;
    bool running;
    ISequence *currSequence;

    // last sequence should be kept in case of abort
    ISequence *lastSequence;
    // ends the current sequence
    void endSequence();
};

#endif //ENGINECONTROLSYSTEM_SEQUENCER_H
