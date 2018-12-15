/*
 * ATM.h
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#ifndef HW2_OS_ATM_H_
#define HW2_OS_ATM_H_

#include <fstream>

#include "account.h"
#include <string.h>
#include <vector>

class ATM_data;
class ATM;
typedef ATM* pATM;

#include "bank.h"

class ATM {
public:
	ATM();
	pthread_t* get_pthread();
	int get_thread_id();
	void set_id(int);
	void* atm_run(void*);
private:
	pthread_t atm_thread;
	int thread_id;

	void new_acc(bank*,int id,int pass, unsigned int balnace);
	void make_vip(bank* ,int id, int pass);
	void deposit(bank*,int id, int pass, int amount);
	void withdraw(bank* ,int id, int pass, int amount);
	void check_balance(bank* ,int id, int pass);
	void move_money(bank*,int src_id ,int pass, int dest_id,int amount);
};



class ATM_data {
public:
	char* file;
	bank* pBank;
};


#endif /* HW2_OS_ATM_H_ */
