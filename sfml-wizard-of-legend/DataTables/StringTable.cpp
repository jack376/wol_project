#include "stdafx.h"
#include "StringTable.h"
#include "rapidcsv.h"

const std::string& StringTable::Get(const std::string& id, Languages lang) const
{
	auto& table = tables[(int)lang];
	auto find = table.find(id);
	if (find == table.end())
	{
		return "ERR: Undefined ID";
	}
	return find->second;
}


bool StringTable::Load()
{
	std::vector<std::string> filenames;
	filenames.push_back("tables/StringTable_KOR.csv");
	filenames.push_back("tables/StringTable_ENG.csv");
	filenames.push_back("tables/StringTable_JP.csv");

	for (int i = 0; i < tables.size(); ++i)
	{
		rapidcsv::Document doc(filenames[i]);
		std::vector<std::string> ids = doc.GetColumn<std::string>(0);
		std::vector<std::string> values = doc.GetColumn<std::string>(1);

		for (int j = 0; j < ids.size(); ++j)
		{
			//std::cout << ids[i] << " " << values[i] << std::endl;
			tables[i].insert({ ids[j], values[j] });
		}
	}

	return true;
}

void StringTable::Release()
{
	for (auto table : tables)
	{
		table.clear();
	}
	//tables.clear();
}

std::wstring multibyte_to_uni(const std::string& str) {
	int nLen = str.size();
	wchar_t warr[256];
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, warr, nLen);

	return warr;
	//char carr[256];
	//memset(carr, '\0', sizeof(carr));
	//WideCharToMultiByte(CP_UTF8, 0, warr, -1, carr, 256, NULL, NULL);
	//return carr;
}

const std::wstring StringTable::GetUni(const std::string& id, Languages lang)
{
	std::string multibyteString = Get(id, lang);
	return multibyte_to_uni(multibyteString);
}