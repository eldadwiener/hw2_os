/*
 * bank_accounts.cpp
 *
 *  Created on: Dec 17, 2018
 *      Author: compm
 */
#include "bank_accounts.h"
#define MIN_FEE 2

//********************************************
// function name: W_lock
// Description: lock given writers mutex
// Parameters: class with the relevant shared resource mutexes
// Returns: void
//********************************************
void bank_accounts::W_lock(ReadWriteMutex* sharedItemMutex)

{
	pthread_mutex_lock(&(sharedItemMutex->write_mutex));
}

//********************************************
// function name: R_lock
// Description: add a reader by locking mutex
// Parameters: class with the relevant shared resource mutexes
// Returns: void
//********************************************
void bank_accounts::R_lock(ReadWriteMutex* sharedItemMutex)
{
	pthread_mutex_lock(&(sharedItemMutex->read_mutex));
	//update readers counter
	sharedItemMutex->Rcounter++;
	//first reader lock from writing
	if (sharedItemMutex->Rcounter == 1)
	{
		pthread_mutex_lock(&(sharedItemMutex->write_mutex));
	}
	pthread_mutex_unlock(&(sharedItemMutex->read_mutex));
}


//********************************************
// function name: W_unlock
// Description: release a write from shared resource
// Parameters: class with the relevant shared resource mutexes
// Returns: void
//********************************************
void bank_accounts::W_unlock(ReadWriteMutex* sharedItemMutex)
{
	pthread_mutex_unlock(&(sharedItemMutex->write_mutex));
}

//********************************************
// function name: R_unlock
// Description: relase a reader from shared resource
// Parameters: class with the relevant shared resource mutexes
// Returns: void
//********************************************
void bank_accounts::R_unlock(ReadWriteMutex* sharedItemMutex)
{
	pthread_mutex_lock(&(sharedItemMutex->read_mutex));
	sharedItemMutex->Rcounter--;
	if (sharedItemMutex->Rcounter == 0)
	{//last reader finishes relasse for writing
		pthread_mutex_unlock(&(sharedItemMutex->write_mutex));
	}
	pthread_mutex_unlock(&(sharedItemMutex->read_mutex));
}



//********************************************
// function name: bank_accounts
// Description:	constructor
// Parameters: void
// Returns: void
//********************************************
bank_accounts::bank_accounts()
{
	pthread_mutex_init(&logmutex, NULL);
}

//********************************************
// function name: bank_accounts
// Description:	destructor
// Parameters: void
// Returns: void
//********************************************
bank_accounts::~bank_accounts()
{
	map<string, account*>::iterator itr = acc_map_.begin();
	while (itr != acc_map_.end())
	{//delete all accounts
		delete itr->second;
		itr++;
	}
}


//********************************************
// function name: new_acc
// Description:	create new account
// Parameters: // Parameters:  which thread/ATM runs the func,acc id,pass,balance
// Returns: void
//********************************************
void bank_accounts::new_acc(int thread_id, string id, string pass, unsigned int balance)
{
	account* tempAcc = new account(id, pass, balance);//DEBUG delete at the end
	pair<map<string, account*>::iterator, bool> ret;
	W_lock(&map_mutexes_);//write to shared resource- accounts map
	ret = acc_map_.insert(pair<string, account*>(id, tempAcc));
	W_unlock(&map_mutexes_);
	pthread_mutex_lock(&logmutex);//writing to log file-a shared resoucre
	sleep(1);
	if (ret.second == false)
	{//if insert failed- the map didnt push cause it exists
		delete tempAcc;
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – account with the same id exists"
				<< endl;
	} else {
		log_file_ << thread_id << ": New account id is " << id << " with password "
				<< pass << " and initial balance " << balance << endl;
	}
	pthread_mutex_unlock(&logmutex);
}

//********************************************
// function name: make_vip
// Description:make a speecific ac vip
// Parameters:  which thread/ATM runs the func,acc id,pass
// Returns: void
//********************************************
void bank_accounts::make_vip(int thread_id, string id, string pass) {
	//searching(reading) threw the map - a shared resource
	R_lock(&map_mutexes_);
	map<string, account*>::iterator itr = acc_map_.find(id);
	R_unlock(&map_mutexes_);
	if (itr == acc_map_.end()) { //account dosnt exist
		//write to log
		pthread_mutex_lock(&logmutex);
		sleep(1);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	}
	else if (itr->second->get_pass() != pass)
	{ //wrong password
		//write to log
		pthread_mutex_lock(&logmutex);
		sleep(1);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	}
	else
	{
		//write to specific account
		W_lock(&itr->second->acc_mutexes_);
		sleep(1);
		itr->second->change_vip(true);//WRITE
		W_unlock(&itr->second->acc_mutexes_);
	}
}


