#include "common.hh"
#include "containers.hh"

struct TStaticStringTableBlock {
	int TotalTextLength;
	char Text[65536];
};

struct TDynamicStringTableBlock {
	int FreeEntries;
	int TotalTextLength;
	bool Dirty;
	uint8 EntryType[256];
	uint16 StringOffset[256];
	char Text[32768];
};

constexpr int StaticBlockSize = NARRAY(TStaticStringTableBlock::Text);
constexpr int DynamicBlockSize = NARRAY(TDynamicStringTableBlock::Text);
constexpr int DynamicBlockEntries = NARRAY(TDynamicStringTableBlock::EntryType);

enum : uint8 {
	DYNAMIC_STRING_FREE = 0,
	DYNAMIC_STRING_ALLOCATED = 1,
	DYNAMIC_STRING_DELETED = 2,
};

static list<TStaticStringTableBlock> StaticStringTable;
static list<TDynamicStringTableBlock> DynamicStringTable;

const char *AddStaticString(const char *String){
	int StringLen = (int)strlen(String);
	if((StringLen + 1) > StaticBlockSize){
		error("AddStaticString: String zu lang (%d).\n", StringLen);
		return NULL;
	}

	if(StringLen == 0){
		return "";
	}

	listnode<TStaticStringTableBlock> *Node = StaticStringTable.firstNode;
	while(Node != NULL){
		if((Node->data.TotalTextLength + StringLen + 1) <= StaticBlockSize){
			break;
		}
		Node = Node->next;
	}

	if(Node == NULL){
		Node = StaticStringTable.append();
		memset(&Node->data, 0, sizeof(TStaticStringTableBlock));
	}

	TStaticStringTableBlock *Block = &Node->data;
	char *Result = &Block->Text[Block->TotalTextLength];
	memcpy(Result, String, StringLen + 1);
	Block->TotalTextLength += StringLen + 1;
	return Result;
}

uint32 AddDynamicString(const char *String){
	int StringLen = (int)strlen(String);
	if((StringLen + 1) > DynamicBlockSize){
		error("AddDynamicString: String zu lang (%d)\n", StringLen);
		return 0;
	}

	if(StringLen == 0){
		return 0;
	}

	int BlockIndex = 0;
	listnode<TDynamicStringTableBlock> *Node = DynamicStringTable.firstNode;
	while(Node != NULL){
		if(Node->data.FreeEntries > 0){
			if((Node->data.TotalTextLength + StringLen + 1) <= DynamicBlockSize){
				break;
			}
		}
		Node = Node->next;
		BlockIndex += 1;
	}

	if(Node == NULL){
		Node = DynamicStringTable.append();
		memset(&Node->data, 0, sizeof(TDynamicStringTableBlock));
		Node->data.FreeEntries = DynamicBlockEntries;
	}

	TDynamicStringTableBlock *Block = &Node->data;
	int EntryIndex = -1;
	for(int i = 0; i < DynamicBlockEntries; i += 1){
		if(Block->EntryType[i] == DYNAMIC_STRING_FREE){
			EntryIndex = i;
			break;
		}
	}

	if(EntryIndex == -1){
		error("AddDynamicString: Keinen freien Platz gefunden.\n");
		return 0;
	}

	int StringOffset = Block->TotalTextLength;
	Block->FreeEntries -= 1;
	Block->TotalTextLength += StringLen + 1;
	Block->EntryType[EntryIndex] = DYNAMIC_STRING_ALLOCATED;
	Block->StringOffset[EntryIndex] = (uint16)StringOffset;
	memcpy(&Block->Text[StringOffset], String, StringLen + 1);

	return (uint32)(1 + BlockIndex * DynamicBlockEntries + EntryIndex);
}

const char *GetDynamicString(uint32 Number){
	if(Number == 0){
		return NULL;
	}

	int BlockIndex = (int)(Number - 1) / DynamicBlockEntries;
	int EntryIndex = (int)(Number - 1) % DynamicBlockEntries;
	listnode<TDynamicStringTableBlock> *Node = DynamicStringTable.firstNode;
	while(BlockIndex > 0 && Node != NULL){
		Node = Node->next;
		BlockIndex -= 1;
	}

	if(Node == NULL){
		error("GetDynamicString: Block für String %u existiert nicht\n", Number);
		return NULL;
	}

	TDynamicStringTableBlock *Block = &Node->data;
	if(Block->EntryType[EntryIndex] != DYNAMIC_STRING_ALLOCATED){
		error("GetDynamicString: Eintrag für String %u existiert nicht\n", Number);
		return NULL;
	}

	int StringOffset = (int)Block->StringOffset[EntryIndex];
	return &Block->Text[StringOffset];
}

void DeleteDynamicString(uint32 Number){
	if(Number == 0){
		return;
	}

	int BlockIndex = (int)(Number - 1) / DynamicBlockEntries;
	int EntryIndex = (int)(Number - 1) % DynamicBlockEntries;
	listnode<TDynamicStringTableBlock> *Node = DynamicStringTable.firstNode;
	while(BlockIndex > 0 && Node != NULL){
		Node = Node->next;
		BlockIndex -= 1;
	}

	if(Node == NULL){
		error("DeleteDynamicString: Block für String %u existiert nicht\n", Number);
		return;
	}

	TDynamicStringTableBlock *Block = &Node->data;
	if(Block->EntryType[EntryIndex] != DYNAMIC_STRING_ALLOCATED){
		error("DeleteDynamicString: Eintrag für String %u existiert nicht\n", Number);
		return;
	}

	Block->EntryType[EntryIndex] = DYNAMIC_STRING_DELETED;
	Block->Dirty = true;
}

void CleanupDynamicStrings(void){
	// IMPORTANT(fusion): The way we manage dynamic strings also mean pointers
	// returned from `GetDynamicString` aren't stable.
	for(listnode<TDynamicStringTableBlock> *Node = DynamicStringTable.firstNode;
			Node != NULL;
			Node = Node->next){
		TDynamicStringTableBlock *Block = &Node->data;
		if(!Block->Dirty){
			continue;
		}

		for(int i = 0; i < DynamicBlockEntries; i += 1){
			if(Block->EntryType[i] == DYNAMIC_STRING_DELETED){
				int StringOffset = Block->StringOffset[i];
				char *String = &Block->Text[StringOffset];
				int StringLen = (int)strlen(String);
				int StringEnd = StringOffset + StringLen + 1;
				Block->EntryType[i] = DYNAMIC_STRING_FREE;

				if(StringEnd > DynamicBlockSize){
					error("CleanupDynamicStrings: Stringende fehlt\n");
					continue;
				}

				if(StringEnd < DynamicBlockSize){
					memmove(String, String + StringEnd, DynamicBlockSize - StringEnd);
				}

				for(int j = 0; j < DynamicBlockEntries; j += 1){
					if(Block->EntryType[j] != DYNAMIC_STRING_FREE
					&& Block->StringOffset[j] > StringOffset){
						Block->StringOffset[j] -= StringLen + 1;
					}
				}
			}
		}
	}
}

void InitStrings(void){
	// no-op
}

void ExitStrings(void){
	// no-op
}
