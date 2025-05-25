#include "map.hh"
#include "containers.hh"

// NOTE(fusion): This is used by hash table entries and sectors to tell whether
// they're currently loaded or swapped out to disk.
enum : uint8 {
	STATUS_FREE = 0,
	STATUS_LOADED = 1,
	STATUS_SWAPPED = 2,

	// TODO(fusion): It seems this is only used with the `NONE` entry in the
	// hash table. I haven't seen it used **yet** but It may have a purpose
	// aside from preventing swap outs.
	STATUS_PERMANENT = 255,
};

// NOTE(fusion): This is used to determine precedence order of different objects
// in a tile.
enum : int {
	PRIORITY_BANK = 0,
	PRIORITY_CLIP = 1,
	PRIORITY_BOTTOM = 2,
	PRIORITY_TOP = 3,
	PRIORITY_CREATURE = 4,
	PRIORITY_OTHER = 5,
};

static int OBCount;
static int SectorXMin;
static int SectorXMax;
static int SectorYMin;
static int SectorYMax;
static int SectorZMin;
static int SectorZMax;
static int RefreshedCylinders;
static int NewbieStartPositionX;
static int NewbieStartPositionY;
static int NewbieStartPositionZ;
static int VeteranStartPositionX;
static int VeteranStartPositionY;
static int VeteranStartPositionZ;

static matrix3d<TSector*> *Sector;
static TObjectBlock **ObjectBlock;
static TObject *FirstFreeObject;
static TObject **HashTableData;
static uint8 *HashTableType;
static uint32 HashTableSize;
static uint32 HashTableMask;
static uint32 HashTableFree;
static uint32 ObjectCounter;

static vector<TCronEntry> CronEntry(0, 256, 256);
static int CronHashTable[2047];
static int CronEntries;

static vector<TDepotInfo> DepotInfo(0, 4, 5);
static int Depots;

static vector<TMask> Mark(0, 4, 5);
static int Marks;

static TDynamicWriteBuffer HelpBuffer(0x10000);

// Map Init/Exit and Helpers
// =============================================================================
static void SwapObject(TWriteBinaryFile *File, Object Obj, uint32 FileNumber){
	ASSERT(Obj != NONE);

	// NOTE(fusion): Does it make sense to swap an object that isn't loaded? We
	// were originally calling `Object::exists` that would swap in the object's
	// sector if it was swapped out. We should probably have an assertion here.
	if(HashTableType[Obj.ObjectID & HashTableMask] != STATUS_LOADED){
		error("SwapObject: Object doesn't exists or is not currently loaded.\n");
		return;
	}

	TObject *Entry = HashTableData[Obj.ObjectID & HashTableMask];
	if(Entry->ObjectID != Obj.ObjectID){
		error("SwapObject: Übergebenes Objekt existiert nicht.\n");
		return;
	}

	File->writeBytes((const uint8*)Entry, sizeof(TObject));
	if(Entry->Type.getFlag(CONTAINER) || Entry->Type.getFlag(CHEST)){
		Object Current = Obj.getAttribute(CONTENT);
		while(Current != NONE){
			Object Next = Current.getNextObject();
			SwapObject(File, Current, FileNumber);
			Current = Next;
		}
	}

	PutFreeObjectSlot(Entry);
	HashTableType[EntryIndex] = STATUS_SWAPPED;
	HashTableData[EntryIndex] = (TObject*)FileNumber;
}

static void SwapSector(void){
	static uint32 FileNumber = 0;

	TSector *Oldest = NULL;
	int OldestSectorX = 0;
	int OldestSectorY = 0;
	int OldestSectorZ = 0;
	uint32 OldestTimeStamp = RoundNr + 1;

	ASSERT(Sector != NULL);
	for(int SectorZ = SectorZMin; SectorZ <= SectorZMax; SectorZ += 1)
	for(int SectorY = SectorYMin; SectorY <= SectorYMax; SectorY += 1)
	for(int SectorX = SectorXMin; SectorX <= SectorXMax; SectorX += 1){
		TSector *CurrentSector = *Sector->at(SectorX, SectorY, SectorZ);
		if(CurrentSector != NULL
				&& CurrentSector->Status == STATUS_LOADED
				&& CurrentSector->TimeStamp < OldestTimeStamp){
			Oldest = CurrentSector;
			OldestSectorX = SectorX;
			OldestSectorY = SectorY;
			OldestSectorZ = SectorZ;
			OldestTimeStamp = CurrentSector->TimeStamp;
		}
	}

	if(Oldest == NULL){
		error("FATAL ERROR in SwapSector: Es kann kein Sektor ausgelagert werden.\n");
		abort();
	}

	char FileName[4096];
	do{
		FileNumber += 1; // NOTE(fusion): Let it wrap naturally.
		snprintf(FileName, sizeof(FileName), "%s/%010u.swp", SAVEPATH, FileNumber);
	}while(FileExists(FileName));

	TWriteBinaryFile File;
	try{
		File.open(FileName);
		print(2, "Lagere Sektor %d/%d/%d aus...\n", OldestSectorX, OldestSectorY, OldestSectorZ);
		File.writeQuad((uint32)OldestSectorX);
		File.writeQuad((uint32)OldestSectorY);
		File.writeQuad((uint32)OldestSectorZ);
		// TODO(fusion): I think tiles are stored in column major order but it doesn't
		// really matter as long as optimize for sequential access.
		for(int X = 0; X < 32; X += 1){
			for(int Y = 0; Y < 32; Y += 1){
				SwapObject(&File, Oldest->MapCon[X][Y], FileNumber);
			}
		}
		Oldest->Status = STATUS_SWAPPED;
		File.close();
	}catch(const char *str){
		error("FATAL ERROR in SwapSector: Kann Datei \"%s\" nicht anlegen.\n", FileName);
		error("# Fehler: %s\n", str);
		abort();
	}
}

