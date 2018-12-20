/*
 * bank_accounts.cpp
 *
 *  Created on: Dec 17, 2018
 *      Author: compm
 */
#include "bank_accounts.h"
#define MIN_FEE 2

void bank_accounts::W_lock(string id)
{
	map<string, ReadWriteMutex*>::iterator itr = mutexs_map_.find(id);
	pthread_mutex_lock(&itr->second->write_mutex);
}
void bank_accounts::R_lock(string id)
{
	map<string, ReadWriteMutex*>::iterator itr = mutexs_map_.find(id);
	pthread_mutex_lock(&itr->second->read_mutex);
	(itr->second->Rcounter)++;
	if (itr->second->Rcounter == 1)
	{
		pthread_mutex_lock(&itr->second->write_mutex);
	}
	pthread_mutex_unlock(&itr->second->read_mutex);
}

void bank_accounts::W_unlock(string id)
{
	map<string, ReadWriteMutex*>::iterator itr = mutexs_map_.find(id);
	pthread_mutex_unlock(&itr->second->write_mutex);
}
void bank_accounts::R_unlock(string id)
{
	map<string, ReadWriteMutex*>::iterator itr = mutexs_map_.find(id);
	pthread_mutex_lock(&itr->second->read_mutex);
	(itr->second->Rcounter)--;
	if (itr->second->Rcounter == 0)
	{
		pthread_mutex_unlock(&itr->second->write_mutex);

	}
	pthread_mutex_unlock(&itr->second->read_mutex);
}




bank_accounts::bank_accounts()/*:log_file_("log.txt", ofstream::out)*/
{
	pthread_mutex_init(&logmutex, NULL);
}


void bank_accounts::new_acc(int thread_id, string id, string pass, unsigned int balance)
{
	sleep(1);
	account* tempAcc = new account(id, pass, balance);//DEBUG delete at the end
	ReadWriteMutex* tempWRM = new ReadWriteMutex;
	pair<map<string, account*>::iterator, bool> ret;
	ret = acc_map_.insert(pair<string, account*>(id, tempAcc));
	mutexs_map_.insert(pair<string, ReadWriteMutex*>(id, tempWRM));
	pthread_mutex_lock(&logmutex);
	if (ret.second == false) {
		delete tempAcc;
		delete tempWRM;
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – account with the same id exists"
				<< endl;
	} else {
		log_file_ << thread_id << ": New account id is " << id << " with password "
				<< pass << " and initial balance " << balance << endl;
	}
	pthread_mutex_unlock(&logmutex);
}

void bank_accounts::make_vip(int thread_id, string id, string pass) {
	map<string, account*>::iterator itr = acc_map_.find(id);
	if (itr == acc_map_.end()) { //account dosnt exist
		pthread_mutex_lock(&logmutex);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	} else if (itr->second->get_pass() != pass) { //wrong password
		pthread_mutex_lock(&logmutex);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	} else
	{
		W_lock(id);
		sleep(1);
		itr->second->change_vip(true);//WRITE
		W_unlock(id);
	}
}

void bank_accounts::deposit(int thread_id, string id, string pass, int amount) {
	map<string, account*>::iterator itr = acc_map_.find(id);
	if (itr == acc_map_.end()) { //account dosnt exist
		pthread_mutex_lock(&logmutex);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	} else if (itr->second->get_pass() != pass) { //wrong password
		pthread_mutex_lock(&logmutex);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	} else {
		W_lock(id);
		sleep(1);
		int newBalance = itr->second->get_balance() + amount;//READ2WRITE
		itr->second->change_balance(newBalance);//WRITE
		W_unlock(id);
		pthread_mutex_lock(&logmutex);
		log_file_ << thread_id << ": Account " << id << " new balance is "
				<< newBalance << " after " << amount
				<< " $ was deposited" << endl;
		pthread_mutex_unlock(&logmutex);
	}
}

void bank_accounts::withdraw(int thread_id, string id, string pass, int amount) {
	map<string, account*>::iterator itr = acc_map_.find(id);
	if (itr == acc_map_.end()) { //account dosnt exist
		pthread_mutex_lock(&logmutex);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	} else if (itr->second->get_pass() != pass) { //wrong password
		pthread_mutex_lock(&logmutex);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	} else {
		W_lock(id);
		sleep(1);
		int newBalance = itr->second->get_balance() - amount;//READ2WRITE
		if (newBalance < 0)
		{
			W_unlock(id);
			pthread_mutex_lock(&logmutex);
			log_file_ << "Error " << thread_id
					<< ": Your transaction failed – account id " << id
					<< " balance is lower than " << amount << endl;
			pthread_mutex_unlock(&logmutex);
		}
		else { //good- give him money
			itr->second->change_balance(newBalance);//WRITE
			W_unlock(id);
			pthread_mutex_lock(&logmutex);
			log_file_ << thread_id << ": Account " << id << " new balance is "
					<< newBalance << " after " << amount
					<< " $ was withdraw" << endl;
			pthread_mutex_unlock(&logmutex);
		}
	}
}

