#include "TGMNetworkConfigurator.h"
#include <sstream>

const int IP_NET_SHIFT = 16;
const unsigned int NET_MASK = 0xffff0000;
Define_Module( TGMNetworkConfigurator);

using namespace std;
using namespace TGMNetConf;

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

TGMNetworkConfigurator::TGMNetworkConfigurator()
{

}

TGMNetworkConfigurator::~TGMNetworkConfigurator()
{
}

/**
 * Main method of the network configurator.
 *
 * Topology is extracted from NED file, IP addresses are assigned,
 * and routing paths are established.
 */
void TGMNetworkConfigurator::initialize(int stage)
{
	if (stage != 2)
		return;
	noAS = par("totalCountOfAS");
	if (noAS >= 65535)
		opp_error("too many AS nodes in AS-level topology, max 2^16 -1");

	// extract topology nodes and assign IP addresses
	extractTopology();

	// assign an IPAddress to all nodes in the network
	//FIXME: does asNodeVec.size() work as intended? may need to use noAS instead
	//for(int i=0; i<noAS; i++)
	for (unsigned int i = 0; i < asNodeVec.size(); i++)
	{
		if (rlTopology[i]->getNumNodes() > (0xffffffff - NET_MASK))
			opp_error("to many nodes in current topology");
		std::cout << asNodeVec[i].module->getFullPath() << std::endl;
		//
		//  insert each router-level node into a node map
		//
		for (int j = 0; j < rlTopology[i]->getNumNodes(); j++)
		{
			nodeInfoRL curRLNode(rlTopology[i]->getNode(j));
			asNodeVec[i].nodeMap.insert(NODE_MAP_PAIR(curRLNode.moduleId, curRLNode));
		}

		// assign IP address and add default route
		assignAddressAndSetDefaultRoutes(asNodeVec[i]);
	}

	// add all further routes in router-level topology
	// (unequal to default route)
	for (unsigned int i = 0; i < asNodeVec.size(); i++)
		setIntraASRoutes(*rlTopology[i], asNodeVec[i]);

	// Having configured all router topologies, add Inter-AS routing paths
	if (noAS > 0)
		createInterASPaths();

	// free Memory
	for (int i = 0; i < noAS; i++)
	{
		rlTopology[i]->clear();
		delete rlTopology[i];
		asNodeVec[i].nodeMap.clear();
	}
	asTopology.clear();
	asNodeVec.clear();
}

/**
 * Calculate all Inter-AS. This is achieved by calculating all shortest paths between
 * all core routers of the whole topology.
 * Temporarily disable all stub links during calculation of shortest paths to ensure
 * that a stub AS is not crossed but may only be present at start or end of a routing
 * path.
 */
void TGMNetworkConfigurator::createInterASPaths()
{
	IPAddress netmask(NET_MASK);
	int asIdHistory = 0;
	unsigned int tmpAddr = 0;
	for (int i = 0; i < asTopology.getNumNodes(); i++)
	{
		// calculate prefix of current core node
		nodeInfoRL destCore(asTopology.getNode(i));
		tmpAddr = destCore.addr.getInt() >> IP_NET_SHIFT;
		tmpAddr = tmpAddr << IP_NET_SHIFT;
		destCore.addr = IPAddress(tmpAddr);
		asIdHistory = -1;
		for (int j = 0; j < asTopology.getNumNodes(); j++)
		{
			if (i == j)
				continue;
			nodeInfoRL srcCore(asTopology.getNode(j));
			tmpAddr = srcCore.addr.getInt() >> IP_NET_SHIFT;
			tmpAddr = tmpAddr << IP_NET_SHIFT;
			srcCore.addr = IPAddress(tmpAddr);

			// do not calculate paths between nodes of the same AS
			if (destCore.asId == srcCore.asId)
				continue;

			// cross only transit AS in order to reach destination core node
			// therefore, temporarily disable all stub links
			if (asIdHistory != srcCore.asId)
			{
				disableStubLinks(destCore, srcCore);
				asTopology.calculateUnweightedSingleShortestPathsTo(destCore.node);
			}
			// add routing entry from srcCore to destCore into routing table of srcCore
			InterfaceEntry *ie = srcCore.ift->getInterfaceByNodeOutputGateId(srcCore.node->getPath(0)->getLocalGate()->getId());
			IPRoute *e = new IPRoute();
			e->setHost(destCore.addr);
			e->setNetmask(netmask);
			e->setInterface(ie);
			e->setType(IPRoute::DIRECT);
			e->setSource(IPRoute::MANUAL);
			srcCore.rt->addRoute(e);

			// re-enable all stub links
			if (asIdHistory != srcCore.asId)
			{
				enableStubLinks();
			}
			asIdHistory = srcCore.asId;
		}
	}
}

