#include "ra_pch.h"
#include "OptionsReader.h"
#include <fstream>
#include <map>

static inline std::string trim_right_copy(const std::string& s, const std::string& delimiters = " \f\n\r\t\v")
{
	return s.substr(0, s.find_last_not_of(delimiters) + 1);
}

static inline std::string trim_left_copy(const std::string& s, const std::string& delimiters = " \f\n\r\t\v")
{
	return s.substr(s.find_first_not_of(delimiters));
}

static inline std::string trim_copy(const std::string& s, const std::string& delimiters = " \f\n\r\t\v")
{
	return trim_left_copy(trim_right_copy(s, delimiters), delimiters);
}

class OptionsReaderImpl
{
private:
	class Section
	{
		std::string m_Name;
		typedef std::map<std::string, std::string> PropertyMap;
		PropertyMap m_PropertyMap;
	public:

		Section(const std::string& name): m_Name(name) {}

		int ReadInt(const char* property, int defaultValue) const
		{
			auto it = m_PropertyMap.find(property);

			return (it == m_PropertyMap.end() ? defaultValue : atoi(it->second.c_str()));
		}

		double ReadDouble(const char* property, double defaultValue) const
		{
			auto it = m_PropertyMap.find(property);

			return (it == m_PropertyMap.end() ? defaultValue : atof(it->second.c_str()));
		}

		std::string ReadString(const char* property, std::string defaultValue) const
		{
			auto it = m_PropertyMap.find(property);

			return (it == m_PropertyMap.end() ? defaultValue : it->second);
		}

		void Insert(const std::string& key, const std::string& value)
		{
			m_PropertyMap.insert(PropertyMap::value_type(key, value));
		}

		void SetFloat(const char* property, float value)
		{
			auto it = m_PropertyMap.find(property);

			if(it != m_PropertyMap.end())
			{
				char buff[128] = { 0 };
				sprintf_s(buff, "%f", value);
				(*it).second = buff;
			}
		}

		bool Write(std::ostream& os)
		{
			os << "[" << m_Name << "]" << std::endl;
			for(auto it = m_PropertyMap.begin(); it != m_PropertyMap.end(); it++)
			{
				os << (*it).first << "=" << (*it).second << std::endl; 
			}

			return true;
		}
	};

public:
	bool ReadOptionsStream(std::istream& stream)
	{
		if (!stream.good())
		{
			std::cout << "WARNING: Unable to read options stream" << std::endl;
			return false;
		}

		Section* curSec = nullptr;
		std::string line;
		while (std::getline(stream, line))
		{

			if (line.length() == 0)
			{
				continue;
			}

			if (line.at(0) == ';')
			{
				continue;
			}

			if (line.at(0) == '[')
			{
				//parese section name
				size_t posSecEnd = line.find(']');
				if (posSecEnd == std::string::npos)
				{
					continue;
				}

				std::string secName = line.substr(1, posSecEnd-1);
				if (secName.empty())
				{
					continue;
				}

				//open new section
				curSec = new Section(secName);
				if (!curSec)
				{
					std::cout << "ERROR: out of memory" << std::endl;
					return false;
				}

				m_SectionMap.insert(SectionMap::value_type(secName, curSec));
				continue;
			}

			size_t posEqual = line.find('=');
			if (posEqual == std::string::npos)
			{
				continue;
			}

			std::string key = trim_copy(line.substr(0, posEqual));
			if (key.empty() || (line.length() <= posEqual + 1))
			{
				continue;
			}

			std::string value = trim_copy(line.substr(posEqual + 1));
			if (value.empty())
			{
				continue;
			}

			assert(curSec);
			if(curSec)
			{
				curSec->Insert(key, value);
			}
		}

		return true;
	}

	bool ReadOptionsFile(const char* filename)
	{
		std::ifstream ifs(filename);

		if (!ifs.good())
		{	
			std::cout << "WARNING: Unable to read options file: " << filename << std::endl;
			return false;
		}

		return ReadOptionsStream(ifs);
	}

	bool WriteOptionsFile(const char* filename)
	{
		std::ofstream ofs(filename);

		if (!ofs.good())
		{	
			std::cout << "WARNING: Unable to write options file: " << filename << std::endl;
			return false;
		}

		for(auto it = m_SectionMap.begin(); it != m_SectionMap.end(); it++)
		{
			(*it).second->Write(ofs);
		}

		return true;
	}

	int ReadInt(const char* section, const char* property, int defaultValue) const
	{
		auto it = m_SectionMap.find(section);
		return (it == m_SectionMap.end() || !it->second ? defaultValue : it->second->ReadInt(property, defaultValue));
	}

	double ReadDouble(const char* section, const char* property, double defaultValue) const
	{
		auto it = m_SectionMap.find(section);
		return (it == m_SectionMap.end() || !it->second ? defaultValue : it->second->ReadDouble(property, defaultValue));
	}

	std::string ReadString(const char* section, const char* property, std::string defaultValue) const
	{
		auto it = m_SectionMap.find(section);
		return (it == m_SectionMap.end() || !it->second ? defaultValue : it->second->ReadString(property, defaultValue));
	}


	void SetFloat(const char* section, const char* property, float value)
	{
		auto it = m_SectionMap.find(section);
		if(it != m_SectionMap.end())
		{
			(*it).second->SetFloat(property, value);
		}
	}


private:

	typedef std::map<std::string, OptionsReaderImpl::Section*> SectionMap;
	SectionMap m_SectionMap;
};

//--------------------------------------------------------------------------------------

OptionsReader::OptionsReader()
	: m_Impl(new OptionsReaderImpl)
{
}

OptionsReader::~OptionsReader()
{
	delete m_Impl;
}

bool OptionsReader::ReadOptionsFile(const char* filename)
{
	return m_Impl->ReadOptionsFile(filename);
}

bool OptionsReader::ReadOptionsStream(std::istream& stream)
{
	return m_Impl->ReadOptionsStream(stream);
}

bool OptionsReader::WriteOptionsFile(const char* filename)
{
	return m_Impl->WriteOptionsFile(filename);
}

int	OptionsReader::ReadInt(const char* section, const char* property, int defaultValue) const
{
	return m_Impl->ReadInt(section, property, defaultValue);
}

unsigned int OptionsReader::ReadUInt(const char* section, const char* property, unsigned int defaultValue) const
{
	return static_cast<unsigned int>(m_Impl->ReadInt(section, property, *reinterpret_cast<int*>(&defaultValue)));
}

bool OptionsReader::ReadBool(const char* section, const char* property, bool defaultValue) const
{
	int defaultInt = defaultValue ? 1 : 0;
	return m_Impl->ReadInt(section, property, defaultInt) ? true : false;
}

double OptionsReader::ReadDouble(const char* section, const char* property, double defaultValue) const
{
	return m_Impl->ReadDouble(section, property, defaultValue);
}

float OptionsReader::ReadFloat(const char* section, const char* property, float floatValue) const
{
	return static_cast<float>(m_Impl->ReadDouble(section, property, floatValue));
}

std::string OptionsReader::ReadString(const char* section, const char* property, std::string defaultValue) const
{
	return m_Impl->ReadString(section, property, defaultValue);
}

void OptionsReader::SetFloat(const char* section, const char* property, float value)
{
	m_Impl->SetFloat(section, property, value);
}
