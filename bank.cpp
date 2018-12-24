/*
 * bank.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#include "bank.h"


//********************************************
// function name: constructor
// Description: initialize and make the ATM's array and give each a unique number
// Parameters:  number of ATM's to create
// Returns: void
//*******************************************
bank::bank(int num_of_ATMs) :
	end_(false), bank_balance_(0), num_of_ATMs_(num_of_ATMs)
{
	atm_arr = new pATM[num_of_ATMs_];
	for (int i = 0; i < num_of_ATMs_; i++) {
		atm_arr[i] = new ATM;
		atm_arr[i]->set_id(i);
	}
}



//********************************************
// function name: destructor
// Description: delete the ATM's array
// Parameters:  void
// Returns: void
//*******************************************
bank::~bank() {
	for(int i=0;i<num_of_ATMs_;i++)
	{
		delete atm_arr[i];
	}
	delete[] atm_arr;
}


//********************************************
// function name: get_atm
// Description: find a specific atm
// Parameters:  atm index
// Returns: pointer to the ATM
//*******************************************
pATM bank::get_atm(int index)
{
	if ((index >= num_of_ATMs_) || (index < 0)) { //might change error handling
		printf("invalid atm_\n");//DEBUG
		return NULL;
	} else {
		return atm_arr[index];
	}
}


//********************************************
// function name: get_thread
// Description: get one of the 2 threads running directly from bank(not ATM threads)
// Parameters:  index - 0 or 1 - there are 2 threds
// Returns: pointer to the thread
//*******************************************
pthread_t* bank::get_thread(bool index) { //0 for commisions and 1 for print
	int i = (int) index;
	return &back_op_thr[i];
}



//********************************************
// function name: print_bank
// Description: call bank accounts to print each 0.5 seconds and bank balance
// Parameters:  void
// Returns: void
//*******************************************
void bank::print_bank() {
	while (!end_)
	{//if ATM'S didnt finish their actions yet-continue printing
		if (usleep(500000)) {
			perror("sleep failed\n");
		}
		printf("\033[2J");
		printf("\033[1;1H");
		accounts_map.print_accounts();
		cout << "The Bank has " << bank_balance_ << " $" << endl;
	}
}


//********************************************
// function name: take_commisions
// Description: call each 3 seconds to take commisions of bank accounts
//*******************************************
void bank::take_commisions() {
	while (!end_) {
		if (sleep(3)) {
			perror("usleep failed\n");
		}
		bank_balance_+= accounts_map.take_commisions();
	}
}

