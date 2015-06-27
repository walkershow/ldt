//

//IniFile.cpp

//


#include "cini.h"
#include <fstream>
#include <ctype.h>
#include <sstream>
#include <iostream>


const std::string IniFile::GROUP_DEFAULT("default");
IniFile::IniFile() : modifyed(false)
{
}

IniFile::~IniFile()
{
	if (modifyed)
	{
		save();
	}
}

void IniFile::SetFile(std::string filename)
{
	pathname = filename;
}

void IniFile::trimString(std::string &str)
{
	int i = 0;
	for(; i < (int)str.length() && isspace(str[i]); i++);

	str = str.substr(i);

	for(i = (int)str.length() - 1; i >= 0 && isspace(str[i]); i--);
	str.erase(i + 1);

}

bool IniFile::load(int &errLine)
{
	std::locale::global(std::locale(""));
	std::ifstream inf(pathname.c_str());
	inf.is_open();
	long size = inf.tellg();
	std::string value;
	paragraph.clear();
	errLine = 0;
	std::string currentGroup = GROUP_DEFAULT;
	Pairs pairs;
	while(std::getline(inf, value))
	{
		errLine++;
		trimString(value);
		if(value[0] == '#')
			continue;

		if(value.length() == 0)
			continue;
		std::string::size_type idx = value.find("=");
		if(idx == std::string::npos)
		{
			if(value[0] == '[' && value[value.length() - 1] == ']')
			{
				value.erase(value.length() - 1);
				value.erase(0, 1);

				paragraph[currentGroup] = pairs;
				currentGroup = value;
				pairs.clear();
			} else
				return false;
		}
		else
		{
			std::string key = value.substr(0, idx);
			trimString(key);

			std::string val, tmpval = value.substr(idx + 1);
			std::string::size_type idx1 = tmpval.find("#"); //'#' for comment

			if (idx1 != std::string::npos)
			{
				val = tmpval.substr(0, idx1);
			}
			else
			{
				val = tmpval;
			}
			trimString(val);
			pairs[key] = val;
		}
	}

	paragraph[currentGroup] = pairs;
	std::locale::global(std::locale("C"));
	return true;
} 

bool IniFile::ReadBoolean(std::string selection, std::string key, bool defvalue)
{
	bool val = defvalue;
	std::string value = get(selection, key);
	if (value.length()>0)
	{
		if (value == std::string("true"))
		{
			val = true;
		}
		else
		{
			val = false;
		}
	}
	return val;
}

void IniFile::WriteBoolean(std::string selection, std::string key, bool value)
{
	std::string str_value = (value ? "true" : "false");
	WriteString(selection, key, str_value);
}

double IniFile::ReadDouble(std::string selection, std::string key, double defvalue)
{
	double val = defvalue;
	std::string value = get(selection, key);
	if (value.length()>0)
	{
		std::istringstream in(value);
		in >> val; 
	}
	return val;
}

void IniFile::WriteDouble(std::string selection, std::string key, double value)
{
	std::ostringstream os;
	os << value;

	WriteString(selection, key, os.str());
}

int IniFile::ReadInteger(std::string selection, std::string key, int defvalue)
{
	int val = defvalue;
	std::string value = get(selection, key);
	if (value.length()>0)
	{
		std::istringstream in(value);
		in >> val; 
	}
	return val;
}

void IniFile::WriteInteger(std::string selection, std::string key, int value)
{
	std::ostringstream os;
	os << value;
	WriteString(selection, key, os.str());
}

std::string IniFile::ReadString(std::string selection, std::string key, std::string defvalue)
{
	std::string value = get(selection, key);
	return (value.length()>0 ? value : defvalue);
}

void IniFile::WriteString(std::string selection, std::string key, std::string value)
{
	set(selection, key, value);
}
void IniFile::dump(std::ostream &os)
{
	//dump DEFAULT_GROUP group first

	Pairs pairs = paragraph[GROUP_DEFAULT];
	Pairs::iterator p = pairs.begin();
	for(; p != pairs.end(); ++p)
	{
		os << p->first << '=' << p->second << std::endl;
	}
	std::map<std::string, Pairs, std::greater<std::string> >::iterator pos = paragraph.begin();

	for(; pos != paragraph.end(); ++pos)
	{
		pairs = pos->second;
		if(pos->first == GROUP_DEFAULT)
			continue;
		os << '[' << pos->first << ']' << std::endl;
		p = pairs.begin();
		for(; p != pairs.end(); ++p)
		{
			os << p->first << '=' << p->second << std::endl;
		}
	}
}

void IniFile::save()
{
	std::ofstream out(pathname.c_str());
	dump(out);
}

std::string IniFile::get(std::string selection, std::string key)
{
	std::string val, group = selection;
	std::map<std::string, Pairs, std::greater<std::string> >::iterator pos = paragraph.find(group);
	if(pos != paragraph.end())
	{
		Pairs pairs = pos->second;
		Pairs::iterator p = pairs.find(key);
		if(p != pairs.end())
			val = p->second;
	}
	return val;
}

void IniFile::set(std::string selection, std::string key, std::string value)
{
	modifyed = true;
	std::string group = selection;
	Pairs &pairs = paragraph[group];
	pairs[key] = value;
}