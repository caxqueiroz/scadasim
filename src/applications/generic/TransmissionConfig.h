#ifndef TRANSMISSIONCONFIG_H_
#define TRANSMISSIONCONFIG_H_

#include <omnetpp.h>
#include "IPvXAddress.h"
#include <cmath>

using std::pow;

/// Available application types
/// @{
#define NOTSET 0
#define UDP_APP 1
#define ICMP_APP 2
#define TCP_APP 3
/// @}

/// Available traffic profiles
/// @{
#define TCP_BACKUP 1
#define TCP_INTERACTIVE 2
#define TCP_WEB 3
#define TCP_MAIL 4
#define TCP_MODBUS 5

#define UDP_NAMESERVER 11
#define UDP_STREAMING 12
#define UDP_MISC 13

#define ICMP_PING 21
/// @}

// ConstValues for the TrafficProfile
#define MAX_PACKET_SIZE 60000

/// generate a random number based on the pareto distribution (3 parameters)
inline double pareto(double shape, double location, double scale, int rgn)
{
	return ( location + (scale * ( pow( ( 1.-uniform(0,1,rgn) ), -shape) -1. ) )/shape);
}

/// shape parameter for packet size
const double SHAPE_SIZE = 3.0;
/// shape parameter for inter-packet time
const double SHAPE_TIME = 1.0;

/**
 * Structure for storing all information about the currently used
 * traffic profile. This structure also provides methods for 
 * filling the fields based on some distribution with random values.
 */
struct TrafficProfile
{
	int requestLength, requestsPerSession, replyLength, replyPerRequest; 
	double timeBetweenRequests, timeToRespond, timeBetweenSessions;
	double probability, WANprob;
	int profileID;
	std::string label;

	// specific values for UDP/ TCP
	int ownPort, hopLimit;
	
	TrafficProfile()
	{
		requestLength = requestsPerSession = replyLength = replyPerRequest = 1;
		timeBetweenRequests = timeToRespond = timeBetweenSessions = 1.0;
		probability = WANprob = 0.0;
		profileID = -1;
		label = "unspec";
		ownPort = 0;
		hopLimit = 255;
	}

	/**
	 * Instanciate traffic profile with given values
	 */
	TrafficProfile(TrafficProfile &o)
	{
		this->requestLength = o.requestLength;
		this->requestsPerSession = o.requestsPerSession;
		this->replyLength = o.replyLength;
		this->replyPerRequest = o.replyPerRequest;
		this->timeBetweenRequests = o.timeBetweenRequests;
		this->timeBetweenSessions = o.timeBetweenSessions;
		this->timeToRespond = o.timeToRespond;
		this->probability = o.probability;
		this->WANprob = o.WANprob;
		this->profileID = o.profileID;
		this->label = o.label;
		this->ownPort = o.ownPort;
		this->hopLimit = o.hopLimit;
	} 

	/**
	 * @brief Generate random values for a given traffic profile.
	 *
	 * Most values of the profile are base values for some distributions.
	 * This method generates the random values and saves them in the
	 * according parameters.
	 *
	 * @param p Given traffic profile that should be filled with random values
	 */
	void getNoisyProfile(TrafficProfile *p)
	{
		unsigned int tmp;
		tmp = (unsigned int)pareto( SHAPE_SIZE, requestLength, 1, 2);
		p->requestLength = (tmp < MAX_PACKET_SIZE) ? tmp : MAX_PACKET_SIZE;
		p->requestsPerSession = abs((int)normal(0,1,2)) + requestsPerSession;
		tmp = (unsigned int) pareto( SHAPE_SIZE, replyLength, 1, 2);
		p->replyLength = (tmp < MAX_PACKET_SIZE) ? tmp : MAX_PACKET_SIZE;
		p->replyPerRequest = abs((int)normal(0,1,2)) + replyPerRequest;
		p->timeBetweenRequests = pareto( SHAPE_TIME, timeBetweenRequests, 1, 2);
		p->timeToRespond =  pareto( SHAPE_TIME, timeToRespond,1, 2);
		p->timeBetweenSessions = pareto( SHAPE_TIME, timeBetweenSessions,1, 2);
		p->probability = probability;
		p-> WANprob = WANprob;
		p->profileID = profileID;
		p->ownPort = ownPort;
		p->hopLimit = hopLimit; 
	}

