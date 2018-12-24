/*
 * account.h
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#ifndef HW2_OS_ACCOUNT_H_
#define HW2_OS_ACCOUNT_H_

#include <iostream>
#include <stdio.h>
#include <pthread.h>

using namespace std;
extern ofstream log_file_;

//extra info at PDF
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


//extra info at PDF
class account {

public:
	account(string, string, unsigned int);
	string get_id();
	string get_pass();
	unsigned int get_balance();
	bool get_vip();
	bool change_balance(int);
	void change_vip(bool);
	void print_account();
private:
	string id_;
	string pass_;
	unsigned int balance_;
	bool vip_;
public:
	ReadWriteMutex acc_mutexes_;
};




#endif /* HW2_OS_ACCOUNT_H_ */
