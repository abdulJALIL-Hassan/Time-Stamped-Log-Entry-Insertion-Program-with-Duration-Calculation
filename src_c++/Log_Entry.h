#include <string>
#include <regex>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;
struct timeDifference
{
	int secs;
	int mins;
	int hours;
	int days;
};
class LogEntry
{
	private:
		string formatedEntry;

		string s_index;
		int i_index;
		
		string s_ordinal;
		int i_ordinal;

		string s_datetime;
		time_t i_datetime; // for duration
		tm tm_datetime; //tm: a structure in ctime

		string s_duration;
		long i_duration;
		timeDifference duration;

	public:
		LogEntry();
		 //Default Constructor
		 //i_dateTime is set to current dateTime
		 //i_index is set to 1
		 //i_ordinal is set to 1
		 //i_duration is set to 0;
		 //
		 //For 1st formatedEntry of File
	
		LogEntry(string);	
		 //Constructor with parameter: string = formatedEntry
		 //Parameter provide formatedEntry data member
		 //i_dateTime is set as in formatedEntry(Parameter)
		 //i_index is set as in parameter
		 //i_ordinal is set as in parameter
		 //i_duration is set as in parameter
		 //
		 //To Parse formated string to LogEntry Object
		 //Intended to use it to parse lastEntry of LogFile to LogEntry
	 
		explicit LogEntry(const LogEntry&);
		 //Constructor with parameter: LogEntry& = prevEntry
		 //i_dateTime is set to current Date Time
		 //i_index is set to prevEntry.i_index + 1
		 //i_ordinal is set accordingly
		 //i_duration is set as i_dateTime - prevEntry.i_dateTime
		 //
		 //Constructor to create next LogEntry object using the prvEntry
	
		LogEntry(const LogEntry&, string);
		 //Constructor with parameter: LogEntry& = prevEntry, string = s_dateTime
		 //i_dateTime is set as in datTime string(Parameter)
		 //i_index is set as prevEntry.i_index + 1
		 //i_ordinal is set accordingly
		 //i_duration is set as i_dateTime - prevEntry.i_dateTime
		 //
		 //Consructor to create LogEntry object according to provided datetime string
		
		string getFormatedEntry() const;
		string getDuration_s() const;
		string getDateTime_s() const;
		string getOrdinal_s() const;
		string getIndex_s() const;
		time_t getDateTime_t()	const;


	
	private:
    	void setIndex_s();	//Extract s_index from formatedEntry
		void setOrdinal_s();	//Extract s_ordinal from formatedEntry
		void setDateTime_s();	//Extract s_dateTime from formatedEntry
		void setDuration_s();	//Extract s_duration from formatedEntry
		
		timeDifference long_to_timeDiff(long);		//Parse i_duration to tm structure
		timeDifference string_to_timeDiff();	//Parse s_duration to timeDifference object
		string getOridnalPostfix(int);	//set the ordinal postfix according to i_ordinal 
		string patternMatch(string , string);	//match and return the match string according to pattern provided in
											//1st argument and targetSequence in 2nd argument  
	
};

//----------------------------------------------------------------Constructors
LogEntry::LogEntry()
{
	//cout<<"Constructor 1 called\n";
	this->i_index = 1;
	s_index = to_string(i_index);

	this->i_datetime = time(nullptr);
	tm_datetime = *localtime(&i_datetime);
	ostringstream oss;
   	oss << put_time(&tm_datetime, "%d-%m-%Y %H:%M:%S");
   	this->s_datetime = oss.str();

	i_ordinal = 1;	
	this->s_ordinal = to_string(i_ordinal) + getOridnalPostfix(i_ordinal);

	i_duration = 000000;
	duration = long_to_timeDiff(i_duration);
	
	char timeBuffer[26];	//25 + 1(for null ternminator)
	sprintf(timeBuffer, "%02iDays:%02iHrs:%02iMins:%02iSec", duration.days, duration.hours, duration.mins , duration.secs);
	s_duration = string(timeBuffer);
	
	char buffer[61];	//60 + 1(for null ternminator)
	sprintf(buffer, "|%4s|%4s|%19s|  |%25s|",s_index.c_str(),s_ordinal.c_str(),s_datetime.c_str(),s_duration.c_str());
    formatedEntry = string(buffer);
}

LogEntry::LogEntry(string formatedEntry)
{
	//cout<<"Constructor 2 called\n";
    this->formatedEntry = formatedEntry;

	setIndex_s();
	i_index= stoi(s_index);

	setOrdinal_s();
	i_ordinal = stoi(s_ordinal);

	setDateTime_s();
	istringstream temp(s_datetime);
	temp >> get_time(&tm_datetime, "%d-%m-%Y %H:%M:%S");
	i_datetime = mktime(&tm_datetime);

	duration = {};
	setDuration_s();
	duration = string_to_timeDiff();
	
}

