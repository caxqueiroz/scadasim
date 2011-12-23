#include "TrafficProfileManager.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

/**
 * @brief Manages available traffic profiles and randomly selects one for
 * the next client/server communication.
 *
 * The traffic profile manager reads all available traffic profiles from
 * a configuration file and returns a random traffic profile if requested.
 * In addition it decides if the server communication endpoints lies in
 * the current AS or in a foreign AS.
 *
 * @class TrafficProfileManager
 */

Define_Module(TrafficProfileManager);

using namespace std;

inline char* GET_OFFSET(char *buf) {
	char *c1 = strrchr(buf, '<');
	if (c1 == NULL)
		return NULL;
	c1++;
	char *c2 = strrchr(buf, '>');
	if (c2 != NULL)
		*c2 = '\0';
	return c1;
}

TrafficProfileManager::TrafficProfileManager() {
}

TrafficProfileManager::~TrafficProfileManager() {
	for (unsigned int i = 0; i < profiles.size(); i++)
		delete profiles[i];
}

/*
 * This method randomly selects a traffic profile and the location of the
 * communication endpoint.
 * The random numbers for selection are given as parameters.
 *
 * @param t Randomly selected traffic profile that is returned
 * @param profileProb Random number for profile selection
 * @param foreignServerProb Random number of endpoint location selection
 * @param isForeign Returns wether the endpoint server is in a foreign AS
 */
void TrafficProfileManager::getTrafficProfile(TrafficProfile &t,
		double profileProb, double foreignServerProb, bool &isForeign) {
	double cum_prob = 0.;
	unsigned int i = 0;
	for (; i < (profiles.size() - 1); i++) {
		cum_prob += profiles[i]->probability;
		if (cum_prob >= profileProb)
			break;
	}

	if (profiles[i]->WANprob >= foreignServerProb)
		isForeign = true;

	t = *profiles[i];
}

void TrafficProfileManager::getFixTrafficProfile(TrafficProfile &t, bool &isForeign) {
	unsigned int i = 0;
	double foreignServerProb;
	foreignServerProb = uniform(0, 1, 1);
	for (; i < (profiles.size() - 1); i++) {
		if (profiles[i]->profileID == t.profileID){
			break;
		}
	}

	if (profiles[i]->WANprob >= foreignServerProb)
		isForeign = true;

	t = *profiles[i];
}

/*
 * Available traffic profiles are read from config file and checked for
 * validity. Then, selection probabilities for profiles are normalized to
 * 100%.
 */
void TrafficProfileManager::initialize(int stages) {
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

	char buf[255];
	; //FIXME: c2 is unused -Claus
	int status = 0;
	TrafficProfile *t = NULL;

	while (in) {
		in.getline(buf, 255);
		// ignore comments
		if (strstr(buf, "#"))
			continue;

		if (strstr(buf, "<Profile>")) {
			t = new TrafficProfile();
			status = 1;
			continue;
		}

		if (status == 1) {
			if (strstr(buf, "<Id>"))
				t->profileID = atoi(GET_OFFSET(buf));
			else if (strstr(buf, "<Label>")) {
				t->label = "";
				t->label += GET_OFFSET(buf);
				t->label += "\0";
			} else if (strstr(buf, "<RequestLength>"))
				t->requestLength = atoi(GET_OFFSET(buf));
			else if (strstr(buf, "<RequestPerSession>"))
				t->requestsPerSession = atoi(GET_OFFSET(buf));
			else if (strstr(buf, "<ReplyLength>"))
				t->replyLength = atoi(GET_OFFSET(buf));
			else if (strstr(buf, "<ReplyPerRequest>"))
				t->replyPerRequest = atoi(GET_OFFSET(buf));
			else if (strstr(buf, "<TimeBetweenRequests>"))
				t->timeBetweenRequests = atof(GET_OFFSET(buf));
			else if (strstr(buf, "<TimeToRespond>"))
				t->timeToRespond = atof(GET_OFFSET(buf));
			else if (strstr(buf, "<TimeBetweenSessions>"))
				t->timeBetweenSessions = atof(GET_OFFSET(buf));
			else if (strstr(buf, "<Ratio>"))
				t->probability = atof(GET_OFFSET(buf));
			else if (strstr(buf, "<WANRatio>"))
				t->WANprob = atof(GET_OFFSET(buf));
			else if (strstr(buf, "<Port>"))
				t->ownPort = atoi(GET_OFFSET(buf));
			else if (strstr(buf, "<HopLimit>"))
				t->hopLimit = atoi(GET_OFFSET(buf));
			else if (strstr(buf, "</Profile>")) {
				status = 0;
				// all data for the current profile is read
				// -> validation check
				if (t->label == "unspec") {
					delete t;
					continue;
				}
				if (t->profileID == -1) {
					delete t;
					continue;
				}
				if (t->ownPort == 0)
					// get a random Port
					t->ownPort = intuniform(1500, 60000);

				profiles.push_back(t);
			}
		}
	}

	// now we have read the complete config file
	// close the config file and update the probabilities
	in.close();

	if (profiles.size() == 0)
		opp_error(
				"No traffic profiles specified for generation of background traffic");

#ifndef DONT_NORMALIZE_TOTAL_TRAFFIC_PROBS
	// normalization can be switched off by above macro definition (for debugging purposes only)
	double cum_ratio = 0.;
	for (unsigned int i = 0; i < profiles.size(); i++)
		cum_ratio += profiles[i]->probability;

	for (unsigned int i = 0; i < profiles.size(); i++) {
		profiles[i]->probability /= cum_ratio;
		profiles[i]->WANprob /= 100;
	}
#endif
}

/*
 * No messages are expected. Thus, throw error if some message is received.
 */
void TrafficProfileManager::handleMessage(cMessage *msg) {
	opp_error("TrafficProfileManager has no messages to handle!");
}

