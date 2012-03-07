#ifndef TGMNETWORKCONFIGURATOR_H_
#define TGMNETWORKCONFIGURATOR_H_

#include <omnetpp.h>
#include <cctype>
#include <vector>
#include <map>
#include <ctopology.h>
#include <string>
#include <iostream>
#include "INETDefs.h"
#include "IPvXAddress.h"
#include "RoutingTable.h"
#include "InterfaceTable.h"
#include "IPvXAddressResolver.h"
//#include "NetworkConfigurator.h"
#include "IPv4InterfaceData.h"
#include "InterfaceEntry.h"
#include "SCADASIMDefs.h"

using std::vector;
using std::map;
using std::string;
using std::cerr;
using std::cout;
using std::endl;

const int INIT_STAGES = 3;
// unique hex values for different router- and AS-level node types
const int TRANSIT_AS = 1;
const int STUB_AS = 2;
const int UNSPECIFIED = -1;
const int CORE = 1;
const int GW = 2;
const int EDGE = 3;
const int ENDSYS = 4;

typedef vector<string> StringVector;

/**
 * @brief Structure that contains all information about a router-level node.
 *
 * During initialization all necessary information is extraced of the given node:
 * Does it belong to Stub or Transit AS, ID, router type.
 * In addition, default interfaces are determined for gateway, edge, and host
 * routers. Core routers do not have any default routes.
 */
struct nodeInfoRL {
	bool isIPNode;
	IInterfaceTable *ift;
	InterfaceEntry *defaultRouteIE;
	int asId, asType, routerType, moduleId;
	IRoutingTable *rt;
	IPvXAddress addr;
	bool usesDefaultRoute;
	cModule *module;
	cTopology::Node *node;

	nodeInfoRL() {
	}
	;
	nodeInfoRL(cTopology::Node *node) {
		this->node = node;
		module = node->getModule();
		moduleId = module->getId();
		ift = IPvXAddressResolver().findInterfaceTableOf(module);
		rt = IPvXAddressResolver().findRoutingTableOf(module);
		isIPNode = (rt != NULL);
		int index = 0;
		string fullPath = module->getFullPath();

		// check if stubstring "sas" (StubAS) or "tas" (TransitAS)
		// is contained in fullPath
		if ((index = fullPath.find("corporate")) != -1)
			asType = STUB_AS;
		else if ((index = fullPath.find("remote")) != -1)
			asType = STUB_AS;
		else if ((index = fullPath.find("field")) != -1)
			asType = STUB_AS;
		else if ((index = fullPath.find("SCADA")) != -1)
			asType = UNSPECIFIED;
		else {
			cerr
					<< "found module that doesn't belong to Transit AS (tas) or Stub AS (sas): "
					<< fullPath << endl;
			opp_error(
					"found module that doesn't belong to Transit AS (tas) or Stub AS (sas)");
		}

		// set index to char position after substring "sas/tas"
//		if (asType == STUB_AS || asType == TRANSIT_AS) {
//			index += 3;
//			string currentId;
//			while (isdigit(fullPath[index]) && (index < fullPath.length()))
//				currentId += fullPath[index++];
//			asId = atoi(currentId.data());
//		}
		asId = random();

		if (fullPath.find("core") != string::npos)
			routerType = CORE;
		else if (fullPath.find("gw") != string::npos)
			routerType = GW;
		else if (fullPath.find("edge") != string::npos)
			routerType = EDGE;
		else if (fullPath.find("host") != string::npos)
			routerType = ENDSYS;
		else if (fullPath.find("Server") != string::npos)
			routerType = ENDSYS;
		else {
			cerr << "found module without valid type: " << fullPath << endl;
			opp_error("found module without valid type");
		}
		//
		// determine default interface
		//
		if (routerType == CORE) {
			// find last interface that is not loopback
			for (int i = 0; i < ift->getNumInterfaces(); i++)
				if (!ift->getInterface(i)->isLoopback())
					addr = ift->getInterface(i)->ipv4Data()->getIPAddress();
			defaultRouteIE = NULL;
		} else {
			for (int i = 0; i < ift->getNumInterfaces(); i++) {
				if (!ift->getInterface(i)->isLoopback()) {
					// find first interface that is not loopback and is connected to
					// a higher level node. Then, create default route
					addr = ift->getInterface(i)->ipv4Data()->getIPAddress();
					if (routerType == GW) {
						if (module->gate(
								ift->getInterface(i)->getNodeOutputGateId())\
->getNextGate()->getOwnerModule()->getFullPath().find(
								"core") != string::npos) {
							defaultRouteIE = ift->getInterface(i);
							break;
						}
					} else if (routerType == EDGE) {
						if (module->gate(
								ift->getInterface(i)->getNodeOutputGateId())->getNextGate()->getOwnerModule()->getFullPath().find(
								"gw") != string::npos) {
							defaultRouteIE = ift->getInterface(i);
							break;
						}
					} else if (routerType == ENDSYS) {
						if (module->gate(
								ift->getInterface(i)->getNodeOutputGateId())->getNextGate()->getOwnerModule()->getFullPath().find(
								"edge") != string::npos) {
							defaultRouteIE = ift->getInterface(i);
							break;
						}
					}
				}
			}
		}
	}
	;

//	void setIPAddress(int addr)
//	{
//		if(rt->isUnspecified())
//			rt->setRouterId(IPAddress(addr));
//		for(int i=0; i<ift->getNumInterfaces(); i++)
//		{
//			InterfaceEntry *ie = ift->interFaceAt(i);
//			if(!ie->isLoopback())
//			{
//				ie->ipv4Data()->setIPAddress(IPAddress(addr));
//				ie->ipv4Data()->setNetmask(IPAddress::ALLONES_ADDRESS);
//			}
//		}
//	};
};

