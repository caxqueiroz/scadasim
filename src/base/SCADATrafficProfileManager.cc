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

#include "SCADATrafficProfileManager.h"
#include <string>

#include <iostream>
#include <fstream>

using namespace std;

Define_Module(SCADATrafficProfileManager);

void SCADATrafficProfileManager::initialize(int stages)
{
    if (stages != 0)
            return;

        // open config-file and initiate the profiles
        string val = par("configFileName");
        string fileName(val);
        ifstream in(fileName.data());
        if (!in) {
            opp_error("Couldn't open config file %s", fileName.data());
            return;
        }
}

void SCADATrafficProfileManager::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}
