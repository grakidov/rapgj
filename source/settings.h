#pragma once

#define PROPERTIES_ITERATOR(MACRO) \
	MACRO(std::string, String, m_SceneFileName,     "",      "Scene",             SceneFileName) \
	MACRO(int,         Int,    m_Width,             1280,    "Width",             WindowWidth) \
	MACRO(int,         Int,    m_Height,            720,     "Height",            WindowHeight)	\
	MACRO(int,         Int,    m_Multisample,       0,       "Multisample",       Multisample) \

#define DEFINE_GETTERS(CType, InternalType, MemberName, DefaultValue, IniProperty, GetterName) \
	const CType& Get##GetterName() const \
	{ \
		return MemberName; \
	} \

#define DECLARE_MEMBERS(CType, InternalType, MemberName, DefaultValue, IniProperty, GetterName) \
	CType MemberName;

class Settings
{
public:
	bool LoadSettings(int argc, char* argv[]);
	
	PROPERTIES_ITERATOR(DEFINE_GETTERS);

private:

	PROPERTIES_ITERATOR(DECLARE_MEMBERS);
};

