#This is a Makefile for the Date example 
CCC = g++ 
CXXFLAGS = -Wall -g 
CXXLINK = $(CCC) -pthread
OBJS = account.o bank_accounts.o ATM.o bank.o main.o    
RM = rm -f 
#Default target (usually "all") 
all: ATMS 
#Creating the executables
ATMS: $(OBJS) 
	$(CXXLINK) -o ATMS $(OBJS) 
#Creating object files using default rules 
main.o: main.cpp bank.h ATM.h bank_accounts.h account.h 
account.o: account.cpp account.h
bank_accounts.o: bank_accounts.cpp bank_accounts.h account.h
ATM.o: ATM.cpp ATM.h account.h bank_accounts.h bank.h
bank.o: bank.cpp bank.h ATM.h account.h bank_accounts.h
#Cleaning old files before new make 
clean:
	$(RM) ATMS *.o *.bak *~ "#"* core
