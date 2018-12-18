/*
 * bank_accounts.h
 *
 *  Created on: Dec 17, 2018
 *      Author: compm
 */

#ifndef BANK_ACCOUNTS_H_
#define BANK_ACCOUNTS_H_

#include "account.h"
#include <map>

class bank_accounts {
public:
	bank_accounts() {
	}
	;
	void new_acc(int thread_id, int id, int pass, unsigned int balnace);
	void make_vip(int thread_id, int id, int pass);
	void deposit(int thread_id, int id, int pass, int amount);
	void withdraw(int thread_id, int id, int pass, int amount);
	void check_balance(int thread_id, int id, int pass);
	void move_money(int thread_id, int src_id, int pass, int dest_id,
			int amount);
	map<int, account*> acc_map_;
private:

};

#endif /* BANK_ACCOUNTS_H_ */
