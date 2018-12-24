/*
 * bank.h
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#ifndef HW2_OS_BANK_H_
#define HW2_OS_BANK_H_

#include "ATM.h"

#define NUM_BANK_OPS 2

//extra info at PDF
class bank {
public:
	bank(int);
	~bank();
	void print_bank();
	void take_commisions();
	friend ATM;
	void end_bank() {end_ = true;};
	pATM get_atm(int index);
	pthread_t* get_thread(bool);
private:
	bool end_;
	int bank_balance_;
	int num_of_ATMs_;
	pthread_t back_op_thr[NUM_BANK_OPS];
	pATM* atm_arr; //arr of pATM'
public:
	bank_accounts accounts_map;
};

#endif /* HW2_OS_BANK_H_ */
