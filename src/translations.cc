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
		{"INVALID_TYPE_D", "Invalid Type %d."},
		{"TYPE_HAS_NO_FLAG_FOR_ATTRIBUTE", "Type %d has no Flag %d for Attribute %d."},
		{"CREATURE_TYPE_HAS_NO_NAME", "The creature type has no name."},
		{"OBJECT_TYPE_DOEST_NOT_EXIST", "Object type %u/%u does not exist."},
		{"SEARCHNAME_IS_NULL", "SearchName is NULL."},
		{"NO_OBJECT_TYPE__MEANING_DEFINED", "No object type with meaning %d defined."},
		{"INVALID_MEANING", "Invalid meaning %d."},
		{"CANNOT_OPEN_FILE", "Cannot open file %s."},
		{"CANNOT_CREATE_THREAD__ERROR_CODE", "Cannot create thread; Error code %d."},
		{"CANNOT_RELEASE_MUTEX", "Cannot release mutex: (%d) %s."},
		{"CANNOT_RELEASE_WAIT_CONDITION", "Cannot release wait condition: (%d) %s."},
		{"CANNOT_SET_MUTEX", "Cannot set mutex."},
		{"CANNOT_SET_WAIT_CONDITION", "Cannot set wait condition."},
		{"SHAREDMEMORY_DOES_NOT_EXIST", "SharedMemory does not exist."},
		{"CANNOT_DELETE_SHAREDMEMORY", "Cannot delete SharedMemory."},
		{"CANNOT_CREATE_SHAREDMEMORY", "Cannot create SharedMemory (Error %d)."},
		{"CANNOT_GET_SHAREDMEMORY", "Cannot get SharedMemory."},
		{"CANNOT_DETACH_SHAREDMEMORY", "Cannot detach SharedMemory."},
		{"TOO_MANY_ERROR_NO_FURTHER_LOGGING", "Too many errors. No further logging."},
		{"ORDER_BUFFER_IS_FULL__INCREASE", "Order buffer is full => Increase."},
		{"CANNOT_ALLOCATE_A_SLOT_FOR_PLAYER_DATA", "Cannot allocate a slot for player data."},
		{"UNKNOWN_COMMAND", "Unknown command %d."},
		{"BUFFER_IS_FULL_WAITING", "Buffer is full; waiting..."},
		{"UNKNOWN_RESPONSE", "Unknown response %d."},
		{"INVALID_FIELD_SIZE_2_D", "Invalid field size %d to %d."},
		{"INVALID_BLOCK_SIZE", "Invalid block size %d."},
		{"QUEUE_IS_EMPTY", "Queue is empty."},
		{"INVALID_FIELD_SIZE_4_D", "Invalid field size %d..%d, %d..%d."},
		{"INVALID_INDEX_2_D", "Invalid index %d/%d."},
		{"INVALID_FIELD_SIZE_6_D", "Invalid field size %d..%d, %d..%d, %d..%d."},
		{"INVALID_INDEX_3_D", "Invalid index %d/%d/%d."},
		{"NODE_IS_NULL", "node is NULL."},
		{"FIFO_IS_EMPTY", "Fifo is empty."},
		{"CREATUREOBJECT_DOES_NOT_EXIST", "CreatureObject does not exist."},
		{"INVALID_ATTACK_MODE", "Invalid attack mode %d."},
		{"INVALID_TRACKING_MODE", "Invalid tracking mode %d."},
		{"INVALID_SECURITY_MODE", "Invalid security mode %d."},
		{"NO_MASTER_SET", "No Master set!"},
		{"COULD_NOT_MOVE_DELETE_AMMO", "Could not move/delete ammo (Exception %d, [%d,%d,%d]."},
		{"HAS_DIED_DISTRIBUTE_EXP", "%s has died. Distribute %u EXP..."},
		{"RECEIVES_EXP", "%s receives %d EXP.\n"},
		{"STRING_TOO_LONG", "String too long (%d)."},
		{"NO_FREE_SPACE_FOUND", "No more free space."},
		{"BLOCK_FOR_STRING_DOES_NOT_EXIST", "Block for string %u does not exist"},
		{"ENTRY_FOR_STRING_DOES_NOT_EXIST", "Entry for string %u does not exist"},
		{"STRINGEND_IS_MISSING", "StringEnd is missing"},
		{"PASSED_STRING_DOES_NOT_EXIST", "Passed string does not exist."},
		{"PASSED_SEARCH_TERM_DOES_NOT_EXIST", "Passed search term does not exist."},
		{"PASSED_TEXT_DOES_NOT_EXIST", "Passed text does not exist."},
		{"ILLEGAL_SEARCH_NUMBER", "Illegal search number %d."},
		{"PATTERN_IS_NULL", "Pattern is NULL."},
		{"STRING_IS_NULL", "String is NULL."},
		{"SOURCE_IS_NULL", "Source is NULL."},
		{"DESTINATION_IS_NULL", "Destination is NULL."},
		{"TEXT_IS_NULL", "Text is NULL."},
		{"FILENAME_IS_NULL", "Filename is NULL."},
		{"SOURCE_FILE_DOES_NOT_EXIST", "Source file %s does not exist."},
		{"CANNOT_CREATE_TARGET_FILE", "Cannot create target file %s."},
		{"ERROR_READING_SOURCE_FILE", "Error reading source file %s."},
		{"ERROR_WRITING_TARGET_FILE", "Error writing the target file %s."},
		{"ERROR_CLOSING_SOURCE_FILE", "Error %d closing source file."},
		{"ERROR_CLOSING_TARGET_FILE", "Error %d closing the target file."},
		{"FILE_IS_STILL_OPEN", "File is still open."},
		{"ERROR_CLOSING_FILE", "Error %d closing file."},
		{"FILE_ERROR_CODE", "# File: %s, Error code: %d (%s)"},
		{"FILE_POSITION_RETURN_VALUE_ERROR_CODE", "# File: %s, Position: %d, Return value: %d, Error code: %d (%s)"},
		{"ERROR_WHILE_READING_A_BYTE", "Error while reading a byte"},
		{"ERROR_READING_BYTES", "Error reading %d bytes"},
		{"NO_SCRIPT_OPEN_FOR_WRITING", "No script open for writing."},
		{"INVALID_COORDINATES", "Invalid coordinates [%d,%d,%d]."},
		{"SEQUENCE_IS_NULL", "Sequence is NULL."},
		{"INVALID_SEQUENCE_LENGTH", "Invalid sequence length."},
		{"CANNOT_CREATE_FILE", "Cannot create file %s."},
		{"TWO_OBJECTS_ON_ARRAY", "Two %s objects (%d and %d) on array [%d,%d,%d]."},
		{"INVALID_FLUID_TYPE", "Invalid fluid type %d"},
		{"CREATURE_D_DOES_NOT_EXIST", "Creature %d does not exist."},
		{"CREATURE_U_DOES_NOT_EXIST", "Creature %u does not exist."},
		{"PASSED_OBJECT_DOES_NOT_EXIST", "Passed object does not exist."},
		{"OBJECT_TYPE_NOT_REMOVABLE", "Object type %d is not removable."},
		{"OBJECT_IS_MAPCONTAINER", "Object is MapContainer."},
		{"OBJECT_NOT_IN_CONTAINER", "Object is not in container"},
		{"INVALID_CREATURE_ID", "Invalid creature ID."},
		{"INVALID_CREATURE_ID_D", "Invalid creature CreatureID=%d passed."},
		{"CREATURE_DOES_NOT_EXIST", "Creature does not exist."},
		{"INVALID_POSITION", "Invalid position: %d"},
		{"CREATURE_OBJECT_S_DOES_NOT_EXIST_POS", "Creature object of %s does not exist (pos %d)."},
		{"ONLY_PLAYERS_HAVE_OPEN_CONTAINERS", "Only players have open containers."},
		{"CONTAINER_DOES_NOT_EXIST", "Container does not exist."},
		{"CREATURE_D_DOES_NOT_EXIST_OBJECT_TYPE", "Creature %d does not exist; object type %d."},
		{"CREATURE_S_NO_CREATURE_OBJECT", "Creature %s has no creature object."},
		{"COINS_NOT_ENOUGH", "%d/%d/%d coins are not enough to pay %d."},
		{"PAY_WITH_COINS", "Pay %d with %d/%d/%d coins..."},
		{"INVALID_BAN_REASON", "Invalid ban reason %d."},
		{"INVALID_BAN_REASON_BY_PLAYER", "Invalid ban reason %d from player %d."},
		{"PLAYER_DOES_NOT_EXIST", "Player does not exist."},
		{"PLAYER_DOES_NOT_EXIST_RIGHT", "Player does not exist; Right=%d."},
		{"INVALID_RIGHT_NUMBER", "Invalid Right number %d."},
		{"INVALID_DIRECTION", "Invalid direction %d."},
		{"REFUGEE_DOES_NOT_EXIST", "Refugee does not exist."},
		{"PURSUER_DOES_NOT_EXIST", "Pursuer does not exist."},
		{"REFUGEE_PURSUER_DIFFERENT_LEVELS", "Refugee and pursuer are on different levels."},
		{"INCORRECT_CALCULATION", "Incorrect calculation: %d/%d/%d coins for %d."},
		{"USE_COINS", "Use %d/%d/%d coins."},
		{"INVALID_ACTION_BY_PLAYER", "Invalid action %d by player %d."},
		{"INVALID_CONTAINER_CODE", "Invalid ContainerCode x=%d,y=%d,z=%d,RNum=%d,Type=%d."},

		// Added for connections.cc migration from Translate() to t()
		{"CONNECTION_IS_NOT_ASSIGNED", "Connection is not assigned."},
		{"CONNECTION_IS_NOT_CONNECTED", "Connection is not connected."},
		{"CONNECTION_IS_NOT_FREE", "Connection is not free."},
		{"CONNECTION_IS_NOT_ASSIGNED_TO_A_THREAD", "Connection is not assigned to a thread."},
		{"INVALID_CONNECTION_STATE_D", "Invalid connection state %d."},
		{"ERROR_READING_BUFFER", "Error reading buffer (%s)."},
		{"UNKNOWN_TERMINAL_TYPE_D", "Unknown terminal type %d."},
		{"PLAYER_IS_CURRENTLY_DYING__LOGIN_FAILED", "Player %s is currently dying - login failed."},
		{"PLAYER_IS_CURRENTLY_LOGGING_OUT__LOGIN_FAILED", "Player %s is currently logging out - Login failed."},
		{"KNOWNCREATURETABLE_FULL", "KnownCreatureTable full."},
		{"SLOT_IS_NOT_FREE", "Slot is not free."},
		{"CREATURE_U_NOT_KNOWN_BY_ANYONE", "Creature %u is not known by anyone."},
		{"CREATURE_U_NOT_KNOWN", "Creature %u is not known."},

		// Added for utils.cc and sending.cc migrations
		{"UNEXPECTED_ERROR_CODE_D", "Unexpected error code %d."},
		{"PASSED_BUFFER_DOES_NOT_EXIST", "Passed buffer does not exist."},
		{"DATA_IS_NULL", "data is NULL."},
		{"INVALID_DATA_SIZE_D", "Invalid data size %d."},
		{"OBJECT_IS_NOT_A_CREATURE", "Object is not a creature."},
		{"FLAG_FOR_ATTRIBUTE_NOT_SET", "Flag for attribute %d not set for object type %d."},
		{"INVALID_OFFSET_FOR_ATTRIBUTE_ON_OBJECT_TYPE", "Invalid offset %d for attribute %d on object type %d."},
		{"HASHTABLE_TOO_SMALL__DOUBLE_TO_D", "INFO: HashTable too small. Size will be doubled to %d."},
		{"ERROR_REORGANIZING_HASHTABLE", "Error reorganizing the HashTable."},
		{"ENTRY_IS_NULL", "Entry is NULL."},
		{"NO_SECTOR_CAN_BE_SWAPPED_OUT", "No sector can be swapped out."},
		{"STORAGE_SECTOR_D_D_D", "Storage sector %d/%d/%d..."},
		{"STORE_SECTOR_D_D_D", "Store sector %d/%d/%d..."},
		{"HASH_ERROR_S", "# Error: %s"},
		{"SECTOR_D_D_D_DOES_NOT_EXIST", "Sector %d/%d/%d does not exist."},
		{"SECTOR_D_D_D_IS_NOT_SWAPPED_OUT", "Sector %d/%d/%d is not swapped out."},
		{"OBJECT_U_ALREADY_EXISTS", "Object %u already exists."},
		{"CANNOT_READ_FILE", "Cannot read file %s."},
		{"SUBDIRECTORY_S_NOT_FOUND", "Subdirectory %s not found"},
		{"LOADING_MAP", "Loading map ..."},
		{"SECTORS_LOADED_D", "%d Sectors loaded."},
		{"OBJECTS_LOADED_D", "%d Objects loaded."},
		{"LASTOBJ_IS_NONE_1", "LastObj is NONE (1)"},
		{"LASTOBJ_IS_NONE_2", "LastObj is NONE (2)"},
		{"SECTOR_D_D_D_IS_EMPTY", "Sector %d/%d/%d is empty."},
		{"CANNOT_WRITE_FILE", "Cannot write file %s."},
		{"MAP_IS_ALREADY_BEING_SAVED", "Map is already being saved."},
		{"SAVING_MAP", "Saving map ..."},
		{"OBJECTS_SAVED_D", "%d Objects saved."},
		{"SAVING_SECTOR_D_D_D", "Saving sector %d/%d/%d ..."},
		{"CON_D_IS_NOT_A_CONTAINER", "Con (%d) is not a container."},
		{"CONTAINER_DOES_NOT_EXIST", "Container does not exist."},
		{"REFRESH_SECTOR_D_D_D", "Refresh sector %d/%d/%d ..."},
		{"CREATING_SECTOR_D_D_D", "Recreate sector %d/%d/%d."},
		{"LOADING_SECTOR_D_D_D", "Loading sector %d/%d/%d ..."},
		{"ERROR_D_RENAMING_S", "Error %d renaming %s."},
		{"HASH_ERROR_D_S", "# Error %d: %s."},
		{"INVALID_OBJECT_NUMBER_ZERO", "Invalid object number zero."},
		{"CANNOT_CREATE_OBJECT", "Cannot create object."},
		{"INVALID_OBJECT_NUMBER_D", "Invalid object number %d."},
		{"OBJECT_NOT_IN_MEMORY", "Object is not in memory."},
		{"DESTINATION_CONTAINER_DOES_NOT_EXIST", "Destination container does not exist."},
		{"TEMPLATE_DOES_NOT_EXIST", "Template does not exist."},
		{"CREATURES_MAY_NOT_BE_COPIED", "Creatures may not be copied."},
		{"INVALID_RUNNING_NUMBER_D", "Invalid serial number %d."},
		{"OBJECT_IS_NOT_MAPCONTAINER", "Object is not a MapContainer."},
		{"MAPCONTAINER_FOR_POINT_D_D_D_DOES_NOT_EXIST", "Map container for point [%d,%d,%d] does not exist."},
		{"FIELD_D_D_D_ALREADY_BELONGS_TO_A_HOUSE", "Field [%d,%d,%d] already belongs to a house."},
		{"TOWN_IS_NULL", "Town is NULL."},
		{"INVALID_NAME_FOR_DEPOT_D", "Invalid name for depot %d."},
		{"INVALID_DEPOT_NUMBER_D", "Invalid depot number %d."},
		{"INVALID_DEPOT_SIZE_D_FOR_DEPOT_D", "Invalid depot size %d for depot %d."},
		{"OBJECT_NOT_CUMULATIVE", "Object is not cumulative."},
		{"INVALID_COUNTER_D", "Invalid counter %d."},
		{"PASSED_TARGET_DOES_NOT_EXIST", "Transferred target does not exist."},
		{"OBJECT_TYPES_D_AND_D_NOT_IDENTICAL", "Object types %d and %d are not identical."},
		{"OBJECT_TYPE_D_NOT_CUMULATIVE", "Object type %d is not cumulative."},
		{"OBJECT_CONTAINS_0_PARTS", "Object contains 0 parts."},
		{"TARGET_OBJECT_CONTAINS_0_PARTS", "Target object contains 0 parts."},
		{"NEW_OBJECT_CONTAINS_MORE_THAN_100_PARTS", "New object contains more than 100 parts."},
		{"ACTOR_IS_NULL", "Actor is NULL."},
		{"VICTIM_DOES_NOT_EXIST", "Victim does not exist."},
		{"POWER_IS_NEGATIVE__ACTOR_S", "Power is negative (Actor: %s)."},
		{"CONNECTION_IS_NOT_WILLING_TO_SEND", "Connection is not willing to send."},
		{"BUFFER_OF_CONNECTION_D_IS_FULL", "Buffer of connection %d is full."},
		{"CONNECTION_IS_NOT_ONLINE", "Connection is not online."},
		{"BUFFER_IS_FULL__PACKAGE_NOT_SENT", "Buffer is full. Package will not be sent."},
		{"INVALID_TEXT_LENGTH_D", "invalid text length %d."},
		{"STRING_IS_NULL", "String is NULL."},
		{"CREATURE_HAS_NO_CREATURE_OBJECT", "Creature has no creature object."},
		{"NO_PLAYER_BELONGS_TO_THIS_CONNECTION", "No player belongs to this connection."},
		{"CONTAINER_D_DOES_NOT_EXIST", "Container %d does not exist."},
		{"NAME_IS_NULL", "Name is NULL."},
		{"TEXT_IS_TOO_LONG", "Text is too long."},
		{"SENDER_IS_NULL", "Sender is NULL."},
		{"INVALID_MODE_D", "Invalid mode %d."},
		{"INVALID_CHANNEL_D", "Invalid channel %d."},
		{"CHANNEL_IS_GM_REQUEST_QUEUE", "Channel is GM-Request-Queue."},
		{"OBJECT_DOES_NOT_EXIST", "Object does not exist."},
		{"TEXT_IS_EMPTY", "Text is empty."},
		{"OBJECT_NOT_IN_CRON_SYSTEM", "Object is not registered in the cron system."},

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
		{"INVALID_TYPE_D", "Ungültiger Typ %d."},
		{"TYPE_HAS_NO_FLAG_FOR_ATTRIBUTE", "Typ %d hat kein Flag %d für Attribut %d."},
		{"CREATURE_TYPE_HAS_NO_NAME", "Der Kreaturtyp hat keinen Namen."},
		{"OBJECT_TYPE_DOEST_NOT_EXIST", "Objekttyp %u/%u existiert nicht."},
		{"SEARCHNAME_IS_NULL", "SearchName ist NULL."},
		{"NO_OBJECT_TYPE__MEANING_DEFINED", "Kein Objekttyp mit Bedeutung %d definiert."},
		{"INVALID_MEANING", "Ungültige Bedeutung %d."},
		{"CANNOT_OPEN_FILE", "Kann Datei %s nicht öffnen %s."},
		{"CANNOT_CREATE_THREAD__ERROR_CODE", "Kann Thread nicht anlegen; Fehlercode %d."},
		{"CANNOT_RELEASE_MUTEX", "Kann Mutex nicht freigeben: (%d) %s."},
		{"CANNOT_RELEASE_WAIT_CONDITION", "Kann Wartebedingung nicht freigeben: (%d) %s."},
		{"CANNOT_SET_MUTEX", "Kann Mutex nicht einrichten."},
		{"CANNOT_SET_WAIT_CONDITION", "Kann Wartebedingung nicht einrichten."},
		{"SHAREDMEMORY_DOES_NOT_EXIST", "SharedMemory existiert nicht."},
		{"CANNOT_DELETE_SHAREDMEMORY", "Kann SharedMemory nicht löschen."},
		{"CANNOT_CREATE_SHAREDMEMORY", "Kann SharedMemory nicht anlegen (Fehler %d).."},
		{"CANNOT_GET_SHAREDMEMORY", "Kann SharedMemory nicht fassen."},
		{"CANNOT_DETACH_SHAREDMEMORY", "Kann SharedMemory nicht löschen."},
		{"TOO_MANY_ERROR_NO_FURTHER_LOGGING", "Zu viele Fehler. Keine weitere Protokollierung."},
		{"ORDER_BUFFER_IS_FULL__INCREASE", "Order-Puffer ist voll => Vergrößern."},
		{"CANNOT_ALLOCATE_A_SLOT_FOR_PLAYER_DATA", "Kann keinen Slot für Spielerdaten zuweisen."},
		{"UNKNOWN_COMMAND", "Unbekanntes Kommando %d."},
		{"BUFFER_IS_FULL_WAITING", "Puffer ist voll; warte..."},
		{"UNKNOWN_RESPONSE", "Unbekannte Rückmeldung %d."},
		{"INVALID_FIELD_SIZE_2_D", "Ungueltige Feldgroesse %d bis %d."},
		{"INVALID_BLOCK_SIZE", "Ungueltige Blockgroesse %d."},
		{"QUEUE_IS_EMPTY", "Warteschlange ist leer."},
		{"INVALID_FIELD_SIZE_4_D", "Ungueltige Feldgroesse %d..%d, %d..%d."},
		{"INVALID_INDEX_2_D", "Ungueltiger Index %d/%d."},
		{"INVALID_FIELD_SIZE_6_D", "Ungueltige Feldgroesse %d..%d, %d..%d, %d..%d."},
		{"INVALID_INDEX_3_D", "Ungueltiger Index %d/%d/%d."},
		{"NODE_IS_NULL", "node ist NULL."},
		{"FIFO_IS_EMPTY", "Fifo ist leer."},
		{"CREATUREOBJECT_DOES_NOT_EXIST", "Kreatur-Objekt existiert nicht."},
		{"INVALID_ATTACK_MODE", "Ungültiger Angriffsmodus %d."},
		{"INVALID_TRACKING_MODE", "Ungültiger Verfolgungsmodus %d."},
		{"INVALID_SECURITY_MODE", "Ungültiger Sicherheitsmodus %d."},
		{"NO_MASTER_SET", "Kein Master gesetzt!"},
		{"COULD_NOT_MOVE_DELETE_AMMO", "Konnte Ammo nicht verschieben/löschen (Exception %d, [%d,%d,%d]."},
		{"HAS_DIED_DISTRIBUTE_EXP", "%s ist gestorben. Verteile %u EXP..."},
		{"RECEIVES_EXP", "%s erhält %d EXP."},
		{"STRING_TOO_LONG", "String zu lang (%d)."},
		{"NO_FREE_SPACE_FOUND", "Kein freier Platz mehr."},
		{"BLOCK_FOR_STRING_DOES_NOT_EXIST", "Block für String %u existiert nicht"},
		{"ENTRY_FOR_STRING_DOES_NOT_EXIST", "Eintrag für String %u existiert nicht"},
		{"STRINGEND_IS_MISSING", "Stringende fehlt"},
		{"PASSED_STRING_DOES_NOT_EXIST", "Übergebener String existiert nicht."},
		{"PASSED_SEARCH_TERM_DOES_NOT_EXIST", "Übergebenes Suchwort existiert nicht."},
		{"PASSED_TEXT_DOES_NOT_EXIST", "Übergebener Text existiert nicht."},
		{"ILLEGAL_SEARCH_NUMBER", "Illegale Suchnummer %d."},
		{"PATTERN_IS_NULL", "Pattern ist NULL."},
		{"STRING_IS_NULL", "String ist NULL."},
		{"SOURCE_IS_NULL", "Source ist NULL."},
		{"DESTINATION_IS_NULL", "Destination ist NULL."},
		{"TEXT_IS_NULL", "Text ist NULL."},
		{"FILENAME_IS_NULL", "Filename ist NULL."},
		{"SOURCE_FILE_DOES_NOT_EXIST", "Quelldatei %s existiert nicht."},
		{"CANNOT_CREATE_TARGET_FILE", "Kann Zieldatei %s nicht anlegen."},
		{"ERROR_READING_SOURCE_FILE", "Fehler beim Lesen der Quelldatei %s."},
		{"ERROR_WRITING_TARGET_FILE", "Fehler beim Schreiben der Zieldatei %s."},
		{"ERROR_CLOSING_SOURCE_FILE", "Fehler %d beim Schließen der Quelldatei."},
		{"ERROR_CLOSING_TARGET_FILE", "Fehler %d beim Schließen der Zieldatei."},
		{"FILE_IS_STILL_OPEN", "Datei ist noch offen."},
		{"ERROR_CLOSING_FILE", "Fehler %d beim Schließen der Datei."},
		{"FILE_ERROR_CODE", "# Datei: %s, Fehlercode: %d (%s)"},
		{"FILE_POSITION_RETURN_VALUE_ERROR_CODE", "# Datei: %s, Position: %d, Rückgabewert: %d, Fehlercode: %d (%s)"},
		{"ERROR_WHILE_READING_A_BYTE", "Fehler beim Lesen eines Bytes"},
		{"ERROR_READING_BYTES", "Fehler beim Lesen von %d Bytes"},
		{"NO_SCRIPT_OPEN_FOR_WRITING", "Kein Skript zum Schreiben geöffnet."},
		{"INVALID_COORDINATES", "Ungültige Koordinaten [%d,%d,%d]."},
		{"SEQUENCE_IS_NULL", "Sequence ist NULL."},
		{"INVALID_SEQUENCE_LENGTH", "Ungültige Sequenzlänge."},
		{"CANNOT_CREATE_FILE", "Kann Datei %s nicht anlegen."},
		{"TWO_OBJECTS_ON_ARRAY", "Zwei %s-Objekte (%d und %d) auf Feld [%d,%d,%d]."},
		{"INVALID_FLUID_TYPE", "Ungültiger Flüssigkeitstyp %d"},
		{"CREATURE_D_DOES_NOT_EXIST", "Kreatur %d existiert nicht."},
		{"CREATURE_U_DOES_NOT_EXIST", "Kreatur %u existiert nicht."},
		{"PASSED_OBJECT_DOES_NOT_EXIST", "Übergebenes Objekt existiert nicht."},
		{"OBJECT_TYPE_NOT_REMOVABLE", "Objekttyp %d ist nicht nehmbar."},
		{"OBJECT_IS_MAPCONTAINER", "Objekt ist MapContainer."},
		{"OBJECT_NOT_IN_CONTAINER", "Objekt liegt nicht in Container"},
		{"INVALID_CREATURE_ID", "Ungültige Kreatur-ID."},
		{"INVALID_CREATURE_ID_D", "Ungültige Kreatur CreatureID=%d übergeben."},
		{"CREATURE_DOES_NOT_EXIST", "Kreatur existiert nicht."},
		{"INVALID_POSITION", "Ungültige Position: %d"},
		{"CREATURE_OBJECT_S_DOES_NOT_EXIST_POS", "Kreatur-Objekt von %s existiert nicht (Pos %d)."},
		{"ONLY_PLAYERS_HAVE_OPEN_CONTAINERS", "Nur Spieler haben offene Container."},
		{"CONTAINER_DOES_NOT_EXIST", "Container existiert nicht."},
		{"CREATURE_D_DOES_NOT_EXIST_OBJECT_TYPE", "Kreatur %d existiert nicht; Objekttyp %d."},
		{"CREATURE_S_NO_CREATURE_OBJECT", "Kreatur %s hat kein Kreatur-Objekt."},
		{"COINS_NOT_ENOUGH", "%d/%d/%d Münzen reichen nicht zur Bezahlung von %d."},
		{"PAY_WITH_COINS", "Zahle %d mit %d/%d/%d Münzen..."},
		{"INVALID_BAN_REASON", "Ungültiger Verbannungsgrund %d."},
		{"INVALID_BAN_REASON_BY_PLAYER", "Ungültiger Banngrund %d von Spieler %d."},
		{"PLAYER_DOES_NOT_EXIST", "Spieler existiert nicht."},
		{"PLAYER_DOES_NOT_EXIST_RIGHT", "Spieler existiert nicht; Right=%d."},
		{"INVALID_RIGHT_NUMBER", "Ungültige Rechtnummer %d."},
		{"INVALID_DIRECTION", "Ungültige Richtung %d."},
		{"REFUGEE_DOES_NOT_EXIST", "Flüchtling existiert nicht."},
		{"PURSUER_DOES_NOT_EXIST", "Verfolger existiert nicht."},
		{"REFUGEE_PURSUER_DIFFERENT_LEVELS", "Flüchtling und Verfolger sind auf verschiedenen Ebenen."},
		{"INCORRECT_CALCULATION", "Fehlerhafte Berechnung: %d/%d/%d Münzen für %d."},
		{"USE_COINS", "Verwende %d/%d/%d Münzen."},
		{"INVALID_ACTION_BY_PLAYER", "Ungültige Aktion %d von Spieler %d."},
		{"INVALID_CONTAINER_CODE", "Ungültiger ContainerCode x=%d,y=%d,z=%d,RNum=%d,Type=%d."},

		// Added for connections.cc migration from Translate() to t()
		{"CONNECTION_IS_NOT_ASSIGNED", "Verbindung ist nicht zugewiesen."},
		{"CONNECTION_IS_NOT_CONNECTED", "Verbindung ist nicht angeschlossen."},
		{"CONNECTION_IS_NOT_FREE", "Verbindung ist nicht frei."},
		{"CONNECTION_IS_NOT_ASSIGNED_TO_A_THREAD", "Verbindung ist keinem Thread zugewiesen."},
		{"INVALID_CONNECTION_STATE_D", "Ungültiger Verbindungszustand %d."},
		{"ERROR_READING_BUFFER", "Fehler beim Auslesen des Puffers (%s)."},
		{"UNKNOWN_TERMINAL_TYPE_D", "Unbekannter Terminal-Typ %d."},
		{"PLAYER_IS_CURRENTLY_DYING__LOGIN_FAILED", "Spieler %s ist gerade am Sterben - Einloggen gescheitert."},
		{"PLAYER_IS_CURRENTLY_LOGGING_OUT__LOGIN_FAILED", "Spieler %s loggt gerade aus - Einloggen gescheitert."},
		{"KNOWNCREATURETABLE_FULL", "KnownCreatureTable ausgelastet."},
		{"SLOT_IS_NOT_FREE", "Slot ist nicht gelöscht."},
		{"CREATURE_U_NOT_KNOWN_BY_ANYONE", "Kreatur %u kennt niemand."},
		{"CREATURE_U_NOT_KNOWN", "Kreatur %u ist nicht bekannt."},

		// Added for utils.cc and sending.cc migrations
		{"UNEXPECTED_ERROR_CODE_D", "Unerwarteter Fehlercode %d."},
		{"PASSED_BUFFER_DOES_NOT_EXIST", "Übergebener Puffer existiert nicht."},
		{"DATA_IS_NULL", "data ist NULL."},
		{"INVALID_DATA_SIZE_D", "Ungültige Datengröße %d."},
		{"OBJECT_IS_NOT_A_CREATURE", "Objekt ist keine Kreatur."},
		{"FLAG_FOR_ATTRIBUTE_NOT_SET", "Flag für Attribut %d bei Objekttyp %d nicht gesetzt."},
		{"INVALID_OFFSET_FOR_ATTRIBUTE_ON_OBJECT_TYPE", "Ungültiger Offset %d für Attribut %d bei Objekttyp %d."},
		{"HASHTABLE_TOO_SMALL__DOUBLE_TO_D", "INFO: HashTabelle zu klein. Größe wird verdoppelt auf %d."},
		{"ERROR_REORGANIZING_HASHTABLE", "Fehler beim Reorganisieren der HashTabelle."},
		{"ENTRY_IS_NULL", "Entry ist NULL."},
		{"NO_SECTOR_CAN_BE_SWAPPED_OUT", "Es kann kein Sektor ausgelagert werden."},
		{"STORAGE_SECTOR_D_D_D", "Lagere Sektor %d/%d/%d aus..."},
		{"STORE_SECTOR_D_D_D", "Lagere Sector %d/%d/%d ein..."},
		{"HASH_ERROR_S", "# Fehler: %s"},
		{"SECTOR_D_D_D_DOES_NOT_EXIST", "Sektor %d/%d/%d existiert nicht."},
		{"SECTOR_D_D_D_IS_NOT_SWAPPED_OUT", "Sektor %d/%d/%d ist nicht ausgelagert."},
		{"OBJECT_U_ALREADY_EXISTS", "Objekt %u existiert schon."},
		{"CANNOT_READ_FILE", "Kann Datei %s nicht lesen."},
		{"SUBDIRECTORY_S_NOT_FOUND", "Unterverzeichnis %s nicht gefunden"},
		{"LOADING_MAP", "Lade Karte ..."},
		{"SECTORS_LOADED_D", "%d Sektoren geladen."},
		{"OBJECTS_LOADED_D", "%d Objekte geladen."},
		{"LASTOBJ_IS_NONE_1", "LastObj ist NONE (1)"},
		{"LASTOBJ_IS_NONE_2", "LastObj ist NONE (2)"},
		{"SECTOR_D_D_D_IS_EMPTY", "Sektor %d/%d/%d ist leer."},
		{"CANNOT_WRITE_FILE", "Kann Datei %s nicht schreiben."},
		{"MAP_IS_ALREADY_BEING_SAVED", "Karte wird schon gespeichert."},
		{"SAVING_MAP", "Speichere Karte ..."},
		{"OBJECTS_SAVED_D", "%d Objekte gespeichert."},
		{"SAVING_SECTOR_D_D_D", "Speichere Sektor %d/%d/%d ..."},
		{"CON_D_IS_NOT_A_CONTAINER", "Con (%d) ist kein Container."},
		{"CONTAINER_DOES_NOT_EXIST", "Container existiert nicht."},
		{"REFRESH_SECTOR_D_D_D", "Refreshe Sektor %d/%d/%d ..."},
		{"CREATING_SECTOR_D_D_D", "Lege Sektor %d/%d/%d neu an."},
		{"LOADING_SECTOR_D_D_D", "Lade Sektor %d/%d/%d ..."},
		{"ERROR_D_RENAMING_S", "Fehler %d beim Umbenennen von %s."},
		{"HASH_ERROR_D_S", "# Fehler %d: %s."},
		{"INVALID_OBJECT_NUMBER_ZERO", "Ungültige Objektnummer Null."},
		{"CANNOT_CREATE_OBJECT", "Kann Objekt nicht anlegen."},
		{"INVALID_OBJECT_NUMBER_D", "Ungültige Objektnummer %d."},
		{"OBJECT_NOT_IN_MEMORY", "Objekt steht nicht im Speicher."},
		{"DESTINATION_CONTAINER_DOES_NOT_EXIST", "Zielcontainer existiert nicht."},
		{"TEMPLATE_DOES_NOT_EXIST", "Vorlage existiert nicht."},
		{"CREATURES_MAY_NOT_BE_COPIED", "Kreaturen dürfen nicht kopiert werden."},
		{"INVALID_RUNNING_NUMBER_D", "Ungültige laufende Nummer %d."},
		{"OBJECT_IS_NOT_MAPCONTAINER", "Objekt ist kein MapContainer."},
		{"MAPCONTAINER_FOR_POINT_D_D_D_DOES_NOT_EXIST", "Kartencontainer für Punkt [%d,%d,%d] existiert nicht."},
		{"FIELD_D_D_D_ALREADY_BELONGS_TO_A_HOUSE", "Feld [%d,%d,%d] gehört schon zu einem Haus."},
		{"TOWN_IS_NULL", "Town ist NULL."},
		{"INVALID_NAME_FOR_DEPOT_D", "Ungültiger Name für Depot %d."},
		{"INVALID_DEPOT_NUMBER_D", "Ungültige Depotnummer %d."},
		{"INVALID_DEPOT_SIZE_D_FOR_DEPOT_D", "Ungültige Depotgröße %d für Depot %d."},
		{"OBJECT_NOT_CUMULATIVE", "Objekt ist nicht kumulierbar."},
		{"INVALID_COUNTER_D", "Ungültiger Zähler %d."},
		{"PASSED_TARGET_DOES_NOT_EXIST", "Übergebenes Ziel existiert nicht."},
		{"OBJECT_TYPES_D_AND_D_NOT_IDENTICAL", "Objekttypen %d und %d sind nicht identisch."},
		{"OBJECT_TYPE_D_NOT_CUMULATIVE", "Objekttyp %d ist nicht kumulierbar."},
		{"OBJECT_CONTAINS_0_PARTS", "Objekt enthält 0 Teile."},
		{"TARGET_OBJECT_CONTAINS_0_PARTS", "Zielobjekt enthält 0 Teile."},
		{"NEW_OBJECT_CONTAINS_MORE_THAN_100_PARTS", "Neues Objekt enthält mehr als 100 Teile."},
		{"ACTOR_IS_NULL", "Actor ist NULL."},
		{"VICTIM_DOES_NOT_EXIST", "Opfer existiert nicht."},
		{"POWER_IS_NEGATIVE__ACTOR_S", "Power ist negativ (Actor: %s)."},
		{"CONNECTION_IS_NOT_WILLING_TO_SEND", "Verbindung ist nicht sendewillig."},
		{"BUFFER_OF_CONNECTION_D_IS_FULL", "Puffer von Verbindung %d ist voll."},
		{"CONNECTION_IS_NOT_ONLINE", "Verbindung ist nicht online."},
		{"BUFFER_IS_FULL__PACKAGE_NOT_SENT", "Puffer ist voll. Paket wird nicht versandt."},
		{"INVALID_TEXT_LENGTH_D", "Ungültige Textlänge %d."},
		{"STRING_IS_NULL", "String ist NULL."},
		{"CREATURE_HAS_NO_CREATURE_OBJECT", "Kreatur hat kein Kreatur-Objekt."},
		{"NO_PLAYER_BELONGS_TO_THIS_CONNECTION", "Zu dieser Verbindung gehört kein Spieler."},
		{"CONTAINER_D_DOES_NOT_EXIST", "Container %d existiert nicht."},
		{"NAME_IS_NULL", "Name ist NULL."},
		{"TEXT_IS_TOO_LONG", "Text ist zu lang."},
		{"SENDER_IS_NULL", "Sender ist NULL."},
		{"INVALID_MODE_D", "Ungültiger Modus %d."},
		{"INVALID_CHANNEL_D", "Ungültiger Kanal %d."},
		{"CHANNEL_IS_GM_REQUEST_QUEUE", "Kanal ist GM-Request-Queue."},
		{"OBJECT_DOES_NOT_EXIST", "Objekt existiert nicht."},
		{"TEXT_IS_EMPTY", "Text ist leer."},
		{"OBJECT_NOT_IN_CRON_SYSTEM", "Objekt ist nicht im Cron-System eingetragen."},

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
