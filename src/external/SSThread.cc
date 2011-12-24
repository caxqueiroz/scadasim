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
/*
 * SSThread.cpp
 *
 *  Created on: May 13, 2011
 *      Author: root
 */

#include "SSThread.h"

SSThread::SSThread() {

}

SSThread::~SSThread() {
	// TODO Auto-generated destructor stub
}

int SSThread::start(void * arg_) {
	arg(arg_);

	int code = pthread_create(&thread, 0, &SSThread::entryPoint, this);

	return code;
}

void SSThread::run(void * arg_) {
	setup();
	execute(arg_);
}

/*static */
void * SSThread::entryPoint(void * pthis) {
	SSThread * pt = (SSThread*) pthis;
	pt->run(pt->arg());
	return NULL;
}

int SSThread::wait() {
	return pthread_cond_wait(&cond, &mutex);
}

int SSThread::wait(long ms) {
	timeval now;
	timespec timeout;
	gettimeofday(&now, NULL);
	TIMEVAL_TO_TIMESPEC(&now, &timeout);
	if (ms > 1000) {
		timeout.tv_sec = timeout.tv_sec + (ms / 1000);
		timeout.tv_nsec = timeout.tv_nsec  + ((ms % 1000) * 1000000);
	} else{
		timeout.tv_nsec = timeout.tv_nsec + (ms * 1000000);
	}
	return pthread_cond_timedwait(&cond, &mutex, &timeout);
}

int SSThread::notify() {
	return pthread_cond_signal(&cond);
}

int SSThread::lock() {
	return pthread_mutex_lock(&mutex);
}
int SSThread::unlock() {
	return pthread_mutex_unlock(&mutex);
}

void SSThread::setup() {
	nextMsg = false;
	if (pthread_mutex_init(&mutex, NULL) != 0) {
		cout << "error initialising mutex" << endl;
	}
	if (pthread_cond_init(&cond, NULL) != 0) {
		cout << "error initialising cond" << endl;
	}

}

void SSThread::execute(void* arg) {
	// Your code goes here
}

int SSThread::join() {
	return pthread_join(thread, NULL);
}
