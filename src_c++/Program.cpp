#include <iostream>
#include <conio.h>
#include "Log_File.h"
using namespace std;

string getDateTimeString(const LogEntry &lastEntry)
{	
	string provided_datetime_s;
	time_t provided_datetime_t;
	tm provided_datetime_tm;
	regex dateTimePattern(R"(^(0?[1-9]|[12][0-9]|3[01])-(0?[1-9]|1[012])-(\d{4})\s([01][0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])$)");
	do
	{
		do
		{
			cout<<"\nEnter dateTime DD-MM-YYYY HH:MM:SS:\n";
			getline(cin,provided_datetime_s);
		
		}while(!regex_match(provided_datetime_s,dateTimePattern));


		istringstream tem(provided_datetime_s);
		tem	>> get_time(&provided_datetime_tm, "%d-%m-%Y %H:%M:%S");
		provided_datetime_t = mktime(&provided_datetime_tm);	
		//cout<<difftime(provided_datetime_t,lastEntry.getDateTime_t())<<endl;
	
	}while(difftime(provided_datetime_t,lastEntry.getDateTime_t()) < 0);
	
	return provided_datetime_s;
}
int main()
{
	LogFile file("..//Log.txt");
	LogEntry currentEntry;
	if(file.getTotalEntries() == 0)
	{}
	else
	{
		LogEntry lastEntry;
		file.getLastEntry(lastEntry);
		char choice;
		cout<<"For Entry having dateTime = Current -------- Press 1\nFor Entry having dateTime = Entered -------- Press 2\n";
		choice = getche();
		if((int)choice == 49)
			currentEntry = LogEntry(lastEntry);
		else if((int)choice == 50)
			currentEntry = LogEntry(lastEntry,getDateTimeString(lastEntry));
	}

	file.writeCurrentEntry(currentEntry);

	
	return 0;
}