//********************************************
// function name: deposit
// Description: deposit money to account
// Parameters:  which thread/ATM runs the func, acc id,pass
// Returns: void
//********************************************
void bank_accounts::deposit(int thread_id, string id, string pass, int amount) {
	//search threw the map for the account
	R_lock(&map_mutexes_);
	map<string, account*>::iterator itr = acc_map_.find(id);
	R_unlock(&map_mutexes_);
	if (itr == acc_map_.end())
	{ //account dosnt exist
		pthread_mutex_lock(&logmutex);
		sleep(1);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	}
	else if (itr->second->get_pass() != pass)
	{ //wrong password
		//write to log
		pthread_mutex_lock(&logmutex);
		sleep(1);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	}
	else
	{//write to account and logfile after finish action
		W_lock(&itr->second->acc_mutexes_);
		int newBalance = itr->second->get_balance() + amount;//READ2WRITE
		itr->second->change_balance(newBalance);//WRITE
		pthread_mutex_lock(&logmutex);
		sleep(1);
		log_file_ << thread_id << ": Account " << id << " new balance is "
				<< newBalance << " after " << amount
				<< " $ was deposited" << endl;
		pthread_mutex_unlock(&logmutex);
		W_unlock(&itr->second->acc_mutexes_);
	}
}


//********************************************
// function name: withdraw
// Description: withdraw money from account
// Parameters:  which thread/ATM runs the func, acc id,pass,amount
// Returns: void
//********************************************
void bank_accounts::withdraw(int thread_id, string id, string pass, int amount) {
	//search threw map for account
	R_lock(&map_mutexes_);
	map<string, account*>::iterator itr = acc_map_.find(id);
	R_unlock(&map_mutexes_);
	if (itr == acc_map_.end())
	{ //account dosnt exist
		//write to log
		pthread_mutex_lock(&logmutex);
		sleep(1);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	}
	else if (itr->second->get_pass() != pass)
	{ //wrong password
		//write to log
		pthread_mutex_lock(&logmutex);
		sleep(1);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	}
	else
	{//write to account and to log file
		W_lock(&itr->second->acc_mutexes_);
		int newBalance = itr->second->get_balance() - amount;//READ2WRITE
		if (newBalance < 0)
		{//not eanogh money to withdraw
			pthread_mutex_lock(&logmutex);
			sleep(1);
			log_file_ << "Error " << thread_id
					<< ": Your transaction failed – account id " << id
					<< " balance is lower than " << amount << endl;
			pthread_mutex_unlock(&logmutex);
			W_unlock(&itr->second->acc_mutexes_);
		}
		else
		{ //good- give him money
			itr->second->change_balance(newBalance);//WRITE
			//write to log file
			pthread_mutex_lock(&logmutex);
			sleep(1);
			log_file_ << thread_id << ": Account " << id << " new balance is "
					<< newBalance << " after " << amount
					<< " $ was withdraw" << endl;
			pthread_mutex_unlock(&logmutex);
			W_unlock(&itr->second->acc_mutexes_);
		}
	}
}


//********************************************
// function name: check_balance
// Description: read the existing balance of an account and print to log file
// Parameters:  which thread/ATM runs the func, acc id,pass
// Returns: void
//*******************************************
void bank_accounts::check_balance(int thread_id, string id, string pass) {
	//find acc threw map
	R_lock(&map_mutexes_);
	map<string, account*>::iterator itr = acc_map_.find(id);
	R_unlock(&map_mutexes_);
	if (itr == acc_map_.end()) { //account dosnt exist
		pthread_mutex_lock(&logmutex);
		sleep(1);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
		pthread_mutex_unlock(&logmutex);
	}
	else if (itr->second->get_pass() != pass)
	{ //wrong password
		pthread_mutex_lock(&logmutex);
		sleep(1);
		log_file_ << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
		pthread_mutex_unlock(&logmutex);
	}
	else
	{//get the balance from the acc
		R_lock(&itr->second->acc_mutexes_);
		int balance = itr->second->get_balance();//READ
		pthread_mutex_lock(&logmutex);
		sleep(1);
		log_file_ << thread_id << ": Account " << id << " balance is "
				<< balance << endl;
		pthread_mutex_unlock(&logmutex);
		R_unlock(&itr->second->acc_mutexes_);
	}
}