/**
 * Disable all incoming links to core nodes of each stub AS that do not
 * match on given dst or src router-level node ID.
 */
void TGMNetworkConfigurator::disableStubLinks(nodeInfoRL &dst, nodeInfoRL &src)
{
	for (unsigned int i = 0; i < asNodeVec.size(); i++)
	{
		if ((asNodeVec[i].id == dst.asId) || (asNodeVec[i].id == src.asId))
			continue;
		if (asNodeVec[i].asType == TRANSIT_AS)
			continue;

		for (unsigned int j = 0; j < asNodeVec[i].coreNode.size(); j++)
		{
			for (int k = 0; k < asNodeVec[i].coreNode[j].node->getNumInLinks(); k++)
				asNodeVec[i].coreNode[j].node->getLinkIn(k)->disable();
		}
	}
}

/**
 * Enable all incoming links to core nodes of each stub AS
 */
void TGMNetworkConfigurator::enableStubLinks()
{
	for (unsigned int i = 0; i < asNodeVec.size(); i++)
	{
		if (asNodeVec[i].asType == TRANSIT_AS)
			continue;
		for (unsigned int j = 0; j < asNodeVec[i].coreNode.size(); j++)
		{
			for (int k = 0; k < asNodeVec[i].coreNode[j].node->getNumInLinks(); k++)
				asNodeVec[i].coreNode[j].node->getLinkIn(k)->enable();
		}
	}
}

/**
 * Extracts AS-level topology and each router-level topology
 * into asTopology and rlTopology.
 * Additionally, each AS gets assigned a unique /16 prefix
 */
void TGMNetworkConfigurator::extractTopology()
{
	cTopology currentAS;

	// get the AS-level topology
	if (noAS > 0)
	{
		currentAS.extractByProperty("AS"); //TODO: check if this is acceptable
		if (currentAS.getNumNodes() != noAS)
			opp_error("Error: AS-Topology contains %u elements - expected %u\n", currentAS.getNumNodes(), noAS);
	}
	else if (noAS == 0)
	{
		// Network is router-level only
		currentAS.extractByProperty("Internet"); //TODO: check if this is acceptable
		if (currentAS.getNumNodes() != 1)
			opp_error("Error: tried to extract router-level only topology, but found more than 1 Inet module\n");
	}

	// get each router-level topology
	unsigned int netIP = 1 << IP_NET_SHIFT;
	for (int i = 0; i < currentAS.getNumNodes(); i++)
	{
		cTopology *tmpTopo = new cTopology();
		// extract router-level nodes from NED file
		tmpTopo->extractFromNetwork(getRouterLevelNodes, (void *) currentAS.getNode(i)->getModule()->getName());
		rlTopology.push_back(tmpTopo);
		// assign unique /16 IP address prefix to each AS
		asNodeVec.push_back(nodeInfoAS(currentAS.getNode(i), IPAddress(netIP), IPAddress(NET_MASK)));
		netIP += 1 << IP_NET_SHIFT;
	}

	asTopology.extractFromNetwork(getCoreNodes); //TODO: the extra function may be superfuous. extraction could be probably be done via asTopology.extractByProperty("CoreRouter"); -Claus
}

/**
 * Callback method that is used by extractFromNetwork. This method
 * includes all nodes for which the callback method returns a non-zero
 * value. The second argument is given to the callback method as second
 * argument.
 * Our callback method returns a topology consisting of all router-level nodes
 * (core, gateway, edge, host, and servers) that belong to the given AS.
 * It does so by searching for the RL property within the given AS.
 *
 * @return Returns 1 for nodes that are included into the topology,
 *         0 for nodes that are ignored
 */
bool TGMNetConf::getRouterLevelNodes(cModule *curMod, void *name)
{
	char *curName = (char*) name;
	if (curName == NULL)
		opp_error("Error while casting void* name to char*\n");

	string sCurName = curName;
	sCurName += ".";
	string curModPath = curMod->getFullPath();
	if (curModPath.find(sCurName) == string::npos)
		return 0;
	//TODO: took some code from ctopology.cc to implement this, check if functionality is correct -Claus
	const char* property = "RL";
	cProperty *prop = curMod->getProperties()->get(property);
	if (!prop)
		return 0;
	const char *value = prop->getValue(cProperty::DEFAULTKEY, 0);
	return opp_strcmp(value, "false")!=0;
}

