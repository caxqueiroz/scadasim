#
# OMNeT++/OMNEST Makefile for scadasim
#
# This file was generated with the command:
#  opp_makemake -f --deep -I../../inet/src/networklayer/rsvp_te -I../../inet/src/networklayer/common -I../../inet/src/networklayer/icmpv6 -I../../inet/src/transport/tcp -I../../inet/src/networklayer/mpls -I../../inet/src/networklayer/ted -I../../inet/src/networklayer/contract -I../../inet/src/util -I../../inet/src/transport/contract -I../../inet/src/linklayer/mfcore -I../../inet/src/networklayer/ldp -I../../inet/src/networklayer/ipv4 -I../../inet/src/base -I../../inet/src/util/headerserializers -I../../inet/src/networklayer/ipv6 -I../../inet/src/transport/sctp -I../../inet/src/world -I../../inet/src/applications/pingapp -I../../inet/src/linklayer/contract -I../../inet/src/networklayer/arp -I../../inet/src/transport/udp -L../../inet/out/$(CONFIGNAME)/src -linet -KINET_PROJ=../../inet
#

# Name of target to be created (-o option)
TARGET = scadasim$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../../inet/src/networklayer/rsvp_te \
    -I../../inet/src/networklayer/common \
    -I../../inet/src/networklayer/icmpv6 \
    -I../../inet/src/transport/tcp \
    -I../../inet/src/networklayer/mpls \
    -I../../inet/src/networklayer/ted \
    -I../../inet/src/networklayer/contract \
    -I../../inet/src/util \
    -I../../inet/src/transport/contract \
    -I../../inet/src/linklayer/mfcore \
    -I../../inet/src/networklayer/ldp \
    -I../../inet/src/networklayer/ipv4 \
    -I../../inet/src/base \
    -I../../inet/src/util/headerserializers \
    -I../../inet/src/networklayer/ipv6 \
    -I../../inet/src/transport/sctp \
    -I../../inet/src/world \
    -I../../inet/src/applications/pingapp \
    -I../../inet/src/linklayer/contract \
    -I../../inet/src/networklayer/arp \
    -I../../inet/src/transport/udp \
    -I. \
    -Iapplications \
    -Iapplications/ddos \
    -Iapplications/generic \
    -Iapplications/modbusApp \
    -Iapplications/pingApp \
    -Iapplications/scada \
    -Iapplications/tcpApp \
    -Iapplications/worm \
    -Ibase \
    -Iexternal \
    -Inetworklayer \
    -Inetworklayer/autorouting \
    -Inetworklayer/contract \
    -Inetworklayer/ipv4 \
    -Inetworklayer/queue \
    -Inodes \
    -Inodes/inet \
    -Inodes/scada \
    -Itransport \
    -Itransport/contract \
    -Itransport/tcp \
    -Itransport/udp

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../inet/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath ../../inet/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/applications/ddos/TribeFloodNetwork.o \
    $O/applications/generic/InetUser.o \
    $O/applications/generic/GenericApplication.o \
    $O/applications/modbusApp/ModbusApplicationSlaveThread.o \
    $O/applications/modbusApp/ModbusUser.o \
    $O/applications/modbusApp/ModbusTCP.o \
    $O/applications/modbusApp/ModbusTCPApplication.o \
    $O/applications/modbusApp/ModbusApplicationMasterThread.o \
    $O/applications/pingApp/GenericICMPPingApplication.o \
    $O/applications/scada/SCADATrafficProfileManager.o \
    $O/applications/scada/SCADAUser.o \
    $O/applications/tcpApp/GenericTCPApplicationClientThread.o \
    $O/applications/tcpApp/GenericTCPApplication.o \
    $O/applications/tcpApp/GenericTCPApplicationServerThread.o \
    $O/applications/worm/UDPWormVictim.o \
    $O/base/TrafficProfileManager.o \
    $O/base/ConnectionManager.o \
    $O/external/SSScheduler.o \
    $O/external/ModbusGate.o \
    $O/external/SSThread.o \
    $O/external/SSGate.o \
    $O/external/modbus.o \
    $O/external/SSProxy.o \
    $O/networklayer/autorouting/TGMNetworkConfigurator.o \
    $O/networklayer/ipv4/IP_hack.o \
    $O/networklayer/queue/DropTailTraceQueue.o \
    $O/transport/contract/UDPSocketVector.o \
    $O/transport/tcp/TCP_hack.o \
    $O/transport/tcp/TCPConnection_hack.o \
    $O/transport/udp/UDP_hack.o \
    $O/applications/generic/GenericApplicationMessage_m.o \
    $O/applications/modbusApp/ModbusMessage_m.o \
    $O/applications/scada/scadamessage_m.o \
    $O/applications/worm/UDPWormQueryMessage_m.o \
    $O/networklayer/contract/IPControlInfo_hacked_m.o \
    $O/networklayer/contract/IPv6ControlInfo_hacked_m.o \
    $O/networklayer/ipv4/IPDatagram_hacked_m.o

