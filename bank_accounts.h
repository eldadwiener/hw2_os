/*
 * bank_accounts.h
 *
 *  Created on: Dec 17, 2018
 *      Author: compm
 */

#ifndef BANK_ACCOUNTS_H_
#define BANK_ACCOUNTS_H_

#include <stdlib.h>
#include "account.h"
#include <map>

class bank_accounts {
public:
	bank_accounts() {
	}
	;
	void new_acc(int thread_id, string id, string pass, unsigned int balnace);
	void make_vip(int thread_id, string id, string pass);
	void deposit(int thread_id, string id, string pass, int amount);
	void withdraw(int thread_id, string id, string pass, int amount);
	void check_balance(int thread_id, string id, string pass);
	void move_money(int thread_id, string src_id, string pass, string dest_id, int amount);
	void print_accounts();
	int take_commisions();
private:
	map<string, account*> acc_map_;
};

#endif /* BANK_ACCOUNTS_H_ */
