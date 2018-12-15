/*
 * ATM.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */


#include "ATM.h"
#include <cstring>
ATM::ATM():
	atm_thread(NULL),
	thread_id(0)
	{}

pthread_t* ATM::get_pthread()
{
	return &(atm_thread);
}


void ATM::new_acc(bank* the_bank ,int id,int pass, unsigned int balance)
{
	account* tempAcc = new account(id,pass,balance);
	map<int,account*>::iterator itr = the_bank->acc_map_.begin();
	itr = the_bank->acc_map_.find(id);
	if (itr != the_bank->acc_map_.end())
	{
		delete tempAcc;
		the_bank->log_file_ << "Error "<< thread_id <<
				": Your transaction failed – account with the same id exists" << endl;
	}
	else
	{
		the_bank->acc_map_.insert(itr,pair<int,account*> (id,tempAcc));
		the_bank->log_file_ << thread_id <<": New account id is "<< id <<
				" with password "<< pass << " and initial balance " << balance << endl;
	}
}

void ATM::make_vip(bank* the_bank,int id, int pass)
{
	map<int,account*>::iterator itr	= the_bank->acc_map_.find(id);
	if (itr == the_bank->acc_map_.end() )
	{//account dosnt exist
		the_bank->log_file_ << "Error "<< thread_id <<
				": Your transaction failed – account id " << id <<
				" does not exist"<< endl;
	}
	else if (itr->second->get_pass() != pass)
	{//wrong password
		the_bank->log_file_ << "Error "<< thread_id <<
			": Your transaction failed – password for account id " << id <<
			" is incorrect"<< endl;
	}
	else
	{
		itr->second->change_vip(true);
	}
}

void ATM::deposit(bank* the_bank,int id, int pass, int amount)
{
	map<int,account*>::iterator itr	= the_bank->acc_map_.find(id);
	if (itr == the_bank->acc_map_.end() )
	{//account dosnt exist
		the_bank->log_file_ << "Error "<< thread_id <<
				": Your transaction failed – account id " << id <<
				" does not exist"<< endl;
	}
	else if (itr->second->get_pass() != pass)
	{//wrong password
		the_bank->log_file_ << "Error "<< thread_id <<
			": Your transaction failed – password for account id " << id <<
			" is incorrect"<< endl;
	}
	else
	{
		int newBalance = itr->second->get_balance() + amount;
		itr->second->change_balance(newBalance);
		the_bank->log_file_ << thread_id <<  ": Account " << id <<
				" new balance is " << itr->second->get_balance() << " after "
				<< amount <<" $ was deposited"<< endl;
	}
}

void ATM::withdraw(bank* the_bank,int id, int pass, int amount)
{
	map<int,account*>::iterator itr	= the_bank->acc_map_.find(id);
	if (itr == the_bank->acc_map_.end() )
	{//account dosnt exist
		the_bank->log_file_ << "Error "<< thread_id <<
				": Your transaction failed – account id " << id <<
				" does not exist"<< endl;
	}
	else if (itr->second->get_pass() != pass)
	{//wrong password
		the_bank->log_file_ << "Error "<< thread_id <<
			": Your transaction failed – password for account id " << id <<
			" is incorrect"<< endl;
	}
	else
	{
		int newBalance = itr->second->get_balance() - amount;
		if (!itr->second->change_balance(newBalance))
		{//not eanogh money
		the_bank->log_file_ <<	"Error "<< thread_id << ": Your transaction failed – account id "
				<< id << " balance is lower than " << amount << endl;
		}
		else
		{//good- give him money
			the_bank->log_file_ << thread_id <<  ": Account " << id <<
				" new balance is " << itr->second->get_balance() << " after "
				<< amount <<" $ was withdraw"<< endl;
		}
	}
}