//********************************************
// function name: move_money
// Description: move money from acc to other acc
// Parameters:  which thread/ATM runs the func, src acc id and pass, dest acc id
// Returns: void
//*******************************************
void bank_accounts::move_money(int thread_id, string src_id, string pass, string dest_id,
	int amount) {
	//find the src and dest acc's
	R_lock(&map_mutexes_);
	map<string, account*>::iterator itr_s = acc_map_.find(src_id);
	map<string, account*>::iterator itr_d = acc_map_.find(dest_id);
	R_unlock(&map_mutexes_);
	if (src_id != dest_id)
	{
		if ((itr_s == acc_map_.end()))
		{ //src account dosnt exist
			pthread_mutex_lock(&logmutex);
			sleep(1);
			log_file_ << "Error " << thread_id
					<< ": Your transaction failed – account id " << src_id
					<< " does not exist" << endl;
			pthread_mutex_unlock(&logmutex);
		} else if (itr_s->second->get_pass() != pass)
		{ //src account wrong password
			pthread_mutex_lock(&logmutex);
			sleep(1);
			log_file_ << "Error " << thread_id
					<< ": Your transaction failed – password for account id "
					<< src_id << " is incorrect" << endl;
			pthread_mutex_unlock(&logmutex);
		} else if ((itr_d == acc_map_.end()))
		{ //dest account dosnt exist
			pthread_mutex_lock(&logmutex);
			sleep(1);
			log_file_ << "Error " << thread_id
					<< ": Your transaction failed – account id " << dest_id
					<< " does not exist" << endl;
			pthread_mutex_unlock(&logmutex);
		}
		else
		{//lets try to tranfer
			W_lock(&itr_s->second->acc_mutexes_);
			int newBalance_s = itr_s->second->get_balance() - amount;//READ2WRITE1
			if (newBalance_s < 0)
			{ //not eanogh money to tranfer
				pthread_mutex_lock(&logmutex);
				sleep(1);
				log_file_ << "Error " << thread_id
						<< ": Your transaction failed – account id " << src_id
						<< " balance is lower than " << amount << endl;
				pthread_mutex_unlock(&logmutex);
				W_unlock(&itr_s->second->acc_mutexes_);
			}
			else
			{ //good- transfer him the money, lock both accounts for the tranfer and print to log
				itr_s->second->change_balance(newBalance_s);//WRITE1
				W_lock(&itr_d->second->acc_mutexes_);
				int newBalance_d = itr_d->second->get_balance() + amount;//READ2WRITE2
				itr_d->second->change_balance(newBalance_d);//WRITE2
				pthread_mutex_lock(&logmutex);
				sleep(1);
				log_file_ << thread_id << ": Transfer " << amount << " from account "
						<< src_id << " to account " << dest_id << " new account "
						<< "balance is " << newBalance_s
						<< " new target account balance is "
						<< newBalance_d << endl;
				pthread_mutex_unlock(&logmutex);
				W_unlock(&itr_d->second->acc_mutexes_);
				W_unlock(&itr_s->second->acc_mutexes_);
			}
		}
	}
}


//********************************************
// function name: take_commisions
// Description: take from all accounts a comission of 2 to 4 % of acc balance each 3 seconds
// Parameters: void
// Returns: total commis from all accounts each round
//*******************************************
int bank_accounts::take_commisions()
{
	//lock for wring over map - no added accounts allowed while taking comission
	R_lock(&map_mutexes_);
	map<string, account*>::iterator itr;
	itr = acc_map_.begin();
	int total_commis = 0;
	while (itr != acc_map_.end())
	{//READ2WRITE
		W_lock(&itr->second->acc_mutexes_);
		if (!itr->second->get_vip())
		{ //no vip here
			string acc_id = itr->first;
			double fee_percent = MIN_FEE*(1 + ((double) rand() / (double) RAND_MAX));
			int bal = itr->second->get_balance();//READ2WRITE
			int commis = (bal * (fee_percent / 100)) + 0.5; //round to the closest int
			itr->second->change_balance(bal - commis);//WRITE
			total_commis += commis;
			pthread_mutex_lock(&logmutex);
			log_file_ << "Bank: commissions of " << fee_percent
					<< " % were charged, the bank gained " << commis
					<< " $ from account " << acc_id << endl;
			pthread_mutex_unlock(&logmutex);
			W_unlock(&itr->second->acc_mutexes_);
		}
		else
		{
			W_unlock(&itr->second->acc_mutexes_);//unlock
		}
		itr++;
	}
	R_unlock(&map_mutexes_);
	return total_commis;
}


//********************************************
// function name: print_accounts
// Description: print all bank accounts data snapshot
// Parameters:  void
// Returns: void
//*******************************************
void bank_accounts::print_accounts()
{
	//catch all accounts
	R_lock(&map_mutexes_);
	map<string, account*>::iterator itr;
	itr = acc_map_.begin();
	while (itr != acc_map_.end())
	{
		R_lock(&itr->second->acc_mutexes_);
		itr++;
	}
	//take the snapshot
	cout << "Current Bank Status" << endl;
	itr = acc_map_.begin();
	while (itr != acc_map_.end()) {
		itr->second->print_account();//READ
		itr++;
	}
	//release all acocounts after snapshot taken
	itr = acc_map_.begin();
	while (itr != acc_map_.end())
	{
		R_unlock(&itr->second->acc_mutexes_);
		itr++;
	}
	R_unlock(&map_mutexes_);
}