static void UnswapSector(uint32 FileNumber){
	char FileName[4096];
	snprintf(FileName, sizeof(FileName), "%s/%010u.swp", SAVEPATH, FileNumber);

	TReadBinaryFile File;
	try{
		File.open(FileName);
		int SectorX = (int)File.readQuad();
		int SectorY = (int)File.readQuad();
		int SectorZ = (int)File.readQuad();
		print(2, "Lagere Sector %d/%d/%d ein...\n", SectorX, SectorY, SectorZ);

		ASSERT(Sector != NULL);
		TSector *LoadingSector = *Sector->at(SectorX, SectorY, SectorZ);
		if(LoadingSector == NULL){
			error("UnswapSector: Sektor %d/%d/%d existiert nicht.\n", SectorX, SectorY, SectorZ);
			File.close();
			return;
		}

		if(LoadingSector->Status != STATUS_SWAPPED){
			error("UnswapSector: Sektor %d/%d/%d ist nicht ausgelagert.\n", SectorX, SectorY, SectorZ);
			File.close();
			return;
		}

		int Size = File.getSize();
		while(File.getPosition() < Size){
			TObject Entry;
			File.readBytes((uint8*)&Entry, sizeof(TObject));

			uint32 EntryIndex = Entry.ObjectID & HashTableMask;
			if(HashTableType[EntryIndex] == STATUS_SWAPPED){
				// NOTE(fusion): Make sure we only allocate the object if we confirm
				// its status. The original code would call `readBytes` on the result
				// from `GetFreeObjectSlot()` directly and would then leak it if the
				// entry status was not `STATUS_SWAPPED`.
				TObject *EntryPointer = GetFreeObjectSlot();
				*EntryPointer = Entry;
				HashTableData[EntryIndex] = EntryPointer;
				HashTableType[EntryIndex] = STATUS_LOADED;
			}else{
				error("UnswapSector: Objekt %u existiert schon.\n", Entry.ObjectID);
			}
		}
		LoadingSector->Status = STATUS_LOADED;
		File.close();
		unlink(FileName);
	}catch(const char *str){
		error("FATAL ERROR in UnswapSector: Kann Datei \"%s\" nicht lesen.\n", FileName);
		error("# Fehler: %s\n", str);
		abort();
	}
}

static void DeleteSwappedSectors(void){
	DIR *SwapDir = opendir(SAVEPATH);
	if(SwapDir == NULL){
		error("DeleteSwappedSectors: Unterverzeichnis %s nicht gefunden\n", SAVEPATH);
		return;
	}

	char FilePath[4096];
	while(dirent *DirEntry = readdir(SwapDir)){
		if(DirEntry->d_type == DT_REG){
			// NOTE(fusion): `DirEntry->d_name` will only contain the filename
			// so we need to assemble the actual file path (relative in this
			// case) to properly unlink it. Windows has the same behavior with
			// its `FindFirstFile`/`FindNextFile` API.
			const char *FileExt = findLast(DirEntry->d_name, '.');
			if(FileExt != NULL && strcmp(FileExt, ".swp") == 0){
				snprintf(FilePath, sizeof(FilePath), "%s/%s", SAVEPATH, DirEntry->d_name);
				unlink(FilePath);
			}
		}
	}

	closedir(SwapDir);
}

static void LoadObjects(TReadScriptFile *Script, TWriteStream *Stream, bool Skip){
	int Depth = 1;
	bool ParseAttribute = false;
	Script->readSymbol('{');
	Script->nextToken();
	while(true){
		if(!ParseAttribute){
			if(Script->Token != SPECIAL){
				int TypeID = Script->getNumber();
				if(!ObjectTypeExists(TypeID)){
					Script->error("unknown object type");
				}

				if(!Skip){
					Stream->writeWord((uint16)TypeID);
				}

				ParseAttribute = true;
			}else{
				char Special = Script->getSpecial();
				if(Special == '}'){
					if(!Skip){
						Stream->writeWord(0xFFFF);
					}

					Depth -= 1;
					if(Depth <= 0){
						break;
					}
				}else if(Special != ','){
					Script->error("expected comma");
				}
			}
			Script->nextToken();
		}else{
			if(Script->Token != SPECIAL){
				int Attribute = GetInstanceAttributeByName(Script->getIdentifier());
				if(Attribute == -1){
					Script->error("unknown attribute");
				}

				Script->readSymbol('=');
				if(!Skip){
					Stream->writeByte((uint8)Attribute);
				}

				if(Attribute == CONTENT){
					Script->readSymbol('{');
					Depth += 1;
					ParseAttribute = false;
				}else if(Attribute == TEXTSTRING || Attribute == EDITOR){
					const char *String = Script->readString();
					if(!Skip){
						Stream->writeString(String);
					}
				}else{
					int Number = Script->readNumber();
					if(!Skip){
						Stream->writeQuad((uint32)Number);
					}
				}

				Script->nextToken();
			}else{
				// NOTE(fusion): Attributes are key-value pairs separated by whitespace.
				// If we find a special token (probably ',' or '}'), then we're done
				// parsing attributes for the current object.
				if(!Skip){
					Stream->writeByte(0xFF);
				}
				ParseAttribute = false;
			}
		}
	}
}

static void LoadObjects(TReadStream *Stream, Object Con){
	int Depth = 1;
	Object Obj = NONE;
	while(true){
		if(Obj == NONE){
			int TypeID = (int)Stream->readWord();
			if(TypeID != 0xFFFF){
				ObjectCounter += 1;
				ObjectType ObjType(TypeID);

				if(ObjType.isBodyContainer()){
					Obj = GetContainerObject(Con, (TypeID - 1));
				}else{
					Obj = AppendObject(Con, ObjType);
				}
			}else{
				Obj = Con;
				Con = Con.getContainer();
				Depth -= 1;
				if(Depth <= 0){
					break;
				}
			}
		}else{
			int Attribute = (int)Stream->readByte();
			if(Attribute != 0xFF){
				if(Attribute == CONTENT){
					Con = Obj;
					Obj = NONE;
					Depth += 1;
				}else if(Attribute == TEXTSTRING || Attribute == EDITOR){
					uint32 Value = AddDynamicString(Stream->readString());
					Obj.setAttribute(Attribute, Value);
				}else if(Attribute != REMAININGEXPIRETIME){
					uint32 Value = Stream->readQuad();
					Obj.setAttribute(Attribute, Value);
				}else{
					uint32 Value = Stream->readQuad();
					if(Value != 0){
						CronChange(Obj, (int)Value);
					}
				}
			}else{
				Obj = NONE;
			}
		}
	}
}

