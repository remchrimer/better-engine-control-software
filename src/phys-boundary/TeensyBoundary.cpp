//
// Created by kevin on 6/17/2022.
//

#include "TeensyBoundary.h"
#include <utility>

namespace{
    /*
     * Currently, we only have one data packet that contains all our sensordata,
     * so this function should hit all the sensor fields (so non-effector fields) in SensorData
     */
    void updateFromPropBoard(SensorData& data, PropBoardSensorData& propPacket){
        static_assert(SensorData::majorVersion == 3,
                      "Function not updated from SensorData change, please update this function and the static_assert");

        data.loxInletDucer = propPacket.adc10;
        data.kerTankDucer = propPacket.adc1;
        data.purgeDucer = propPacket.adc7;
        data.loxInletDucer = propPacket.adc11;
        data.kerInletDucer = propPacket.adc9;
        data.kerPintleDucer = propPacket.adc10;
        data.loxVenturi = propPacket.adc5;
        data.kerVenturi = propPacket.adc6;
        data.loadCell = propPacket.loadCellRaw;
        data.pneumaticDucer = propPacket.adc6;
        data.loxRegDucer = propPacket.adc2;
        data.kerRegDucer = propPacket.adc4;
        data.n2pressDucer = propPacket.adc12;

        data.loxTankTC = propPacket.tcTemp3;
        data.kerInletTC = propPacket.tcTemp1;
        data.kerOutletTC = propPacket.tcTemp2;
        data.miscTC = propPacket.tcTemp4;
    }
}


TeensyBoundary::TeensyBoundary(std::unique_ptr<IECSValve> loxPressurant_, std::unique_ptr<IECSValve> kerPressurant_,
                               std::unique_ptr<IECSValve> loxPurge_, std::unique_ptr<IECSValve> kerPurge_,
                               std::unique_ptr<IECSValve> loxVent_, std::unique_ptr<IECSValve> kerVent_,
                               std::unique_ptr<IECSValve> loxFlow_, std::unique_ptr<IECSValve> kerFlow_,
                               std::unique_ptr<IPacketSource<PropBoardSensorData>> pSource,
                               std::vector<SensorDataCalibrator> cList):
        loxPressurant(std::move(loxPressurant_)),
        kerPressurant(std::move(kerPressurant_)),
        loxPurge(std::move(loxPurge_)),
        kerPurge(std::move(kerPurge_)),
        loxVent(std::move(loxVent_)),
        kerVent(std::move(kerVent_)),
        loxFlow(std::move(loxFlow_)),
        kerFlow(std::move(kerFlow_)),
        packetSource(std::move(pSource)),
        calibratorList(std::move(cList))
{
    static_assert(CommandData::majorVersion == 2,
                  "Function not updated from CommandData change, please update this function and the static_assert");
}

SensorData TeensyBoundary::readFromBoundary() {
    SensorData data;

    PropBoardSensorData pData = this->packetSource->getPacket();
    updateFromPropBoard(data, pData);

    this->readFromEffectors(data);

    for(auto& calibrator: this->calibratorList){
        calibrator.applyCalibration(data);
    }

    return data;
}

/*
 * this function should hit all the fields in CommandData
 */
void TeensyBoundary::writeToBoundary(const CommandData &data) {
    static_assert(CommandData::majorVersion == 2,
                  "Function not updated from CommandData change, please update this function and the static_assert");

    this->loxVent->setValveState(data.loxVent);
    this->kerVent->setValveState(data.kerVent);

    this->loxPressurant->setValveState(data.loxPressurant);
    this->kerPressurant->setValveState(data.kerPressurant);

    this->loxFlow->setValveState(data.loxFlow);
    this->kerFlow->setValveState(data.kerFlow);

    this->loxPurge->setValveState(data.loxPurge);
    this->kerPurge->setValveState(data.kerPurge);
}

/*
 * This function should hit all the effector fields in SensorData
 */
void TeensyBoundary::readFromEffectors(SensorData& storedData) {
    static_assert(CommandData::majorVersion == 2,
                  "Function not updated from CommandData change, please update this function and the static_assert");
    storedData.loxVent = this->loxVent->getValveState();
    storedData.kerVent = this->kerVent->getValveState();
    storedData.loxPressurant = this->loxPressurant->getValveState();
    storedData.kerPressurant = this->kerPressurant->getValveState();
    storedData.loxFlow = this->loxFlow->getValveState();
    storedData.kerFlow = this->kerFlow->getValveState();
    storedData.loxPurge = this->loxPurge->getValveState();
    storedData.kerPurge = this->kerPurge->getValveState();
}




