/*
 * main.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#include "bank.h"



int main(int argc, char* argv[])
{
	int num_of_ATMS = atoi(argv[1]);
	bank the_bank = bank(num_of_ATMS);
	for (int i=0; i<num_of_ATMS;i++)
	{
		strcpy(the_bank.ATM_data_arr[i]->file,argv[i+2]);
		the_bank.ATM_data_arr[i]->pBank = &the_bank;
		pATM curr_atm = the_bank.get_atm(i);
		pthread_create(curr_atm->get_pthread(),NULL,curr_atm->atm_run,(void*)(&the_bank.ATM_data_arr[i]));
	}

	pthread_t * commisions_thr = the_bank.get_thread(0);

	pthread_create(commisions_thr,NULL,the_bank.take_commisions,NULL);

	pthread_t * print_thr = the_bank.get_thread(1);
	pthread_create(print_thr,NULL,the_bank.print_bank,NULL);

	for (int i=0; i<num_of_ATMS;i++)
	{
		pATM curr_atm = the_bank.get_atm(i);
		pthread_join(*curr_atm->get_pthread(),NULL);
	}
	return EXIT_SUCCESS;
}