static void InitSector(int SectorX, int SectorY, int SectorZ){
	ASSERT(Sector);
	if(*Sector->at(SectorX, SectorY, SectorZ) != NULL){
		error("InitSector: Sektor %d/%d/%d existiert schon.\n", SectorX, SectorY, SectorZ);
		return;
	}

	TSector *NewSector = (TSector*)malloc(sizeof(TSector));
	for(int X = 0; X < 32; X += 1){
		for(int Y = 0; Y < 32; Y += 1){
			Object MapCon = CreateObject();
			// NOTE(fusion): `Attributes[0]` is probably the object id of the
			// first object in the container.
			Access(MapCon)->Attributes[1] = SectorX * 32 + X;
			Access(MapCon)->Attributes[2] = SectorY * 32 + Y;
			Access(MapCon)->Attributes[3] = SectorZ;
			NewSector->MapCon[X][Y] = MapCon;
		}
	}
	NewSector->TimeStamp = RoundNr;
	NewSector->Status = STATUS_LOADED;
	NewSector->MapFlags = 0;

	*Sector->at(SectorX, SectorY, SectorZ) = NewSector;
}

static void LoadSector(const char *FileName, int SectorX, int SectorY, int SectorZ){
	if(SectorX < SectorXMin || SectorXMax < SectorX
			|| SectorY < SectorYMin || SectorYMax < SectorY
			|| SectorZ < SectorZMin || SectorZMax < SectorZ){
		return;
	}

	InitSector(SectorX, SectorY, SectorZ);

	ASSERT(Sector != NULL);
	TSector *LoadingSector = *Sector->at(SectorX, SectorY, SectorZ);
	ASSERT(LoadingSector != NULL);

	TReadScriptFile Script;
	try{
		Script.open(FileName);
		print(1, "Lade Sektor %d/%d/%d ...\n", SectorX, SectorY, SectorZ);

		int OffsetX = -1;
		int OffsetY = -1;
		while(true){
			Script.nextToken();
			if(Script.Token == ENDOFFILE){
				Script.close();
				return;
			}

			if(Script.Token == SPECIAL && Script.getSpecial() == ','){
				continue;
			}

			if(Script.Token == BYTES){
				uint8 *SectorOffset = Script.getBytesequence();
				OffsetX = (int)SectorOffset[0];
				OffsetY = (int)SectorOffset[1];
				Script.readSymbol(':');
				continue;
			}

			if(Script.Token != IDENTIFIER){
				Script.error("next map point expected");
			}

			if(OffsetX == -1 || OffsetY == -1){
				Script.error("coordinate expected");
			}

			const char *Identifier = Script.getIdentifier();
			if(strcmp(Identifier, "refresh") == 0){
				LoadingSector->MapFlags |= 1;
				AccessObject(LoadingSector->MapCon[OffsetX][OffsetY])->Attributes[3] |= 0x100;
			}else if(strcmp(Identifier, "nologout") == 0){
				LoadingSector->MapFlags |= 2;
				AccessObject(LoadingSector->MapCon[OffsetX][OffsetY])->Attributes[3] |= 0x200;
			}else if(strcmp(Identifier, "protectionzone") == 0){
				LoadingSector->MapFlags |= 4;
				AccessObject(LoadingSector->MapCon[OffsetX][OffsetY])->Attributes[3] |= 0x400;
			}else if(strcmp(Identifier, "content") == 0){
				Script.readSymbol('=');
				HelpBuffer.reset();
				LoadObjects(&Script, &HelpBuffer, false);
				TReadBuffer ReadBuffer(HelpBuffer);
				LoadObjects(&ReadBuffer, LoadingSector->MapCon[OffsetX][OffsetY]);
			}else{
				Script.error("unknown map flag");
			}
		}
	}catch(const char *str){
		error("LoadSector: Kann Datei \"%s\" nicht lesen.\n", FileName);
		error("# Fehler: %s\n", str);
		throw "Cannot load sector";
	}
}

static void LoadMap(void){
	DIR *MapDir = opendir(MAPPATH);
	if(MapDir == NULL){
		error("LoadMap: Unterverzeichnis %s nicht gefunden\n", MAPPATH);
		throw "Cannot load map";
	}

	print(1, "Lade Karte ...\n");
	ObjectCounter = 0;

	char FilePath[4096];
	while(dirent *DirEntry = readdir(MapDir)){
		if(DirEntry->d_type == DT_REG){
			// NOTE(fusion): See note in `DeleteSwappedSectors`.
			const char *FileExt = findLast(DirEntry->d_name, '.');
			if(FileExt != NULL && strcmp(FileExt, ".sec") == 0){
				int SectorX, SectorY, SectorZ;
				if(sscanf("%d-%d-%d.sec", &SectorX, &SectorY, &SectorZ) == 3){
					snprintf(FilePath, sizeof(FilePath), "%s/%s", SAVEPATH, DirEntry->d_name);
					LoadSector(FilePath, SectorX, SectorY, SectorZ);
					SectorCounter += 1;
				}
			}
		}
	}

	closedir(MapDir);
	print(1, "%d Sektoren geladen.\n", SectorCounter);
	print(1, "%d Objekte geladen.\n", ObjectCounter);
}

//SaveObjects
//SaveObjects