typedef std::vector<nodeInfoRL> NODE_INFO_RL_VEC;
typedef std::map<int, nodeInfoRL> NODE_MAP;
typedef std::pair<int, nodeInfoRL> NODE_MAP_PAIR;

/**
 * @brief Structure that contains all information about an AS-level node.
 *
 * During initialization all necessary information is extraced of the given node:
 * Router type and ID.
 */
struct nodeInfoAS {
	int id;
	int asType;
	cTopology::Node *node;
	cModule *module;
	NODE_MAP nodeMap;
	NODE_INFO_RL_VEC coreNode;
	IPvXAddress addr;
	IPvXAddress netmask;

	nodeInfoAS(cTopology::Node *node, IPvXAddress a, IPvXAddress m) {
		this->node = node;
		this->module = node->getModule();
		addr = a;
		netmask = m;
		int index = 0;
		string fullPath = node->getModule()->getFullPath();

		// check if stubstring "sas" (StubAS) or "tas" (TransitAS)
		// is contained in fullPath
		if ((index = fullPath.find("Internet")) != -1)
			asType = STUB_AS;
		else if ((index = fullPath.find("brc")) != -1)
			asType = STUB_AS;
		else if ((index = fullPath.find("brr")) != -1)
			asType = STUB_AS;
		else if ((index = fullPath.find("brf00")) != -1)
		            asType = STUB_AS;
		else if ((index = fullPath.find("brf01")) != -1)
		            asType = STUB_AS;
		else if ((index = fullPath.find("brf02")) != -1)
		            asType = STUB_AS;
		else if ((index = fullPath.find("CaseStudy")) != -1)
			asType = UNSPECIFIED;
		else {
			cerr << "found module that doesn't belong to TAS or SAS: "
					<< fullPath << endl;
			opp_error("found module that doesn't belong to TAS or SAS");
		}

		// set index to char position after substring "sas/tas"
		if (asType == STUB_AS || asType == TRANSIT_AS) {
			index += 3;
			string currentId;
			while (isdigit(fullPath[index]) && (index < fullPath.length()))
				currentId += fullPath[index++];
			id = atoi(currentId.data());
		}
	}
};

typedef std::vector<nodeInfoAS> NODE_INFO_AS_VEC;

/**
 * @brief Configures the nodes belonging to the topology before starting
 * the actual simulation.
 *
 * This class is responsible for assignment of IP addresses to all nodes
 * of the topology. Furthermore, routing tables have to be filled and
 * default routing paths must be created.
 * Routing is separated into Intra-AS and Inter-AS routing.
 *
 * @class TGMNetworkConfigurator
 */
class SCADASIM_API TGMNetworkConfigurator: public cSimpleModule {
protected:
	std::vector<cTopology*> rlTopology;
	cTopology asTopology;
	int noAS;
	NODE_INFO_AS_VEC asNodeVec;
public:
	TGMNetworkConfigurator();
	virtual ~TGMNetworkConfigurator();

protected:
	//
	// stage = 0 --> register interfaces
	//
	virtual int numInitStages() const {
		return 3;
	}
	virtual void initialize(int stage);
	virtual void handleMessage(cMessage *msg) {
		opp_error("message received");
	}
	;
	/// @brief Add Inter-AS routing paths between core nodes
	void createInterASPaths();
	/// @brief Disable all incoming links of Stub AS except to and from dst and src
	void disableStubLinks(nodeInfoRL &dst, nodeInfoRL &src);
	/// @brief Enable all incoming links of Stub AS
	void enableStubLinks();
	/// @brief Extract topology from NED file
	void extractTopology();
	//int getCoreNodes(cModule *curMod, void *nullPointer){return 0;};
//	int getRouterLevelNodes(cModule *curMod, void *name){return 0;};
	/// @brief Assign IP address and add default route
	void assignAddressAndSetDefaultRoutes(nodeInfoAS &asInfo);
	/// @brief Add explicit Intra-AS routing paths (except of default routes)
	void setIntraASRoutes(cTopology &topology, nodeInfoAS &asInfo);

};
namespace TGMNetConf {
static bool getCoreNodes(cModule *curMod, void *nullPointer);
static bool getRouterLevelNodes(cModule *curMod, void *name);
}
;
#endif /*TGMNETWORKCONFIGURATOR_H_*/
