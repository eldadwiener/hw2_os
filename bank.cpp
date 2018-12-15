/*
 * bank.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#include "bank.h"
#define MIN_FEE 2

bank::bank(int num_of_ATMs):
	bank_balance_(0),
	num_of_ATMs_(num_of_ATMs),
	log_file_("log.txt")
	//back_op_thr(NULL)
	//acc_map_(NULL)
{
	atm_arr = new pATM[num_of_ATMs_];
	for(int i=0;i<num_of_ATMs_;i++)
	{
		atm_arr[i]->set_id(i);
	}
	ATM_data_arr = new ATM_data*[num_of_ATMs_];
}

bank::~bank()
{
	delete atm_arr[num_of_ATMs_];
	delete ATM_data_arr[num_of_ATMs_];
}

pATM bank::get_atm(int index)

{
	if ((index >= num_of_ATMs_ )|| (index < 0) )
	{//might change error handling
		printf("invalid atm_\n");
		return NULL;
	}
	else
	{
		return atm_arr[index];
	}
}


pthread_t* bank::get_thread(bool index)
{//0 for commisions and 1 for print
	int i = (int)index;
	return back_op_thr[i];
}



void bank::print_bank()
{
	map<int,account*>::iterator itr;
	while(true)
	{
		if (!usleep(500)) {perror("usleep failed\n");}
		printf("\033[2J");
		printf("\033[1;1H");
		itr = acc_map_.begin();
		cout << "Current Bank Status"<< endl;
		while (itr != acc_map_.end())
		{
			itr->second->print_account();
			itr++;
		}
		cout << "The Bank has " << bank_balance_ << " $" << endl;
	}
}


void bank::take_commisions()
{
	map<int,account*>::iterator itr;
	while(true)
	{
		if (!usleep(3000)) {perror("usleep failed\n");}
		itr = acc_map_.begin();
		while (itr != acc_map_.end())
		{
			if (!itr->second->get_vip())
			{//no vip here
				int bal = itr->second->get_balance();
				double fee_percent = MIN_FEE*(1+((double)rand()/(double)RAND_MAX));
				printf("fee is %f\n",fee_percent);//DEBUG
				int commis = (bal*(fee_percent/100))+0.5;//round to the closest int
				itr->second->change_balance(bal -commis);
				bank_balance_+= commis;
				log_file_<<"Bank: commissions of "<< fee_percent <<" % were charged, the bank gained "
						<< commis <<" $ from account "<< itr->first << endl;
			}
			itr++;
		}
	}
}