void SaveSector(char *FileName, int SectorX, int SectorY, int SectorZ){
	ASSERT(Sector);
	TSector *SavingSector = *Sector->at(SectorX, SectorY, SectorZ);
	if(!SavingSector){
		return;
	}

	bool Empty = true;
	TWriteScriptFile Script;
	try{
		Script.open(FileName);
		print(1, "Speichere Sektor %d/%d/%d ...\n", SectorX, SectorY, SectorZ);

		Script.writeText("# Tibia - graphical Multi-User-Dungeon");
		Script.writeLn();
		Script.writeText("# Data for sector ");
		Script.writeNumber(SectorX);
		Script.writeText("/");
		Script.writeNumber(SectorY);
		Script.writeText("/");
		Script.writeNumber(SectorZ);
		Script.writeLn();
		Script.writeLn();

		for(int X = 0; X < 32; X += 1){
			for(int Y = 0; Y < 32; Y += 1){
				Object First = Object(SavingSector->MapCon[X][Y].getAttribute(CONTENT));
				uint8 Flags = GetMapContainerFlags(SavingSector->MapCon[X][Y]);
				if(First != NONE || Flags != 0){
					Script.writeNumber(X);
					Script.writeText("-");
					Script.writeNumber(Y);
					Script.writeText(": ");

					int AttrCount = 0;

					if(Flags & 1){
						if(AttrCount > 0){
							Script.writeText(", ");
						}
						Script.writeText("Refresh");
						AttrCount += 1;
					}

					if(Flags & 2){
						if(AttrCount > 0){
							Script.writeText(", ");
						}
						Script.writeText("NoLogout");
						AttrCount += 1;
					}

					if(Flags & 4){
						if(AttrCount > 0){
							Script.writeText(", ");
						}
						Script.writeText("ProtectionZone");
						AttrCount += 1;
					}

					if(First != NONE){
						if(AttrCount > 0){
							Script.writeText(", ");
						}
						Script.writeText("Content=");
						HelpBuffer.reset();
						SaveObjects(First, &HelpBuffer, false);
						TReadBuffer ReadBuffer(HelpBuffer);
						SaveObjects(&ReadBuffer, &Script);
						AttrCount += 1;
					}

					Script.writeLn();
					Empty = false;
				}
			}
		}

		Script.close();
		if(Empty){
			error("SaveSector: Sektor %d/%d/%d ist leer.\n", SectorX, SectorY, SectorZ);
			unlink(FileName);
		}
	}catch(const char *str){
		error("SaveSector: Kann Datei %s nicht schreiben.\n", FileName);
		error("# Fehler: %s\n", str);
	}
}

static void SaveMap(void){
	// NOTE(fusion): I guess this could happen if we're already saving the map
	// and a signal causes `exit` to execute cleanup functions registered with
	// `atexit`, among which is `ExitAll` which may call `SaveMap` throught
	// `ExitMap`.
	static bool SavingMap = false;
	if(SavingMap){
		error("SaveMap: Karte wird schon gespeichert.\n");
		return;
	}

	SavingMap = true;
	print(1, "Speichere Karte ...\n");
	ObjectCounter = 0;

	char FileName[4096];
	for(int SectorZ = SectorZMin; SectorZ <= SectorZMax; SectorZ += 1)
	for(int SectorY = SectorYMin; SectorY <= SectorYMax; SectorY += 1)
	for(int SectorX = SectorXMin; SectorX <= SectorXMax; SectorX += 1){
		snprintf(FileName, sizeof(FileName), "%s/%04d-%04d-%02d.sec",
				MAPPATH, SectorX, SectorY, SectorZ);
		SaveSector(FileName, SectorX, SectorY, SectorZ);
	}

	print(1, "%d Objekte gespeichert.\n", ObjectCounter);
	SavingMap = false;
}

static void ResizeHashTable(void){
	uint32 OldSize = HashTableSize;
	uint32 NewSize = OldSize * 2;
	ASSERT(ISPOW2(OldSize));
	ASSERT(NewSize > OldSize);

	// TODO(fusion): See note below.
	error("FATAL ERROR in ResizeHashTable: Resizing the object hash table is"
			" currently disabled. You may increase `Objects` in the map config"
			" from %d to %d, to achieve the same effect.", OldSize, NewSize);
	abort();

	error("INFO: HashTabelle zu klein. Größe wird verdoppelt auf %d.\n", NewSize);

	uint32 NewMask = NewSize - 1;
	TObject **NewData = (TObject**)malloc(NewSize * sizeof(TObject*));
	uint8 *NewType = (uint8*)malloc(NewSize * sizeof(uint8));
	memset(NewType, 0, NewSize * sizeof(uint8));

	// TODO(fusion): This rehash loop doesn't make a lot of sense. It wants to
	// access all existing objects but doing so would cause all sectors to be
	// swapped in at some time or another. This may be bad for performance but
	// the real problem is that `UnswapSector` may swap out some other sector
	// whose objects were already put into `NewType` and `NewData`, causing
	// multiple object ids to reference the same `TObject`.
	//	Looking at some of the logs included with this executable, it doesn't
	// look like this function was ever called which explains why it wasn't fixed
	// earlier.
	//	It would be possible to do this rehashing without swapping anything out,
	// if we stored ObjectID somewhere (maybe `HashTableData`). Doubling the size
	// of the table means there are two possible indices for each previous entry,
	// and we can only determine which one to actually move it with the ObjectID.

	NewType[0] = STATUS_PERMANENT;
	NewData[0] = HashTableData[0];
	for(uint32 i = 1; i < NewSize; i += 1){
		if(HashTableType[i] != STATUS_FREE){
			if(HashTableType[i] == STATUS_SWAPPED){
				UnswapSector((uintptr)HashTableData[i]);
			}

			if(HashTableType[i] == STATUS_LOADED){
				TObject *Entry = HashTableData[i];
				NewType[Entry->ObjectID & NewMask] = STATUS_LOADED;
				NewData[Entry->ObjectID & NewMask] = Entry;
			}else{
				error("ResizeHashTable: Fehler beim Reorganisieren der HashTabelle.\n");
			}
		}
	}

	free(HashTableData);
	free(HashTableType);

	HashTableData = NewData;
	HashTableType = NewType;
	HashTableSize = NewSize;
	HashTableMask = NewMask;
	HashTableFree += (NewSize - OldSize);
}

static TObject *GetFreeObjectSlot(void){
	if(FirstFreeObject == NULL){
		SwapSector();
	}

	TObject *Entry = FirstFreeObject;
	if(Entry == NULL){
		error("GetFreeObjectSlot: Kein freier Platz mehr.\n");
		return NULL;
	}

	// NOTE(fusion): The next object pointer was originally stored in `Entry->NextObject.ObjectID`
	// which is a problem when compiling in 64 bits mode. For this reason, I've changed it to be
	// stored at the beggining of `TObject`.
	FirstFreeObject = *((TObject**)Entry);

	memset(Entry, 0, sizeof(TObject));

	return Entry;
}