/**
 * Callback method that is used by extractFromNetwork. This method
 * includes all nodes for which the callback method returns a non-zero
 * value. The second argument is given to the callback method as second
 * argument.
 * Our callback method returns a topology consisting of all core nodes.
 * It does so by searching for the CoreRouter property.
 *
 * @return Returns 1 for nodes that are included into the topology,
 *         0 for nodes that are ignored
 */
bool TGMNetConf::getCoreNodes(cModule *curMod, void *nullPointer)
{
	//TODO: took some code from ctopology.cc to implement this, check if functionality is correct -Claus
	const char* property = "CoreRouter";
	cProperty *prop = curMod->getProperties()->get(property);
	if (!prop)
		return 0;
	const char *value = prop->getValue(cProperty::DEFAULTKEY, 0);
	return opp_strcmp(value, "false")!=0;
}

/**
 * @brief Assigns an IP address of the /16 prefix to each of the router-level nodes.
 *
 * Additionally, default routes are added for gateway, edge, and host nodes.
 * Core nodes are stored into an additional list for later processing.
 *
 * @param asInfo AS for which IP addresses should be assigned to router-level nodes.
 */
void TGMNetworkConfigurator::assignAddressAndSetDefaultRoutes(nodeInfoAS &asInfo)
{
	unsigned int currentIP = asInfo.addr.getInt() + 1;

	NODE_MAP::iterator mapIt = asInfo.nodeMap.begin();
	while (mapIt != asInfo.nodeMap.end())
	{
		for (int j = 0; j < mapIt->second.ift->getNumInterfaces(); j++)
		{
			//
			// all interfaces except loopback get the same IP address
			//
			InterfaceEntry *ie = mapIt->second.ift->getInterface(j);
			if (!ie->isLoopback())
			{
				ie->ipv4Data()->setIPAddress(IPAddress(currentIP));
				ie->ipv4Data()->setNetmask(IPAddress::ALLONES_ADDRESS);
			}
		}
		if (mapIt->second.rt->getRouterId().isUnspecified())
			mapIt->second.rt->setRouterId(IPAddress(currentIP));
		mapIt->second.addr.set(currentIP);

		// remember core nodes of each AS in additional list for assignment
		// of Inter-AS routing paths
		if (mapIt->second.routerType == CORE)
			asInfo.coreNode.push_back(mapIt->second);
		else
		{
			//
			// add default route in case of gw, edge, or host
			//
			IPRoute *e = new IPRoute();
			e->setHost(IPAddress());
			e->setNetmask(IPAddress());
			e->setInterface(mapIt->second.defaultRouteIE);
			e->setType(IPRoute::REMOTE);
			e->setSource(IPRoute::MANUAL);
			//e->setMetric(1);
			mapIt->second.rt->addRoute(e);
		}

		currentIP++;
		mapIt++;
	}
}

/**
 * Calculate all Intra-AS routes that are unequal to the default routes.
 * Therefore, all shortest paths between all router-level nodes are calculated.
 * If the first hop is unequal to the default route, a new specific route is added.
 *
 * @param topology
 * @param asInfo AS for which Intra-AS routes should be determined
 */
void TGMNetworkConfigurator::setIntraASRoutes(cTopology &topology, nodeInfoAS &asInfo)
{
	// calculate static routes from each of the AS's router-level nodes to all
	// other nodes of the AS
	for (int i = 0; i < topology.getNumNodes(); i++)
	{
		nodeInfoRL destNode = asInfo.nodeMap[topology.getNode(i)->getModule()->getId()];
		//
		// calculate shortest path form everywhere toward destNode
		//
		topology.calculateUnweightedSingleShortestPathsTo(destNode.node);
		for (int j = 0; j < topology.getNumNodes(); j++)
		{
			if (j == i)
				continue;
			nodeInfoRL srcNode = asInfo.nodeMap[topology.getNode(j)->getModule()->getId()];
			// no route exists at all
			if (srcNode.node->getNumPaths() == 0)
				continue;
			// end systems only know a default route to the edge router
			else if (srcNode.routerType == ENDSYS)
				continue;
			else
			{
				//
				// if destination is reachable through default route, no routing entry is necessary
				//
				InterfaceEntry *ie = srcNode.ift->getInterfaceByNodeOutputGateId(srcNode.node->getPath(0)->getLocalGate()->getId());
				if (ie == srcNode.defaultRouteIE)
					continue;
				else
				{
					// add specific routing entry into routing table
					IPRoute *e = new IPRoute();
					e->setHost(destNode.addr);
					e->setNetmask(IPAddress(255, 255, 255, 255));
					e->setInterface(ie);
					e->setType(IPRoute::DIRECT);
					e->setSource(IPRoute::MANUAL);
					srcNode.rt->addRoute(e);
				}
			}
		}
	}
}

