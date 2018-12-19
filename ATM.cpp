/*
 * ATM.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: compm
 */

#include "ATM.h"
#include <cstring>
ATM::ATM() :
		thread_id(0), atm_thread(0) {
}

pthread_t* ATM::get_pthread() {
	return &atm_thread;
}

void ATM::atm_run(bank_accounts* accounts_map, string atm_file) {
	ifstream myfile;
	myfile.open(atm_file.c_str(), std::ifstream::in);
	string line;
	vector < string > tokens;
	while (getline(myfile, line)) {
		sleep(3); //DEBUG fix to 0.1
		stringstream ss(line);
		while (getline(ss, line, ' ')) {
			tokens.push_back(line);
		}
		if (*tokens[0].c_str() == 'O') {
			accounts_map->new_acc(thread_id, tokens[1], tokens[2], atoi(tokens[3].c_str()));
		} else if (*tokens[0].c_str() == 'L') {
			accounts_map->make_vip(thread_id, tokens[1],tokens[2]);
		} else if (*tokens[0].c_str() == 'D') {
			accounts_map->deposit(thread_id, tokens[1],tokens[2], atoi(tokens[3].c_str()));
		} else if (*tokens[0].c_str() == 'W') {
			accounts_map->withdraw(thread_id,tokens[1], tokens[2], atoi(tokens[3].c_str()));
		} else if (*tokens[0].c_str() == 'B') {
			accounts_map->check_balance(thread_id,tokens[1], tokens[2]);
		} else if (*tokens[0].c_str() == 'T') {
			accounts_map->move_money(thread_id,tokens[1], tokens[2], tokens[3],atoi(tokens[4].c_str()));
		} else {
			cout << "input invalid" << endl;
		}
		tokens.clear();
	}
}