static void PutFreeObjectSlot(TObject *Entry){
	if(Entry == NULL){
		error("PutFreeObjectSlot: Entry ist NULL.\n");
		return;
	}

	// NOTE(fusion): See note in `GetFreeObjectSlot`, just above.
	*((TObject**)Entry) = FirstFreeObject;
	FirstFreeObject = Entry;
}

static void ReadMapConfig(void){
	OBCount = 0xA0000;
	SectorXMin = 1000;
	SectorXMax = 1015;
	SectorYMin = 1000;
	SectorYMax = 1015;
	SectorZMin = 0;
	SectorZMax = 15;
	RefreshedCylinders = 1;
	NewbieStartPositionX = 0;
	NewbieStartPositionY = 0;
	NewbieStartPositionZ = 0;
	VeteranStartPositionX = 0;
	VeteranStartPositionY = 0;
	VeteranStartPositionZ = 0;
	HashTableSize = 0x100000;
	Marks = 0;
	Depots = 0;

	char FileName[4096];
	snprintf(FileName, sizeof(FileName), "%s/map.dat", DATAPATH);

	TReadScriptFile Script;
	Script.open(FileName);
	while(true){
		Script.nextToken();
		if(Script.Token == ENDOFFILE){
			Script.close():
			break;
		}

		char Identifier[MAX_IDENT_LENGTH];
		strcpy(Identifier, Script.readIdentifier());
		Script.readSymbol('=');

		if(strcmp(Identifier, "sectorxmin") == 0){
			SectorXMin = Script.readNumber();
		}else if(strcmp(Identifier, "sectorxmax") == 0){
			SectorXMax = Script.readNumber();
		}else if(strcmp(Identifier, "sectorymin") == 0){
			SectorYMin = Script.readNumber();
		}else if(strcmp(Identifier, "sectorymax") == 0){
			SectorYMax = Script.readNumber();
		}else if(strcmp(Identifier, "sectorzmin") == 0){
			SectorZMin = Script.readNumber();
		}else if(strcmp(Identifier, "sectorzmax") == 0){
			SectorZMax = Script.readNumber();
		}else if(strcmp(Identifier, "refreshedcylinders") == 0){
			RefreshedCylinders = Script.readNumber();
		}else if(strcmp(Identifier, "objects") == 0){
			HashTableSize = (uint32)Script.readNumber();
		}else if(strcmp(Identifier, "cachesize") == 0){
			OBCount = Script.readNumber();
		}else if(strcmp(Identifier, "depot") == 0){
			int DepotIndex = 0;
			TDepotInfo TempInfo = {};
			Script.readSymbol('(');
			DepotIndex = Script.readNumber();
			Script.readSymbol(',');
			const char *Town = Script.readString();
			if(strlen(Town) >= NARRAY(TempInfo.Town)){
				Script.error("town name too long");
			}
			strcpy(TempInfo.Town, Town);
			Script.readSymbol(',');
			TempInfo.Size = Script.readNumber();
			Script.readSymbol(')');
			*DepotInfo.at(DepotIndex) = TempInfo;
		}else if(strcmp(Identifier, "mark") == 0){
			TMark TempMark = {};
			Script.readSymbol('(');
			const char *Name = Script.readString();
			if(strlen(Name) >= NARRAY(TempMark.Name)){
				Script.error("mark name too long");
			}
			strcpy(TempMark.Name, Name);
			Script.readSymbol(',');
			Script.readCoordinate(&TempMark.x, &TempMark.y, &TempMark.z);
			Script.readSymbol(')');
			*Mark.at(Marks) = TempMark;
			Marks += 1;
		}else if(strcmp(Identifier, "newbiestart") == 0){
			 Script.readCoordinate(
					&NewbieStartPositionX,
					&NewbieStartPositionY,
					&NewbieStartPositionZ);
		}else if(strcmp(Identifier, "veteranstart") == 0){
			 Script.readCoordinate(
					&VeteranStartPositionX,
					&VeteranStartPositionY,
					&VeteranStartPositionZ);
		}else{
			// TODO(fusion):
			//error("Unknown map configuration key \"%s\"", Identifier);
		}
	}

	// NOTE(fusion): If each sector is 32 x 32 tiles and the whole world is
	// 65535 x 65535 tiles, then the maximum number of sectors is 65535 / 32
	// which is the 2047 used below. We should probably define these contants.
	//	Notice that sectors at the edge of the XY-plane are also considered
	// invalid, probably to add some buffer to avoid wrapping or other types
	// of problems.

	if(SectorXMin <= 0){
		throw "illegal value for SectorXMin";
	}

	if(SectorXMax >= 2047){
		throw "illegal value for SectorXMax";
	}

	if(SectorYMin <= 0){
		throw "illegal value for SectorYMin";
	}

	if(SectorYMax >= 2047){
		throw "illegal value for SectorYMax";
	}

	if(SectorZMin < 0){
		throw "illegal value for SectorZMin";
	}

	if(SectorZMax > 15){
		throw "illegal value for SectorZMax";
	}

	if(SectorXMin > SectorXMax){
		throw "SectorXMin is greater than SectorXMax";
	}

	if(SectorYMin > SectorYMax){
		throw "SectorYMin is greater than SectorYMax";
	}

	if(SectorZMin > SectorZMax){
		throw "SectorZMin is greater than SectorZMax";
	}

	// TODO(fusion): Just align up from whatever value we got? And use `ObjectsPerBlock`?
	if(OBCount % 32768 != 0){
		throw "CacheSize must be a multiple of 32768";
	}

	if(OBCount <= 0){
		throw "illegal value for CacheSize";
	}

	OBCount /= 32768;

	if(HashTableSize <= 0){
		throw "illegal value for Objects";
	}

	if(!ISPOW2(HashTableSize)){
		throw "Objects must be a power of 2";
	}

	if(NewbieStartPositionX == 0){
		throw "no start position for newbies specified";
	}

	if(VeteranStartPositionX == 0){
		throw "no start position for veterans specified";
	}
}