void bank_accounts::check_balance(int thread_id, string id, string pass) {
	map<string, account*>::iterator itr = acc_map_.find(id);
	if (itr == acc_map_.end()) { //account dosnt exist
		pthread_mutex_lock(&logmutex);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	} else if (itr->second->get_pass() != pass) { //wrong password
		pthread_mutex_lock(&logmutex);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	} else {
		R_lock(id);
		sleep(1);
		int balance = itr->second->get_balance();//READ
		R_unlock(id);
		pthread_mutex_lock(&logmutex);
		log_file_ << thread_id << ": Account " << id << " balance is "
				<< balance << endl;
		pthread_mutex_unlock(&logmutex);
	}
}

void bank_accounts::move_money(int thread_id, string src_id, string pass, string dest_id,
		int amount) {
	map<string, account*>::iterator itr_s = acc_map_.find(src_id);
	map<string, account*>::iterator itr_d = acc_map_.find(dest_id);
	if ((itr_s == acc_map_.end())) { //src account dosnt exist
		pthread_mutex_lock(&logmutex);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – account id " << src_id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	} else if (itr_s->second->get_pass() != pass) { //src account wrong password
		pthread_mutex_lock(&logmutex);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – password for account id "
				<< src_id << " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	} else if ((itr_d == acc_map_.end())) { //dest account dosnt exist
		pthread_mutex_lock(&logmutex);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – account id " << dest_id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	}
	else
	{
		W_lock(src_id);
		int newBalance_s = itr_s->second->get_balance() - amount;//READ2WRITE1
		if (newBalance_s < 0)
		{ //not eanogh money
			W_unlock(src_id);
			pthread_mutex_lock(&logmutex);
			log_file_ << "Error " << thread_id
					<< ": Your transaction failed – account id " << src_id
					<< " balance is lower than " << amount << endl;
			pthread_mutex_unlock(&logmutex);
		}
		else
		{ //good- transfer him the money
			itr_s->second->change_balance(newBalance_s);//WRITE1
			W_unlock(src_id);
			W_lock(dest_id);
			sleep(1);
			int newBalance_d = itr_d->second->get_balance() + amount;//READ2WRITE2
			itr_d->second->change_balance(newBalance_d);//WRITE2
			W_unlock(dest_id);
			pthread_mutex_lock(&logmutex);
			log_file_ << thread_id << ": Transfer " << amount << " from account "
					<< src_id << " to account " << dest_id << " new account "
					<< "balance is " << newBalance_s
					<< " new target account balance is "
					<< newBalance_d << endl;
			pthread_mutex_unlock(&logmutex);
		}
	}
}

int bank_accounts::take_commisions()
{
	map<string, account*>::iterator itr;
	itr = acc_map_.begin();
	int total_commis =0;
	while (itr != acc_map_.end())
	{//READ2WRITE
		W_lock(itr->second->get_id());
		if (!itr->second->get_vip())
		{ //no vip here
			string acc_id = itr->first;
			double fee_percent = MIN_FEE*(1 + ((double) rand() / (double) RAND_MAX));
			int bal = itr->second->get_balance();//READ2WRITE
			int commis = (bal * (fee_percent / 100)) + 0.5; //round to the closest int
			itr->second->change_balance(bal - commis);//WRITE

			W_unlock(itr->second->get_id());

			total_commis += commis;
			pthread_mutex_lock(&logmutex);
			log_file_ << "Bank: commissions of " << fee_percent
					<< " % were charged, the bank gained " << commis
					<< " $ from account " << acc_id << endl;
			pthread_mutex_unlock(&logmutex);
		}
		else
		{
			W_unlock(itr->second->get_id());//unlock
		}
		itr++;
	}
	return total_commis;
}

void bank_accounts::print_accounts()
{
	map<string, account*>::iterator itr;
	itr = acc_map_.begin();
	while (itr != acc_map_.end()) {
		R_lock(itr->second->get_id());
		itr->second->print_account();//READ
		R_unlock(itr->second->get_id());
		itr++;
	}
}