# Message files
MSGFILES = \
    applications/generic/GenericApplicationMessage.msg \
    applications/modbusApp/ModbusMessage.msg \
    applications/scada/scadamessage.msg \
    applications/worm/UDPWormQueryMessage.msg \
    networklayer/contract/IPControlInfo_hacked.msg \
    networklayer/contract/IPv6ControlInfo_hacked.msg \
    networklayer/ipv4/IPDatagram_hacked.msg

# Other makefile variables (-K)
INET_PROJ=../../inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY:

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f scadasim scadasim.exe libscadasim.so libscadasim.a libscadasim.dll libscadasim.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f applications/*_m.cc applications/*_m.h
	-rm -f applications/ddos/*_m.cc applications/ddos/*_m.h
	-rm -f applications/generic/*_m.cc applications/generic/*_m.h
	-rm -f applications/modbusApp/*_m.cc applications/modbusApp/*_m.h
	-rm -f applications/pingApp/*_m.cc applications/pingApp/*_m.h
	-rm -f applications/scada/*_m.cc applications/scada/*_m.h
	-rm -f applications/tcpApp/*_m.cc applications/tcpApp/*_m.h
	-rm -f applications/worm/*_m.cc applications/worm/*_m.h
	-rm -f base/*_m.cc base/*_m.h
	-rm -f external/*_m.cc external/*_m.h
	-rm -f networklayer/*_m.cc networklayer/*_m.h
	-rm -f networklayer/autorouting/*_m.cc networklayer/autorouting/*_m.h
	-rm -f networklayer/contract/*_m.cc networklayer/contract/*_m.h
	-rm -f networklayer/ipv4/*_m.cc networklayer/ipv4/*_m.h
	-rm -f networklayer/queue/*_m.cc networklayer/queue/*_m.h
	-rm -f nodes/*_m.cc nodes/*_m.h
	-rm -f nodes/inet/*_m.cc nodes/inet/*_m.h
	-rm -f nodes/scada/*_m.cc nodes/scada/*_m.h
	-rm -f transport/*_m.cc transport/*_m.h
	-rm -f transport/contract/*_m.cc transport/contract/*_m.h
	-rm -f transport/tcp/*_m.cc transport/tcp/*_m.h
	-rm -f transport/udp/*_m.cc transport/udp/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc applications/*.cc applications/ddos/*.cc applications/generic/*.cc applications/modbusApp/*.cc applications/pingApp/*.cc applications/scada/*.cc applications/tcpApp/*.cc applications/worm/*.cc base/*.cc external/*.cc networklayer/*.cc networklayer/autorouting/*.cc networklayer/contract/*.cc networklayer/ipv4/*.cc networklayer/queue/*.cc nodes/*.cc nodes/inet/*.cc nodes/scada/*.cc transport/*.cc transport/contract/*.cc transport/tcp/*.cc transport/udp/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/applications/ddos/TribeFloodNetwork.o: applications/ddos/TribeFloodNetwork.cc \
	$(INET_PROJ)/src/applications/pingapp/PingPayload_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/arp/ARP.h \
	$(INET_PROJ)/src/networklayer/arp/ARPPacket_m.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6ControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6ControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMP.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_PROJ)/src/transport/tcp/TCPSegment.h \
	$(INET_PROJ)/src/transport/tcp/TCPSegment_m.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket_m.h \
	applications/ddos/TribeFloodNetwork.h \
	base/SCADASIMDefs.h \
	networklayer/contract/IPControlInfo_hacked_m.h \
	networklayer/contract/IPv6ControlInfo_hacked_m.h
$O/applications/generic/GenericApplication.o: applications/generic/GenericApplication.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/generic/GenericApplication.h \
	applications/generic/InetUser.h \
	applications/generic/TransmissionConfig.h \
	base/ConnectionManager.h \
	base/SCADASIMDefs.h \
	base/TrafficProfileManager.h
$O/applications/generic/GenericApplicationMessage_m.o: applications/generic/GenericApplicationMessage_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	applications/generic/GenericApplicationMessage_m.h \
	base/SCADASIMDefs.h
$O/applications/generic/InetUser.o: applications/generic/InetUser.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	applications/generic/GenericApplication.h \
	applications/generic/InetUser.h \
	applications/generic/TransmissionConfig.h \
	applications/pingApp/GenericICMPPingApplication.h \
	applications/tcpApp/GenericTCPApplication.h \
	base/ConnectionManager.h \
	base/SCADASIMDefs.h \
	base/TrafficProfileManager.h
$O/applications/modbusApp/ModbusApplicationMasterThread.o: applications/modbusApp/ModbusApplicationMasterThread.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	applications/generic/GenericApplication.h \
	applications/generic/InetUser.h \
	applications/generic/TransmissionConfig.h \
	applications/modbusApp/ModbusApplicationMasterThread.h \
	applications/modbusApp/ModbusMessage_m.h \
	applications/modbusApp/ModbusTCP.h \
	applications/modbusApp/ModbusTCPApplication.h \
	applications/modbusApp/ModbusUser.h \
	applications/tcpApp/GenericTCPApplication.h \
	applications/tcpApp/GenericTCPApplicationClientThread.h \
	base/ConnectionManager.h \
	base/SCADASIMDefs.h \
	base/TrafficProfileManager.h \
	external/modbus.h
$O/applications/modbusApp/ModbusApplicationSlaveThread.o: applications/modbusApp/ModbusApplicationSlaveThread.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	applications/generic/GenericApplication.h \
	applications/generic/InetUser.h \
	applications/generic/TransmissionConfig.h \
	applications/modbusApp/ModbusApplicationSlaveThread.h \
	applications/modbusApp/ModbusMessage_m.h \
	applications/modbusApp/ModbusTCP.h \
	applications/modbusApp/ModbusTCPApplication.h \
	applications/modbusApp/ModbusUser.h \
	applications/tcpApp/GenericTCPApplication.h \
	applications/tcpApp/GenericTCPApplicationServerThread.h \
	base/ConnectionManager.h \
	base/SCADASIMDefs.h \
	base/TrafficProfileManager.h \
	external/modbus.h
$O/applications/modbusApp/ModbusMessage_m.o: applications/modbusApp/ModbusMessage_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	applications/modbusApp/ModbusMessage_m.h
$O/applications/modbusApp/ModbusTCP.o: applications/modbusApp/ModbusTCP.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	applications/modbusApp/ModbusTCP.h \
	external/modbus.h
$O/applications/modbusApp/ModbusTCPApplication.o: applications/modbusApp/ModbusTCPApplication.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	applications/generic/GenericApplication.h \
	applications/generic/InetUser.h \
	applications/generic/TransmissionConfig.h \
	applications/modbusApp/ModbusApplicationMasterThread.h \
	applications/modbusApp/ModbusApplicationSlaveThread.h \
	applications/modbusApp/ModbusTCP.h \
	applications/modbusApp/ModbusTCPApplication.h \
	applications/modbusApp/ModbusUser.h \
	applications/tcpApp/GenericTCPApplication.h \
	applications/tcpApp/GenericTCPApplicationClientThread.h \
	applications/tcpApp/GenericTCPApplicationServerThread.h \
	base/ConnectionManager.h \
	base/SCADASIMDefs.h \
	base/TrafficProfileManager.h \
	external/modbus.h
$O/applications/modbusApp/ModbusUser.o: applications/modbusApp/ModbusUser.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/generic/GenericApplication.h \
	applications/generic/InetUser.h \
	applications/generic/TransmissionConfig.h \
	applications/modbusApp/ModbusUser.h \
	base/ConnectionManager.h \
	base/SCADASIMDefs.h \
	base/TrafficProfileManager.h
$O/applications/pingApp/GenericICMPPingApplication.o: applications/pingApp/GenericICMPPingApplication.cc \
	$(INET_PROJ)/src/applications/pingapp/PingPayload_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6ControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6ControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/generic/GenericApplication.h \
	applications/generic/InetUser.h \
	applications/generic/TransmissionConfig.h \
	applications/pingApp/GenericICMPPingApplication.h \
	base/ConnectionManager.h \
	base/SCADASIMDefs.h \
	base/TrafficProfileManager.h
$O/applications/scada/SCADATrafficProfileManager.o: applications/scada/SCADATrafficProfileManager.cc \
	applications/scada/SCADATrafficProfileManager.h
$O/applications/scada/SCADAUser.o: applications/scada/SCADAUser.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/scada/SCADATransmissionConfig.h \
	applications/scada/SCADAUser.h \
	base/SCADASIMDefs.h
$O/applications/scada/scadamessage_m.o: applications/scada/scadamessage_m.cc \
	applications/scada/scadamessage_m.h
$O/applications/tcpApp/GenericTCPApplication.o: applications/tcpApp/GenericTCPApplication.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	applications/generic/GenericApplication.h \
	applications/generic/InetUser.h \
	applications/generic/TransmissionConfig.h \
	applications/tcpApp/GenericTCPApplication.h \
	applications/tcpApp/GenericTCPApplicationClientThread.h \
	applications/tcpApp/GenericTCPApplicationServerThread.h \
	base/ConnectionManager.h \
	base/SCADASIMDefs.h \
	base/TrafficProfileManager.h
$O/applications/tcpApp/GenericTCPApplicationClientThread.o: applications/tcpApp/GenericTCPApplicationClientThread.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	applications/generic/GenericApplication.h \
	applications/generic/GenericApplicationMessage_m.h \
	applications/generic/InetUser.h \
	applications/generic/TransmissionConfig.h \
	applications/tcpApp/GenericTCPApplication.h \
	applications/tcpApp/GenericTCPApplicationClientThread.h \
	base/ConnectionManager.h \
	base/SCADASIMDefs.h \
	base/TrafficProfileManager.h
$O/applications/tcpApp/GenericTCPApplicationServerThread.o: applications/tcpApp/GenericTCPApplicationServerThread.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	applications/generic/GenericApplication.h \
	applications/generic/GenericApplicationMessage_m.h \
	applications/generic/InetUser.h \
	applications/generic/TransmissionConfig.h \
	applications/tcpApp/GenericTCPApplication.h \
	applications/tcpApp/GenericTCPApplicationServerThread.h \
	base/ConnectionManager.h \
	base/SCADASIMDefs.h \
	base/TrafficProfileManager.h
$O/applications/worm/UDPWormQueryMessage_m.o: applications/worm/UDPWormQueryMessage_m.cc \
	applications/worm/UDPWormQueryMessage_m.h \
	base/SCADASIMDefs.h
$O/applications/worm/UDPWormVictim.o: applications/worm/UDPWormVictim.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6ControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6ControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_PROJ)/src/transport/contract/UDPSocket.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket_m.h \
	applications/worm/UDPWormQueryMessage_m.h \
	applications/worm/UDPWormVictim.h \
	base/SCADASIMDefs.h \
	networklayer/contract/IPControlInfo_hacked_m.h \
	networklayer/contract/IPv6ControlInfo_hacked_m.h
$O/base/ConnectionManager.o: base/ConnectionManager.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/generic/TransmissionConfig.h \
	base/ConnectionManager.h \
	base/SCADASIMDefs.h \
	base/TrafficProfileManager.h
$O/base/TrafficProfileManager.o: base/TrafficProfileManager.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/generic/TransmissionConfig.h \
	base/SCADASIMDefs.h \
	base/TrafficProfileManager.h
$O/external/ModbusGate.o: external/ModbusGate.cc \
	external/ModbusGate.h \
	external/SSGate.h \
	external/SSProxy.h \
	external/SSThread.h \
	external/modbus.h
$O/external/SSGate.o: external/SSGate.cc \
	external/ModbusGate.h \
	external/SSGate.h \
	external/SSProxy.h \
	external/SSScheduler.h \
	external/SSThread.h \
	external/modbus.h
$O/external/SSProxy.o: external/SSProxy.cc \
	external/ModbusGate.h \
	external/SSGate.h \
	external/SSProxy.h \
	external/SSScheduler.h \
	external/SSThread.h \
	external/modbus.h
$O/external/SSScheduler.o: external/SSScheduler.cc \
	external/ModbusGate.h \
	external/SSGate.h \
	external/SSProxy.h \
	external/SSScheduler.h \
	external/SSThread.h \
	external/modbus.h
$O/external/SSThread.o: external/SSThread.cc \
	external/SSThread.h
$O/external/modbus.o: external/modbus.cc \
	external/modbus.h
$O/networklayer/autorouting/TGMNetworkConfigurator.o: networklayer/autorouting/TGMNetworkConfigurator.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTable.h \
	base/SCADASIMDefs.h \
	networklayer/autorouting/TGMNetworkConfigurator.h
$O/networklayer/contract/IPControlInfo_hacked_m.o: networklayer/contract/IPControlInfo_hacked_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	base/SCADASIMDefs.h \
	networklayer/contract/IPControlInfo_hacked_m.h
$O/networklayer/contract/IPv6ControlInfo_hacked_m.o: networklayer/contract/IPv6ControlInfo_hacked_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6ControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6ControlInfo_m.h \
	base/SCADASIMDefs.h \
	networklayer/contract/IPv6ControlInfo_hacked_m.h
$O/networklayer/ipv4/IPDatagram_hacked_m.o: networklayer/ipv4/IPDatagram_hacked_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram_m.h \
	base/SCADASIMDefs.h \
	networklayer/ipv4/IPDatagram_hacked_m.h
$O/networklayer/ipv4/IP_hack.o: networklayer/ipv4/IP_hack.cc \
	$(INET_PROJ)/src/base/AbstractQueue.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/base/ProtocolMap.h \
	$(INET_PROJ)/src/base/QueueBase.h \
	$(INET_PROJ)/src/base/ReassemblyBuffer.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/arp/ARPPacket_m.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMP.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPAccess.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IP.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPFragBuf.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket_m.h \
	base/SCADASIMDefs.h \
	networklayer/contract/IPControlInfo_hacked_m.h \
	networklayer/ipv4/IPDatagram_hacked_m.h \
	networklayer/ipv4/IP_hack.h
$O/networklayer/queue/DropTailTraceQueue.o: networklayer/queue/DropTailTraceQueue.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/IPassiveQueue.h \
	$(INET_PROJ)/src/base/PassiveQueueBase.h \
	networklayer/queue/DropTailTraceQueue.h
$O/transport/contract/UDPSocketVector.o: transport/contract/UDPSocketVector.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_PROJ)/src/transport/contract/UDPSocket.h \
	applications/generic/TransmissionConfig.h \
	base/SCADASIMDefs.h \
	transport/contract/UDPSocketVector.h
$O/transport/tcp/TCPConnection_hack.o: transport/tcp/TCPConnection_hack.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/tcp/TCP.h \
	$(INET_PROJ)/src/transport/tcp/TCPAlgorithm.h \
	$(INET_PROJ)/src/transport/tcp/TCPConnection.h \
	$(INET_PROJ)/src/transport/tcp/TCPReceiveQueue.h \
	$(INET_PROJ)/src/transport/tcp/TCPSACKRexmitQueue.h \
	$(INET_PROJ)/src/transport/tcp/TCPSegment.h \
	$(INET_PROJ)/src/transport/tcp/TCPSegment_m.h \
	$(INET_PROJ)/src/transport/tcp/TCPSendQueue.h \
	base/SCADASIMDefs.h \
	transport/tcp/TCPConnection_hack.h \
	transport/tcp/TCP_hack.h
$O/transport/tcp/TCP_hack.o: transport/tcp/TCP_hack.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/tcp/TCP.h \
	$(INET_PROJ)/src/transport/tcp/TCPConnection.h \
	$(INET_PROJ)/src/transport/tcp/TCPSegment.h \
	$(INET_PROJ)/src/transport/tcp/TCPSegment_m.h \
	base/SCADASIMDefs.h \
	transport/tcp/TCPConnection_hack.h \
	transport/tcp/TCP_hack.h
$O/transport/udp/UDP_hack.o: transport/udp/UDP_hack.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_PROJ)/src/transport/udp/UDP.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket_m.h \
	applications/generic/GenericApplicationMessage_m.h \
	base/SCADASIMDefs.h \
	transport/udp/UDP_hack.h