void InitMap(void){
	ReadMapConfig();

	Sector = new matrix3d<TSector*>(SectorXMin, SectorXMax,
			SectorYMin, SectorYMax, SectorZMin, SectorZMax, NULL);

	DeleteSwappedSectors();

	// NOTE(fusion): Object storage is FIXED and determined at startup.
	ObjectBlock = (TObjectBlock**)malloc(OBCount * sizeof(TObjectBlock*));
	for(int i = 0; i < OBCount; i += 1){
		ObjectBlock[i] = (TObjectBlock*)malloc(sizeof(TObjectBlock));
	}

	// NOTE(fusion): Setup free object list. See note in `GetFreeObjectSlot`.
	constexpr int ObjectsPerBlock = NARRAY(TObjectBlock::Object);
	for(int i = 0; i < OBCount; i += 1){
		for(int j = 0; j < ObjectsPerBlock; j += 1){
			*((TObject**)&ObjectBlock[i]->Object[j]) = &ObjectBlock[i]->Object[j + 1];
		}

		if(i < (OBCount - 1)){
			// NOTE(fusion): Link last entry of this block, with the first entry of the next one.
			*((TObject**)&ObjectBlock[i]->Object[ObjectsPerBlock - 1]) = &ObjectBlock[i + 1]->Object[0];
		}else{
			// NOTE(fusion): End of free object list.
			*((TObject**)&ObjectBlock[i]->Object[ObjectsPerBlock - 1]) = NULL;
		}
	}
	FirstFreeObject = &ObjectBlock[0]->Object[0];

	// NOTE(fusion): Initialize object hash table.
	ASSERT(ISPOW2(HashTableSize));
	HashTableMask = HashTableSize - 1;
	HashTableData = (TObject**)malloc(HashTableSize * sizeof(TObject*));
	HashTableType = (uint8*)malloc(HashTableSize * sizeof(uint8));
	memset(HashTableType, 0, HashTableSize * sizeof(uint8));
	HashTableFree = HashTableSize - 1;
	// NOTE(fusion): This is probably reserved for `NONE`.
	HashTableType[0] = STATUS_PERMANENT;
	HashTableData[0] = GetFreeObjectSlot();

	// NOTE(fusion): Initialize cron hash table (whatever that is).
	for(int i = 0; i < NARRAY(CronHashTable); i += 1){
		CronHashTable[i] = -1;
	}
	CronEntries = 0;

	LoadMap();
}

void ExitMap(bool Save){
	if(Save){
		SaveMap();
	}

	free(HashTableData);
	free(HashTableType);

	for(int i = 0; i < OBCount; i += 1){
		free(ObjectBlock[i]);
	}
	free(ObjectBlock);

	if(Sector != NULL){
		for(int SectorZ = SectorZMin; SectorZ <= SectorZMax; SectorZ += 1)
		for(int SectorY = SectorYMin; SectorY <= SectorYMax; SectorY += 1)
		for(int SectorX = SectorXMin; SectorX <= SectorXMax; SectorX += 1){
			TSector *CurrentSector = *Sector->at(SectorX, SectorY, SectorZ);
			if(CurrentSector != NULL){
				free(CurrentSector);
			}
		}
		delete Sector;
	}

	DeleteSwappedSectors();
}

// TODO(fusion): For some reason this is not a member of `Object`?
static TObject *AccessObject(Object Obj){
	if(Obj == NONE){
		error("AccessObject: Ungültige Objektnummer Null.\n");
		return HashTableData[0];
	}

	uint32 Index = Obj.ObjectID
	if(HashTableType[Index] == STATUS_SWAPPED){
		UnswapSector((uintptr)HashTableData[Index]);
	}

	if(HashTableType[Index] == STATUS_LOADED && HashTableData[Index]->ObjectID == Obj.ObjectID){
		return HashTableData[Index];
	}else{
		return HashTableData[0];
	}
}

static Object CreateObject(void){
	static uint32 NextObjectID = 1;

	// NOTE(fusion): Load factor of 1/16.
	if(HashTableFree < (HashTableSize / 16)){
		ResizeHashTable();
	}

	// NOTE(fusion): If we properly manage the load factor and the number of free
	// entries, we should have no trouble finding an empty table entry here. Use
	// a bounded loop nevertheless, just to be safe.
	for(uint32 i = 0; i < HashTableSize; i += 1){
		if(HashTableType[NextObjectID & HashTableMask] == 0)
			break;
		NextObjectID += 1;
	}
	ASSERT(HashTableType[NextObjectID & HashTableMask] == 0);

	TObject *Entry = GetFreeObjectSlot();
	if(Entry == NULL){
		error("CreateObject: Kann Objekt nicht anlegen.\n");
		return NONE;
	}

	Entry->ObjectID = NextObjectID;
	HashTableData[NextObjectID & HashTableMask] = Entry;
	HashTableType[NextObjectID & HashTableMask] = STATUS_LOADED;
	HashTableFree -= 1;
	IncrementObjectCounter();

	return Object(NextObjectID);
}

// DestroyObject
// DeleteObject

// Object
// =============================================================================
bool Object::exists(void){
	if(*this == NONE){
		return false;
	}

	uint32 Index = this->ObjectID & HashTableMask;
	if(HashTableType[Index] == STATUS_SWAPPED){
		UnswapSector(HashTableData[Index]);
	}

	return HashTableType[Index] == STATUS_LOADED
		&& HashTableData[Index]->ObjectID == this->ObjectID;
}

ObjectType Object::getObjectType(void){
	return AccessObject(*this)->Type;
}

void Object::setObjectType(ObjectType Type){
	AccessObject(*this)->Type = Type;
}

Object Object::getNextObject(void){
	return AccessObject(*this)->NextObject;
}

void Object::setNextObject(Object NextObject){
	AccessObject(*this)->NextObject = NextObject;
}

Object Object::getContainer(void){
	return AccessObject(*this)->Container;
}

void Object::setContainer(Object Container){
	AccessObject(*this)->Container = Container;
}

uint32 Object::getCreatureID(void){
	// TODO(fusion): We call `AccessObject` once in `getObjectType` then again
	// after checking the TypeID, when we could call it once to check both type
	// and access `Attributes[1]`.

	if(!this->getObjectType().isCreatureContainer()){
		error("Object::getCreatureID: Objekt ist keine Kreatur.\n");
		return 0;
	}

	return AccessObject(*this)->Attributes[1];
}

