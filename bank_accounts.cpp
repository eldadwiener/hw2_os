/*
 * bank_accounts.cpp
 *
 *  Created on: Dec 17, 2018
 *      Author: compm
 */
#include "bank_accounts.h"
#define MIN_FEE 2


bank_accounts::bank_accounts()/*:log_file_("log.txt", ofstream::out)*/
{
	pthread_mutex_init(&logmutex, NULL);
}


void bank_accounts::new_acc(int thread_id, string id, string pass, unsigned int balance)
{
	account* tempAcc = new account(id, pass, balance);
	pair<map<string, account*>::iterator, bool> ret;
	ret = acc_map_.insert(pair<string, account*>(id, tempAcc));
	pthread_mutex_lock(&logmutex);
	if (ret.second == false) {
		delete tempAcc;
		cout << "Error " << thread_id
				<< ": Your transaction failed – account with the same id exists"
				<< endl;
	} else {
		cout << thread_id << ": New account id is " << id << " with password "
				<< pass << " and initial balance " << balance << endl;
	}
	pthread_mutex_unlock(&logmutex);
}

void bank_accounts::make_vip(int thread_id, string id, string pass) {
	map<string, account*>::iterator itr = acc_map_.find(id);
	if (itr == acc_map_.end()) { //account dosnt exist
		pthread_mutex_lock(&logmutex);
		cout << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	} else if (itr->second->get_pass() != pass) { //wrong password
		pthread_mutex_lock(&logmutex);
		cout << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	} else {
		itr->second->change_vip(true);
	}
}

void bank_accounts::deposit(int thread_id, string id, string pass, int amount) {
	map<string, account*>::iterator itr = acc_map_.find(id);
	if (itr == acc_map_.end()) { //account dosnt exist
		pthread_mutex_lock(&logmutex);
		cout << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	} else if (itr->second->get_pass() != pass) { //wrong password
		pthread_mutex_lock(&logmutex);
		cout << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	} else {
		int newBalance = itr->second->get_balance() + amount;
		itr->second->change_balance(newBalance);
		pthread_mutex_lock(&logmutex);
		cout << thread_id << ": Account " << id << " new balance is "
				<< newBalance << " after " << amount
				<< " $ was deposited" << endl;
		pthread_mutex_unlock(&logmutex);
	}
}

void bank_accounts::withdraw(int thread_id, string id, string pass, int amount) {
	map<string, account*>::iterator itr = acc_map_.find(id);
	if (itr == acc_map_.end()) { //account dosnt exist
		pthread_mutex_lock(&logmutex);
		cout << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	} else if (itr->second->get_pass() != pass) { //wrong password
		pthread_mutex_lock(&logmutex);
		cout << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	} else {
		int newBalance = itr->second->get_balance() - amount;//READ
		if (newBalance < 0)
		{
			pthread_mutex_lock(&logmutex);
			cout << "Error " << thread_id
					<< ": Your transaction failed – account id " << id
					<< " balance is lower than " << amount << endl;
			pthread_mutex_unlock(&logmutex);
		}
		else { //good- give him money
			itr->second->change_balance(newBalance);//WRITE
			pthread_mutex_lock(&logmutex);
			cout << thread_id << ": Account " << id << " new balance is "
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
		cout << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	} else if (itr->second->get_pass() != pass) { //wrong password
		pthread_mutex_lock(&logmutex);
		cout << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	} else {
		int balance = itr->second->get_balance();//READ
		pthread_mutex_lock(&logmutex);
		cout << thread_id << ": Account " << id << " balance is "
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
		cout << "Error " << thread_id
				<< ": Your transaction failed – account id " << src_id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	} else if (itr_s->second->get_pass() != pass) { //src account wrong password
		pthread_mutex_lock(&logmutex);
		cout << "Error " << thread_id
				<< ": Your transaction failed – password for account id "
				<< src_id << " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	} else if ((itr_d == acc_map_.end())) { //dest account dosnt exist
		pthread_mutex_lock(&logmutex);
		cout << "Error " << thread_id
				<< ": Your transaction failed – account id " << dest_id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	}
	else
	{
		int newBalance_s = itr_s->second->get_balance() - amount;//READ
		if (newBalance_s < 0)
		{ //not eanogh money
			pthread_mutex_lock(&logmutex);
			cout << "Error " << thread_id
					<< ": Your transaction failed – account id " << src_id
					<< " balance is lower than " << amount << endl;
			pthread_mutex_unlock(&logmutex);
		}
		else
		{ //good- transfer him the money
			itr_s->second->change_balance(newBalance_s);//WRITE
			int newBalance_d = itr_d->second->get_balance() + amount;//READ
			itr_d->second->change_balance(newBalance_d);//WRITE
			pthread_mutex_lock(&logmutex);
			cout << thread_id << ": Transfer " << amount << " from account "
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
	{
		if (!itr->second->get_vip())
		{ //no vip here
			string acc_id = itr->first;
			double fee_percent = MIN_FEE*(1 + ((double) rand() / (double) RAND_MAX));
			int bal = itr->second->get_balance();//READ
			int commis = (bal * (fee_percent / 100)) + 0.5; //round to the closest int
			itr->second->change_balance(bal - commis);//WRITE
			total_commis += commis;
			pthread_mutex_lock(&logmutex);
			cout << "Bank: commissions of " << fee_percent
					<< " % were charged, the bank gained " << commis
					<< " $ from account " << acc_id << endl;
			pthread_mutex_unlock(&logmutex);
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
		itr->second->print_account();
		itr++;
	}
}





