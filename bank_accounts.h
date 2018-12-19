/*
 * bank_accounts.h
 *
 *  Created on: Dec 17, 2018
 *      Author: compm
 */

#ifndef BANK_ACCOUNTS_H_
#define BANK_ACCOUNTS_H_

#include <pthread.h>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <map>
#include "account.h"

class ReadWriteMutex;

class bank_accounts {
public:
	bank_accounts();
	~bank_accounts() {/*log_file_.close();*/};
	void new_acc(int thread_id, string id, string pass, unsigned int balnace);
	void make_vip(int thread_id, string id, string pass);
	void deposit(int thread_id, string id, string pass, int amount);
	void withdraw(int thread_id, string id, string pass, int amount);
	void check_balance(int thread_id, string id, string pass);
	void move_money(int thread_id, string src_id, string pass, string dest_id, int amount);
	void print_accounts();
	int take_commisions();
private:
	//ofstream log_file_;
	pthread_mutex_t logmutex;
	map<string, account*> acc_map_;
	map<string,ReadWriteMutex*> mutexs_map_;
	void W_lock(string);
	void R_lock(string);
	void W_unlock(string);
	void R_unlock(string);
};

class ReadWriteMutex
{
public:
	ReadWriteMutex():Rcounter(0)
	{
		pthread_mutex_init(&read_mutex, NULL);
		pthread_mutex_init(&write_mutex, NULL);
	};
	int Rcounter;
	pthread_mutex_t read_mutex;
	pthread_mutex_t write_mutex;

};


#endif /* BANK_ACCOUNTS_H_ */