uint32 Object::getAttribute(INSTANCEATTRIBUTE Attribute){
	ObjectType ObjType = this->getObjectType();
	int AttributeOffset = ObjType.getAttributeOffset(Attribute);
	if(AttributeOffset == -1){
		error("Object::getAttribute: Flag für Attribut %d bei Objekttyp %d nicht gesetzt.\n",
				Attribute, ObjType.TypeID);
		return 0;
	}

	if(AttributeOffset < 0 || AttributeOffset >= NARRAY(TObject::Attributes)){
		error("Object::getAttribute: Ungültiger Offset %d für Attribut %d bei Objekttyp %d.\n",
				AttributeOffset, Attribute, ObjType.TypeID);
		return 0;
	}

	return AccessObject(*this)->Attributes[AttributeOffset];
}

void Object::setAttribute(INSTANCEATTRIBUTE Attribute, uint32 Value){
	ObjectType ObjType = this->getObjectType();
	int AttributeOffset = ObjType.getAttributeOffset(Attribute);
	if(AttributeOffset == -1){
		error("Object::setAttribute: Flag für Attribut %d bei Objekttyp %d nicht gesetzt.\n",
				Attribute, ObjType.TypeID);
		return;
	}

	if(AttributeOffset < 0 || AttributeOffset >= NARRAY(TObject::Attributes)){
		error("Object::setAttribute: Ungültiger Offset %d für Attribut %d bei Objekttyp %d.\n",
				AttributeOffset, Attribute, ObjType.TypeID);
		return;
	}

	if(Value == 0){
		if(Attribute == AMOUNT || Attribute == POOLLIQUIDTYPE || Attribute == CHARGES){
			Value = 1;
		}else if(Attribute == REMAININGUSES){
			Value = ObjType.getAttribute(TOTALUSES);
		}
	}

	AccessObject(*this)->Attributes[AttributeOffset] = Value;
}

// Object Functions
// =============================================================================
void ChangeObject(Object Obj, ObjectType NewType){
	if(!Obj.exists()){
		error("ChangeObject: Übergebenes Objekt existiert nicht.\n");
		return;
	}

	uint32 Amount = 0;
	uint32 SavedExpireTime = 0;
	int Delay = -1;

	ObjectType OldType = Obj.getObjectType();
	if(!OldType.isMapContainer()){
		if(OldType.getFlag(CUMULATIVE)){
			Amount = Obj.getAttribute(AMOUNT);
		}

		if(OldType.getFlag(EXPIRE)){
			SavedExpireTime = CronStop(Obj);
			if(NewType.getFlag(CUMULATIVE)){
				Amount = 1;
			}
		}

		if(OldType.getFlag(TEXT) && !NewType.getFlag(TEXT)){
			DeleteDynamicString(Obj.getAttribute(TEXTSTRING));
			Obj.setAttribute(TEXTSTRING, 0);

			DeleteDynamicString(Obj.getAttribute(EDITOR));
			Obj.setAttribute(EDITOR, 0);
		}

		if(OldType.getFlag(EXPIRESTOP)){
			uint32 Value = Obj.getAttribute(SAVEDEXPIRETIME);
			if(Value != 0){
				Delay = (int)Value;
			}
		}

		if(OldType.getFlag(MAGICFIELD)){
			Obj.setAttribute(RESPONSIBLE, 0);
		}
	}

	Obj.setObjectType(NewType);

	if(NewType.getFlag(CUMULATIVE)){
		if(Amount == 0){
			Amount = 1;
		}
		Obj.setAttribute(AMOUNT, Amount);
	}

	if(NewType.getFlag(RUNE)){
		if(Obj.getAttribute(CHARGES) == 0){
			Obj.setAttribute(CHARGES, 1);
		}
	}

	if(NewType.getFlag(LIQUIDPOOL)){
		if(Obj.getAttribute(POOLLIQUIDTYPE) == 0){
			Obj.setAttribute(POOLLIQUIDTYPE, 1); // LIQUID_TYPE_NONE (?)
		}
	}

	if(NewType.getFlag(WEAROUT)){
		if(Obj.getAttribute(REMAININGUSES) == 0){
			Obj.setAttribute(REMAININGUSES, NewType.getAttribute(TOTALUSES));
		}
	}

	if(NewType.getFlag(EXPIRESTOP)){
		Obj.setAttribute(SAVEDEXPIRETIME, SavedExpireTime);
	}

	CronExpire(Obj, Delay);
}

int GetObjectPriority(Object Obj){
	if(!Obj.exists()){
		error("GetObjectPriority: Übergebenes Objekt existiert nicht.\n");
		return -1;
	}

	int ObjPriority;
	ObjectType ObjType = Obj.getObjectType();
	if(ObjType.getFlag(BANK)){
		ObjPriority = PRIORITY_BANK;
	}else if(ObjType.getFlag(CLIP)){
		ObjPriority = PRIORITY_CLIP;
	}else if(ObjType.getFlag(BOTTOM)){
		ObjPriority = PRIORITY_BOTTOM;
	}else if(ObjType.getFlag(TOP)){
		ObjPriority = PRIORITY_TOP;
	}else if(ObjType.isCreatureContainer()){
		ObjPriority = PRIORITY_CREATURE;
	}else{
		ObjPriority = PRIORITY_OTHER;
	}
	return ObjPriority;
}

