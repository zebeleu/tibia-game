#include "common.hh"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace
{

	struct TranslationEntry
	{
		const char *Key;
		const char *Value;
	};

	struct LanguageDefinition
	{
		const char *Language;
		const TranslationEntry *Entries;
		size_t EntryCount;
	};

	const TranslationEntry ENTranslations[] = {
		{"TWO_OBJECTS_ON_ARRAY", "Two %s objects (%d and %d) on array [%d,%d,%d]."},
	};

	const TranslationEntry DETranslations[] = {
		{"TWO_OBJECTS_ON_ARRAY", "Zwei %s-Objekte (%d und %d) auf Feld [%d,%d,%d]."},
	};

	const LanguageDefinition AllLanguages[] = {
		{"en", ENTranslations, NARRAY(ENTranslations)},
		{"de", DETranslations, NARRAY(DETranslations)},
	};

	std::unordered_map<std::string, std::string> TranslationTable;
	std::unordered_set<std::string> MissingKeys;
	std::string CurrentLanguage;

	const LanguageDefinition *FindLanguage(const std::string &Language)
	{
		for (size_t Index = 0; Index < NARRAY(AllLanguages); Index += 1)
		{
			const LanguageDefinition &Definition = AllLanguages[Index];
			if (Language == Definition.Language)
			{
				return &Definition;
			}
		}

		return NULL;
	}

	void LoadLanguage(const LanguageDefinition &Definition)
	{
		std::unordered_map<std::string, std::string> Table;
		Table.reserve(Definition.EntryCount);

		for (size_t Index = 0; Index < Definition.EntryCount; Index += 1)
		{
			const TranslationEntry &Entry = Definition.Entries[Index];
			if (Entry.Key == NULL || Entry.Key[0] == 0)
			{
				continue;
			}

			if (Entry.Value == NULL)
			{
				continue;
			}

			Table.emplace(Entry.Key, Entry.Value);
		}

		TranslationTable.swap(Table);
		MissingKeys.clear();
	}

	const char *LookupFormat(const char *Key)
	{
		if (Key == NULL || Key[0] == 0)
		{
			return "";
		}

		auto It = TranslationTable.find(Key);
		if (It != TranslationTable.end())
		{
			return It->second.c_str();
		}

		if (MissingKeys.insert(Key).second)
		{
			if (CurrentLanguage.empty())
			{
				error("t: Translation key %s not found.\n", Key);
			}
			else
			{
				error("t: Translation key %s not found (language %s).\n",
					  Key, CurrentLanguage.c_str());
			}
		}

		return Key;
	}

	const char *Format(const char *FormatString, va_list Args)
	{
		static thread_local std::vector<char> Buffer;
		if (Buffer.empty())
		{
			Buffer.resize(256);
		}

		while (true)
		{
			va_list ArgsCopy;
			va_copy(ArgsCopy, Args);
			int Written = vsnprintf(Buffer.data(), Buffer.size(), FormatString, ArgsCopy);
			va_end(ArgsCopy);

			if (Written < 0)
			{
				Buffer[0] = 0;
				break;
			}

			if (static_cast<size_t>(Written) < Buffer.size())
			{
				break;
			}

			Buffer.resize(static_cast<size_t>(Written) + 1);
		}

		return Buffer.data();
	}

}

void InitTranslations(const char *Language)
{
	std::string RequestedLanguage;
	if (Language != NULL && Language[0] != 0)
	{
		RequestedLanguage = Language;
	}
	else
	{
		RequestedLanguage = "de";
	}

	const LanguageDefinition *Definition = FindLanguage(RequestedLanguage);
	if (Definition != NULL)
	{
		LoadLanguage(*Definition);
		CurrentLanguage = Definition->Language;
		print(1, "InitTranslations: Using internal translations for %s.\n",
			  CurrentLanguage.c_str());
		return;
	}

	const LanguageDefinition *Fallback = FindLanguage("en");
	if (Fallback != NULL)
	{
		LoadLanguage(*Fallback);
		CurrentLanguage = Fallback->Language;
		if (RequestedLanguage != CurrentLanguage)
		{
			error("InitTranslations: No translations for %s, use default language %s.\n",
				  RequestedLanguage.c_str(), CurrentLanguage.c_str());
		}
		return;
	}

	TranslationTable.clear();
	MissingKeys.clear();
	CurrentLanguage.clear();
	error("InitTranslations: No built-in translations available.\n");
}

void ExitTranslations(void)
{
	TranslationTable.clear();
	MissingKeys.clear();
	CurrentLanguage.clear();
}

const char *t(const char *Key, ...)
{
	const char *FormatString = LookupFormat(Key);

	va_list Args;
	va_start(Args, Key);
	const char *Result = Format(FormatString, Args);
	va_end(Args);

	return Result;
}