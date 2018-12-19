/*
 * bank.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#include "bank.h"

bank::bank(int num_of_ATMs) :
		bank_balance_(0), num_of_ATMs_(num_of_ATMs) {
	atm_arr = new pATM[num_of_ATMs_];
	for (int i = 0; i < num_of_ATMs_; i++) {
		atm_arr[i] = new ATM;
		atm_arr[i]->set_id(i);
	}
}

bank::~bank() {
	delete atm_arr[num_of_ATMs_];
}

pATM bank::get_atm(int index)

{
	if ((index >= num_of_ATMs_) || (index < 0)) { //might change error handling
		printf("invalid atm_\n");
		return NULL;
	} else {
		return atm_arr[index];
	}
}

pthread_t* bank::get_thread(bool index) { //0 for commisions and 1 for print
	int i = (int) index;
	return &back_op_thr[i];
}

void bank::print_bank() {
	while (true) {
		if (sleep(4)) {//DEBUG fix to 0.5
			perror("sleep failed\n");
		}
		printf("\033[2J");
		printf("\033[1;1H");
		cout << "Current Bank Status" << endl;
		accounts_map.print_accounts();
		cout << "The Bank has " << bank_balance_ << " $" << endl;
	}
}

void bank::take_commisions() {
	while (true) {
		if (sleep(2)) {//DEBUG fix to 3
			perror("usleep failed\n");
		}
		bank_balance_+= accounts_map.take_commisions();
	}
}