void PlaceObject(Object Obj, Object Con, bool Append){
	if(!Obj.exists()){
		error("PlaceObject: Übergebenes Objekt existiert nicht.\n");
		return;
	}

	if(!Con.exists()){
		error("PlaceObject: Übergebener Container existiert nicht.\n");
		return;
	}

	ObjectType ConType = Con.getObjectType();
	if(!ConType.getFlag(CONTAINER) || !ConType.getFlag(CHEST)){
		error("PlaceObject: Con (%d) ist kein Container.\n", ConType.TypeID);
		return;
	}

	Object Prev = NONE;
	Object Cur(Con.getAttribute(CONTENT));
	if(ConType.isMapContainer()){
		// TODO(fusion): Review. The loop below was a bit rough but it seems that
		// append is forced for PRIORITY_CREATURE and PRIORITY_OTHER. We should
		// confirm this whenever the server is up and running. It'll show.
		int ObjPriority = GetObjectPriority(Obj);
		Append = Append
			|| ObjPriority == PRIORITY_CREATURE
			|| ObjPriority == PRIORITY_OTHER;

		while(Cur != NONE){
			int CurPriority = GetObjectPriority(Cur);
			if(CurPriority == ObjPriority
					&& (ObjPriority == PRIORITY_BANK
						|| ObjPriority == PRIORITY_BOTTOM
						|| ObjPriority == PRIORITY_TOP)){
				// TODO(fusion): Replace item? I think the client might assert
				// if there are two of these objects on a single tile.
				const char *PriorityString = "";
				if(ObjPriority == PRIORITY_BACK){
					PriorityString == "BANK";
				}else if(ObjPriority == PRIORITY_BOTTOM){
					PriorityString == "BOTTOM";
				}else if(ObjPriority == PRIORITY_TOP){
					PriorityString == "TOP";
				}

				int CoordX, CoordY, CoordZ;
				GetObjectCoordinates(Con, CoordX, CoordY, CoordZ);

				ObjectType ObjType = Obj.getObjectType();
				ObjectType CurType = Cur.getObjectType();
				error("PlaceObject: Zwei %s-Objekte (%d und %d) auf Feld [%d,%d,%d].\n",
					PriorityString, ObjType.TypeID, CurType.TypeID, CoordX, CoordY, CoordZ);
			}

			if(Append && CurPriority > ObjPriority) break;
			if(!Append && CurPriority >= ObjPriority) break;

			Prev = Cur;
			Cur = Cur.getNextObject();
		}
	}else{
		if(Append){
			while(Cur != NONE){
				Prev = Cur;
				Cur = Cur.getNextObject();
			}
		}
	}

	if(Prev != NONE){
		Prev.setNextObject(Obj);
	}else{
		Con.setAttribute(CONTENT, Obj.ObjectID);
	}
	Obj.setNextObject(Cur);
	Obj.setContainer(Con);
}

Object AppendObject(Object Con, ObjectType Type){
	if(!Con.exists()){
		error("AppendObject: Übergebener Container existiert nicht.\n");
		return NONE;
	}

	Object Obj = CreateObject();
	ChangeObject(Obj, Type);
	PlaceObject(Obj, Con, true);
	return Obj;
}

Object GetFirstContainerObject(Object Con){
	if(Con == NONE){
		error("GetFirstContainerObject: Übergebener Container existiert nicht.\n");
		return NONE;
	}

	ObjectType ConType = Con.getObjectType(Con);
	if(!ConType.getFlag(CONTAINER) && !ConType.getFlag(CHEST)){
		error("GetFirstContainerObject: Con (%d) ist kein Container.\n", ConType.TypeID);
		return NONE;
	}

	return Object(Con.getAttribute(CONTENT));
}

Object GetContainerObject(Object Con, int Index){
	if(Index < 0){
		error("GetContainerObject: Ungültige laufende Nummer %d.\n", Index);
		return NONE;
	}

	if(!Con.exists()){
		error("GetContainerObject: Übergebener Container existiert nicht.\n");
		return NONE;
	}

	Object Current = GetFirstContainerObject(Con);
	while(Current != NONE && Index > 0){
		Current = CurrentObject.getNextObject();
	}

	return Current;
}

void GetObjectCoordinates(Object Obj, int *x, int *y, int *z){
	if(!Obj.exists()){
		error("GetObjectCoordinates: Übergebenes Objekt existiert nicht.\n");
		*x = 0;
		*y = 0;
		*z = 0;
		return;
	}

	// TODO(fusion): I'm not sure I like this approach with calling `AccessObject`
	// multiple times for the same object. Furthermore, using `Object::getObjectType`
	// (at least until now) is very weird when we could just get the same information
	// from the `TObject` which we'll access ANYWAYS.

	while(true){
		if(Obj.getObjectType().isMapContainer())
			break;
		Obj = Obj.getContainer();
	}

	*x = AccessObject(Obj)->Attributes[1];
	*y = AccessObject(Obj)->Attributes[2];

	// NOTE(fusion): The first 8 bits of `Attributes[3]` holds the Z coordinate
	// of a map container. The next 8 bits holds the flags of a map container.
	*z = AccessObject(Obj)->Attributes[3] & 0xFF;

	return;
}

uint8 GetMapContainerFlags(Object Obj){
	if(!Obj.exists() || !Obj.getObjectType().isMapContainer()){
		error("GetMapContainerFlags: Objekt ist kein MapContainer.\n");
		return 0;
	}

	// NOTE(fusion): See note in `GetObjectCoordinates` just above.
	return (uint8)(AccessObject(Obj)->Attributes[3] >> 8);
}

Object GetMapContainer(int x, int y, int z){
	int SectorX = x / 32;
	int SectorY = y / 32;
	int SectorZ = z;

	if(SectorX < SectorXMin || SectorXMax < SectorX
			|| SectorY < SectorYMin || SectorYMax < SectorY
			|| SectorZ < SectorZMin || SectorZMax < SectorZ){
		return NONE;
	}

	ASSERT(Sector != NULL);
	TSector *ConSector = *Sector->at(SectorX, SectorY, SectorZ);
	if(ConSector == NULL){
		return NONE;
	}


	int OffsetX = x % 32;
	int OffsetY = y % 32;
	ConSector->TimeStamp = RoundNr;
	return ConSector->MapCon[OffsetX][OffsetY];
}

Object GetMapContainer(Object Obj){
	if(!Obj.exists()){
		error("GetMapContainer: Übergebenes Objekt existiert nicht\n")
		return NONE;
	}

	while(true){
		if(Obj.getObjectType().isMapContainer())
			break;
		Obj = Obj.getContainer();
	}

	return Obj;
}

Object GetFirstObject(int x, int y, int z){
	Object MapCon = GetMapContainer(x, y, z);
	if(MapCon != NONE){
		return Object(MapCon.getAttribute(CONTENT));
	}else{
		return NONE;
	}
}
