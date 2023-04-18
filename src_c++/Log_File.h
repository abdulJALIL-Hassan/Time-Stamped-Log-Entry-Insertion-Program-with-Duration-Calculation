#include <fstream>
#include <string>
#include "Log_Entry.h"

using namespace std;

class LogFile
{
    private:
        string filePath;

        ifstream fileToRead;
        ofstream fileToWrite;   

        int totalEntries;
        int entryLength;

        LogEntry lastEntry;
    
        void setTotalEnteries();
        void setEntryLength();
        void setLastEntry();
    public:
        LogFile(string);
        ~LogFile();

        void getLastEntry(LogEntry&) const;
        int getTotalEntries() const;
        int getEntryLength() const;
        void writeCurrentEntry(const LogEntry&);

        
};
//--------------------------------------------------------
LogFile::LogFile(string filePath)
{
    this->filePath = filePath;

    fileToRead.open(this->filePath);
    
    setTotalEnteries();
    if(totalEntries == 0)
    {

    }
    else
    {
        setEntryLength();
        setLastEntry();
    }
}

LogFile::~LogFile()
{
    fileToRead.close();
    fileToWrite.close();
}

//----------------------------------------------------------
void LogFile::setTotalEnteries()
{
    fileToRead.seekg(0,ios::beg);
    int totalEntries = 0;
    string buffer;

    while(getline(fileToRead,buffer))
    {
        totalEntries++;
    }
    fileToRead.clear();
    this->totalEntries = totalEntries;    
}

void LogFile::setEntryLength()
{
    fileToRead.seekg(0,ios::beg);
    int entryLength;
    string buffer;

    getline(fileToRead,buffer);
    entryLength = buffer.length();   
   
    this->entryLength = entryLength;
}
void LogFile::setLastEntry()
{
    fileToRead.seekg(-62,ios::end);     //add 2 (1 for new line character and 1 for) 
    string s_lastEntry;
    getline(fileToRead,s_lastEntry);
    lastEntry = LogEntry(s_lastEntry);  //Constructor of class LogEntry to parse string(entry) to LogEntry object
}

//-----------------------------------------------------------------
void LogFile::getLastEntry(LogEntry& lastEntry) const { lastEntry = this->lastEntry;}   //pass by refernce: To prevemt calling copy constructor
int LogFile::getTotalEntries() const {return totalEntries;}
int LogFile::getEntryLength() const {return entryLength;}

void LogFile::writeCurrentEntry(const LogEntry& currentEntry)
{
    //fileToWrite.seekp(-1,ios::end);
    fileToWrite.open(filePath,ios::app);
    fileToWrite<<currentEntry.getFormatedEntry()<<endl;
    //fileToWrite.close();
}

