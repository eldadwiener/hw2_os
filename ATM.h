/*
 * ATM.h
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#ifndef HW2_OS_ATM_H_
#define HW2_OS_ATM_H_

#include <string.h>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "bank_accounts.h"

class ATM;
typedef ATM* pATM;

class ATM {
public:
	ATM();
	pthread_t* get_pthread();
	int get_thread_id();
	void set_id(int i) {
		thread_id = i;
	}
	;
	void atm_run(bank_accounts*, string);
private:
	int thread_id;
	pthread_t atm_thread;

};

class ATM_data {
public:
	string file;
	bank_accounts* pAccountsMap;
	pATM atmP;
};

#endif /* HW2_OS_ATM_H_ */
