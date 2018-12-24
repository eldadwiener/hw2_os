/*
 * account.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */
#include "account.h"

//********************************************
// function name: constructor
// Description:
// Parameters:  id,pass,balance for new account
// Returns: void
//*******************************************
account::account(string id, string pass, unsigned int balance) :
		id_(id), pass_(pass), balance_(balance), vip_(false) {}

//********************************************
// function name: get id
// Description:get account id
// Returns: id
//*******************************************
string account::get_id() {
	return id_;
}

//********************************************
// function name: get_pass
// Description:get_pass
// Parameters:
// Returns: password
//*******************************************
string account::get_pass() {
	return pass_;
}

//********************************************
// function name: get_balance
// Parameters:
// Returns: current balance
//*******************************************
unsigned int account::get_balance() {
	return balance_;
}

//********************************************
// function name: get_vip
// Description: get vip status
// Parameters:
// Returns: vip status - TURE OR FALSE
//*******************************************
bool account::get_vip() {
	return vip_;
}

//********************************************
// function name: change_balance
// Description: change balance of the account
// Parameters:  new balance
// Returns: success or failed
//*******************************************
bool account::change_balance(int newBalance) {
	if (newBalance < 0) {
		return false;
	}
	balance_ = newBalance;
	return true;
}

//********************************************
// function name: change_vip
// Description: change acc status to vip
// Parameters: new status of vip
// Returns: void
//*******************************************
void account::change_vip(bool newVIPstatus) {
	vip_ = newVIPstatus;
}

//********************************************
// function name: print_account
// Description: print account info
// Parameters:
// Returns: void
//*******************************************
void account::print_account() {
	cout << "Account " << id_ << ": Balance - " << balance_
			<< "\t$ , Account Password - " << pass_ << endl;
}

