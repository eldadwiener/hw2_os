/*
 * main.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#include "bank.h"
ofstream log_file_("log.txt", ofstream::out);
bool validity_check(int argc, char* argv[]);
void* atm_run(void*);
void* take_commisions(void*);
void* print_bank(void*);


int main(int argc, char* argv[]) {
	//TODO: add validity check
	if (!validity_check(argc, argv))
	{
		cout << "illegal arguments" << endl;
		return -1;
	}
	//collect the arguments into making threads to run the files
	int num_of_ATMS = atoi(argv[1]);
	bank the_bank = bank(num_of_ATMS);
	//create all ATM's threads to run with their arguments
	for (int i = 0; i < num_of_ATMS; i++) {
		//make the class of info arguments for the ATM to run
		ATM_data* temp_atm_data = new ATM_data;
		temp_atm_data->file = argv[i + 2];
		temp_atm_data->pAccountsMap = &the_bank.accounts_map;
		temp_atm_data->atmP = the_bank.get_atm(i);
		//run the thread/ATM with the arguments collected
		pATM curr_atm = the_bank.get_atm(i);
		pthread_create(curr_atm->get_pthread(), NULL, atm_run,
				(void*) (temp_atm_data));
	}
	//bank commissions thread
	pthread_t * commisions_thr = the_bank.get_thread(0);
	pthread_create(commisions_thr, NULL, take_commisions, (void*) (&the_bank));
	//bank print thread
	pthread_t * print_thr = the_bank.get_thread(1);
	pthread_create(print_thr, NULL, print_bank, (void*) (&the_bank));
	//join threw all ATM's threads
	for (int i = 0; i < num_of_ATMS; i++) {
		pATM curr_atm = the_bank.get_atm(i);
		pthread_join(*curr_atm->get_pthread(), NULL);
	}
	//ATM's all finished - take down the bank's print and commisions thread's
	the_bank.end_bank();
	log_file_.close();
	return EXIT_SUCCESS;
}

//thread action functions:


//********************************************
// function name: atm_run
// Description: thread action func - split back the arguments apart
//	and run the atm_run func of the ATM with them
// Parameters:  arg - pointer to atm_data class
// Returns: void
//*******************************************
void* atm_run(void* arg) {
	//split back the arguments out of the arg for atm_run to use
	ATM_data* atm_data = (ATM_data*) arg;
	bank_accounts* accounts_map = atm_data->pAccountsMap;
	string atm_file = atm_data->file;
	//run atm_run of ATM
	pATM curr_atm = atm_data->atmP;
	curr_atm->atm_run(accounts_map, atm_file);
	delete atm_data;
	pthread_exit (NULL);
}

//********************************************
// function name: take_commisions
// Description: thread action func - split back the arguments apart
//	and run the take comissions func of bank
// Parameters:  arg - pointer to the bank
// Returns: void
//*******************************************
void* take_commisions(void* arg) {
	bank* the_bank = (bank*) arg;
	the_bank->take_commisions();
	pthread_exit (NULL);
}


//********************************************
// function name: print_bank
// Description: thread action func - split back the arguments apart
//	and print the bank by print_bank() func
// Parameters:  arg - pointer to the bank
// Returns: void
//*******************************************
void* print_bank(void* arg) {
	bank* the_bank = (bank*) arg;
	the_bank->print_bank();
	pthread_exit (NULL);
}

//********************************************
// function name: validity_check
// Description: check the validity of the input
// Parameters:  argc - num of Args from the input
//				argv - the arguments
// Returns: void
//*******************************************


bool validity_check(int argc, char* argv[])
{
	if ((argc<0)||(atoi(argv[1]) != argc-2))
	{
		return false;
	}
	return true;
}


