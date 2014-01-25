#pragma once

class OptionsReaderImpl;

class OptionsReader
{
public:
	OptionsReader();
	~OptionsReader();

	bool ReadOptionsFile(const char* filename);
	bool ReadOptionsStream(std::istream& stream);

	bool WriteOptionsFile(const char* filename);

	int					ReadInt(const char* section, const char* property, int defaultValue = 0xFFFFFFFF) const;
	unsigned int		ReadUInt(const char* section, const char* property, unsigned int defaultValue = 0xFFFFFFFF) const;
	bool				ReadBool(const char* section, const char* property, bool defaultValue = false) const;
	double				ReadDouble(const char* section, const char* property, double defaultValue = DBL_MAX) const;
	float				ReadFloat(const char* section, const char* property, float floatValue = FLT_MAX) const;
	std::string			ReadString(const char* section, const char* property, std::string defaultValue = "") const;
	void				SetFloat(const char* section, const char* property, float value);

private:
	OptionsReaderImpl* m_Impl;
};

