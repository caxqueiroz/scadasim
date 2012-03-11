//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __SCADASIM_SCADAUSER_H_
#define __SCADASIM_SCADAUSER_H_

#include <omnetpp.h>
#include "SCADASIMDefs.h"
#include "SCADATransmissionConfig.h"

#include <string>

/**
 * TODO - Generated class
 */
class SCADASIM_API SCADAUser: public cSimpleModule {

protected:
    /// Container for collection of communication statistics
    SCADAUserCommunicationStatistics communicationStatistics;

public:
    SCADAUser();
    virtual ~SCADAUser();
    /// @brief Collects some statistics of the previous transmission and starts a new one
    void transmissionDone(TransmissionStatistics t);

protected:

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    /// @brief Actually starts a new transmission
    void transmissionDone();
    /// @brief Updates the information displayed in the GUI
    void updateDisplay();

    virtual void finish();
};

#endif