	/*
	 * Get method for request length. It can be configured if a new
	 * random value is generated before returning the value.
	 *
	 * @param noisy Set to true if random values should be generated
	 */
	int getRequestLength(bool noisy)
	{
		if(noisy)
		{
			unsigned int tmp = (unsigned int) pareto( SHAPE_SIZE, requestLength,1, 2);
			return tmp < MAX_PACKET_SIZE ? tmp : MAX_PACKET_SIZE;
		}
		return requestLength;
	}

	int getRequestsPerSession(bool noisy)
	{
		if(noisy)
			return abs((int)normal(0,1,2)) + requestsPerSession;
		return requestsPerSession;
	}

	int getReplyLength(bool noisy)
	{
		if(noisy)
		{
			unsigned int tmp = (unsigned int) pareto( SHAPE_SIZE, replyLength,1, 2);
			return tmp < MAX_PACKET_SIZE ? tmp : MAX_PACKET_SIZE;
		}
		return replyLength;
	}

	int getReplyPerRequest(bool noisy)
	{
		if(noisy)
			return abs((int)normal(0,1,2)) + replyPerRequest;
		return replyPerRequest;
	}

	double getTimeBetweenRequests(bool noisy)
	{
		if(noisy)
			return pareto( SHAPE_TIME, timeBetweenRequests, 1, 2);
		return timeBetweenRequests;
	}

	double getTimeToRespond(bool noisy)
	{
		if(noisy)
			return pareto( SHAPE_TIME, timeToRespond, 1, 2);
		return timeToRespond;
	}

	double getTimeBetweenSessions(bool noisy)
	{
		if(noisy)
			return pareto( SHAPE_TIME, timeBetweenSessions, 1, 2);
		return timeBetweenSessions;
	}
};

/**
 * Structure for storing all information available about a
 * communication endpoint: IP address and port.
 */
struct TargetInfo
{
	IPvXAddress address;
	int port;

	TargetInfo() {; };

	TargetInfo(IPvXAddress address, int port)
	{
		this->port = port;
		this->address = address;
	}
};

/**
 * Structure for storing information about a communication
 * between two endpoints: based on bytes and packets.
 */
struct TransmissionStatistics
{
	
	unsigned long bytesSent, packetSent, packetReceived, bytesReceived;

	TransmissionStatistics()
	    : bytesSent(0), packetSent(0), packetReceived(0), bytesReceived(0)
	{

	}

	TransmissionStatistics(int bytesSent, int bytesReceived, int packetSent, int packetReceived)
	{
		this->bytesSent = bytesSent;
		this->bytesReceived = bytesReceived;
		this->packetSent = packetSent;
		this->packetReceived = packetReceived;
	}

        void printStatistics()
	{
	        std::cout<<"bytes sent: "<<this->bytesSent<<", bytes received: "<<this->bytesReceived<<", packets sent: "<<this->packetSent<<", packets received: "<<this->packetReceived<<endl;
	}
};

/**
 * Structure for storing information about all communication
 * of a single user: based on bytes, packets, and sessions.
 */
struct UserCommunicationStatistics
{
	TransmissionStatistics total, tcp, udp, icmp;
        unsigned long totalSessions, tcpSessions, udpSessions, icmpSessions;

	UserCommunicationStatistics()
	    : totalSessions(0), tcpSessions(0), udpSessions(0), icmpSessions(0)
	{

	}

	void updateStatistics(TransmissionStatistics stat)
	{
		this->total.bytesSent += stat.bytesSent;
		this->total.bytesReceived += stat.bytesReceived;
		this->total.packetSent += stat.packetSent;
		this->total.packetReceived += stat.packetReceived;
		this->totalSessions++;
	}

	void updateTcpStatistics(TransmissionStatistics stat)
	{
		this->tcp.bytesSent += stat.bytesSent;
		this->tcp.bytesReceived += stat.bytesReceived;
		this->tcp.packetSent += stat.packetSent;
		this->tcp.packetReceived += stat.packetReceived;
		this->tcpSessions++;
	}

	void updateUdpStatistics(TransmissionStatistics stat)
	{
		this->udp.bytesSent += stat.bytesSent;
		this->udp.bytesReceived += stat.bytesReceived;
		this->udp.packetSent += stat.packetSent;
		this->udp.packetReceived += stat.packetReceived;
		this->udpSessions++;
	}

	void updateIcmpStatistics(TransmissionStatistics stat)
	{
		this->icmp.bytesSent += stat.bytesSent;
		this->icmp.bytesReceived += stat.bytesReceived;
		this->icmp.packetSent += stat.packetSent;
		this->icmp.packetReceived += stat.packetReceived;
		this->icmpSessions++;
	}
};

#endif /*TRANSMISSIONCONFIG_H_*/
