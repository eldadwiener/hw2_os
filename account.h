/*
 * account.h
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#ifndef HW2_OS_ACCOUNT_H_
#define HW2_OS_ACCOUNT_H_
#include  <iostream>
using namespace std;

class account {

public:
	account(int,int,unsigned int);
	int get_id();
	int get_pass();
	unsigned int get_balance();
	bool get_vip();
	bool change_balance(int);
	void change_vip(bool);
	void print_account();

private:
	int id_;
	int pass_;
	unsigned int balance_;
	bool vip_;
};


#endif /* HW2_OS_ACCOUNT_H_ */
