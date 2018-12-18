/*
 * bank.h
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#ifndef HW2_OS_BANK_H_
#define HW2_OS_BANK_H_

#include <stdlib.h>
<<<<<<< HEAD
=======
#include <pthread.h>
#include <map>
#include "account.h"
class bank;
>>>>>>> ddf5d575177b567f76b3fa2f911b7716219d9522
#include "ATM.h"

#define NUM_BANK_OPS 2

class bank {

public:
	bank(int);
	~bank();
	void print_bank();
	void take_commisions();
	friend ATM;
	pATM get_atm(int index);
	pthread_t* get_thread(bool);
private:
	int bank_balance_;
	int num_of_ATMs_;
	pthread_t back_op_thr[NUM_BANK_OPS];
	pATM* atm_arr; //arr of pATM'
public:
	//ATM_data** ATM_data_arr;
	bank_accounts accounts_map;
};

#endif /* HW2_OS_BANK_H_ */