LogEntry::LogEntry(const LogEntry& prev)
{
	//cout<<"Constructor 3 called\n";
	i_index = prev.i_index+1;
	s_index = to_string(i_index);

	i_datetime = time(nullptr);
	tm_datetime = *localtime(&i_datetime);
	ostringstream oss;
   	oss << put_time(&tm_datetime, "%d-%m-%Y %H:%M:%S");
   	s_datetime = oss.str();

	if (tm_datetime.tm_mon == prev.tm_datetime.tm_mon)
		i_ordinal = prev.i_ordinal+1;
	else
		i_ordinal = 1;
	
	s_ordinal = to_string(i_ordinal) + getOridnalPostfix(i_ordinal);

	i_duration = difftime(i_datetime,prev.i_datetime);
	duration = long_to_timeDiff(i_duration);
	
	char timeBuffer[26];	//25 + 1(for null ternminator)
	sprintf(timeBuffer, "%02iDays:%02iHrs:%02iMins:%02iSec", duration.days, duration.hours, duration.mins , duration.secs);
	s_duration = string(timeBuffer);
	
	char buffer[61];	//60 + 1(for null ternminator)
	sprintf(buffer, "|%4s|%4s|%19s|  |%25s|",s_index.c_str(),s_ordinal.c_str(),s_datetime.c_str(),s_duration.c_str());
    formatedEntry = string(buffer);
}

LogEntry::LogEntry(const LogEntry& prev, string s_datetime)
{
	//cout<<"Constructor 4 called\n";
	i_index = prev.i_index+1;
	s_index = to_string(i_index);

	istringstream temp(s_datetime);
	temp >> get_time(&tm_datetime, "%d-%m-%Y %H:%M:%S");
	i_datetime = mktime(&tm_datetime);

	if (tm_datetime.tm_mon == prev.tm_datetime.tm_mon)
		i_ordinal = prev.i_ordinal+1;
	else
		i_ordinal = 1;
		
	s_ordinal = to_string(i_ordinal) + getOridnalPostfix(i_ordinal);

	i_duration = difftime(i_datetime,prev.i_datetime);	//Correct if diff is less than 1 month/ 31 days
	duration = long_to_timeDiff(i_duration);
	
	char timeBuffer[26];	//25 + 1(for null ternminator)
	sprintf(timeBuffer, "%02iDays:%02iHrs:%02iMins:%02iSec", duration.days, duration.hours, duration.mins , duration.secs);
	s_duration = string(timeBuffer);
	
	char buffer[61];	//60 + 1(for null ternminator)
	sprintf(buffer, "|%4s|%4s|%19s|  |%25s|",s_index.c_str(),s_ordinal.c_str(),s_datetime.c_str(),s_duration.c_str());
    formatedEntry = string(buffer);
}

//-------------------------------------------------------------------Getter


string LogEntry::getFormatedEntry() const {return formatedEntry;}
string LogEntry::getDuration_s() const {return s_duration;}
string LogEntry::getDateTime_s() const {return s_datetime;}
string LogEntry::getOrdinal_s() const {return s_ordinal;}
string LogEntry::getIndex_s() const {return s_index;}
time_t LogEntry::getDateTime_t() const {return i_datetime;}


//-------------------------------------------------------------------Setter
void LogEntry::setIndex_s()
{
	string indexWhole, s_ind;
	indexWhole = patternMatch(R"(\|\s{0,3}\d{1,4}\|)",this->formatedEntry);
	s_ind = patternMatch(R"(\d{1,4})",indexWhole);

	this->s_index = s_ind;
}

void LogEntry::setOrdinal_s()
{
	string ordinalWhole, s_ord;
	ordinalWhole = patternMatch(R"(\|\s{0,1}\d{1,2}\w{2}\|)",this->formatedEntry);
	s_ord = patternMatch(R"(\d{1,2})",ordinalWhole);
	this->s_ordinal = s_ord;
}

void LogEntry::setDateTime_s()
{
	string s_dt;
	s_dt = patternMatch(R"(\d{2}-\d{2}-\d{4}\s\d{2}:\d{2}:\d{2})",this->formatedEntry);
	this->s_datetime = s_dt;
}

void LogEntry::setDuration_s()
{
	string s_du;
	s_du = patternMatch(R"((?:\d{2}Days:\d{2}Hrs:\d{2}Mins:\d{2}Sec))",this->formatedEntry);
	this->s_duration = s_du;
}


//---------------------------------------------------------------Other function
timeDifference LogEntry::long_to_timeDiff(long diff_seconds)
{
	timeDifference diff_time = {};
	diff_time.secs = diff_seconds % 60;
	diff_time.mins = (diff_seconds / 60) % 60;
	diff_time.hours = (diff_seconds / 3600) % 24;
	diff_time.days = (diff_seconds / (3600 * 24));
	
	return diff_time;
}

timeDifference LogEntry::string_to_timeDiff()
{
	timeDifference temp;
	regex pattern(R"((\d{2})Days:(\d{2})Hrs:(\d{2})Mins:(\d{2})Sec)");
	smatch match;
	regex_search(s_duration,match,pattern);

	temp.days = stoi(match[1]);
	temp.hours = stoi(match[2]);
	temp.mins = stoi(match[3]);
	temp.secs = stoi(match[4]);

	return temp;
}

string LogEntry::getOridnalPostfix(int ordinal)
{
	string ordinalPostfix[] = {"st","nd","rd","th"};
	if((ordinal%10==1&&ordinal<10)||(ordinal%10==1&&ordinal>20))
        return ordinalPostfix[0];
    else if((ordinal%10==2&&ordinal<10)||(ordinal%10==2&&ordinal>20))
        return ordinalPostfix[1];
    else if((ordinal%10==3&&ordinal<10)||(ordinal%10==3&&ordinal>20))
        return ordinalPostfix[2];
    else
        return ordinalPostfix[3];
}

string LogEntry::patternMatch(string pattern, string targSeq)
{
	regex pattern_(pattern);
	smatch match;
	if(regex_search(targSeq,match,pattern_))
		return match[0];

	return "";
}