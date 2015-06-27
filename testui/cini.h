//

//IniFile.h

//


#ifndef __INI_FILE_H__
#define __INI_FILE_H__

#include <map>
#include <string>
#include <iostream>

class IniFile
{
public:
	IniFile();
	~IniFile();

	void SetFile(std::string filename);
	int ReadInteger(std::string selection, std::string key, int defvalue = 0);
	void WriteInteger(std::string selection, std::string key, int value);
	std::string ReadString(std::string selection, std::string key, std::string defvalue = "");
	void WriteString(std::string selection, std::string key, std::string value);
	double ReadDouble(std::string selection, std::string key, double defvalue = 0.0f);
	void WriteDouble(std::string selection, std::string key, double value);
	bool ReadBoolean(std::string selection, std::string key, bool defvalue = false);
	void WriteBoolean(std::string selection, std::string key, bool value);
public:

	bool load(int &errLine);
	void dump(std::ostream &);
	void save();
protected:
	static const std::string GROUP_DEFAULT;
	static void trimString(std::string &str);
	std::string get(std::string selection, std::string key);
	void set(std::string selection, std::string key, std::string value);
private:
	std::string pathname;
	typedef std::map<std::string, std::string, std::greater<std::string> > Pairs;
	Pairs pairs;
	std::map<std::string, Pairs, std::greater<std::string> > paragraph;
private:
	bool modifyed;
};

#endif