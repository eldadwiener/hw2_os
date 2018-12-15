/*
 * account.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */
#include "account.h"


account::account(int id,int pass,unsigned int balance):
	id_(id),
	pass_(pass),
	balance_(balance),
	vip_(false)
	{}

int account::get_id() {return id_;}

int account::get_pass() {return pass_;}

unsigned int account::get_balance()	{return balance_;}

bool account::get_vip()	{return vip_;}

bool account::change_balance(int newBalance)
{
	if (newBalance < 0) {return false;}
	balance_ = newBalance;
	return true;
}

void account::change_vip(bool newVIPstatus) {vip_ = newVIPstatus;}

void account::print_account()
{
	cout<<"Account " << id_ << ": Balance - " << balance_ "\t$ , Account Password - " << pass_ << endl;


}









