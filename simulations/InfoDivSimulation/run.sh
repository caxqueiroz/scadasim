#!/bin/sh
cd /Users/cq/Dev/SCADASimWorkspace/scadasim/simulations/InfoDivSimulation
../../src/scadasim -r 0 -u Cmdenv -n ..:../../src:../../../inet/examples:../../../inet/src -l ../../../inet/src/inet omnetpp.ini
# for shared lib, use: opp_run -l ../src/scadasim -n .:../src $*
