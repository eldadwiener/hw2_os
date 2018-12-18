/*
 * bank_accounts.cpp
 *
 *  Created on: Dec 17, 2018
 *      Author: compm
 */
#include "bank_accounts.h"

void bank_accounts::new_acc(int thread_id, int id, int pass,
		unsigned int balance) {
	account* tempAcc = new account(id, pass, balance);
	map<int, account*>::iterator itr = acc_map_.begin();
	itr = acc_map_.find(id);
	if (itr != acc_map_.end()) {
		delete tempAcc;
		cout << "Error " << thread_id
				<< ": Your transaction failed – account with the same id exists"
				<< endl;
	} else {
		acc_map_.insert(itr, pair<int, account*>(id, tempAcc));
		cout << thread_id << ": New account id is " << id << " with password "
				<< pass << " and initial balance " << balance << endl;
	}
}

void bank_accounts::make_vip(int thread_id, int id, int pass) {
	map<int, account*>::iterator itr = acc_map_.find(id);
	if (itr == acc_map_.end()) { //account dosnt exist
		cout << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
	} else if (itr->second->get_pass() != pass) { //wrong password
		cout << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
	} else {
		itr->second->change_vip(true);
	}
}

void bank_accounts::deposit(int thread_id, int id, int pass, int amount) {
	map<int, account*>::iterator itr = acc_map_.find(id);
	if (itr == acc_map_.end()) { //account dosnt exist
		cout << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
	} else if (itr->second->get_pass() != pass) { //wrong password
		cout << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
	} else {
		int newBalance = itr->second->get_balance() + amount;
		itr->second->change_balance(newBalance);
		cout << thread_id << ": Account " << id << " new balance is "
				<< itr->second->get_balance() << " after " << amount
				<< " $ was deposited" << endl;
	}
}

void bank_accounts::withdraw(int thread_id, int id, int pass, int amount) {
	map<int, account*>::iterator itr = acc_map_.find(id);
	if (itr == acc_map_.end()) { //account dosnt exist
		cout << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
	} else if (itr->second->get_pass() != pass) { //wrong password
		cout << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
	} else {
		int newBalance = itr->second->get_balance() - amount;
		if (!itr->second->change_balance(newBalance)) { //not eanogh money
			cout << "Error " << thread_id
					<< ": Your transaction failed – account id " << id
					<< " balance is lower than " << amount << endl;
		} else { //good- give him money
			cout << thread_id << ": Account " << id << " new balance is "
					<< itr->second->get_balance() << " after " << amount
					<< " $ was withdraw" << endl;
		}
	}
}

void bank_accounts::check_balance(int thread_id, int id, int pass) {
	map<int, account*>::iterator itr = acc_map_.find(id);
	if (itr == acc_map_.end()) { //account dosnt exist
		cout << "Error " << thread_id
				<< ": Your transaction failed – account id " << id
				<< " does not exist" << endl;
	} else if (itr->second->get_pass() != pass) { //wrong password
		cout << "Error " << thread_id
				<< ": Your transaction failed – password for account id " << id
				<< " is incorrect" << endl;
	} else {
		cout << thread_id << ": Account " << id << " balance is "
				<< itr->second->get_balance() << endl;
	}
}

void bank_accounts::move_money(int thread_id, int src_id, int pass, int dest_id,
		int amount) {
	map<int, account*>::iterator itr_s = acc_map_.find(src_id);
	map<int, account*>::iterator itr_d = acc_map_.find(dest_id);
	if ((itr_s == acc_map_.end())) { //src account dosnt exist
		cout << "Error " << thread_id
				<< ": Your transaction failed – account id " << src_id
				<< " does not exist" << endl;
	} else if (itr_s->second->get_pass() != pass) { //src account wrong password
		cout << "Error " << thread_id
				<< ": Your transaction failed – password for account id "
				<< src_id << " is incorrect" << endl;
	} else if ((itr_d == acc_map_.end())) { //dest account dosnt exist
		cout << "Error " << thread_id
				<< ": Your transaction failed – account id " << dest_id
				<< " does not exist" << endl;
	} else {
		int newBalance_s = itr_s->second->get_balance() - amount;
		if (!itr_s->second->change_balance(newBalance_s)) { //not eanogh money
			cout << "Error " << thread_id
					<< ": Your transaction failed – account id " << src_id
					<< " balance is lower than " << amount << endl;
		} else { //good- transfer him the money
			int newBalance_d = itr_d->second->get_balance() + amount;
			itr_d->second->change_balance(newBalance_d);

			cout << thread_id << ": Transfer " << amount << " from account "
					<< src_id << " to account " << dest_id << " new account "
					<< "balance is " << itr_s->second->get_balance()
					<< " new target account balance is "
					<< itr_d->second->get_balance() << endl;
		}
	}
}

