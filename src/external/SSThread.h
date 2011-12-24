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
 * Implementation is based on the following article:
 * http://www.linuxselfhelp.com/HOWTO/C++Programming-HOWTO-18.html
 *
 * SSThread.h
 *
 *  Created on: May 13, 2011
 *      Author: root
 */

#ifndef SSTHREAD_H_
#define SSTHREAD_H_
#include <pthread.h>
#include <sys/time.h>
#include <iostream>

using namespace std;

class SSThread {

private:
	void * arg_;
	pthread_t thread;
	pthread_cond_t cond;
	pthread_mutex_t mutex;


protected:
	bool nextMsg;
	void run(void * arg);
	static void * entryPoint(void*);
	virtual void setup();
	virtual void execute(void*);
	int lock();
	int unlock();
	void * arg() const {
		return arg_;
	}
	void arg(void* a) {
		arg_ = a;
	}
public:
	SSThread();
	virtual ~SSThread();
	int start(void * arg_);
	int join();
	int wait();
	int wait(long ms);
	int notify();

};

#endif /* SSTHREAD_H_ */