void ATM::check_balance(bank* the_bank,int id, int pass)
{
	map<int,account*>::iterator itr	= the_bank->acc_map_.find(id);
	if (itr == the_bank->acc_map_.end() )
	{//account dosnt exist
		the_bank->log_file_ << "Error "<< thread_id <<
				": Your transaction failed – account id " << id <<
				" does not exist"<< endl;
	}
	else if (itr->second->get_pass() != pass)
	{//wrong password
		the_bank->log_file_ << "Error "<< thread_id <<
			": Your transaction failed – password for account id " << id <<
			" is incorrect"<< endl;
	}
	else
	{
		the_bank->log_file_ << thread_id <<  ": Account " << id <<
			" balance is " << itr->second->get_balance() << endl;
	}
}

void ATM::move_money(bank* the_bank,int src_id, int pass , int dest_id, int amount)
{
	map<int,account*>::iterator itr_s= the_bank->acc_map_.find(src_id);
	map<int,account*>::iterator itr_d= the_bank->acc_map_.find(dest_id);
	if ( (itr_s == the_bank->acc_map_.end() ))
	{//src account dosnt exist
		the_bank->log_file_ << "Error "<< thread_id <<
				": Your transaction failed – account id " << src_id <<
				" does not exist"<< endl;
	}
	else if (itr_s->second->get_pass() != pass )
	{//src account wrong password
		the_bank->log_file_ << "Error "<< thread_id <<
			": Your transaction failed – password for account id " << src_id <<
			" is incorrect"<< endl;
	}
	else if ( (itr_d == the_bank->acc_map_.end() ))
	{//dest account dosnt exist
		the_bank->log_file_ << "Error "<< thread_id <<
				": Your transaction failed – account id " << dest_id <<
				" does not exist"<< endl;
	}
	else
	{
		int newBalance_s = itr_s->second->get_balance() - amount;
		if (!itr_s->second->change_balance(newBalance_s))
		{//not eanogh money
		the_bank->log_file_ <<	"Error "<< thread_id << ": Your transaction failed – account id "
				<< src_id << " balance is lower than " << amount << endl;
		}
		else
		{//good- transfer him the money
			int newBalance_d = itr_d->second->get_balance() + amount;
			itr_d->second->change_balance(newBalance_d);


			the_bank->log_file_ << thread_id << ": Transfer "<< amount <<" from account " <<
					src_id <<" to account " << dest_id << " new account " <<
					"balance is "<< itr_s->second->get_balance() << " new target account balance is " <<
					itr_d->second->get_balance() << endl;
		}
	}
}

void ATM::atm_run(bank* the_bank, char*  atm_file)
{
	ifstream myfile;
	myfile.open(atm_file);
	string line;
	vector<string> tokens;

	while(getline(myfile,line))
	{
		tokens.clear();
		string args;
		char* temp;
		strcpy(temp, line.c_str());
		char* cmd = strtok(temp," /t");
		while(cmd !=  NULL)
		{
			tokens.push_back(cmd);
			cmd = strtok(temp," /t");
		}
		if (*tokens[0].c_str() == 'O')
		{
			new_acc(the_bank,atoi(tokens[1].c_str()),atoi(tokens[2].c_str()),atoi(tokens[3].c_str()));
		}
		else if (*tokens[0].c_str() == 'L')
		{
			make_vip(the_bank,atoi(tokens[1].c_str()),atoi(tokens[2].c_str()));
		}
		else if (*tokens[0].c_str() == 'D')
		{
			deposit(the_bank,atoi(tokens[1].c_str()),atoi(tokens[2].c_str()),atoi(tokens[3].c_str()));
		}
		else if (*tokens[0].c_str() == 'W')
		{

			withdraw(the_bank,atoi(tokens[1].c_str()),atoi(tokens[2].c_str()),atoi(tokens[3].c_str()));
		}
		else if (*tokens[0].c_str() == 'B')
		{
			check_balance(the_bank,atoi(tokens[1].c_str()),atoi(tokens[2].c_str()));
		}
		else if (*tokens[0].c_str() == 'T')
		{

			move_money(the_bank,atoi(tokens[1].c_str()),atoi(tokens[2].c_str()),
					atoi(tokens[3].c_str()),atoi(tokens[4].c_str()));
		}
		else
		{
			cout <<"input invalid" << endl;
		}
	}
}

