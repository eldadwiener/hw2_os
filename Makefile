#This is a Makefile for the Date example 
CCC = g++ 
CXXFLAGS = -Wall -g 
CXXLINK = $(CCC) -pthread
OBJS = account.o ATM.o bank.o main.o 
RM = rm -f 
#Default target (usually "all") 
all: ATMS 
#Creating the executables
ATMS: $(OBJS) 
	$(CXXLINK) -o example $(OBJS) 
#Creating object files using default rules 
account.o: account.cpp account.h
ATM.o: ATM.cpp ATM.h account.h bank.h
bank.o: bank.cpp bank.h ATM.h account.h
main.o: main.cpp bank.h ATM.h account.h 
#Cleaning old files before new make 
clean:
	$(RM) ATMS *.o *.bak *~ "#"* core
