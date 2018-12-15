/*
 * main.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#include "bank.h"

void* atm_run(void*);
void* take_commisions(void*);
void* print_bank(void*);


int main(int argc, char* argv[])
{
	int num_of_ATMS = atoi(argv[1]);
	bank the_bank = bank(num_of_ATMS);
	for (int i=0; i<num_of_ATMS;i++)
	{
		strcpy(the_bank.ATM_data_arr[i]->file,argv[i+2]);
		the_bank.ATM_data_arr[i]->pBank = &the_bank;
		the_bank.ATM_data_arr[i]->atmP = the_bank.get_atm(i);
		pATM curr_atm = the_bank.get_atm(i);
		pthread_create(curr_atm->get_pthread(),NULL,atm_run,(void*)(&the_bank.ATM_data_arr[i]));
	}

	pthread_t * commisions_thr = the_bank.get_thread(0);

	pthread_create(commisions_thr,NULL,take_commisions,(void*)(&the_bank));

	pthread_t * print_thr = the_bank.get_thread(1);
	pthread_create(print_thr,NULL,print_bank,(void*)(&the_bank));

	for (int i=0; i<num_of_ATMS;i++)
	{
		pATM curr_atm = the_bank.get_atm(i);
		pthread_join(*curr_atm->get_pthread(),NULL);
	}
	return EXIT_SUCCESS;
}

void* atm_run(void* arg)
{
	ATM_data* atm_data = (ATM_data*)arg;
	bank* the_bank = atm_data->pBank;
	char* atm_file = atm_data->file;
	pATM curr_atm = atm_data->atmP;
	curr_atm->atm_run(the_bank,atm_file);
	pthread_exit(NULL);
}

void* take_commisions(void* arg)
{
	bank* the_bank =(bank*)arg;
	the_bank->take_commisions();
	pthread_exit(NULL);
}

void* print_bank(void* arg)
{
	bank* the_bank =(bank*)arg;
	the_bank->print_bank();
	pthread_exit(NULL);
}












