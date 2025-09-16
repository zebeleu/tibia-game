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
		{"BUFFER_IS_NULL", "Buffer is NULL."},
		{"LIBRARY_DOES_NOT_SUPPORT_ITS_OWN_STACKS", "Library does not support its own stacks."},
		{"NOT_ALL_STACKS_RELEASED", "Not all stacks released."},
		{"MAXIMUM_STACK_USAGE", "Maximum stack usage: %d..%d"},
		{"LAG_DETECTED", "Lag detected!"},
		{"ERROR_SENDING_TO_SOCKET", "Error %d sending to socket %d."},
		{"CONNECTION_ON_SOCKET_FAILED", "Connection on socket %d failed."},
		{"INVALID_MESSAGE_TYPE", "Invalid message type %d."},
		{"MESSAGE_IS_NULL", "Message is NULL."},
		{"INVALID_WAITING_TIME", "Invalid waiting time %d."},
		{"MESSAGE_TOO_LONG", "Message too long (%s)."},		
		{"CONNECTION_IS_NULL", "Connection is NULL."},
		{"ERROR_FILLING_BUFFER", "Error filling buffer (%s)"},
		{"ADDING_TO_THE_WAITING_LIST", "Adding %s to the waiting list."},
		{"ORDER_BUFFER_ALMOST_FULL", "Order buffer is almost full."},
		{"NO_FREE_ACC_ALLOWED_AFTER_MASS_KICK", "No free accounts allowed after mass kick."},
		{"NO_MORE_FREE_ACC_ALLOWED", "No more free accounts allowed."},
		{"NO_MORE_ROOM_NEWBIES_FREE_ACC", "No more room for newbies with free account."},
		{"TOO_MANY_PLAYERS_ONLINE", "Too many players online."},
		{"NO_MORE_ROOM_NEWBIES", "No more room for newbies."},
		{"CANT_CONNECT_TO_QUERY_MANAGER", "Can't connect to query manager."},
		{"PLAYER_DOESNT_EXIST", "Player doesn't exist."},
		{"PLAYER_WAS_DELETED", "Player was deleted."},
		{"PLAYER_DOESNT_LIVE_THIS_WORLD", "Player doesn't live on this world."},
		{"PLAYER_NOT_INVITED", "Player is not invited."},
		{"INCORRECT_PASSWORD_FOR_PLAYER__LOGIN_FROM", "Incorrect password for player %s; Login from %s."},
		{"PLAYER_BLOCKED__LOGIN_FROM", "Player %s is blocked; Login from %s."},
		{"ACCOUNT_OF_PLAYER_WAS_DELETED", "Account of player %s was deleted."},
		{"IP_BLOCKED_FOR_PLAYER", "IP address %s blocked for player."},
		{"ACCOUNT_IS_BANISHED", "Account is banished."},
		{"CHARACTER_MUST_BE_RENAMED", "Character must be renamed."},
		{"YOUR_IP_IS_BANISHED", "Your IP address is banished."},
		{"OTHER_CHARACTERS_SAME_ACC_ALREADY_LOGGED_IN", "Other characters of the same account are already logged in."},
		{"LOGIN_GM_CLIENT_NO_GM_ACCOUNT", "Login with Gamemaster client on non-Gamemaster account."},
		{"INCORRECT_ACCOUNT_NUMBER_FOR", "Incorrect account number %u for %s."},
		{"UNKNOWN_RETURN_CODE_FROM_QUERYMANAGER", "Unknown return code from QueryManager."},
		{"PLAYER_NOT_ASSIGNED_TO_ACCOUNT", "Player %s is not assigned to an account."},
		{"PLAYER_LOGS_ON_SOCKET__FROM", "Player %s logs in on socket %d from %s."},
		{"CANT_ASSIGN_SLOT_FOR_PLAYER_DATA", "Can't assign slot for player data."},
		{"INVALID_INIT_COMMAND", "Invalid init command %d."},
		{"ERROR_READING_COMMAND", "Error reading command (%s)."},
		{"ERROR_DECRYPTING", "Error decrypting."},
		{"ERROR_READING_LOGIN_DATA", "Error reading login data (%s)."},
		{"PLAYER_ON_WAITING_LIST", "Player on waiting list."},
		{"PLAYER_NOT_ON_WAITING_LIST", "Player not on waiting list."},
		{"PLAYER_LOGIN_EARLY", "%s is trying to login %d seconds too early."},
		{"PLAYER_LOGIN_LATE", "%s is trying to login %d seconds too late."},
		{"ERROR_BUILDING_LOGIN_PACKET", "Error building login packet (%s)."},
		{"NOT_ENOUGH_DATA_ON_SOCKET", "Not enough data on socket %d."},
		{"PACKET_TO_SOCKET_TOO_LARGE_EMPTY", "Packet to socket %d too large or empty, discarding (%d bytes)"},
		{"INVALID_PACKET_SIZE__FROM", "Invalid packet size %d for encrypted packet from %s"},
		{"PAYLOAD_FROM_PACKET_TOO_LARGE_EMPTY", "Payload (%d Bytes) from packet to socket %d too large or empty."},
		{"NO_FREE_CONNECTION_AVAILABLE", "No free connection available."},
		{"ERROR_CLOSING_SOCKET_1", "Error %d closing socket (1)."},
		{"ERROR_CLOSING_SOCKET_2", "Error %d closing socket (2)."},
		{"ERROR_CLOSING_SOCKET_3", "Error %d closing socket (3)."},
		{"ERROR_CLOSING_SOCKET_4", "Error %d closing socket (4)."},
		{"F_SETOWN_EX__FAILED_FOR_SOCKET", "F_SETOWN_EX failed for socket %d."},
		{"F_SETFL__FAILED_FOR_SOCKET", "F_SETFL failed for socket %d."},
		{"LOGIN_TIMEOUT_FOR_SOCKET", "Login timeout for socket %d."},
		{"UNCAUGHT_EXCEPTION_D", "Uncaught exception %d."},
		{"UNCAUGHT_EXCEPTION_S", "Uncaught exception %s."},
		{"UNCAUGHT_EXCEPTION_UNKNOWN", "Uncaught exception of unknown type."},
		{"STARTING_GAME_SERVER", "Starting game server..."},
		{"LISTENING_ON_PORT", "Pid=%d, Tid=%d - Listening on port %d"},
		{"CANT_OPEN_SOCKET", "Can't open socket."},
		{"ALL_CONNECTIONS_TERMINATED", "All connections terminated."},
		{"TERMINATING_ALL_CONNECTIONS", "Terminating all connections..."},
		{"USING_OWN_STACKS", "Using own stacks."},
		{"USING_REDUCED_LIBRARY_STACKS", "Using reduced library stacks."},
		{"WAITING_COMMUNICATION_THREADS_FINISH", "Waiting for %d communication threads to finish..."},
		{"CANNOT_CREATE_NEW_THREAD", "Cannot create new thread."},
		{"NO_STACK_AREA_AVAILABLE", "No stack area available."},
		{"ERROR_DURING_ACCEPT", "Error %d during accept."},
		{"WAITING_FOR_CLIENTS", "Waiting for clients..."},
		{"ERROR_ON_LISTEN", "Error %d on listen."},
		{"ERROR_ON_BIND", "Error %d on bind."},
		{"ERROR_ON_SETSOCKOPT", "Error %d on setsockopt."},
		{"SOCKET_WAS_NOT_SET_LINGER_0", "Socket was not set to LINGER=0."},
		{"SENT_BYTES", "sent:      %d bytes."},
		{"RECEIVED_BYTES", "received:  %d bytes."},
		







		



		{"TWO_OBJECTS_ON_ARRAY", "Two %s objects (%d and %d) on array [%d,%d,%d]."},
	};

	const TranslationEntry DETranslations[] = {
		{"BUFFER_IS_NULL", "Buffer ist NULL."},
		{"LIBRARY_DOES_NOT_SUPPORT_ITS_OWN_STACKS", "Bibliothek unterstützt keine eigenen Stacks."},
		{"NOT_ALL_STACKS_RELEASED", "Nicht alle Stacks freigegeben."},
		{"MAXIMUM_STACK_USAGE", "Maximale Stack-Ausdehnung: %d..%d"},
		{"LAG_DETECTED", "Lag erkannt!"},
		{"ERROR_SENDING_TO_SOCKET", "Fehler %d beim Senden an Socket %d."},
		{"CONNECTION_ON_SOCKET_FAILED", "Verbindung an Socket %d zusammengebrochen."},
		{"INVALID_MESSAGE_TYPE", "Ungültiger Meldungstyp %d."},
		{"MESSAGE_IS_NULL", "Message ist NULL."},
		{"INVALID_WAITING_TIME", "Ungültige Wartezeit %d."},
		{"MESSAGE_TOO_LONG", "Botschaft zu lang (%s)."},
		{"CONNECTION_IS_NULL", "Verbindung ist NULL."},
		{"ERROR_FILLING_BUFFER", "Fehler beim Füllen des Puffers (%s)"},
		{"ADDING_TO_THE_WAITING_LIST", "Füge %s in die Warteschlange ein."},
		{"ORDER_BUFFER_ALMOST_FULL", "Order-Puffer ist fast voll."},
		{"NO_FREE_ACC_ALLOWED_AFTER_MASS_KICK", "Keine FreeAccounts zugelassen nach MassKick."},
		{"NO_MORE_FREE_ACC_ALLOWED", "Kein Platz mehr für FreeAccounts."},
		{"NO_MORE_ROOM_NEWBIES_FREE_ACC", "Kein Platz mehr für Newbies mit FreeAccount."},
		{"TOO_MANY_PLAYERS_ONLINE", "Zu viele Spieler online."},
		{"NO_MORE_ROOM_NEWBIES", "Kein Platz mehr für Newbies."},
		{"CANT_CONNECT_TO_QUERY_MANAGER", "Kann Verbindung zum Query-Manager nicht herstellen."},
		{"PLAYER_DOESNT_EXIST", "Spieler existiert nicht."},
		{"PLAYER_WAS_DELETED", "Spieler wurde gelöscht."},
		{"PLAYER_DOESNT_LIVE_THIS_WORLD", "Spieler lebt nicht auf dieser Welt."},
		{"PLAYER_NOT_INVITED", "Spieler ist nicht eingeladen."},
		{"INCORRECT_PASSWORD_FOR_PLAYER__LOGIN_FROM", "Falsches Paßwort für Spieler %s; Login von %s."},
		{"PLAYER_BLOCKED__LOGIN_FROM", "Spieler %s blockiert; Login von %s."},
		{"ACCOUNT_OF_PLAYER_WAS_DELETED", "Account von Spieler %s wurde gelöscht."},
		{"IP_BLOCKED_FOR_PLAYER", "IP-Adresse %s für Spieler %s blockiert."},
		{"ACCOUNT_IS_BANISHED", "Account ist verbannt."},
		{"CHARACTER_MUST_BE_RENAMED", "Character muss umbenannt werden."},
		{"YOUR_IP_IS_BANISHED", "IP-Adresse ist gesperrt."},
		{"OTHER_CHARACTERS_SAME_ACC_ALREADY_LOGGED_IN", "Schon andere Charaktere desselben Accounts eingeloggt."},
		{"LOGIN_GM_CLIENT_NO_GM_ACCOUNT", "Login mit Gamemaster-Client auf Nicht-Gamemaster-Account."},
		{"INCORRECT_ACCOUNT_NUMBER_FOR", "Falsche Accountnummer %u für %s."},
		{"UNKNOWN_RETURN_CODE_FROM_QUERYMANAGER", "Unbekannter Rückgabewert vom QueryManager."},
		{"PLAYER_NOT_ASSIGNED_TO_ACCOUNT", "Spieler %s wurde noch keinem Account zugewiesen."},
		{"PLAYER_LOGS_ON_SOCKET__FROM", "Spieler %s loggt ein an Socket %d von %s."},
		{"CANT_ASSIGN_SLOT_FOR_PLAYER_DATA", "Kann keinen Slot für Spielerdaten zuweisen."},
		{"INVALID_INIT_COMMAND", "Ungültiges Init-Kommando %d."},
		{"ERROR_READING_COMMAND", "Fehler beim Auslesen des Kommandos (%s)."},
		{"ERROR_DECRYPTING", "Fehler beim Entschlüsseln."},
		{"ERROR_READING_LOGIN_DATA", "Fehler beim Auslesen der Login-Daten (%s)."},
		{"PLAYER_ON_WAITING_LIST", "Spieler auf Warteliste."},
		{"PLAYER_NOT_ON_WAITING_LIST", "Spieler nicht auf Warteliste."},
		{"PLAYER_LOGIN_EARLY", "%s meldet sich %d Sekunden zu früh an."},
		{"PLAYER_LOGIN_LATE", "%s meldet sich %d Sekunden zu spät an."},
		{"ERROR_BUILDING_LOGIN_PACKET", "Fehler beim Zusammenbauen des Login-Pakets (%s)."},
		{"NOT_ENOUGH_DATA_ON_SOCKET", "Zu wenig Daten an Socket %d."},
		{"PACKET_TO_SOCKET_TOO_LARGE_EMPTY", "Paket an Socket %d zu groß oder leer, wird verworfen (%d Bytes)"},
		{"INVALID_PACKET_SIZE__FROM", "Ungültige Paketlänge %d für verschlüsseltes Paket von %s"},
		{"PAYLOAD_FROM_PACKET_TOO_LARGE_EMPTY", "Nutzdaten (%d Bytes) von Paket an Socket %d zu groß oder leer."},
		{"NO_FREE_CONNECTION_AVAILABLE", "Keine Verbindung mehr frei."},
		{"ERROR_CLOSING_SOCKET", "Fehler %d beim Schließen der Socket."},
		{"ERROR_CLOSING_SOCKET_1", "Fehler %d beim Schließen der Socket (1)."},
		{"ERROR_CLOSING_SOCKET_2", "Fehler %d beim Schließen der Socket (2)."},
		{"ERROR_CLOSING_SOCKET_3", "Fehler %d beim Schließen der Socket (3)."},
		{"ERROR_CLOSING_SOCKET_4", "Fehler %d beim Schließen der Socket (4)."},
		{"F_SETOWN_EX__FAILED_FOR_SOCKET", "F_SETOWN_EX fehlgeschlagen für Socket %d."},
		{"F_SETFL__FAILED_FOR_SOCKET", "F_SETFL fehlgeschlagen für Socket %d."},
		{"LOGIN_TIMEOUT_FOR_SOCKET", "Login-TimeOut für Socket %d."},
		{"UNCAUGHT_EXCEPTION_D", "Nicht abgefangene Exception %d."},
		{"UNCAUGHT_EXCEPTION_S", "Nicht abgefangene Exception %s."},
		{"UNCAUGHT_EXCEPTION_UNKNOWN", "Nicht abgefangene Exception unbekannten Typs."},
		{"STARTING_GAME_SERVER", "Starte Game-Server..."},
		{"LISTENING_ON_PORT", "Pid=%d, Tid=%d - horche an Port %d"},
		{"CANT_OPEN_SOCKET", "Kann Socket nicht öffnen."},
		{"ALL_CONNECTIONS_TERMINATED", "Alle Verbindungen beendet."},
		{"TERMINATING_ALL_CONNECTIONS", "Beende alle Verbindungen..."},
		{"USING_OWN_STACKS", "Verwende eigene Stacks."},
		{"USING_REDUCED_LIBRARY_STACKS", "Verwende verkleinerte Bibliotheks-Stacks."},
		{"WAITING_COMMUNICATION_THREADS_FINISH", "Warte auf Beendigung von %d Communication-Threads..."},
		{"CANNOT_CREATE_NEW_THREAD", "Kann neuen Thread nicht anlegen."},
		{"NO_STACK_AREA_AVAILABLE", "Kein Stack-Bereich mehr frei."},
		{"ERROR_DURING_ACCEPT", "Fehler %d beim Accept."},
		{"WAITING_FOR_CLIENTS", "Warte auf Clients..."},
		{"ERROR_ON_LISTEN", "Fehler %d bei listen."},
		{"ERROR_ON_BIND", "Fehler %d bei bind."},
		{"ERROR_ON_SOCKOPT", "Fehler %d bei setsockopt."},
		{"SOCKET_WAS_NOT_SET_LINGER_0", "Socket wurde nicht auf LINGER=0 gesetzt."},
		{"SENT_BYTES", "gesendet:  %d Bytes."},
		{"RECEIVED_BYTES", "empfangen: %d Bytes."},




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