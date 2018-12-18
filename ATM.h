/*
 * ATM.h
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#ifndef HW2_OS_ATM_H_
#define HW2_OS_ATM_H_

#include <pthread.h>
#include <fstream>
#include <string.h>
#include <sstream>
#include <vector>
<<<<<<< HEAD
#include <stdlib.h>
#include <unistd.h>
#include "bank_accounts.h"
=======
#include "account.h"
>>>>>>> ddf5d575177b567f76b3fa2f911b7716219d9522

class ATM;
typedef ATM* pATM;

class ATM {
public:
	ATM();
	pthread_t* get_pthread();
	int get_thread_id();
<<<<<<< HEAD
	void set_id(int i) {
		thread_id = i;
	}
	;
	void atm_run(bank_accounts*, string);
=======
	void set_id(int);
	void atm_run(bank*, char*);
>>>>>>> ddf5d575177b567f76b3fa2f911b7716219d9522
private:
	int thread_id;
	pthread_t atm_thread;

};

class ATM_data {
public:
<<<<<<< HEAD
	string file;
	bank_accounts* pAccountsMap;
=======
	char* file;
	bank* pBank;
>>>>>>> ddf5d575177b567f76b3fa2f911b7716219d9522
	pATM atmP;
};

#endif /* HW2_OS_ATM_H_ */
