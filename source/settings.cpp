#include "ra_pch.h"
#include "settings.h"
#include "OptionsReader.h"

#define SETTINGS_FILENAME "settings.ini"

#define LOAD_PROPERTIES(CType, InternalType, MemberName, DefaultValue, IniProperty, GetterName) \
	MemberName = (CType)optionsReader->Read##InternalType("General", IniProperty, DefaultValue);


bool Settings::LoadSettings(int argc, char* argv[])
{
	std::unique_ptr<OptionsReader> optionsReader(new OptionsReader);

	bool readOk = optionsReader->ReadOptionsFile(SETTINGS_FILENAME);
	if (!readOk)
	{
		printf("ERROR: Cannot read settings from \"%s\"!\n", SETTINGS_FILENAME);
		return false;
	}

	PROPERTIES_ITERATOR(LOAD_PROPERTIES);

	return true;
}


