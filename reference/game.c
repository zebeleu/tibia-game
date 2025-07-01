// DBFUNCS.CC -> PLAYERLOADER.CC ?
//==============================================================================

// WARNING: Unknown calling convention -- yet parameter storage is locked

void PlayerDataPath(void)

{
    uint in_stack_00000004;
    char *in_stack_0000000c;
    
    sprintf(in_stack_0000000c,"%s/%02ld/%ld.%s",USERPATH,in_stack_00000004 % 100);
    return;
}



bool PlayerDataExists(ulong CharacterID)

{
    bool bVar1;
    char local_100c [4];
    char FileName [4096];
    
    sprintf(local_100c,"%s/%02ld/%ld.%s",USERPATH,CharacterID % 100,CharacterID,&DAT_080f6560);
    bVar1 = FileExists(local_100c);
    return bVar1;
}



// WARNING: Removing unreachable block (ram,0x0806d081)
// WARNING: Removing unreachable block (ram,0x0806d0da)
// WARNING: Removing unreachable block (ram,0x0806d08b)
// WARNING: Removing unreachable block (ram,0x0806d095)
// WARNING: Removing unreachable block (ram,0x0806d0c7)
// WARNING: Removing unreachable block (ram,0x0806d0a0)

bool LoadPlayerData(TPlayerData *Slot)

{
    bool bVar1;
    char cVar2;
    int SkillNr;
    int iVar3;
    uint uVar4;
    int iVar5;
    uchar *puVar6;
    anon_union_4_2_730cd3ca_for_TOutfit_2 *paVar7;
    int DepotNr;
    int SkillNr_2;
    int *piVar8;
    char *pcVar9;
    undefined1 uVar10;
    char *pcVar11;
    undefined1 auStack_500c [3];
    bool FileOk;
    TReadScriptFile Script;
    undefined1 local_102c [4];
    TDynamicWriteBuffer HelpBuffer;
    char FileName [4096];
    
    if (Slot == (TPlayerData *)0x0) {
        pcVar11 = "LoadPlayerData: Slot ist NULL.\n";
    }
    else {
        uVar4 = Slot->CharacterID;
        if (uVar4 != 0) {
            piVar8 = &HelpBuffer.super_TWriteBuffer.Position;
            pcVar11 = USERPATH;
            sprintf((char *)piVar8,"%s/%02ld/%ld.%s",USERPATH,uVar4 % 100,uVar4,&DAT_080f6560);
            bVar1 = FileExists((char *)piVar8);
            if (!bVar1) {
                Slot->Race = 1;
                iVar3 = 0;
                Slot->Profession = 0;
                (Slot->OriginalOutfit).OutfitID = 0;
                (Slot->OriginalOutfit).field_1 = (anon_union_4_2_730cd3ca_for_TOutfit_2)0x0;
                (Slot->CurrentOutfit).OutfitID = 0;
                (Slot->CurrentOutfit).field_1 = (anon_union_4_2_730cd3ca_for_TOutfit_2)0x0;
                Slot->LastLoginTime = 0;
                Slot->LastLogoutTime = 0;
                Slot->startx = 0;
                Slot->starty = 0;
                Slot->startz = 0;
                Slot->posx = 0;
                Slot->posy = 0;
                Slot->posz = 0;
                Slot->PlayerkillerEnd = 0;
                do {
                    Slot->Minimum[iVar3] = -0x80000000;
                    iVar3 = iVar3 + 1;
                } while (iVar3 < 0x19);
                puVar6 = Slot->SpellList;
                for (iVar3 = 0x40; iVar3 != 0; iVar3 = iVar3 + -1) {
                    puVar6[0] = '\0';
                    puVar6[1] = '\0';
                    puVar6[2] = '\0';
                    puVar6[3] = '\0';
                    puVar6 = puVar6 + 4;
                }
                uVar4 = 2000;
                piVar8 = Slot->QuestValues;
                if (((uint)piVar8 & 4) != 0) {
                    piVar8 = Slot->QuestValues + 1;
                    uVar4 = 0x7cc;
                    Slot->QuestValues[0] = 0;
                }
                for (uVar4 = uVar4 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
                    *piVar8 = 0;
                    piVar8 = piVar8 + 1;
                }
                Slot->MurderTimestamps[0] = 0;
                Slot->MurderTimestamps[1] = 0;
                Slot->MurderTimestamps[2] = 0;
                Slot->MurderTimestamps[3] = 0;
                Slot->MurderTimestamps[4] = 0;
                Slot->MurderTimestamps[5] = 0;
                Slot->MurderTimestamps[6] = 0;
                Slot->MurderTimestamps[8] = 0;
                Slot->MurderTimestamps[9] = 0;
                Slot->MurderTimestamps[10] = 0;
                Slot->MurderTimestamps[0xb] = 0;
                Slot->MurderTimestamps[0xc] = 0;
                Slot->MurderTimestamps[7] = 0;
                Slot->MurderTimestamps[0xd] = 0;
                Slot->MurderTimestamps[0xe] = 0;
                Slot->MurderTimestamps[0xf] = 0;
                Slot->MurderTimestamps[0x10] = 0;
                Slot->InventorySize = 0;
                iVar3 = 0;
                Slot->MurderTimestamps[0x11] = 0;
                Slot->MurderTimestamps[0x12] = 0;
                Slot->MurderTimestamps[0x13] = 0;
                Slot->Inventory = (uchar *)0x0;
                do {
                    Slot->Depot[iVar3] = (uchar *)0x0;
                    Slot->DepotSize[iVar3] = 0;
                    iVar3 = iVar3 + 1;
                } while (iVar3 < 9);
                return true;
            }
            TDynamicWriteBuffer::TDynamicWriteBuffer((TDynamicWriteBuffer *)local_102c,0x4000);
                    // try { // try from 0806c499 to 0806c49d has its CatchHandler @ 0806d1cf
            TReadScriptFile::TReadScriptFile((TReadScriptFile *)auStack_500c);
                    // try { // try from 0806c4ab to 0806c5b3 has its CatchHandler @ 0806d1c6
            TReadScriptFile::open((TReadScriptFile *)auStack_500c,(char *)piVar8,(int)pcVar11);
            TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
            TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
            pcVar11 = TReadScriptFile::readString((TReadScriptFile *)auStack_500c);
            strcpy(Slot->Name,pcVar11);
            TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
            iVar3 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
            Slot->Race = iVar3;
            TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
            iVar3 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
            Slot->Profession = iVar3;
            TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
                    // try { // try from 0806c5c8 to 0806c626 has its CatchHandler @ 0806d12c
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'(');
            iVar3 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
            (Slot->OriginalOutfit).OutfitID = iVar3;
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
            if ((Slot->OriginalOutfit).OutfitID == 0) {
                iVar3 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                (Slot->OriginalOutfit).field_1.ObjectType = (ushort)iVar3;
            }
            else {
                    // try { // try from 0806d10d to 0806d111 has its CatchHandler @ 0806d12c
                paVar7 = (anon_union_4_2_730cd3ca_for_TOutfit_2 *)
                         TReadScriptFile::readBytesequence((TReadScriptFile *)auStack_500c);
                (Slot->OriginalOutfit).field_1 = *paVar7;
            }
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,')');
                    // try { // try from 0806c630 to 0806c64a has its CatchHandler @ 0806d1c6
            TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
                    // try { // try from 0806c65f to 0806c6bd has its CatchHandler @ 0806d12c
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'(');
            iVar3 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
            (Slot->CurrentOutfit).OutfitID = iVar3;
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
            if ((Slot->CurrentOutfit).OutfitID == 0) {
                iVar3 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                (Slot->CurrentOutfit).field_1.ObjectType = (ushort)iVar3;
            }
            else {
                    // try { // try from 0806d0f5 to 0806d0f9 has its CatchHandler @ 0806d12c
                paVar7 = (anon_union_4_2_730cd3ca_for_TOutfit_2 *)
                         TReadScriptFile::readBytesequence((TReadScriptFile *)auStack_500c);
                (Slot->CurrentOutfit).field_1 = *paVar7;
            }
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,')');
                    // try { // try from 0806c6c7 to 0806d062 has its CatchHandler @ 0806d1c6
            TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
            iVar3 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
            Slot->LastLoginTime = iVar3;
            TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
            iVar3 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
            Slot->LastLogoutTime = iVar3;
            TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
            TReadScriptFile::readCoordinate((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
            TReadScriptFile::readCoordinate((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
            iVar3 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
            Slot->PlayerkillerEnd = iVar3;
            iVar3 = 0;
            do {
                Slot->Minimum[iVar3] = -0x80000000;
                iVar3 = iVar3 + 1;
                uVar10 = iVar3 == 0x18;
            } while (iVar3 < 0x19);
            do {
                TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
                pcVar11 = TReadScriptFile::getIdentifier((TReadScriptFile *)auStack_500c);
                iVar3 = 6;
                pcVar9 = "skill";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    uVar10 = *pcVar11 == *pcVar9;
                    pcVar11 = pcVar11 + 1;
                    pcVar9 = pcVar9 + 1;
                } while ((bool)uVar10);
                if (!(bool)uVar10) goto LAB_0806cb0a;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'(');
                iVar3 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                bVar1 = false;
                if ((-1 < iVar3) && (iVar3 < 0x19)) {
                    bVar1 = true;
                }
                uVar10 = !bVar1;
                if ((bool)uVar10) {
                    TReadScriptFile::error((TReadScriptFile *)auStack_500c,"illegal skill number");
                }
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->Actual[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->Maximum[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->Minimum[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->DeltaAct[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->MagicDeltaAct[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->Cycle[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->MaxCycle[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->Count[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->MaxCount[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->AddLevel[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->Experience[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->FactorPercent[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->NextLevel[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
                iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
                Slot->Delta[iVar3] = iVar5;
                TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,')');
            } while( true );
        }
        pcVar11 = &DAT_080f6720;
    }
    error(pcVar11);
    return false;
LAB_0806cb0a:
    TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
    TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'{');
LAB_0806cb3b:
    TReadScriptFile::nextToken((TReadScriptFile *)auStack_500c);
    if (_auStack_500c != SPECIAL) goto LAB_0806cb7f;
    cVar2 = TReadScriptFile::getSpecial((TReadScriptFile *)auStack_500c);
    if (cVar2 != '}') goto code_r0x0806cb64;
    TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
    TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
    TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'{');
    while( true ) {
        TReadScriptFile::nextToken((TReadScriptFile *)auStack_500c);
        if (_auStack_500c != SPECIAL) goto LAB_0806cc5a;
        cVar2 = TReadScriptFile::getSpecial((TReadScriptFile *)auStack_500c);
        if (cVar2 == '}') break;
        if (_auStack_500c != SPECIAL) goto LAB_0806cc5a;
        cVar2 = TReadScriptFile::getSpecial((TReadScriptFile *)auStack_500c);
        if (cVar2 != ',') {
            if ((_auStack_500c != SPECIAL) ||
               (cVar2 = TReadScriptFile::getSpecial((TReadScriptFile *)auStack_500c), cVar2 != '('))
            {
LAB_0806cc5a:
                TReadScriptFile::error((TReadScriptFile *)auStack_500c,"\'(\' expected");
            }
            iVar3 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
            bVar1 = false;
            if ((-1 < iVar3) && (iVar3 < 500)) {
                bVar1 = true;
            }
            if (!bVar1) {
                TReadScriptFile::error((TReadScriptFile *)auStack_500c,"illegal quest number");
            }
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,',');
            iVar5 = TReadScriptFile::readNumber((TReadScriptFile *)auStack_500c);
            Slot->QuestValues[iVar3] = iVar5;
            TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,')');
        }
    }
    TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
    TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
    TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'{');
LAB_0806cd45:
    TReadScriptFile::nextToken((TReadScriptFile *)auStack_500c);
    if (_auStack_500c != SPECIAL) goto LAB_0806cd89;
    cVar2 = TReadScriptFile::getSpecial((TReadScriptFile *)auStack_500c);
    if (cVar2 != '}') goto code_r0x0806cd6e;
    TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
    TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
    TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'{');
    HelpBuffer.super_TWriteBuffer.Size = 0;
LAB_0806cdfa:
    TReadScriptFile::nextToken((TReadScriptFile *)auStack_500c);
    if (_auStack_500c != SPECIAL) goto LAB_0806ce42;
    cVar2 = TReadScriptFile::getSpecial((TReadScriptFile *)auStack_500c);
    if (cVar2 != '}') goto code_r0x0806ce27;
    TDynamicWriteBuffer::writeByte((TDynamicWriteBuffer *)local_102c,0xff);
    Slot->InventorySize = HelpBuffer.super_TWriteBuffer.Size;
    puVar6 = (uchar *)operator_new__(HelpBuffer.super_TWriteBuffer.Size);
    Slot->Inventory = puVar6;
    memcpy(puVar6,HelpBuffer.super_TWriteBuffer.super_TWriteStream._vptr_TWriteStream,
           Slot->InventorySize);
    TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
    TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
    pcVar11 = (char *)0x7b;
    TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'{');
    iVar3 = 0;
    do {
        Slot->Depot[iVar3] = (uchar *)0x0;
        iVar3 = iVar3 + 1;
    } while (iVar3 < 9);
LAB_0806cf45:
    TReadScriptFile::nextToken((TReadScriptFile *)auStack_500c);
    if (_auStack_500c == SPECIAL) goto code_r0x0806cf5c;
    goto LAB_0806cf8d;
code_r0x0806cb64:
    if ((_auStack_500c != SPECIAL) ||
       (cVar2 = TReadScriptFile::getSpecial((TReadScriptFile *)auStack_500c), cVar2 != ',')) {
LAB_0806cb7f:
        iVar3 = TReadScriptFile::getNumber((TReadScriptFile *)auStack_500c);
        bVar1 = false;
        if ((-1 < iVar3) && (iVar3 < 0x100)) {
            bVar1 = true;
        }
        if (!bVar1) {
            TReadScriptFile::error((TReadScriptFile *)auStack_500c,"illegal spell number");
        }
        Slot->SpellList[iVar3] = '\x01';
    }
    goto LAB_0806cb3b;
code_r0x0806cd6e:
    if ((_auStack_500c != SPECIAL) ||
       (cVar2 = TReadScriptFile::getSpecial((TReadScriptFile *)auStack_500c), cVar2 != ',')) {
LAB_0806cd89:
        iVar3 = 1;
        do {
            Slot->MurderTimestamps[iVar3 + -1] = Slot->MurderTimestamps[iVar3];
            iVar3 = iVar3 + 1;
        } while (iVar3 < 0x14);
        iVar3 = TReadScriptFile::getNumber((TReadScriptFile *)auStack_500c);
        Slot->MurderTimestamps[0x13] = iVar3;
    }
    goto LAB_0806cd45;
code_r0x0806ce27:
    if ((_auStack_500c != SPECIAL) ||
       (cVar2 = TReadScriptFile::getSpecial((TReadScriptFile *)auStack_500c), cVar2 != ',')) {
LAB_0806ce42:
        iVar3 = TReadScriptFile::getNumber((TReadScriptFile *)auStack_500c);
        TDynamicWriteBuffer::writeByte((TDynamicWriteBuffer *)local_102c,(uchar)iVar3);
        TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
        TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
        LoadObjects((TReadScriptFile *)auStack_500c,(TWriteStream *)local_102c,false);
    }
    goto LAB_0806cdfa;
code_r0x0806cf5c:
    cVar2 = TReadScriptFile::getSpecial((TReadScriptFile *)auStack_500c);
    if (cVar2 == '}') {
        TReadScriptFile::nextToken((TReadScriptFile *)auStack_500c);
        if (_auStack_500c != ENDOFFILE) {
            pcVar11 = "end of file expected";
            TReadScriptFile::error((TReadScriptFile *)auStack_500c,"end of file expected");
        }
        TReadScriptFile::close((TReadScriptFile *)auStack_500c,(int)pcVar11);
                    // try { // try from 0806d073 to 0806d077 has its CatchHandler @ 0806d1cf
        TReadScriptFile::~TReadScriptFile((TReadScriptFile *)auStack_500c,(int)pcVar11);
        TDynamicWriteBuffer::~TDynamicWriteBuffer((TDynamicWriteBuffer *)local_102c,(int)pcVar11);
        return true;
    }
    if ((_auStack_500c != SPECIAL) ||
       (cVar2 = TReadScriptFile::getSpecial((TReadScriptFile *)auStack_500c), cVar2 != ',')) {
LAB_0806cf8d:
        iVar3 = TReadScriptFile::getNumber((TReadScriptFile *)auStack_500c);
        TReadScriptFile::readIdentifier((TReadScriptFile *)auStack_500c);
        TReadScriptFile::readSymbol((TReadScriptFile *)auStack_500c,'=');
        HelpBuffer.super_TWriteBuffer.Size = 0;
        LoadObjects((TReadScriptFile *)auStack_500c,(TWriteStream *)local_102c,false);
        Slot->DepotSize[iVar3] = HelpBuffer.super_TWriteBuffer.Size;
        puVar6 = (uchar *)operator_new__(HelpBuffer.super_TWriteBuffer.Size);
        Slot->Depot[iVar3] = puVar6;
        pcVar11 = (char *)HelpBuffer.super_TWriteBuffer.super_TWriteStream;
        memcpy(puVar6,HelpBuffer.super_TWriteBuffer.super_TWriteStream._vptr_TWriteStream,
               Slot->DepotSize[iVar3]);
    }
    goto LAB_0806cf45;
}



void SavePlayerData(TPlayerData *Slot)

{
    uint uVar1;
    bool bVar2;
    bool FirstSpell;
    byte bVar3;
    time_t tVar4;
    int Number;
    int SpellNr;
    int *piVar5;
    int Limitation;
    int iVar6;
    char *pcVar7;
    undefined *__fd;
    undefined1 local_203c;
    bool FirstDepot;
    bool FirstPosition;
    bool FirstMurder;
    TReadBuffer Buffer;
    TWriteScriptFile Script;
    char local_101c [4];
    char FileName [4096];
    
    if (Slot == (TPlayerData *)0x0) {
        pcVar7 = "SavePlayerData: Slot ist NULL.\n";
    }
    else {
        uVar1 = Slot->CharacterID;
        if (uVar1 != 0) {
            pcVar7 = USERPATH;
            sprintf(local_101c,"%s/%02ld/%ld.%s",USERPATH,uVar1 % 100,uVar1,&DAT_080f6560);
                    // try { // try from 0806d29e to 0806d2a2 has its CatchHandler @ 0806de36
            TWriteScriptFile::TWriteScriptFile((TWriteScriptFile *)&Buffer.Position);
                    // try { // try from 0806d2b6 to 0806d3b6 has its CatchHandler @ 0806de30
            TWriteScriptFile::open((TWriteScriptFile *)&Buffer.Position,local_101c,(int)pcVar7);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"ID              = ");
            TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,Slot->CharacterID);
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"Name            = ");
            TWriteScriptFile::writeString((TWriteScriptFile *)&Buffer.Position,Slot->Name);
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"Race            = ");
            TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,Slot->Race);
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"Profession      = ");
            TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,Slot->Profession);
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"OriginalOutfit  = ");
                    // try { // try from 0806d3cb to 0806d431 has its CatchHandler @ 0806ddc3
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"(");
            TWriteScriptFile::writeNumber
                      ((TWriteScriptFile *)&Buffer.Position,(Slot->OriginalOutfit).OutfitID);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
            if ((Slot->OriginalOutfit).OutfitID == 0) {
                TWriteScriptFile::writeNumber
                          ((TWriteScriptFile *)&Buffer.Position,
                           (uint)(Slot->OriginalOutfit).field_1.ObjectType);
            }
            else {
                    // try { // try from 0806ddad to 0806ddb1 has its CatchHandler @ 0806ddc3
                TWriteScriptFile::writeBytesequence
                          ((TWriteScriptFile *)&Buffer.Position,
                           (Slot->OriginalOutfit).field_1.Colors,4);
            }
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,")");
                    // try { // try from 0806d43b to 0806d455 has its CatchHandler @ 0806de30
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"CurrentOutfit   = ");
                    // try { // try from 0806d46a to 0806d4d0 has its CatchHandler @ 0806ddc3
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"(");
            TWriteScriptFile::writeNumber
                      ((TWriteScriptFile *)&Buffer.Position,(Slot->CurrentOutfit).OutfitID);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
            if ((Slot->CurrentOutfit).OutfitID == 0) {
                TWriteScriptFile::writeNumber
                          ((TWriteScriptFile *)&Buffer.Position,
                           (uint)(Slot->CurrentOutfit).field_1.ObjectType);
            }
            else {
                    // try { // try from 0806dd8b to 0806dd8f has its CatchHandler @ 0806ddc3
                TWriteScriptFile::writeBytesequence
                          ((TWriteScriptFile *)&Buffer.Position,(Slot->CurrentOutfit).field_1.Colors
                           ,4);
            }
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,")");
                    // try { // try from 0806d4da to 0806dd20 has its CatchHandler @ 0806de30
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"LastLogin       = ");
            TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,Slot->LastLoginTime);
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"LastLogout      = ");
            TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,Slot->LastLogoutTime)
            ;
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"StartPosition   = ");
            TWriteScriptFile::writeCoordinate
                      ((TWriteScriptFile *)&Buffer.Position,Slot->startx,Slot->starty,Slot->startz);
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"CurrentPosition = ");
            TWriteScriptFile::writeCoordinate
                      ((TWriteScriptFile *)&Buffer.Position,Slot->posx,Slot->posy,Slot->posz);
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"PlayerkillerEnd = ");
            TWriteScriptFile::writeNumber
                      ((TWriteScriptFile *)&Buffer.Position,Slot->PlayerkillerEnd);
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            iVar6 = 0;
            piVar5 = Slot->Delta;
            do {
                if (piVar5[-0x113] != -0x80000000) {
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"Skill = (");
                    TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,iVar6);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber
                              ((TWriteScriptFile *)&Buffer.Position,piVar5[-0x145]);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,piVar5[-300])
                    ;
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber
                              ((TWriteScriptFile *)&Buffer.Position,piVar5[-0x113]);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber
                              ((TWriteScriptFile *)&Buffer.Position,piVar5[-0xfa]);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber
                              ((TWriteScriptFile *)&Buffer.Position,piVar5[-0xe1]);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,piVar5[-200])
                    ;
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber
                              ((TWriteScriptFile *)&Buffer.Position,piVar5[-0xaf]);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber
                              ((TWriteScriptFile *)&Buffer.Position,piVar5[-0x96]);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber
                              ((TWriteScriptFile *)&Buffer.Position,piVar5[-0x7d]);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,piVar5[-100])
                    ;
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber
                              ((TWriteScriptFile *)&Buffer.Position,piVar5[-0x4b]);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber
                              ((TWriteScriptFile *)&Buffer.Position,piVar5[-0x32]);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber
                              ((TWriteScriptFile *)&Buffer.Position,piVar5[-0x19]);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,*piVar5);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,")");
                    TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
                }
                iVar6 = iVar6 + 1;
                piVar5 = piVar5 + 1;
            } while (iVar6 < 0x19);
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"Spells      = {");
            bVar2 = true;
            iVar6 = 0;
            do {
                if (Slot->SpellList[iVar6] != '\0') {
                    if (!bVar2) {
                        TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    }
                    TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,iVar6);
                    bVar2 = false;
                }
                iVar6 = iVar6 + 1;
            } while (iVar6 < 0x100);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"}");
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"QuestValues = {");
            bVar2 = true;
            iVar6 = 0;
            do {
                if (Slot->QuestValues[iVar6] != 0) {
                    if (!bVar2) {
                        TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    }
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"(");
                    TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,iVar6);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                    TWriteScriptFile::writeNumber
                              ((TWriteScriptFile *)&Buffer.Position,Slot->QuestValues[iVar6]);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,")");
                    bVar2 = false;
                }
                iVar6 = iVar6 + 1;
            } while (iVar6 < 500);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"}");
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"Murders     = {");
            bVar2 = true;
            iVar6 = 0;
            tVar4 = time((time_t *)0x0);
            do {
                Number = Slot->MurderTimestamps[iVar6];
                if (tVar4 + -0x278d00 < Number) {
                    if (!bVar2) {
                        TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                        Number = Slot->MurderTimestamps[iVar6];
                    }
                    TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,Number);
                    bVar2 = false;
                }
                iVar6 = iVar6 + 1;
            } while (iVar6 < 0x14);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"}");
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"Inventory   = {");
            if (Slot->Inventory != (uchar *)0x0) {
                TReadBuffer::TReadBuffer
                          ((TReadBuffer *)&local_203c,Slot->Inventory,Slot->InventorySize);
                bVar2 = true;
                while (bVar3 = TReadBuffer::readByte((TReadBuffer *)&local_203c), bVar3 != 0xff) {
                    if (bVar2) {
                        bVar2 = false;
                    }
                    else {
                        TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,",");
                        TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
                        TWriteScriptFile::writeText
                                  ((TWriteScriptFile *)&Buffer.Position,"               ");
                    }
                    TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,(uint)bVar3);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position," Content=");
                    SaveObjects((TReadStream *)&local_203c,(TWriteScriptFile *)&Buffer.Position);
                }
            }
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"}");
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"Depots      = {");
            bVar2 = true;
            iVar6 = 0;
            do {
                if (Slot->Depot[iVar6] != (uchar *)0x0) {
                    if (bVar2) {
                        bVar2 = false;
                    }
                    else {
                    // try { // try from 0806dd45 to 0806dd6d has its CatchHandler @ 0806de30
                        TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,", ");
                        TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
                        TWriteScriptFile::writeText
                                  ((TWriteScriptFile *)&Buffer.Position,"               ");
                    }
                    TWriteScriptFile::writeNumber((TWriteScriptFile *)&Buffer.Position,iVar6);
                    TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position," Content=");
                    TReadBuffer::TReadBuffer
                              ((TReadBuffer *)&local_203c,Slot->Depot[iVar6],Slot->DepotSize[iVar6])
                    ;
                    SaveObjects((TReadStream *)&local_203c,(TWriteScriptFile *)&Buffer.Position);
                }
                iVar6 = iVar6 + 1;
            } while (iVar6 < 9);
            __fd = &DAT_080f2aa0;
            TWriteScriptFile::writeText((TWriteScriptFile *)&Buffer.Position,"}");
            TWriteScriptFile::writeLn((TWriteScriptFile *)&Buffer.Position);
            TWriteScriptFile::close((TWriteScriptFile *)&Buffer.Position,(int)__fd);
                    // try { // try from 0806dd2a to 0806dd2e has its CatchHandler @ 0806de36
            TWriteScriptFile::~TWriteScriptFile((TWriteScriptFile *)&Buffer.Position,(int)__fd);
            return;
        }
        pcVar7 = &DAT_080f6800;
    }
    error(pcVar7);
    return;
}



void UnlinkPlayerData(ulong CharacterID)

{
    char local_100c [4];
    char FileName [4096];
    
    sprintf(local_100c,"%s/%02ld/%ld.%s",USERPATH,CharacterID % 100,CharacterID,&DAT_080f6560);
    unlink(local_100c);
    return;
}

// QUERY.CC
//==============================================================================

void SetQueryManagerLoginData(int Type,char *Data)

{
    ApplicationType = Type;
    if (Data == (char *)0x0) {
        LoginData[0] = '\0';
    }
    else {
        strncpy(LoginData,Data,0x1e);
    }
    return;
}



// DWARF original prototype: void TQueryManagerConnection(TQueryManagerConnection * this, int
// QueryBufferSize)

void __thiscall
TQueryManagerConnection::TQueryManagerConnection(TQueryManagerConnection *this,int QueryBufferSize)

{
    uint uVar1;
    uchar *puVar2;
    uint *puVar3;
    sockaddr *__addr;
    socklen_t in_stack_ffffffe0;
    uint local_1c [4];
    
    local_1c[3] = 0x4000;
    puVar3 = local_1c + 3;
    if (0x3fff < QueryBufferSize) {
        puVar3 = (uint *)&QueryBufferSize;
    }
    puVar2 = (uchar *)operator_new__(*puVar3);
    this->Buffer = puVar2;
    local_1c[2] = 0x4000;
    puVar3 = local_1c + 2;
    if (0x3fff < QueryBufferSize) {
        puVar3 = (uint *)&QueryBufferSize;
    }
    TReadBuffer::TReadBuffer(&this->ReadBuffer,this->Buffer,*puVar3);
    local_1c[1] = 0x4000;
    puVar3 = local_1c + 1;
    if (0x3fff < QueryBufferSize) {
        puVar3 = (uint *)&QueryBufferSize;
    }
    __addr = (sockaddr *)*puVar3;
    puVar2 = this->Buffer;
    TWriteBuffer::TWriteBuffer(&this->WriteBuffer,puVar2,(int)__addr);
    local_1c[0] = 0x4000;
    puVar3 = local_1c;
    if (0x3fff < QueryBufferSize) {
        puVar3 = (uint *)&QueryBufferSize;
    }
    uVar1 = *puVar3;
    this->Socket = -1;
    this->QueryOk = false;
    this->BufferSize = uVar1;
    connect(this,(int)puVar2,__addr,in_stack_ffffffe0);
    return;
}



// DWARF original prototype: void TQueryManagerConnection(TQueryManagerConnection * this, int
// QueryBufferSize)

void __thiscall
TQueryManagerConnection::TQueryManagerConnection(TQueryManagerConnection *this,int QueryBufferSize)

{
    uint uVar1;
    uchar *puVar2;
    uint *puVar3;
    sockaddr *__addr;
    socklen_t in_stack_ffffffe0;
    uint local_1c [4];
    
    local_1c[3] = 0x4000;
    puVar3 = local_1c + 3;
    if (0x3fff < QueryBufferSize) {
        puVar3 = (uint *)&QueryBufferSize;
    }
    puVar2 = (uchar *)operator_new__(*puVar3);
    this->Buffer = puVar2;
    local_1c[2] = 0x4000;
    puVar3 = local_1c + 2;
    if (0x3fff < QueryBufferSize) {
        puVar3 = (uint *)&QueryBufferSize;
    }
    TReadBuffer::TReadBuffer(&this->ReadBuffer,this->Buffer,*puVar3);
    local_1c[1] = 0x4000;
    puVar3 = local_1c + 1;
    if (0x3fff < QueryBufferSize) {
        puVar3 = (uint *)&QueryBufferSize;
    }
    __addr = (sockaddr *)*puVar3;
    puVar2 = this->Buffer;
    TWriteBuffer::TWriteBuffer(&this->WriteBuffer,puVar2,(int)__addr);
    local_1c[0] = 0x4000;
    puVar3 = local_1c;
    if (0x3fff < QueryBufferSize) {
        puVar3 = (uint *)&QueryBufferSize;
    }
    uVar1 = *puVar3;
    this->Socket = -1;
    this->QueryOk = false;
    this->BufferSize = uVar1;
    connect(this,(int)puVar2,__addr,in_stack_ffffffe0);
    return;
}



// DWARF original prototype: void ~TQueryManagerConnection(TQueryManagerConnection * this, int
// __in_chrg)

void __thiscall
TQueryManagerConnection::~TQueryManagerConnection(TQueryManagerConnection *this,int __in_chrg)

{
    int iVar1;
    int *piVar2;
    
    if (-1 < this->Socket) {
        iVar1 = close(this->Socket);
        if (iVar1 < 0) {
            piVar2 = __errno_location();
            error(&DAT_0810dc00,*piVar2);
        }
        this->Socket = -1;
    }
    if (this->Buffer == (uchar *)0x0) {
        return;
    }
    operator_delete__(this->Buffer);
    return;
}



// DWARF original prototype: void ~TQueryManagerConnection(TQueryManagerConnection * this, int
// __in_chrg)

void __thiscall
TQueryManagerConnection::~TQueryManagerConnection(TQueryManagerConnection *this,int __in_chrg)

{
    int iVar1;
    int *piVar2;
    
    if (-1 < this->Socket) {
        iVar1 = close(this->Socket);
        if (iVar1 < 0) {
            piVar2 = __errno_location();
            error(&DAT_0810dc00,*piVar2);
        }
        this->Socket = -1;
    }
    if (this->Buffer == (uchar *)0x0) {
        return;
    }
    operator_delete__(this->Buffer);
    return;
}



// DWARF original prototype: void connect(TQueryManagerConnection * this)

int __thiscall
TQueryManagerConnection::connect
          (TQueryManagerConnection *this,int __fd,sockaddr *__addr,socklen_t __len)

{
    ushort uVar1;
    int Result;
    int iVar2;
    int *piVar3;
    char *__cp;
    TWriteBuffer *this_00;
    int Try;
    int iVar4;
    int iVar5;
    hostent *local_858;
    hostent *HostEntAddr;
    int ErrorCode;
    char local_84c [4];
    char Buffer [2048];
    hostent HostEnt;
    undefined1 local_2c [4];
    sockaddr_in QueryManagerAddress;
    
    iVar4 = 0;
    if (0 < NumberOfQueryManagers) {
        do {
            local_2c._0_2_ = 0;
            local_2c[2] = '\0';
            local_2c[3] = '\0';
            QueryManagerAddress.sin_family = 0;
            QueryManagerAddress.sin_port = 0;
            QueryManagerAddress.sin_addr.s_addr = 0;
            QueryManagerAddress.sin_zero[0] = '\0';
            QueryManagerAddress.sin_zero[1] = '\0';
            QueryManagerAddress.sin_zero[2] = '\0';
            QueryManagerAddress.sin_zero[3] = '\0';
            QueryManagerAddress._0_4_ = inet_addr(QUERY_MANAGER[iVar4].Host);
            if (QueryManagerAddress._0_4_ == 0xffffffff) {
                HostEntAddr = (hostent *)0x0;
                iVar2 = gethostbyname_r(QUERY_MANAGER[iVar4].Host,(hostent *)(Buffer + 0x7fc),
                                        local_84c,0x800,&local_858,(int *)&HostEntAddr);
                if ((iVar2 == 0) && (local_858 != (hostent *)0x0)) {
                    __cp = inet_ntoa((in_addr)*(in_addr_t *)*local_858->h_addr_list);
                    QueryManagerAddress._0_4_ = inet_addr(__cp);
                    goto LAB_080e0505;
                }
LAB_080e06c8:
                print();
            }
            else {
LAB_080e0505:
                iVar2 = socket(2,1,0);
                this->Socket = iVar2;
                if (iVar2 < 0) goto LAB_080e06c8;
                uVar1 = (ushort)QUERY_MANAGER[iVar4].Port;
                iVar5 = 0x10;
                ErrorCode._2_2_ = uVar1 >> 8 | uVar1 << 8;
                local_2c._2_2_ = ErrorCode._2_2_;
                local_2c._0_2_ = 2;
                iVar2 = ::connect(this->Socket,(sockaddr *)local_2c,0x10);
                if (iVar2 < 0) {
                    print();
                }
                else {
                    this_00 = &this->WriteBuffer;
                    this->QueryOk = true;
                    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e0595 to 080e05a9 has its CatchHandler @ 080e0754
                    TWriteBuffer::writeWord(this_00,0);
                    TWriteBuffer::writeByte(this_00,'\0');
                    // try { // try from 080e05b8 to 080e05bc has its CatchHandler @ 080e0791
                    TWriteBuffer::writeByte(this_00,(uchar)ApplicationType);
                    // try { // try from 080e05d1 to 080e05d5 has its CatchHandler @ 080e07c1
                    TWriteStream::writeString
                              (&this_00->super_TWriteStream,QUERY_MANAGER[iVar4].Password);
                    if (ApplicationType == 1) {
                    // try { // try from 080e069d to 080e06a1 has its CatchHandler @ 080e07f1
                        TWriteStream::writeString(&this_00->super_TWriteStream,LoginData);
                    }
                    iVar5 = executeQuery(this,0x1e,false);
                    if (iVar5 == 0) {
                        return 0;
                    }
                    print();
                }
                if (-1 < this->Socket) {
                    iVar2 = close(this->Socket);
                    if (iVar2 < 0) {
                        piVar3 = __errno_location();
                        error(&DAT_0810dc00,*piVar3,iVar5);
                    }
                    this->Socket = -1;
                }
            }
            iVar4 = iVar4 + 1;
        } while (iVar4 < NumberOfQueryManagers);
    }
    print();
    this->Socket = -1;
    return (int)this;
}



// DWARF original prototype: void disconnect(TQueryManagerConnection * this)

void __thiscall TQueryManagerConnection::disconnect(TQueryManagerConnection *this)

{
    int iVar1;
    int *piVar2;
    
    if (-1 < this->Socket) {
        iVar1 = close(this->Socket);
        if (iVar1 < 0) {
            piVar2 = __errno_location();
            error(&DAT_0810dc00,*piVar2);
        }
        this->Socket = -1;
    }
    return;
}



// WARNING: Variable defined which should be unmapped: Chances
// DWARF original prototype: int write(TQueryManagerConnection * this, uchar * Buffer, int Size)

ssize_t __thiscall
TQueryManagerConnection::write(TQueryManagerConnection *this,int __fd,void *__buf,size_t __n)

{
    ssize_t sVar1;
    int *piVar2;
    int nwritten;
    int nleft;
    void *__n_00;
    int local_14;
    int Chances;
    
    local_14 = 0x32;
    __n_00 = __buf;
    if (0 < (int)__buf) {
        do {
            sVar1 = ::write(this->Socket,(void *)__fd,(size_t)__n_00);
            if (((sVar1 == -1) && (piVar2 = __errno_location(), *piVar2 == 0xb)) && (0 < local_14))
            {
                DelayThread(0,100000);
                local_14 = local_14 + -1;
            }
            else {
                if (sVar1 < 1) {
                    return sVar1;
                }
                __n_00 = (void *)((int)__n_00 - sVar1);
                __fd = __fd + sVar1;
            }
        } while (0 < (int)__n_00);
    }
    return (int)__buf - (int)__n_00;
}



// DWARF original prototype: int read(TQueryManagerConnection * this, uchar * Buffer, int Size, int
// Timeout)

ssize_t __thiscall
TQueryManagerConnection::read(TQueryManagerConnection *this,int __fd,void *__buf,size_t __nbytes)

{
    int iVar1;
    ssize_t sVar2;
    int *piVar3;
    int nread;
    int nleft;
    void *__nbytes_00;
    int local_20;
    int Chances;
    pollfd ufds;
    
    local_20 = 0x32;
    __nbytes_00 = __buf;
    if (0 < (int)__buf) {
        do {
            Chances = this->Socket;
            ufds.fd._0_2_ = 1;
            ufds.fd._2_2_ = 0;
            iVar1 = poll((pollfd *)&Chances,1,__nbytes * 1000);
            if (iVar1 < 1) {
                return -2;
            }
            sVar2 = ::read(this->Socket,(void *)__fd,(size_t)__nbytes_00);
            if (sVar2 == 0) break;
            if (sVar2 < 0) {
                piVar3 = __errno_location();
                if (*piVar3 != 4) {
                    if (*piVar3 != 0xb) {
                        return sVar2;
                    }
                    if (__nbytes_00 == __buf) {
                        return sVar2;
                    }
                    if (local_20 == 0) {
                        return sVar2;
                    }
                    local_20 = local_20 + -1;
                    DelayThread(0,100000);
                }
            }
            else {
                __nbytes_00 = (void *)((int)__nbytes_00 - sVar2);
                __fd = __fd + sVar2;
            }
        } while (0 < (int)__nbytes_00);
    }
    return (int)__buf - (int)__nbytes_00;
}



// DWARF original prototype: void prepareQuery(TQueryManagerConnection * this, int QueryType)

void __thiscall TQueryManagerConnection::prepareQuery(TQueryManagerConnection *this,int QueryType)

{
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e0a08 to 080e0a1c has its CatchHandler @ 080e0a27
    TWriteBuffer::writeWord(&this->WriteBuffer,0);
    TWriteBuffer::writeByte(&this->WriteBuffer,(uchar)QueryType);
    return;
}



// DWARF original prototype: void sendFlag(TQueryManagerConnection * this, bool f)

void __thiscall TQueryManagerConnection::sendFlag(TQueryManagerConnection *this,bool f)

{
    TWriteBuffer::writeByte(&this->WriteBuffer,f);
    return;
}



// DWARF original prototype: void sendByte(TQueryManagerConnection * this, uchar b)

void __thiscall TQueryManagerConnection::sendByte(TQueryManagerConnection *this,uchar b)

{
                    // try { // try from 080e0a9d to 080e0aa1 has its CatchHandler @ 080e0ab0
    TWriteBuffer::writeByte(&this->WriteBuffer,b);
    return;
}



// DWARF original prototype: void sendWord(TQueryManagerConnection * this, ushort w)

void __thiscall TQueryManagerConnection::sendWord(TQueryManagerConnection *this,ushort w)

{
                    // try { // try from 080e0b0d to 080e0b11 has its CatchHandler @ 080e0b20
    TWriteBuffer::writeWord(&this->WriteBuffer,w);
    return;
}



// DWARF original prototype: void sendQuad(TQueryManagerConnection * this, ulong q)

void __thiscall TQueryManagerConnection::sendQuad(TQueryManagerConnection *this,ulong q)

{
                    // try { // try from 080e0b7c to 080e0b80 has its CatchHandler @ 080e0b90
    TWriteBuffer::writeQuad(&this->WriteBuffer,q);
    return;
}



// DWARF original prototype: void sendString(TQueryManagerConnection * this, char * s)

void __thiscall TQueryManagerConnection::sendString(TQueryManagerConnection *this,char *s)

{
                    // try { // try from 080e0bec to 080e0bf0 has its CatchHandler @ 080e0c00
    TWriteStream::writeString(&(this->WriteBuffer).super_TWriteStream,s);
    return;
}



// DWARF original prototype: void sendBytes(TQueryManagerConnection * this, uchar * Buffer, int
// Count)

void __thiscall TQueryManagerConnection::sendBytes(TQueryManagerConnection *this)

{
    uchar *in_stack_00000008;
    int in_stack_0000000c;
    
                    // try { // try from 080e0c63 to 080e0c67 has its CatchHandler @ 080e0c72
    TWriteBuffer::writeBytes(&this->WriteBuffer,in_stack_00000008,in_stack_0000000c);
    return;
}



// DWARF original prototype: bool getFlag(TQueryManagerConnection * this)

bool __thiscall TQueryManagerConnection::getFlag(TQueryManagerConnection *this)

{
    uchar uVar1;
    
                    // try { // try from 080e0cc2 to 080e0cc6 has its CatchHandler @ 080e0cd8
    uVar1 = TReadBuffer::readByte(&this->ReadBuffer);
    return uVar1 != '\0';
}



// DWARF original prototype: uchar getByte(TQueryManagerConnection * this)

uchar __thiscall TQueryManagerConnection::getByte(TQueryManagerConnection *this)

{
    uchar uVar1;
    
                    // try { // try from 080e0d22 to 080e0d26 has its CatchHandler @ 080e0d31
    uVar1 = TReadBuffer::readByte(&this->ReadBuffer);
    return uVar1;
}



// DWARF original prototype: ushort getWord(TQueryManagerConnection * this)

ushort __thiscall TQueryManagerConnection::getWord(TQueryManagerConnection *this)

{
    ushort uVar1;
    
                    // try { // try from 080e0d82 to 080e0d86 has its CatchHandler @ 080e0d91
    uVar1 = TReadBuffer::readWord(&this->ReadBuffer);
    return uVar1;
}



// DWARF original prototype: ulong getQuad(TQueryManagerConnection * this)

ulong __thiscall TQueryManagerConnection::getQuad(TQueryManagerConnection *this)

{
    ulong uVar1;
    
                    // try { // try from 080e0de2 to 080e0de6 has its CatchHandler @ 080e0df0
    uVar1 = TReadBuffer::readQuad(&this->ReadBuffer);
    return uVar1;
}



// DWARF original prototype: void getString(TQueryManagerConnection * this, char * Buffer, int
// MaxLength)

void __thiscall
TQueryManagerConnection::getString(TQueryManagerConnection *this,char *Buffer,int MaxLength)

{
                    // try { // try from 080e0e53 to 080e0e57 has its CatchHandler @ 080e0e62
    TReadStream::readString(&(this->ReadBuffer).super_TReadStream,Buffer,MaxLength);
    return;
}



// DWARF original prototype: void getBytes(TQueryManagerConnection * this, uchar * Buffer, int
// Count)

void __thiscall TQueryManagerConnection::getBytes(TQueryManagerConnection *this)

{
    uchar *in_stack_00000008;
    int in_stack_0000000c;
    
                    // try { // try from 080e0ec0 to 080e0ec4 has its CatchHandler @ 080e0ed0
    TReadBuffer::readBytes(&this->ReadBuffer,in_stack_00000008,in_stack_0000000c);
    return;
}



// WARNING: Variable defined which should be unmapped: Help
// DWARF original prototype: int executeQuery(TQueryManagerConnection * this, int Timeout, bool
// AutoReconnect)

int __thiscall
TQueryManagerConnection::executeQuery(TQueryManagerConnection *this,int Timeout,bool AutoReconnect)

{
    byte bVar1;
    void *pvVar2;
    sockaddr *psVar3;
    ssize_t sVar4;
    int iVar5;
    void *pvVar6;
    int Result;
    TWriteBuffer *this_00;
    int Size;
    int DataSize;
    sockaddr *__buf;
    undefined4 uStackY_40;
    char *Text;
    uchar *puVar7;
    socklen_t in_stack_ffffffd0;
    undefined1 auStack_2c [11];
    char local_21;
    char *pcStack_20;
    bool SecondChance;
    char local_19;
    byte local_18;
    undefined1 local_17 [2];
    bool AutoReconnect_local;
    uchar Help2 [4];
    uchar Help [2];
    
    this_00 = &this->WriteBuffer;
    local_19 = AutoReconnect;
    __buf = (sockaddr *)(this->WriteBuffer).Position;
    (this->WriteBuffer).Position = 0;
    if ((int)__buf < 0xffff) {
                    // try { // try from 080e1376 to 080e137a has its CatchHandler @ 080e13ed
        uStackY_40 = 0x80e137b;
        TWriteBuffer::writeWord(this_00,(short)__buf - 2);
    }
    else {
        if (this->BufferSize < (int)(__buf->sa_data + 2)) {
            Text = "TQueryManagerConnection::executeQuery: Puffer zu klein.\n";
            goto LAB_080e0f4d;
        }
        pcStack_20 = __buf[-1].sa_data + 0xc;
        uStackY_40 = 0x80e0f86;
        memmove(this->Buffer + 6,this->Buffer + 2,(size_t)pcStack_20);
                    // try { // try from 080e0f91 to 080e0f95 has its CatchHandler @ 080e1380
        uStackY_40 = 0x80e0f96;
        TWriteBuffer::writeWord(this_00,0xffff);
                    // try { // try from 080e0fa0 to 080e0fa4 has its CatchHandler @ 080e13bd
        uStackY_40 = 0x80e0fa5;
        TWriteBuffer::writeQuad(this_00,(ulong)pcStack_20);
        __buf = (sockaddr *)(__buf->sa_data + 2);
    }
    local_21 = '\x01';
    if (this->QueryOk != false) {
        do {
            if (this->Socket < 0) {
                if (local_19 == '\0') {
                    return 3;
                }
                if ((int)__buf < 0x4000) {
                    iVar5 = -((uint)(__buf->sa_data + 0xd) & 0xfffffff0);
                    puVar7 = this->Buffer;
                    *(sockaddr **)(Help2 + iVar5 + -0x20) = __buf;
                    *(uchar **)(&stack0xffffffc8 + iVar5) = puVar7;
                    *(undefined1 **)(&stack0xffffffc4 + iVar5) = auStack_2c + iVar5;
                    *(undefined4 *)((int)&uStackY_40 + iVar5) = 0x80e1335;
                    memcpy(*(void **)(&stack0xffffffc4 + iVar5),*(void **)(&stack0xffffffc8 + iVar5)
                           ,*(size_t *)(Help2 + iVar5 + -0x20));
                    *(TQueryManagerConnection **)(&stack0xffffffc4 + iVar5) = this;
                    *(undefined4 *)((int)&uStackY_40 + iVar5) = 0x80e1340;
                    connect(*(TQueryManagerConnection **)(&stack0xffffffc4 + iVar5),
                            *(int *)(&stack0xffffffc8 + iVar5),*(sockaddr **)(Help2 + iVar5 + -0x20)
                            ,*(socklen_t *)(&stack0xffffffd0 + iVar5));
                    puVar7 = this->Buffer;
                    *(undefined1 **)(&stack0xffffffc8 + iVar5) = auStack_2c + iVar5;
                    *(sockaddr **)(Help2 + iVar5 + -0x20) = __buf;
                    *(uchar **)(&stack0xffffffc4 + iVar5) = puVar7;
                    *(undefined4 *)((int)&uStackY_40 + iVar5) = 0x80e1356;
                    memcpy(*(void **)(&stack0xffffffc4 + iVar5),*(void **)(&stack0xffffffc8 + iVar5)
                           ,*(size_t *)(Help2 + iVar5 + -0x20));
                }
                else {
                    uStackY_40 = 0x80e0ff6;
                    pvVar2 = operator_new__((uint)__buf);
                    puVar7 = this->Buffer;
                    uStackY_40 = 0x80e100e;
                    psVar3 = __buf;
                    memcpy(pvVar2,puVar7,(size_t)__buf);
                    uStackY_40 = 0x80e1019;
                    connect(this,(int)puVar7,psVar3,in_stack_ffffffd0);
                    uStackY_40 = 0x80e102f;
                    memcpy(this->Buffer,pvVar2,(size_t)__buf);
                    if (pvVar2 != (void *)0x0) {
                        uStackY_40 = 0x80e103b;
                        operator_delete__(pvVar2);
                    }
                }
                if (this->Socket < 0) {
                    return 3;
                }
            }
            uStackY_40 = 0x80e1069;
            psVar3 = (sockaddr *)write(this,(int)this->Buffer,__buf,in_stack_ffffffd0);
            if (psVar3 == __buf) {
                uStackY_40 = 0x80e1092;
                in_stack_ffffffd0 = Timeout;
                sVar4 = read(this,(int)(Help2 + 2),(void *)0x2,Timeout);
                if (sVar4 == 2) {
                    pvVar2 = (void *)((uint)Help2[3] * 0x100 + (uint)Help2[2]);
                    if (pvVar2 == (void *)0xffff) {
                        uStackY_40 = 0x80e1241;
                        sVar4 = read(this,(int)&local_18,(void *)0x4,Timeout);
                        if (sVar4 != 4) {
                            if (this->Socket < 0) {
                                return 3;
                            }
                            uStackY_40 = 0x80e1263;
                            iVar5 = close(this->Socket);
                            if (iVar5 < 0) {
                                uStackY_40 = 0x80e127b;
                                __errno_location();
                                uStackY_40 = 0x80e128d;
                                error(&DAT_0810dc00);
                            }
                            this->Socket = -1;
                            return 3;
                        }
                        pvVar2 = (void *)((uint)local_18 + (uint)_local_17 * 0x100);
                    }
                    if ((pvVar2 == (void *)0x0) || (this->BufferSize < (int)pvVar2)) {
                        if (-1 < this->Socket) {
                            uStackY_40 = 0x80e114a;
                            iVar5 = close(this->Socket);
                            if (iVar5 < 0) {
                                uStackY_40 = 0x80e115f;
                                __errno_location();
                                uStackY_40 = 0x80e1171;
                                error(&DAT_0810dc00);
                            }
                            this->Socket = -1;
                        }
                        uStackY_40 = 0x80e113d;
                        error(&DAT_0810e120);
                        return 3;
                    }
                    uStackY_40 = 0x80e1190;
                    pvVar6 = (void *)read(this,(int)this->Buffer,pvVar2,Timeout);
                    if (pvVar6 == pvVar2) {
                        (this->ReadBuffer).Size = (int)pvVar6;
                        (this->ReadBuffer).Position = 0;
                    // try { // try from 080e11f7 to 080e11fb has its CatchHandler @ 080e1415
                        uStackY_40 = 0x80e11fc;
                        bVar1 = TReadBuffer::readByte(&this->ReadBuffer);
                        if (bVar1 == 3) {
                            uStackY_40 = 0x80e121a;
                            error("TQueryManagerConnection::executeQuery: Anfrage fehlgeschlagen.\n"
                                 );
                            return 3;
                        }
                        return (uint)bVar1;
                    }
                    if (-1 < this->Socket) {
                        uStackY_40 = 0x80e11bb;
                        iVar5 = close(this->Socket);
                        if (iVar5 < 0) {
                            uStackY_40 = 0x80e11d0;
                            __errno_location();
                            uStackY_40 = 0x80e11e2;
                            error(&DAT_0810dc00);
                        }
                        this->Socket = -1;
                    }
                    Text = 
                    "TQueryManagerConnection::executeQuery: Fehler beim Auslesen der Daten.\n";
                    goto LAB_080e0f4d;
                }
                if (-1 < this->Socket) {
                    uStackY_40 = 0x80e10ce;
                    iVar5 = close(this->Socket);
                    if (iVar5 < 0) {
                        uStackY_40 = 0x80e10e3;
                        __errno_location();
                        uStackY_40 = 0x80e10f5;
                        error(&DAT_0810dc00);
                    }
                    this->Socket = -1;
                }
                if (sVar4 == -2) {
                    return 3;
                }
                if (local_21 == '\0') {
                    return 3;
                }
            }
            else {
                if (-1 < this->Socket) {
                    uStackY_40 = 0x80e12e8;
                    iVar5 = close(this->Socket);
                    if (iVar5 < 0) {
                        uStackY_40 = 0x80e12fd;
                        __errno_location();
                        uStackY_40 = 0x80e130f;
                        error(&DAT_0810dc00);
                    }
                    this->Socket = -1;
                }
                if (local_21 == '\0') {
                    Text = 
                    "TQueryManagerConnection::executeQuery: Fehler beim Abschicken der Anfrage.\n";
                    goto LAB_080e0f4d;
                }
            }
            local_21 = '\0';
        } while( true );
    }
    Text = "TQueryManagerConnection::executeQuery: Fehler beim Zusammenbauen der Anfrage.\n";
LAB_080e0f4d:
    uStackY_40 = 0x80e0f52;
    error(Text);
    return 3;
}



// DWARF original prototype: int checkAccountPassword(TQueryManagerConnection * this, ulong
// AccountID, char * Password, char * IPAddress)

int __thiscall TQueryManagerConnection::checkAccountPassword(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    bool bVar1;
    byte bVar2;
    int iVar3;
    int FailureCode;
    int iVar4;
    uint uVar5;
    ulong in_stack_00000008;
    char *in_stack_0000000c;
    char *in_stack_00000010;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e1465 to 080e1479 has its CatchHandler @ 080e1510
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'\n');
                    // try { // try from 080e1484 to 080e1488 has its CatchHandler @ 080e1550
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e1493 to 080e1497 has its CatchHandler @ 080e1582
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_0000000c);
                    // try { // try from 080e14a2 to 080e14a6 has its CatchHandler @ 080e15b2
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000010);
    iVar3 = executeQuery(this,0x1e,true);
    iVar4 = 0;
    if (iVar3 != 0) {
        if (iVar3 == 1) {
                    // try { // try from 080e14dd to 080e14e1 has its CatchHandler @ 080e15e2
            bVar2 = TReadBuffer::readByte(&this->ReadBuffer);
            uVar5 = (uint)bVar2;
            bVar1 = false;
            if ((uVar5 != 0) && (uVar5 < 5)) {
                bVar1 = true;
            }
            if (bVar1) {
                return uVar5;
            }
            error(&DAT_0810e2e0,uVar5);
        }
        iVar4 = -1;
    }
    return iVar4;
}



// WARNING: Variable defined which should be unmapped: PrivateWorld_local
// DWARF original prototype: int loginAdmin(TQueryManagerConnection * this, ulong AccountID, bool
// PrivateWorld, int * NumberOfCharacters, char[30] * Characters, char[30] * Worlds, uchar[4] *
// IPAddresses, ushort * Ports, ushort * PremiumDaysLeft)

int __thiscall
TQueryManagerConnection::loginAdmin
          (TQueryManagerConnection *this,ulong AccountID,bool PrivateWorld,int *NumberOfCharacters,
          char (*Characters) [30],char (*Worlds) [30],uchar (*IPAddresses) [4],ushort *Ports,
          ushort *PremiumDaysLeft)

{
    TWriteBuffer *this_00;
    TReadBuffer *this_01;
    byte bVar1;
    ushort uVar2;
    int iVar3;
    bool PrivateWorld_local;
    
    this_00 = &this->WriteBuffer;
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e163c to 080e1650 has its CatchHandler @ 080e1782
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'\f');
                    // try { // try from 080e165b to 080e165f has its CatchHandler @ 080e17c0
    TWriteBuffer::writeQuad(this_00,AccountID);
    TWriteBuffer::writeByte(this_00,PrivateWorld);
    iVar3 = executeQuery(this,0x1e,true);
    if (iVar3 == 0) {
        this_01 = &this->ReadBuffer;
                    // try { // try from 080e16d6 to 080e16da has its CatchHandler @ 080e17f2
        bVar1 = TReadBuffer::readByte(this_01);
        iVar3 = 0;
        *NumberOfCharacters = (uint)bVar1;
        if (bVar1 != 0) {
            do {
                    // try { // try from 080e1720 to 080e1724 has its CatchHandler @ 080e181d
                TReadStream::readString(&this_01->super_TReadStream,Characters[iVar3],0x1e);
                    // try { // try from 080e1744 to 080e1748 has its CatchHandler @ 080e184f
                TReadStream::readString(&this_01->super_TReadStream,Worlds[iVar3],0x1e);
                    // try { // try from 080e175e to 080e1762 has its CatchHandler @ 080e1885
                TReadBuffer::readBytes(this_01,IPAddresses[iVar3],4);
                    // try { // try from 080e1766 to 080e176a has its CatchHandler @ 080e18b5
                uVar2 = TReadBuffer::readWord(this_01);
                Ports[iVar3] = uVar2;
                iVar3 = iVar3 + 1;
            } while (iVar3 < *NumberOfCharacters);
        }
                    // try { // try from 080e16ef to 080e16f3 has its CatchHandler @ 080e18e7
        uVar2 = TReadBuffer::readWord(this_01);
        *PremiumDaysLeft = uVar2;
        iVar3 = 0;
    }
    else {
        if (iVar3 == 1) {
                    // try { // try from 080e16a7 to 080e16ab has its CatchHandler @ 080e1919
            bVar1 = TReadBuffer::readByte(&this->ReadBuffer);
            if (bVar1 == 1) {
                return 1;
            }
            error(&DAT_0810e340,(uint)bVar1);
        }
        iVar3 = -1;
    }
    return iVar3;
}



// DWARF original prototype: int loadWorldConfig(TQueryManagerConnection * this, int * WorldType,
// int * RebootTime, int * IPAddress, int * Port, int * MaxPlayers, int * PremiumPlayerBuffer, int *
// MaxNewbies, int * PremiumNewbieBuffer)

int __thiscall TQueryManagerConnection::loadWorldConfig(TQueryManagerConnection *this)

{
    TReadBuffer *this_00;
    byte bVar1;
    ushort uVar2;
    int iVar3;
    uint *in_stack_00000008;
    int *in_stack_0000000c;
    int in_stack_00000010;
    uint *in_stack_00000014;
    uint *in_stack_00000018;
    uint *in_stack_0000001c;
    uint *in_stack_00000020;
    uint *in_stack_00000024;
    
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e1975 to 080e1989 has its CatchHandler @ 080e1a56
    TWriteBuffer::writeWord(&this->WriteBuffer,0);
    TWriteBuffer::writeByte(&this->WriteBuffer,'5');
    iVar3 = executeQuery(this,0x1e,true);
    if (iVar3 == 0) {
        this_00 = &this->ReadBuffer;
                    // try { // try from 080e19b9 to 080e19bd has its CatchHandler @ 080e1a90
        bVar1 = TReadBuffer::readByte(this_00);
        *in_stack_00000008 = (uint)bVar1;
                    // try { // try from 080e19cc to 080e19d0 has its CatchHandler @ 080e1abb
        bVar1 = TReadBuffer::readByte(this_00);
        iVar3 = 0;
        *in_stack_0000000c = (uint)bVar1 * 0x3c;
        do {
                    // try { // try from 080e19e8 to 080e19ec has its CatchHandler @ 080e1ae6
            bVar1 = TReadBuffer::readByte(this_00);
            *(uint *)(in_stack_00000010 + iVar3 * 4) = (uint)bVar1;
            iVar3 = iVar3 + 1;
        } while (iVar3 < 4);
                    // try { // try from 080e1a02 to 080e1a06 has its CatchHandler @ 080e1b14
        uVar2 = TReadBuffer::readWord(this_00);
        *in_stack_00000014 = (uint)uVar2;
                    // try { // try from 080e1a12 to 080e1a16 has its CatchHandler @ 080e1b46
        uVar2 = TReadBuffer::readWord(this_00);
        *in_stack_00000018 = (uint)uVar2;
                    // try { // try from 080e1a22 to 080e1a26 has its CatchHandler @ 080e1b78
        uVar2 = TReadBuffer::readWord(this_00);
        *in_stack_0000001c = (uint)uVar2;
                    // try { // try from 080e1a32 to 080e1a36 has its CatchHandler @ 080e1baa
        uVar2 = TReadBuffer::readWord(this_00);
        *in_stack_00000020 = (uint)uVar2;
                    // try { // try from 080e1a42 to 080e1a46 has its CatchHandler @ 080e1bdc
        uVar2 = TReadBuffer::readWord(this_00);
        *in_stack_00000024 = (uint)uVar2;
        iVar3 = 0;
    }
    else {
        iVar3 = -1;
    }
    return iVar3;
}



// DWARF original prototype: int loginGame(TQueryManagerConnection * this, ulong AccountID, char *
// PlayerName, char * Password, char * IPAddress, bool PrivateWorld, bool PremiumAccountRequired,
// bool GamemasterRequired, ulong * CharacterID, int * Sex, char * Guild, char * Rank, char * Title,
// int * NumberOfBuddies, ulong * BuddyIDs, char[30] * BuddyNames, uchar * Rights, bool *
// PremiumAccountActivated)

int __thiscall
TQueryManagerConnection::loginGame
          (TQueryManagerConnection *this,ulong AccountID,char *PlayerName,char *Password,
          char *IPAddress,bool PrivateWorld,bool PremiumAccountRequired,bool GamemasterRequired,
          ulong *CharacterID,int *Sex,char *Guild,char *Rank,char *Title,int *NumberOfBuddies,
          ulong *BuddyIDs,char (*BuddyNames) [30],uchar *Rights,bool *PremiumAccountActivated)

{
    TWriteBuffer *this_00;
    byte bVar1;
    uchar uVar2;
    int iVar3;
    ulong uVar4;
    int iVar5;
    int iVar6;
    int FailureCode;
    uint uVar7;
    int *piVar8;
    int i_1;
    byte *pbVar9;
    TReadBuffer *this_01;
    byte *pbVar10;
    undefined1 uVar11;
    bool bVar12;
    undefined1 uVar13;
    bool bVar14;
    byte local_60;
    int Right;
    uint local_50;
    int i_2;
    int NumberOfRights;
    bool GamemasterRequired_local;
    bool PremiumAccountRequired_local;
    int iStack_40;
    bool PrivateWorld_local;
    byte local_3c [4];
    char RightName [30];
    
    this_00 = &this->WriteBuffer;
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e1c4a to 080e1c5e has its CatchHandler @ 080e2c00
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'\x14');
                    // try { // try from 080e1c69 to 080e1c6d has its CatchHandler @ 080e2c40
    TWriteBuffer::writeQuad(this_00,AccountID);
                    // try { // try from 080e1c78 to 080e1c7c has its CatchHandler @ 080e2c72
    TWriteStream::writeString(&this_00->super_TWriteStream,PlayerName);
                    // try { // try from 080e1c87 to 080e1c8b has its CatchHandler @ 080e2ca2
    TWriteStream::writeString(&this_00->super_TWriteStream,Password);
                    // try { // try from 080e1c96 to 080e1c9a has its CatchHandler @ 080e2cd2
    TWriteStream::writeString(&this_00->super_TWriteStream,IPAddress);
    TWriteBuffer::writeByte(this_00,PrivateWorld);
    TWriteBuffer::writeByte(this_00,PremiumAccountRequired);
    TWriteBuffer::writeByte(this_00,GamemasterRequired);
    iVar3 = executeQuery(this,0x78,true);
    if (iVar3 == 0) {
        this_01 = &this->ReadBuffer;
                    // try { // try from 080e1d4c to 080e1d50 has its CatchHandler @ 080e2d06
        uVar4 = TReadBuffer::readQuad(this_01);
        *CharacterID = uVar4;
                    // try { // try from 080e1d6d to 080e1d71 has its CatchHandler @ 080e2d38
        TReadStream::readString(&this_01->super_TReadStream,PlayerName,0x1e);
                    // try { // try from 080e1d78 to 080e1d7c has its CatchHandler @ 080e2d6e
        bVar1 = TReadBuffer::readByte(this_01);
        *Sex = (uint)bVar1;
                    // try { // try from 080e1d9d to 080e1da1 has its CatchHandler @ 080e2da0
        TReadStream::readString(&this_01->super_TReadStream,Guild,0x1e);
                    // try { // try from 080e1db7 to 080e1dbb has its CatchHandler @ 080e2dd6
        TReadStream::readString(&this_01->super_TReadStream,Rank,0x1e);
                    // try { // try from 080e1dd1 to 080e1dd5 has its CatchHandler @ 080e2e0c
        TReadStream::readString(&this_01->super_TReadStream,Title,0x1e);
                    // try { // try from 080e1ddc to 080e1de0 has its CatchHandler @ 080e2e42
        bVar1 = TReadBuffer::readByte(this_01);
        *NumberOfBuddies = (uint)bVar1;
        if (100 < bVar1) {
            error(&DAT_0810e400,*NumberOfBuddies,PlayerName);
        }
        iVar3 = 0;
        while( true ) {
            iStack_40 = 100;
            piVar8 = &stack0xffffffc0;
            if (*NumberOfBuddies < 0x65) {
                piVar8 = NumberOfBuddies;
            }
            if (*piVar8 <= iVar3) break;
                    // try { // try from 080e1e16 to 080e1e1a has its CatchHandler @ 080e2e74
            uVar4 = TReadBuffer::readQuad(this_01);
            BuddyIDs[iVar3] = uVar4;
                    // try { // try from 080e1e42 to 080e1e46 has its CatchHandler @ 080e2ea6
            TReadStream::readString(&this_01->super_TReadStream,BuddyNames[iVar3],0x1e);
            iVar3 = iVar3 + 1;
        }
        iVar3 = 100;
        if (100 < *NumberOfBuddies) {
            do {
                    // try { // try from 080e2bad to 080e2bb1 has its CatchHandler @ 080e2ed9
                TReadBuffer::readQuad(this_01);
                    // try { // try from 080e2bc7 to 080e2bcb has its CatchHandler @ 080e2f09
                TReadStream::readString(&this_01->super_TReadStream,(char *)local_3c,0x1e);
                iVar3 = iVar3 + 1;
            } while (iVar3 < *NumberOfBuddies);
        }
                    // try { // try from 080e1e5d to 080e1e61 has its CatchHandler @ 080e2f3c
        bVar1 = TReadBuffer::readByte(this_01);
        uVar7 = (uint)bVar1;
        local_50 = 0;
        uVar11 = uVar7 != 0;
        uVar13 = uVar7 == 0;
        if (uVar7 != 0) {
            do {
                    // try { // try from 080e1e97 to 080e1e9b has its CatchHandler @ 080e2f6e
                TReadStream::readString(&this_01->super_TReadStream,(char *)local_3c,0x1e);
                iVar3 = 0x10;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"PREMIUM_ACCOUNT";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    uVar11 = *pbVar9 < *pbVar10;
                    uVar13 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while ((bool)uVar13);
                iVar3 = 0;
                if (!(bool)uVar13) {
                    iVar3 = -1;
                }
                iVar5 = 9;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NOTATION";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    uVar11 = *pbVar9 < *pbVar10;
                    uVar13 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while ((bool)uVar13);
                iVar6 = 9;
                iVar5 = 1;
                if (!(bool)uVar13) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NAMELOCK";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    uVar11 = *pbVar9 < *pbVar10;
                    uVar13 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while ((bool)uVar13);
                iVar6 = 0x11;
                iVar3 = 2;
                if (!(bool)uVar13) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"STATEMENT_REPORT";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    uVar11 = *pbVar9 < *pbVar10;
                    uVar13 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while ((bool)uVar13);
                iVar6 = 0xb;
                iVar5 = 3;
                if (!(bool)uVar13) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)0x810fcf3;
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    uVar11 = *pbVar9 < *pbVar10;
                    uVar13 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while ((bool)uVar13);
                iVar6 = 0xe;
                iVar3 = 4;
                if (!(bool)uVar13) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"FINAL_WARNING";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    uVar11 = *pbVar9 < *pbVar10;
                    uVar13 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while ((bool)uVar13);
                iVar6 = 0xe;
                iVar5 = 5;
                if (!(bool)uVar13) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"IP_BANISHMENT";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    uVar11 = *pbVar9 < *pbVar10;
                    uVar13 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while ((bool)uVar13);
                iVar6 = 5;
                iVar3 = 6;
                if (!(bool)uVar13) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = &DAT_0810fb59;
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    uVar11 = *pbVar9 < *pbVar10;
                    uVar13 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while ((bool)uVar13);
                iVar6 = 0xe;
                iVar5 = 7;
                if (!(bool)uVar13) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"HOME_TELEPORT";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    uVar11 = *pbVar9 < *pbVar10;
                    uVar13 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while ((bool)uVar13);
                iVar6 = 0x15;
                iVar3 = 8;
                if (!(bool)uVar13) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CLEAR_CHARACTER_INFO";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    uVar11 = *pbVar9 < *pbVar10;
                    uVar13 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while ((bool)uVar13);
                bVar12 = (!(bool)uVar11 && !(bool)uVar13) < (byte)uVar11;
                bVar14 = (!(bool)uVar11 && !(bool)uVar13) == (bool)uVar11;
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0xd;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CLEAR_GUILDS";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0xe;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"DELETE_GUILDS";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0xd;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"BOARD_REPORT";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0x11;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"BOARD_MODERATION";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0x15;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"BOARD_ANONYMOUS_EDIT";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0x13;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"BOARD_PRECONFIRMED";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0xd;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"KEEP_ACCOUNT";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0x15;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"GAMEMASTER_BROADCAST";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar5 = 9;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                iVar3 = 0x14;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"ANONYMOUS_BROADCAST";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar3 = 10;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                iVar5 = 8;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"INVITED";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0xe;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NO_BANISHMENT";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar5 = 0xb;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                iVar3 = 0x12;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"ALLOW_MULTICLIENT";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar5 = -2;
                }
                iVar3 = 0xf;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CIPWATCH_ADMIN";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar5 = -2;
                }
                iVar3 = 0xe;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CIPWATCH_USER";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar5 = -2;
                }
                iVar3 = 0x16;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CREATECHAR_GAMEMASTER";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar5 = -2;
                }
                iVar3 = 0xf;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CREATECHAR_GOD";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar5 = -2;
                }
                iVar3 = 0x10;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CREATECHAR_TEST";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar5 = -2;
                }
                iVar3 = 0xd;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"VIEW_ACCOUNT";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar5 = -2;
                }
                iVar3 = 0x17;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"VIEW_GAMEMASTER_RECORD";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar5 = -2;
                }
                iVar3 = 0x15;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"VIEW_CRIMINAL_RECORD";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar5 = -2;
                }
                iVar3 = 0xf;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"VIEW_LOG_FILES";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar5 = -2;
                }
                iVar3 = 0x12;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"LOG_COMMUNICATION";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar3 = 0xd;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                iVar5 = 0x12;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"MODIFY_BANISHMENT";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0x18;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"READ_GAMEMASTER_CHANNEL";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar5 = 0xe;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                iVar3 = 0x13;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"READ_TUTOR_CHANNEL";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar3 = 0xf;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                iVar5 = 0x17;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"HIGHLIGHT_HELP_CHANNEL";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar5 = 0x10;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                iVar3 = 0x10;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"SEND_BUGREPORTS";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar3 = 0x11;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                iVar5 = 0xc;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"APPOINT_CIP";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0xc;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"APPOINT_SGM";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0xc;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"APPOINT_JGM";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0x10;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"APPOINT_SENATOR";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0x13;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"SET_ACCOUNT_RIGHTS";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0x15;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"SET_CHARACTER_RIGHTS";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0x18;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"SET_ACCOUNTGROUP_RIGHTS";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                if (bVar14) {
                    iVar3 = -2;
                }
                iVar5 = 0x1a;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"SET_CHARACTERGROUP_RIGHTS";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xf;
                iVar5 = -2;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NAME_INSULTING";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xe;
                iVar3 = 0x12;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NAME_SENTENCE";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x19;
                iVar5 = 0x13;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NAME_NONSENSICAL_LETTERS";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x15;
                iVar3 = 0x14;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NAME_BADLY_FORMATTED";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xf;
                iVar5 = 0x15;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NAME_NO_PERSON";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xf;
                iVar3 = 0x16;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NAME_CELEBRITY";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xd;
                iVar5 = 0x17;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NAME_COUNTRY";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x13;
                iVar3 = 0x18;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NAME_FAKE_IDENTITY";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x13;
                iVar5 = 0x19;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NAME_FAKE_POSITION";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x14;
                iVar3 = 0x1a;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"STATEMENT_INSULTING";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar5 = 0x1b;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                iVar3 = 0x13;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"STATEMENT_SPAMMING";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x1a;
                iVar3 = 0x1c;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"STATEMENT_ADVERT_OFFTOPIC";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x17;
                iVar5 = 0x1d;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"STATEMENT_ADVERT_MONEY";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x16;
                iVar3 = 0x1e;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"STATEMENT_NON_ENGLISH";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x1b;
                iVar5 = 0x1f;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"STATEMENT_CHANNEL_OFFTOPIC";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x1d;
                iVar3 = 0x20;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"STATEMENT_VIOLATION_INCITING";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x13;
                iVar5 = 0x21;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CHEATING_BUG_ABUSE";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x17;
                iVar3 = 0x22;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CHEATING_GAME_WEAKNESS";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x13;
                iVar5 = 0x23;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CHEATING_MACRO_USE";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar3 = 0x24;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                iVar5 = 0x19;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CHEATING_MODIFIED_CLIENT";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x11;
                iVar5 = 0x25;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CHEATING_HACKING";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x16;
                iVar3 = 0x26;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CHEATING_MULTI_CLIENT";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x19;
                iVar5 = 0x27;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CHEATING_ACCOUNT_TRADING";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x19;
                iVar3 = 0x28;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CHEATING_ACCOUNT_SHARING";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x17;
                iVar5 = 0x29;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"GAMEMASTER_THREATENING";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x16;
                iVar3 = 0x2a;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"GAMEMASTER_PRETENDING";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x15;
                iVar5 = 0x2b;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"GAMEMASTER_INFLUENCE";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x19;
                iVar3 = 0x2c;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"GAMEMASTER_FALSE_REPORTS";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar5 = 0x2d;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                iVar3 = 0x1e;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"KILLING_EXCESSIVE_UNJUSTIFIED";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x16;
                iVar3 = 0x2e;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"DESTRUCTIVE_BEHAVIOUR";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x11;
                iVar5 = 0x2f;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"SPOILING_AUCTION";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x10;
                iVar3 = 0x30;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"INVALID_PAYMENT";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x16;
                iVar5 = 0x31;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"TELEPORT_TO_CHARACTER";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x11;
                iVar3 = 0x32;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"TELEPORT_TO_MARK";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x12;
                iVar5 = 0x33;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"TELEPORT_VERTICAL";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x17;
                iVar3 = 0x34;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"TELEPORT_TO_COORDINATE";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 9;
                iVar5 = 0x35;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"LEVITATE";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar3 = 0x36;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                iVar5 = 0x10;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"SPECIAL_MOVEUSE";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x12;
                iVar5 = 0x37;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"MODIFY_GOSTRENGTH";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x10;
                iVar3 = 0x38;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"SHOW_COORDINATE";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 9;
                iVar5 = 0x39;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"RETRIEVE";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xd;
                iVar3 = 0x3a;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"INVULNERABLE";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xf;
                iVar5 = 0x3d;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"UNLIMITED_MANA";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xf;
                iVar3 = 0x3e;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"KEEP_INVENTORY";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xb;
                iVar5 = 0x3f;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"ALL_SPELLS";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x13;
                iVar3 = 0x40;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"UNLIMITED_CAPACITY";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar5 = 0x41;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                iVar3 = 0xe;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"ZERO_CAPACITY";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x12;
                iVar3 = 0x42;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"ATTACK_EVERYWHERE";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 10;
                iVar5 = 0x43;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NO_ATTACK";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 9;
                iVar3 = 0x44;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NO_RUNES";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x10;
                iVar5 = 0x45;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NO_LOGOUT_BLOCK";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x12;
                iVar3 = 0x46;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"GAMEMASTER_OUTFIT";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xb;
                iVar5 = 0x47;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"ILLUMINATE";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x12;
                iVar3 = 0x48;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CHANGE_PROFESSION";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x14;
                iVar5 = 0x49;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"IGNORED_BY_MONSTERS";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar3 = 0x4a;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                iVar5 = 0x15;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"SHOW_KEYHOLE_NUMBERS";
                do {
                    if (iVar5 == 0) break;
                    iVar5 = iVar5 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xf;
                iVar5 = 0x4b;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CREATE_OBJECTS";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xd;
                iVar3 = 0x4c;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CREATE_MONEY";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0x10;
                iVar5 = 0x4d;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CREATE_MONSTERS";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xe;
                iVar3 = 0x4e;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CHANGE_SKILLS";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xf;
                iVar5 = 0x4f;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"CLEANUP_FIELDS";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xe;
                iVar3 = 0x50;
                if (!bVar14) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"NO_STATISTICS";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                iVar6 = 0xf;
                iVar5 = 0x51;
                if (!bVar14) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"KEEP_CHARACTER";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 < *pbVar10;
                    bVar14 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar14);
                bVar12 = (!bVar12 && !bVar14) == bVar12;
                if (bVar12) {
                    iVar5 = -2;
                }
                iVar3 = 0x10;
                pbVar9 = local_3c;
                pbVar10 = (byte *)"EXTRA_CHARACTER";
                do {
                    if (iVar3 == 0) break;
                    iVar3 = iVar3 + -1;
                    bVar12 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar12);
                iVar6 = 0xd;
                iVar3 = -2;
                if (!bVar12) {
                    iVar3 = iVar5;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"ENTER_HOUSES";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar12);
                iVar6 = 0x10;
                iVar5 = 0x3b;
                if (!bVar12) {
                    iVar5 = iVar3;
                }
                pbVar9 = local_3c;
                pbVar10 = (byte *)"OPEN_NAMEDDOORS";
                do {
                    if (iVar6 == 0) break;
                    iVar6 = iVar6 + -1;
                    bVar12 = *pbVar9 == *pbVar10;
                    pbVar9 = pbVar9 + 1;
                    pbVar10 = pbVar10 + 1;
                } while (bVar12);
                iVar3 = 0x3c;
                if (!bVar12) {
                    iVar3 = iVar5;
                }
                if (iVar3 != -2) {
                    if (iVar3 == -1) {
                        error("TQueryManagerConnection::loginGame: Unbekanntes Recht %s.\n",local_3c
                             );
                    }
                    else {
                        iVar5 = iVar3 + 7;
                        if (-1 < iVar3) {
                            iVar5 = iVar3;
                        }
                        local_60 = (char)iVar3 + (char)(iVar5 >> 3) * -8;
                        Rights[iVar5 >> 3] = Rights[iVar5 >> 3] | (byte)(1 << (local_60 & 0x1f));
                    }
                }
                local_50 = local_50 + 1;
                uVar11 = local_50 < uVar7;
                uVar13 = local_50 == uVar7;
            } while ((int)local_50 < (int)uVar7);
        }
                    // try { // try from 080e2b7b to 080e2b7f has its CatchHandler @ 080e2fa2
        uVar2 = TReadBuffer::readByte(this_01);
        *PremiumAccountActivated = uVar2 != '\0';
        iVar3 = 0;
    }
    else {
        if (iVar3 == 1) {
                    // try { // try from 080e1d0c to 080e1d10 has its CatchHandler @ 080e2fd4
            bVar1 = TReadBuffer::readByte(&this->ReadBuffer);
            uVar7 = (uint)bVar1;
            bVar12 = false;
            if ((uVar7 != 0) && (uVar7 < 0x10)) {
                bVar12 = true;
            }
            if (bVar12) {
                return uVar7;
            }
            error(&DAT_0810e380,uVar7);
        }
        iVar3 = -1;
    }
    return iVar3;
}



// DWARF original prototype: int logoutGame(TQueryManagerConnection * this, ulong CharacterID, int
// Level, char * Profession, char * Residence, time_t LastLoginTime, int TutorActivities)

int __thiscall TQueryManagerConnection::logoutGame(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    int iVar1;
    ulong in_stack_00000008;
    ushort in_stack_0000000c;
    char *in_stack_00000010;
    char *in_stack_00000014;
    ulong in_stack_00000018;
    ushort in_stack_0000001c;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e3035 to 080e3049 has its CatchHandler @ 080e30d0
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'\x15');
                    // try { // try from 080e3054 to 080e3058 has its CatchHandler @ 080e3110
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e3064 to 080e3068 has its CatchHandler @ 080e3142
    TWriteBuffer::writeWord(this_00,in_stack_0000000c);
                    // try { // try from 080e3073 to 080e3077 has its CatchHandler @ 080e3172
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000010);
                    // try { // try from 080e3082 to 080e3086 has its CatchHandler @ 080e31a2
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000014);
                    // try { // try from 080e3091 to 080e3095 has its CatchHandler @ 080e31d6
    TWriteBuffer::writeQuad(this_00,in_stack_00000018);
                    // try { // try from 080e30a1 to 080e30a5 has its CatchHandler @ 080e320a
    TWriteBuffer::writeWord(this_00,in_stack_0000001c);
    iVar1 = executeQuery(this,0x78,true);
    return (iVar1 == 0) - 1;
}



// DWARF original prototype: int setNotation(TQueryManagerConnection * this, ulong GamemasterID,
// char * PlayerName, char * IPAddress, char * Reason, char * Comment, ulong * BanishmentID)

int __thiscall TQueryManagerConnection::setNotation(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    bool bVar1;
    byte bVar2;
    int iVar3;
    ulong uVar4;
    int FailureCode;
    uint uVar5;
    ulong in_stack_00000008;
    char *in_stack_0000000c;
    char *in_stack_00000010;
    char *in_stack_00000014;
    char *in_stack_00000018;
    ulong *in_stack_0000001c;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e3265 to 080e3279 has its CatchHandler @ 080e3350
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'\x1a');
                    // try { // try from 080e3284 to 080e3288 has its CatchHandler @ 080e3390
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e3293 to 080e3297 has its CatchHandler @ 080e33c2
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_0000000c);
                    // try { // try from 080e32a2 to 080e32a6 has its CatchHandler @ 080e33f2
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000010);
                    // try { // try from 080e32b1 to 080e32b5 has its CatchHandler @ 080e3422
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000014);
                    // try { // try from 080e32c0 to 080e32c4 has its CatchHandler @ 080e3456
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000018);
    iVar3 = executeQuery(this,0x1e,true);
    if (iVar3 == 0) {
                    // try { // try from 080e3340 to 080e3344 has its CatchHandler @ 080e348a
        uVar4 = TReadBuffer::readQuad(&this->ReadBuffer);
        *in_stack_0000001c = uVar4;
        iVar3 = 0;
    }
    else {
        if (iVar3 == 1) {
                    // try { // try from 080e3303 to 080e3307 has its CatchHandler @ 080e34bc
            bVar2 = TReadBuffer::readByte(&this->ReadBuffer);
            uVar5 = (uint)bVar2;
            bVar1 = false;
            if ((uVar5 != 0) && (uVar5 < 3)) {
                bVar1 = true;
            }
            if (bVar1) {
                return uVar5;
            }
            error(&DAT_0810e4a0,uVar5);
        }
        else {
            error("TQueryManagerConnection::setNotation: Anfrage fehlgeschlagen.\n");
        }
        iVar3 = -1;
    }
    return iVar3;
}



// DWARF original prototype: int setNamelock(TQueryManagerConnection * this, ulong GamemasterID,
// char * PlayerName, char * IPAddress, char * Reason, char * Comment)

int __thiscall TQueryManagerConnection::setNamelock(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    bool bVar1;
    byte bVar2;
    int iVar3;
    int FailureCode;
    int iVar4;
    uint uVar5;
    ulong in_stack_00000008;
    char *in_stack_0000000c;
    char *in_stack_00000010;
    char *in_stack_00000014;
    char *in_stack_00000018;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e3515 to 080e3529 has its CatchHandler @ 080e35f0
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'\x17');
                    // try { // try from 080e3534 to 080e3538 has its CatchHandler @ 080e3630
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e3543 to 080e3547 has its CatchHandler @ 080e3662
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_0000000c);
                    // try { // try from 080e3552 to 080e3556 has its CatchHandler @ 080e3692
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000010);
                    // try { // try from 080e3561 to 080e3565 has its CatchHandler @ 080e36c2
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000014);
                    // try { // try from 080e3570 to 080e3574 has its CatchHandler @ 080e36f6
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000018);
    iVar3 = executeQuery(this,0x1e,true);
    iVar4 = 0;
    if (iVar3 != 0) {
        if (iVar3 == 1) {
                    // try { // try from 080e35b7 to 080e35bb has its CatchHandler @ 080e372a
            bVar2 = TReadBuffer::readByte(&this->ReadBuffer);
            uVar5 = (uint)bVar2;
            bVar1 = false;
            if ((uVar5 != 0) && (uVar5 < 5)) {
                bVar1 = true;
            }
            if (bVar1) {
                return uVar5;
            }
            error(&DAT_0810e540,uVar5);
        }
        else {
            error("TQueryManagerConnection::setNamelock: Anfrage fehlgeschlagen.\n");
        }
        iVar4 = -1;
    }
    return iVar4;
}



// DWARF original prototype: int banishAccount(TQueryManagerConnection * this, ulong GamemasterID,
// char * PlayerName, char * IPAddress, char * Reason, char * Comment, bool * FinalWarning, int *
// Days, ulong * BanishmentID)

int __thiscall TQueryManagerConnection::banishAccount(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    TReadBuffer *this_01;
    bool bVar1;
    byte bVar2;
    uchar uVar3;
    int iVar4;
    ulong uVar5;
    int FailureCode;
    uint uVar6;
    ulong in_stack_00000008;
    char *in_stack_0000000c;
    char *in_stack_00000010;
    char *in_stack_00000014;
    char *in_stack_00000018;
    char *in_stack_0000001c;
    uint *in_stack_00000020;
    ulong *in_stack_00000024;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e3785 to 080e3799 has its CatchHandler @ 080e38c0
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'\x19');
                    // try { // try from 080e37a4 to 080e37a8 has its CatchHandler @ 080e3900
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e37b3 to 080e37b7 has its CatchHandler @ 080e3932
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_0000000c);
                    // try { // try from 080e37c2 to 080e37c6 has its CatchHandler @ 080e3962
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000010);
                    // try { // try from 080e37d1 to 080e37d5 has its CatchHandler @ 080e3992
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000014);
                    // try { // try from 080e37e0 to 080e37e4 has its CatchHandler @ 080e39c6
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000018);
                    // try { // try from 080e37f8 to 080e37fc has its CatchHandler @ 080e39fa
    TWriteBuffer::writeByte(this_00,*in_stack_0000001c != '\0');
    iVar4 = executeQuery(this,0x1e,true);
    if (iVar4 == 0) {
        this_01 = &this->ReadBuffer;
                    // try { // try from 080e3878 to 080e387c has its CatchHandler @ 080e3a2e
        uVar5 = TReadBuffer::readQuad(this_01);
        *in_stack_00000024 = uVar5;
                    // try { // try from 080e3887 to 080e388b has its CatchHandler @ 080e3a60
        bVar2 = TReadBuffer::readByte(this_01);
        uVar6 = 0xffffffff;
        if (bVar2 != 0xff) {
            uVar6 = (uint)bVar2;
        }
        *in_stack_00000020 = uVar6;
                    // try { // try from 080e38a8 to 080e38ac has its CatchHandler @ 080e3a92
        uVar3 = TReadBuffer::readByte(this_01);
        *in_stack_0000001c = uVar3 != '\0';
        iVar4 = 0;
    }
    else {
        if (iVar4 == 1) {
                    // try { // try from 080e383b to 080e383f has its CatchHandler @ 080e3ac4
            bVar2 = TReadBuffer::readByte(&this->ReadBuffer);
            uVar6 = (uint)bVar2;
            bVar1 = false;
            if ((uVar6 != 0) && (uVar6 < 4)) {
                bVar1 = true;
            }
            if (bVar1) {
                return uVar6;
            }
            error(&DAT_0810e600,uVar6);
        }
        else {
            error("TQueryManagerConnection::banishAccount: Anfrage fehlgeschlagen.\n");
        }
        iVar4 = -1;
    }
    return iVar4;
}



// WARNING: Variable defined which should be unmapped: i_1
// DWARF original prototype: int reportStatement(TQueryManagerConnection * this, ulong ReporterID,
// char * PlayerName, char * Reason, char * Comment, ulong BanishmentID, ulong StatementID, int
// NumberOfStatements, ulong * StatementIDs, ulong * TimeStamps, ulong * CharacterIDs, char[30] *
// Channels, char[256] * Texts)

int __thiscall TQueryManagerConnection::reportStatement(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    int iVar1;
    bool bVar2;
    byte bVar3;
    int iVar4;
    int i;
    int iVar5;
    uint uVar6;
    ulong in_stack_00000008;
    char *in_stack_0000000c;
    char *in_stack_00000010;
    char *in_stack_00000014;
    ulong in_stack_00000018;
    ulong in_stack_0000001c;
    int in_stack_00000020;
    int *in_stack_00000024;
    int in_stack_00000028;
    int in_stack_0000002c;
    int in_stack_00000030;
    int in_stack_00000034;
    int local_14;
    int i_1;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e3b25 to 080e3b39 has its CatchHandler @ 080e3ce0
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'\x1b');
                    // try { // try from 080e3b44 to 080e3b48 has its CatchHandler @ 080e3d20
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e3b53 to 080e3b57 has its CatchHandler @ 080e3d52
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_0000000c);
                    // try { // try from 080e3b62 to 080e3b66 has its CatchHandler @ 080e3d82
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000010);
                    // try { // try from 080e3b71 to 080e3b75 has its CatchHandler @ 080e3db2
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000014);
                    // try { // try from 080e3b80 to 080e3b84 has its CatchHandler @ 080e3de6
    TWriteBuffer::writeQuad(this_00,in_stack_00000018);
                    // try { // try from 080e3b8f to 080e3b93 has its CatchHandler @ 080e3e1a
    TWriteBuffer::writeQuad(this_00,in_stack_0000001c);
    iVar5 = 0;
    iVar4 = in_stack_00000020;
    if (0 < in_stack_00000020) {
        iVar1 = *in_stack_00000024;
        while (iVar1 == 0) {
            iVar5 = iVar5 + 1;
            iVar4 = iVar4 + -1;
            if (in_stack_00000020 <= iVar5) break;
            iVar1 = in_stack_00000024[iVar5];
        }
    }
                    // try { // try from 080e3bcb to 080e3bcf has its CatchHandler @ 080e3e4e
    TWriteBuffer::writeWord(this_00,(ushort)iVar4);
    local_14 = 0;
    if (0 < in_stack_00000020) {
        do {
            if (in_stack_00000024[local_14] != 0) {
                    // try { // try from 080e3c77 to 080e3c7b has its CatchHandler @ 080e3e82
                TWriteBuffer::writeQuad(this_00,in_stack_00000024[local_14]);
                    // try { // try from 080e3c8c to 080e3c90 has its CatchHandler @ 080e3eb6
                TWriteBuffer::writeQuad(this_00,*(ulong *)(in_stack_00000028 + local_14 * 4));
                    // try { // try from 080e3ca1 to 080e3ca5 has its CatchHandler @ 080e3eea
                TWriteBuffer::writeQuad(this_00,*(ulong *)(in_stack_0000002c + local_14 * 4));
                    // try { // try from 080e3cbe to 080e3cc2 has its CatchHandler @ 080e3f1e
                TWriteStream::writeString
                          (&this_00->super_TWriteStream,
                           (char *)(in_stack_00000030 + local_14 * 0x1e));
                    // try { // try from 080e3cd5 to 080e3cd9 has its CatchHandler @ 080e3f52
                TWriteStream::writeString
                          (&this_00->super_TWriteStream,
                           (char *)(local_14 * 0x100 + in_stack_00000034));
            }
            local_14 = local_14 + 1;
        } while (local_14 < in_stack_00000020);
    }
    iVar5 = executeQuery(this,0xb4,true);
    iVar4 = 0;
    if (iVar5 != 0) {
        if (iVar5 == 1) {
                    // try { // try from 080e3c3d to 080e3c41 has its CatchHandler @ 080e3f86
            bVar3 = TReadBuffer::readByte(&this->ReadBuffer);
            uVar6 = (uint)bVar3;
            bVar2 = false;
            if ((uVar6 != 0) && (uVar6 < 3)) {
                bVar2 = true;
            }
            if (bVar2) {
                return uVar6;
            }
            error(&DAT_0810e6c0,uVar6);
        }
        else {
            error("TQueryManagerConnection::reportStatement: Anfrage fehlgeschlagen.\n");
        }
        iVar4 = -1;
    }
    return iVar4;
}



// DWARF original prototype: int banishIPAddress(TQueryManagerConnection * this, ulong GamemasterID,
// char * PlayerName, char * IPAddress, char * Reason, char * Comment)

int __thiscall TQueryManagerConnection::banishIPAddress(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    bool bVar1;
    byte bVar2;
    int iVar3;
    int FailureCode;
    int iVar4;
    uint uVar5;
    ulong in_stack_00000008;
    char *in_stack_0000000c;
    char *in_stack_00000010;
    char *in_stack_00000014;
    char *in_stack_00000018;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e3fe5 to 080e3ff9 has its CatchHandler @ 080e40c0
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'\x1c');
                    // try { // try from 080e4004 to 080e4008 has its CatchHandler @ 080e4100
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e4013 to 080e4017 has its CatchHandler @ 080e4132
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_0000000c);
                    // try { // try from 080e4022 to 080e4026 has its CatchHandler @ 080e4162
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000010);
                    // try { // try from 080e4031 to 080e4035 has its CatchHandler @ 080e4192
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000014);
                    // try { // try from 080e4040 to 080e4044 has its CatchHandler @ 080e41c6
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000018);
    iVar3 = executeQuery(this,0x1e,true);
    iVar4 = 0;
    if (iVar3 != 0) {
        if (iVar3 == 1) {
                    // try { // try from 080e4087 to 080e408b has its CatchHandler @ 080e41fa
            bVar2 = TReadBuffer::readByte(&this->ReadBuffer);
            uVar5 = (uint)bVar2;
            bVar1 = false;
            if ((uVar5 != 0) && (uVar5 < 3)) {
                bVar1 = true;
            }
            if (bVar1) {
                return uVar5;
            }
            error(&DAT_0810e780,uVar5);
        }
        else {
            error("TQueryManagerConnection::banishIPAddress: Anfrage fehlgeschlagen.\n");
        }
        iVar4 = -1;
    }
    return iVar4;
}



// WARNING: Variable defined which should be unmapped: Unjustified_local
// DWARF original prototype: int logCharacterDeath(TQueryManagerConnection * this, ulong
// CharacterID, int Level, ulong Offender, char * Remark, bool Unjustified, time_t Time)

int __thiscall
TQueryManagerConnection::logCharacterDeath
          (TQueryManagerConnection *this,ulong CharacterID,int Level,ulong Offender,char *Remark,
          bool Unjustified,time_t Time)

{
    TWriteBuffer *this_00;
    int iVar1;
    bool Unjustified_local;
    
    this_00 = &this->WriteBuffer;
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e425c to 080e4270 has its CatchHandler @ 080e4310
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'\x1d');
                    // try { // try from 080e427b to 080e427f has its CatchHandler @ 080e4350
    TWriteBuffer::writeQuad(this_00,CharacterID);
                    // try { // try from 080e428b to 080e428f has its CatchHandler @ 080e4382
    TWriteBuffer::writeWord(this_00,(ushort)Level);
                    // try { // try from 080e429a to 080e429e has its CatchHandler @ 080e43b2
    TWriteBuffer::writeQuad(this_00,Offender);
                    // try { // try from 080e42a9 to 080e42ad has its CatchHandler @ 080e43e2
    TWriteStream::writeString(&this_00->super_TWriteStream,Remark);
                    // try { // try from 080e42be to 080e42c2 has its CatchHandler @ 080e4416
    TWriteBuffer::writeByte(this_00,Unjustified);
                    // try { // try from 080e42cd to 080e42d1 has its CatchHandler @ 080e444a
    TWriteBuffer::writeQuad(this_00,Time);
    iVar1 = executeQuery(this,0x5a,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::logCharacterDeath: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int addBuddy(TQueryManagerConnection * this, ulong AccountID, ulong
// Buddy)

int __thiscall TQueryManagerConnection::addBuddy(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    int iVar1;
    ulong in_stack_00000008;
    ulong in_stack_0000000c;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e44a5 to 080e44b9 has its CatchHandler @ 080e4513
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'\x1e');
                    // try { // try from 080e44c4 to 080e44c8 has its CatchHandler @ 080e4550
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e44d3 to 080e44d7 has its CatchHandler @ 080e4582
    TWriteBuffer::writeQuad(this_00,in_stack_0000000c);
    iVar1 = executeQuery(this,0x5a,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::addBuddy: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int removeBuddy(TQueryManagerConnection * this, ulong AccountID, ulong
// Buddy)

int __thiscall TQueryManagerConnection::removeBuddy(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    int iVar1;
    ulong in_stack_00000008;
    ulong in_stack_0000000c;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e45d5 to 080e45e9 has its CatchHandler @ 080e4643
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'\x1f');
                    // try { // try from 080e45f4 to 080e45f8 has its CatchHandler @ 080e4680
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e4603 to 080e4607 has its CatchHandler @ 080e46b2
    TWriteBuffer::writeQuad(this_00,in_stack_0000000c);
    iVar1 = executeQuery(this,0x5a,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::removeBuddy: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int decrementIsOnline(TQueryManagerConnection * this, ulong
// CharacterID)

int __thiscall
TQueryManagerConnection::decrementIsOnline(TQueryManagerConnection *this,ulong CharacterID)

{
    TWriteBuffer *this_00;
    int iVar1;
    
    this_00 = &this->WriteBuffer;
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e470b to 080e471f has its CatchHandler @ 080e4773
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,' ');
                    // try { // try from 080e472a to 080e472e has its CatchHandler @ 080e47b0
    TWriteBuffer::writeQuad(this_00,CharacterID);
    iVar1 = executeQuery(this,0x1e,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::decrementIsOnline: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int finishAuctions(TQueryManagerConnection * this, int *
// NumberOfAuctions, ushort * HouseIDs, ulong * CharacterIDs, char[30] * CharacterNames, int * Bids)

int __thiscall
TQueryManagerConnection::finishAuctions
          (TQueryManagerConnection *this,int *NumberOfAuctions,ushort *HouseIDs,ulong *CharacterIDs,
          char (*CharacterNames) [30],int *Bids)

{
    ushort uVar1;
    int iVar2;
    ulong uVar3;
    uint uVar4;
    TReadBuffer *this_00;
    int MaxNumberOfAuctions;
    
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e4805 to 080e4819 has its CatchHandler @ 080e4900
    TWriteBuffer::writeWord(&this->WriteBuffer,0);
    TWriteBuffer::writeByte(&this->WriteBuffer,'!');
    iVar2 = executeQuery(this,0x3c,true);
    if (iVar2 == 0) {
        this_00 = &this->ReadBuffer;
        iVar2 = *NumberOfAuctions;
                    // try { // try from 080e485b to 080e485f has its CatchHandler @ 080e4940
        uVar1 = TReadBuffer::readWord(this_00);
        uVar4 = (uint)uVar1;
        *NumberOfAuctions = uVar4;
        if ((int)uVar4 <= iVar2) {
            iVar2 = 0;
            if (uVar4 != 0) {
                do {
                    // try { // try from 080e487c to 080e4880 has its CatchHandler @ 080e496b
                    uVar1 = TReadBuffer::readWord(this_00);
                    HouseIDs[iVar2] = uVar1;
                    // try { // try from 080e488e to 080e4892 has its CatchHandler @ 080e4996
                    uVar3 = TReadBuffer::readQuad(this_00);
                    CharacterIDs[iVar2] = uVar3;
                    // try { // try from 080e48ba to 080e48be has its CatchHandler @ 080e49c4
                    TReadStream::readString(&this_00->super_TReadStream,CharacterNames[iVar2],0x1e);
                    // try { // try from 080e48c2 to 080e48c6 has its CatchHandler @ 080e49fa
                    uVar3 = TReadBuffer::readQuad(this_00);
                    Bids[iVar2] = uVar3;
                    iVar2 = iVar2 + 1;
                } while (iVar2 < *NumberOfAuctions);
            }
            return 0;
        }
        error("TQueryManagerConnection::finishAuctions: zu viele Auktionen (%d>%d).\n",
              *NumberOfAuctions,iVar2);
    }
    else {
        error("TQueryManagerConnection::finishAuctions: Anfrage fehlgeschlagen.\n");
    }
    return -1;
}



// WARNING: Variable defined which should be unmapped: Banish_local
// DWARF original prototype: int excludeFromAuctions(TQueryManagerConnection * this, ulong
// CharacterID, bool Banish)

int __thiscall
TQueryManagerConnection::excludeFromAuctions
          (TQueryManagerConnection *this,ulong CharacterID,bool Banish)

{
    TWriteBuffer *this_00;
    int iVar1;
    bool Banish_local;
    
    this_00 = &this->WriteBuffer;
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e4a5c to 080e4a70 has its CatchHandler @ 080e4ad3
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'3');
                    // try { // try from 080e4a7b to 080e4a7f has its CatchHandler @ 080e4b10
    TWriteBuffer::writeQuad(this_00,CharacterID);
    TWriteBuffer::writeByte(this_00,Banish);
    iVar1 = executeQuery(this,0x1e,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::excludeFromAuctions: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int transferHouses(TQueryManagerConnection * this, int *
// NumberOfTransfers, ushort * HouseIDs, ulong * NewOwnerIDs, char[30] * NewOwnerNames, int *
// Prices)

int __thiscall
TQueryManagerConnection::transferHouses
          (TQueryManagerConnection *this,int *NumberOfTransfers,ushort *HouseIDs,ulong *NewOwnerIDs,
          char (*NewOwnerNames) [30],int *Prices)

{
    ushort uVar1;
    int iVar2;
    ulong uVar3;
    uint uVar4;
    TReadBuffer *this_00;
    int MaxNumberOfTransfers;
    
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e4b65 to 080e4b79 has its CatchHandler @ 080e4c60
    TWriteBuffer::writeWord(&this->WriteBuffer,0);
    TWriteBuffer::writeByte(&this->WriteBuffer,'#');
    iVar2 = executeQuery(this,0x3c,true);
    if (iVar2 == 0) {
        this_00 = &this->ReadBuffer;
        iVar2 = *NumberOfTransfers;
                    // try { // try from 080e4bbb to 080e4bbf has its CatchHandler @ 080e4ca0
        uVar1 = TReadBuffer::readWord(this_00);
        uVar4 = (uint)uVar1;
        *NumberOfTransfers = uVar4;
        if ((int)uVar4 <= iVar2) {
            iVar2 = 0;
            if (uVar4 != 0) {
                do {
                    // try { // try from 080e4bdc to 080e4be0 has its CatchHandler @ 080e4ccb
                    uVar1 = TReadBuffer::readWord(this_00);
                    HouseIDs[iVar2] = uVar1;
                    // try { // try from 080e4bee to 080e4bf2 has its CatchHandler @ 080e4cf6
                    uVar3 = TReadBuffer::readQuad(this_00);
                    NewOwnerIDs[iVar2] = uVar3;
                    // try { // try from 080e4c1a to 080e4c1e has its CatchHandler @ 080e4d24
                    TReadStream::readString(&this_00->super_TReadStream,NewOwnerNames[iVar2],0x1e);
                    // try { // try from 080e4c22 to 080e4c26 has its CatchHandler @ 080e4d5a
                    uVar3 = TReadBuffer::readQuad(this_00);
                    Prices[iVar2] = uVar3;
                    iVar2 = iVar2 + 1;
                } while (iVar2 < *NumberOfTransfers);
            }
            return 0;
        }
        error("TQueryManagerConnection::transferHouses: zu viele Transfers (%d>%d).\n",
              *NumberOfTransfers,iVar2);
    }
    else {
        error("TQueryManagerConnection::transferHouses: Anfrage fehlgeschlagen.\n");
    }
    return -1;
}



// WARNING: Variable defined which should be unmapped: HouseID_local
// DWARF original prototype: int cancelHouseTransfer(TQueryManagerConnection * this, ushort HouseID)

int __thiscall
TQueryManagerConnection::cancelHouseTransfer(TQueryManagerConnection *this,ushort HouseID)

{
    TWriteBuffer *this_00;
    int iVar1;
    ushort HouseID_local;
    
    this_00 = &this->WriteBuffer;
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e4dc2 to 080e4dd6 has its CatchHandler @ 080e4e24
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'4');
                    // try { // try from 080e4de1 to 080e4de5 has its CatchHandler @ 080e4e60
    TWriteBuffer::writeWord(this_00,HouseID);
    iVar1 = executeQuery(this,0x1e,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::cancelHouseTransfer: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int evictFreeAccounts(TQueryManagerConnection * this, int *
// NumberOfEvictions, ushort * HouseIDs, ulong * OwnerIDs)

int __thiscall
TQueryManagerConnection::evictFreeAccounts
          (TQueryManagerConnection *this,int *NumberOfEvictions,ushort *HouseIDs,ulong *OwnerIDs)

{
    ushort uVar1;
    int iVar2;
    ulong uVar3;
    uint uVar4;
    TReadBuffer *this_00;
    int MaxNumberOfEvictions;
    
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e4eb5 to 080e4ec9 has its CatchHandler @ 080e4f77
    TWriteBuffer::writeWord(&this->WriteBuffer,0);
    TWriteBuffer::writeByte(&this->WriteBuffer,'$');
    iVar2 = executeQuery(this,0x3c,true);
    if (iVar2 == 0) {
        this_00 = &this->ReadBuffer;
        iVar2 = *NumberOfEvictions;
                    // try { // try from 080e4f0b to 080e4f0f has its CatchHandler @ 080e4fb1
        uVar1 = TReadBuffer::readWord(this_00);
        uVar4 = (uint)uVar1;
        *NumberOfEvictions = uVar4;
        if ((int)uVar4 <= iVar2) {
            iVar2 = 0;
            if (uVar4 != 0) {
                do {
                    // try { // try from 080e4f33 to 080e4f37 has its CatchHandler @ 080e4fdc
                    uVar1 = TReadBuffer::readWord(this_00);
                    HouseIDs[iVar2] = uVar1;
                    // try { // try from 080e4f45 to 080e4f49 has its CatchHandler @ 080e5007
                    uVar3 = TReadBuffer::readQuad(this_00);
                    OwnerIDs[iVar2] = uVar3;
                    iVar2 = iVar2 + 1;
                } while (iVar2 < *NumberOfEvictions);
            }
            return 0;
        }
        error(&DAT_0810ebc0,*NumberOfEvictions,iVar2);
    }
    else {
        error("TQueryManagerConnection::evictFreeAccounts: Anfrage fehlgeschlagen.\n");
    }
    return -1;
}



// DWARF original prototype: int evictDeletedCharacters(TQueryManagerConnection * this, int *
// NumberOfEvictions, ushort * HouseIDs)

int __thiscall
TQueryManagerConnection::evictDeletedCharacters
          (TQueryManagerConnection *this,int *NumberOfEvictions,ushort *HouseIDs)

{
    ushort uVar1;
    int iVar2;
    uint uVar3;
    int MaxNumberOfEvictions;
    
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e5055 to 080e5069 has its CatchHandler @ 080e5107
    TWriteBuffer::writeWord(&this->WriteBuffer,0);
    TWriteBuffer::writeByte(&this->WriteBuffer,'%');
    iVar2 = executeQuery(this,0x3c,true);
    if (iVar2 == 0) {
        iVar2 = *NumberOfEvictions;
                    // try { // try from 080e50ae to 080e50b2 has its CatchHandler @ 080e5141
        uVar1 = TReadBuffer::readWord(&this->ReadBuffer);
        uVar3 = (uint)uVar1;
        *NumberOfEvictions = uVar3;
        if ((int)uVar3 <= iVar2) {
            iVar2 = 0;
            if (uVar3 != 0) {
                do {
                    // try { // try from 080e50d6 to 080e50da has its CatchHandler @ 080e5170
                    uVar1 = TReadBuffer::readWord(&this->ReadBuffer);
                    HouseIDs[iVar2] = uVar1;
                    iVar2 = iVar2 + 1;
                } while (iVar2 < *NumberOfEvictions);
            }
            return 0;
        }
        error(&DAT_0810ec80,*NumberOfEvictions,iVar2);
    }
    else {
        error("TQueryManagerConnection::evictDeletedCharacters: Anfrage fehlgeschlagen.\n");
    }
    return -1;
}



// WARNING: Variable defined which should be unmapped: i
// DWARF original prototype: int evictExGuildleaders(TQueryManagerConnection * this, int
// NumberOfGuildhouses, int * NumberOfEvictions, ushort * HouseIDs, ulong * Guildleaders)

int __thiscall
TQueryManagerConnection::evictExGuildleaders
          (TQueryManagerConnection *this,int NumberOfGuildhouses,int *NumberOfEvictions,
          ushort *HouseIDs,ulong *Guildleaders)

{
    TWriteBuffer *this_00;
    ushort uVar1;
    int iVar2;
    int i_1;
    int local_14;
    int i;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e51c5 to 080e51d9 has its CatchHandler @ 080e52b0
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'&');
                    // try { // try from 080e51e5 to 080e51e9 has its CatchHandler @ 080e52f0
    TWriteBuffer::writeWord(this_00,(ushort)NumberOfGuildhouses);
    local_14 = 0;
    if (0 < NumberOfGuildhouses) {
        do {
                    // try { // try from 080e5281 to 080e5285 has its CatchHandler @ 080e5322
            TWriteBuffer::writeWord(this_00,HouseIDs[local_14]);
                    // try { // try from 080e5296 to 080e529a has its CatchHandler @ 080e5350
            TWriteBuffer::writeQuad(this_00,Guildleaders[local_14]);
            local_14 = local_14 + 1;
        } while (local_14 < NumberOfGuildhouses);
    }
    iVar2 = executeQuery(this,0x3c,true);
    if (iVar2 == 0) {
                    // try { // try from 080e5236 to 080e523a has its CatchHandler @ 080e5380
        uVar1 = TReadBuffer::readWord(&this->ReadBuffer);
        *NumberOfEvictions = (uint)uVar1;
        iVar2 = 0;
        if (uVar1 != 0) {
            do {
                    // try { // try from 080e5253 to 080e5257 has its CatchHandler @ 080e53b2
                uVar1 = TReadBuffer::readWord(&this->ReadBuffer);
                HouseIDs[iVar2] = uVar1;
                iVar2 = iVar2 + 1;
            } while (iVar2 < *NumberOfEvictions);
        }
        iVar2 = 0;
    }
    else {
        error("TQueryManagerConnection::evictExGuildleaders: Anfrage fehlgeschlagen.\n");
        iVar2 = -1;
    }
    return iVar2;
}



// WARNING: Variable defined which should be unmapped: HouseID_local
// DWARF original prototype: int insertHouseOwner(TQueryManagerConnection * this, ushort HouseID,
// ulong OwnerID, int PaidUntil)

int __thiscall
TQueryManagerConnection::insertHouseOwner
          (TQueryManagerConnection *this,ushort HouseID,ulong OwnerID,int PaidUntil)

{
    TWriteBuffer *this_00;
    int iVar1;
    ushort HouseID_local;
    
    this_00 = &this->WriteBuffer;
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e540c to 080e5420 has its CatchHandler @ 080e5487
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'\'');
                    // try { // try from 080e542b to 080e542f has its CatchHandler @ 080e54c1
    TWriteBuffer::writeWord(this_00,HouseID);
                    // try { // try from 080e543a to 080e543e has its CatchHandler @ 080e54f2
    TWriteBuffer::writeQuad(this_00,OwnerID);
                    // try { // try from 080e5449 to 080e544d has its CatchHandler @ 080e5522
    TWriteBuffer::writeQuad(this_00,PaidUntil);
    iVar1 = executeQuery(this,0x1e,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::insertHouseOwner: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// WARNING: Variable defined which should be unmapped: HouseID_local
// DWARF original prototype: int updateHouseOwner(TQueryManagerConnection * this, ushort HouseID,
// ulong OwnerID, int PaidUntil)

int __thiscall
TQueryManagerConnection::updateHouseOwner
          (TQueryManagerConnection *this,ushort HouseID,ulong OwnerID,int PaidUntil)

{
    TWriteBuffer *this_00;
    int iVar1;
    ushort HouseID_local;
    
    this_00 = &this->WriteBuffer;
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e557c to 080e5590 has its CatchHandler @ 080e55f7
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'(');
                    // try { // try from 080e559b to 080e559f has its CatchHandler @ 080e5631
    TWriteBuffer::writeWord(this_00,HouseID);
                    // try { // try from 080e55aa to 080e55ae has its CatchHandler @ 080e5662
    TWriteBuffer::writeQuad(this_00,OwnerID);
                    // try { // try from 080e55b9 to 080e55bd has its CatchHandler @ 080e5692
    TWriteBuffer::writeQuad(this_00,PaidUntil);
    iVar1 = executeQuery(this,0x1e,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::updateHouseOwner: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// WARNING: Variable defined which should be unmapped: HouseID_local
// DWARF original prototype: int deleteHouseOwner(TQueryManagerConnection * this, ushort HouseID)

int __thiscall
TQueryManagerConnection::deleteHouseOwner(TQueryManagerConnection *this,ushort HouseID)

{
    TWriteBuffer *this_00;
    int iVar1;
    ushort HouseID_local;
    
    this_00 = &this->WriteBuffer;
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e56f2 to 080e5706 has its CatchHandler @ 080e5754
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,')');
                    // try { // try from 080e5711 to 080e5715 has its CatchHandler @ 080e5790
    TWriteBuffer::writeWord(this_00,HouseID);
    iVar1 = executeQuery(this,0x1e,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::deleteHouseOwner: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int getHouseOwners(TQueryManagerConnection * this, int *
// NumberOfHouses, ushort * HouseIDs, ulong * OwnerIDs, char[30] * OwnerNames, int * PaidUntils)

int __thiscall
TQueryManagerConnection::getHouseOwners
          (TQueryManagerConnection *this,int *NumberOfHouses,ushort *HouseIDs,ulong *OwnerIDs,
          char (*OwnerNames) [30],int *PaidUntils)

{
    ushort uVar1;
    int iVar2;
    ulong uVar3;
    uint uVar4;
    TReadBuffer *this_00;
    int MaxNumberOfHouses;
    
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e57e5 to 080e57f9 has its CatchHandler @ 080e58e0
    TWriteBuffer::writeWord(&this->WriteBuffer,0);
    TWriteBuffer::writeByte(&this->WriteBuffer,'*');
    iVar2 = executeQuery(this,0x3c,true);
    if (iVar2 == 0) {
        this_00 = &this->ReadBuffer;
        iVar2 = *NumberOfHouses;
                    // try { // try from 080e583b to 080e583f has its CatchHandler @ 080e5920
        uVar1 = TReadBuffer::readWord(this_00);
        uVar4 = (uint)uVar1;
        *NumberOfHouses = uVar4;
        if ((int)uVar4 <= iVar2) {
            iVar2 = 0;
            if (uVar4 != 0) {
                do {
                    // try { // try from 080e585c to 080e5860 has its CatchHandler @ 080e594b
                    uVar1 = TReadBuffer::readWord(this_00);
                    HouseIDs[iVar2] = uVar1;
                    // try { // try from 080e586e to 080e5872 has its CatchHandler @ 080e5976
                    uVar3 = TReadBuffer::readQuad(this_00);
                    OwnerIDs[iVar2] = uVar3;
                    // try { // try from 080e589a to 080e589e has its CatchHandler @ 080e59a4
                    TReadStream::readString(&this_00->super_TReadStream,OwnerNames[iVar2],0x1e);
                    // try { // try from 080e58a2 to 080e58a6 has its CatchHandler @ 080e59da
                    uVar3 = TReadBuffer::readQuad(this_00);
                    PaidUntils[iVar2] = uVar3;
                    iVar2 = iVar2 + 1;
                } while (iVar2 < *NumberOfHouses);
            }
            return 0;
        }
        error(&DAT_0810eec0,*NumberOfHouses,iVar2);
    }
    else {
        error("TQueryManagerConnection::getHouseOwners: Anfrage fehlgeschlagen.\n");
    }
    return -1;
}



// DWARF original prototype: int getAuctions(TQueryManagerConnection * this, int * NumberOfAuctions,
// ushort * HouseIDs)

int __thiscall
TQueryManagerConnection::getAuctions
          (TQueryManagerConnection *this,int *NumberOfAuctions,ushort *HouseIDs)

{
    ushort uVar1;
    int iVar2;
    uint uVar3;
    int MaxNumberOfAuctions;
    
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e5a35 to 080e5a49 has its CatchHandler @ 080e5ae7
    TWriteBuffer::writeWord(&this->WriteBuffer,0);
    TWriteBuffer::writeByte(&this->WriteBuffer,'+');
    iVar2 = executeQuery(this,0x78,true);
    if (iVar2 == 0) {
        iVar2 = *NumberOfAuctions;
                    // try { // try from 080e5a8e to 080e5a92 has its CatchHandler @ 080e5b21
        uVar1 = TReadBuffer::readWord(&this->ReadBuffer);
        uVar3 = (uint)uVar1;
        *NumberOfAuctions = uVar3;
        if ((int)uVar3 <= iVar2) {
            iVar2 = 0;
            if (uVar3 != 0) {
                do {
                    // try { // try from 080e5ab6 to 080e5aba has its CatchHandler @ 080e5b50
                    uVar1 = TReadBuffer::readWord(&this->ReadBuffer);
                    HouseIDs[iVar2] = uVar1;
                    iVar2 = iVar2 + 1;
                } while (iVar2 < *NumberOfAuctions);
            }
            return 0;
        }
        error("TQueryManagerConnection::getAuctions: zu viele Auktionen (%d>%d).\n",
              *NumberOfAuctions,iVar2);
    }
    else {
        error("TQueryManagerConnection::getAuctions: Anfrage fehlgeschlagen.\n");
    }
    return -1;
}



// WARNING: Variable defined which should be unmapped: HouseID_local
// DWARF original prototype: int startAuction(TQueryManagerConnection * this, ushort HouseID)

int __thiscall TQueryManagerConnection::startAuction(TQueryManagerConnection *this,ushort HouseID)

{
    TWriteBuffer *this_00;
    int iVar1;
    ushort HouseID_local;
    
    this_00 = &this->WriteBuffer;
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e5bb2 to 080e5bc6 has its CatchHandler @ 080e5c14
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,',');
                    // try { // try from 080e5bd1 to 080e5bd5 has its CatchHandler @ 080e5c50
    TWriteBuffer::writeWord(this_00,HouseID);
    iVar1 = executeQuery(this,0x1e,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::startAuction: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// WARNING: Variable defined which should be unmapped: i
// DWARF original prototype: int insertHouses(TQueryManagerConnection * this, int NumberOfHouses,
// ushort * HouseIDs, char * * Names, int * Rents, char * * Descriptions, int * Sizes, int *
// PositionsX, int * PositionsY, int * PositionsZ, char[30] * Towns, bool * Guildhouses)

int __thiscall
TQueryManagerConnection::insertHouses
          (TQueryManagerConnection *this,int NumberOfHouses,ushort *HouseIDs,char **Names,int *Rents
          ,char **Descriptions,int *Sizes,int *PositionsX,int *PositionsY,int *PositionsZ,
          char (*Towns) [30],bool *Guildhouses)

{
    TWriteBuffer *this_00;
    int iVar1;
    int local_14;
    int i;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e5ca5 to 080e5cb9 has its CatchHandler @ 080e5e10
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'-');
                    // try { // try from 080e5cc5 to 080e5cc9 has its CatchHandler @ 080e5e50
    TWriteBuffer::writeWord(this_00,(ushort)NumberOfHouses);
    local_14 = 0;
    if (0 < NumberOfHouses) {
        do {
                    // try { // try from 080e5d23 to 080e5d27 has its CatchHandler @ 080e5e82
            TWriteBuffer::writeWord(this_00,HouseIDs[local_14]);
                    // try { // try from 080e5d38 to 080e5d3c has its CatchHandler @ 080e5eaf
            TWriteStream::writeString(&this_00->super_TWriteStream,Names[local_14]);
                    // try { // try from 080e5d4d to 080e5d51 has its CatchHandler @ 080e5edf
            TWriteBuffer::writeQuad(this_00,Rents[local_14]);
                    // try { // try from 080e5d62 to 080e5d66 has its CatchHandler @ 080e5f13
            TWriteStream::writeString(&this_00->super_TWriteStream,Descriptions[local_14]);
                    // try { // try from 080e5d78 to 080e5d7c has its CatchHandler @ 080e5f47
            TWriteBuffer::writeWord(this_00,*(ushort *)(Sizes + local_14));
                    // try { // try from 080e5d8e to 080e5d92 has its CatchHandler @ 080e5f7b
            TWriteBuffer::writeWord(this_00,*(ushort *)(PositionsX + local_14));
                    // try { // try from 080e5da4 to 080e5da8 has its CatchHandler @ 080e5faf
            TWriteBuffer::writeWord(this_00,*(ushort *)(PositionsY + local_14));
                    // try { // try from 080e5dba to 080e5dbe has its CatchHandler @ 080e5fe3
            TWriteBuffer::writeByte(this_00,(uchar)PositionsZ[local_14]);
                    // try { // try from 080e5dd7 to 080e5ddb has its CatchHandler @ 080e6017
            TWriteStream::writeString(&this_00->super_TWriteStream,Towns[local_14]);
                    // try { // try from 080e5df3 to 080e5df7 has its CatchHandler @ 080e604b
            TWriteBuffer::writeByte(this_00,Guildhouses[local_14] != false);
            local_14 = local_14 + 1;
        } while (local_14 < NumberOfHouses);
    }
    iVar1 = executeQuery(this,0x3c,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::insertHouses: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int clearIsOnline(TQueryManagerConnection * this, int *
// NumberOfAffectedPlayers)

int __thiscall
TQueryManagerConnection::clearIsOnline(TQueryManagerConnection *this,int *NumberOfAffectedPlayers)

{
    ushort uVar1;
    int iVar2;
    
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e60a8 to 080e60bc has its CatchHandler @ 080e6110
    TWriteBuffer::writeWord(&this->WriteBuffer,0);
    TWriteBuffer::writeByte(&this->WriteBuffer,'.');
    iVar2 = executeQuery(this,0x78,true);
    if (iVar2 == 0) {
                    // try { // try from 080e60fa to 080e60fe has its CatchHandler @ 080e6147
        uVar1 = TReadBuffer::readWord(&this->ReadBuffer);
        *NumberOfAffectedPlayers = (uint)uVar1;
        iVar2 = 0;
    }
    else {
        error("TQueryManagerConnection::clearIsOnline: Anfrage fehlgeschlagen.\n");
        iVar2 = -1;
    }
    return iVar2;
}



// DWARF original prototype: int createPlayerlist(TQueryManagerConnection * this, int
// NumberOfPlayers, char * * Names, int * Levels, char[30] * Professions, bool * NewRecord)

int __thiscall
TQueryManagerConnection::createPlayerlist
          (TQueryManagerConnection *this,int NumberOfPlayers,char **Names,int *Levels,
          char (*Professions) [30],bool *NewRecord)

{
    uchar uVar1;
    ushort w;
    TWriteBuffer *this_00;
    int i;
    int iVar2;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e6197 to 080e61ab has its CatchHandler @ 080e6270
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'/');
    w = 0xffff;
    if (-1 < NumberOfPlayers) {
        w = (ushort)NumberOfPlayers;
    }
                    // try { // try from 080e61c4 to 080e61c8 has its CatchHandler @ 080e62b0
    TWriteBuffer::writeWord(this_00,w);
    iVar2 = 0;
    if (0 < NumberOfPlayers) {
        do {
                    // try { // try from 080e6232 to 080e6236 has its CatchHandler @ 080e62e2
            TWriteStream::writeString(&this_00->super_TWriteStream,Names[iVar2]);
                    // try { // try from 080e6245 to 080e6249 has its CatchHandler @ 080e6312
            TWriteBuffer::writeWord(this_00,*(ushort *)(Levels + iVar2));
                    // try { // try from 080e625e to 080e6262 has its CatchHandler @ 080e6345
            TWriteStream::writeString(&this_00->super_TWriteStream,Professions[iVar2]);
            iVar2 = iVar2 + 1;
        } while (iVar2 < NumberOfPlayers);
    }
    iVar2 = executeQuery(this,0xf0,true);
    if (iVar2 == 0) {
                    // try { // try from 080e6211 to 080e6215 has its CatchHandler @ 080e637c
        uVar1 = TReadBuffer::readByte(&this->ReadBuffer);
        *NewRecord = uVar1 != '\0';
        iVar2 = 0;
    }
    else {
        error("TQueryManagerConnection::createPlayerlist: Anfrage fehlgeschlagen.\n");
        iVar2 = -1;
    }
    return iVar2;
}



// WARNING: Variable defined which should be unmapped: i
// DWARF original prototype: int logKilledCreatures(TQueryManagerConnection * this, int
// NumberOfRaces, char * * Names, int * KilledPlayers, int * KilledCreatures)

int __thiscall
TQueryManagerConnection::logKilledCreatures
          (TQueryManagerConnection *this,int NumberOfRaces,char **Names,int *KilledPlayers,
          int *KilledCreatures)

{
    TWriteBuffer *this_00;
    int iVar1;
    int local_14;
    int i;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e63d5 to 080e63e9 has its CatchHandler @ 080e6491
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'0');
                    // try { // try from 080e63f5 to 080e63f9 has its CatchHandler @ 080e64d0
    TWriteBuffer::writeWord(this_00,(ushort)NumberOfRaces);
    local_14 = 0;
    if (0 < NumberOfRaces) {
        do {
                    // try { // try from 080e6452 to 080e6456 has its CatchHandler @ 080e6502
            TWriteStream::writeString(&this_00->super_TWriteStream,Names[local_14]);
                    // try { // try from 080e6467 to 080e646b has its CatchHandler @ 080e6530
            TWriteBuffer::writeQuad(this_00,KilledPlayers[local_14]);
                    // try { // try from 080e647c to 080e6480 has its CatchHandler @ 080e6560
            TWriteBuffer::writeQuad(this_00,KilledCreatures[local_14]);
            local_14 = local_14 + 1;
        } while (local_14 < NumberOfRaces);
    }
    iVar1 = executeQuery(this,0xf0,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::logKilledCreatures: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int loadPlayers(TQueryManagerConnection * this, ulong
// MinimumCharacterID, int * NumberOfPlayers, char[30] * Names, ulong * CharacterIDs)

int __thiscall
TQueryManagerConnection::loadPlayers
          (TQueryManagerConnection *this,ulong MinimumCharacterID,int *NumberOfPlayers,
          char (*Names) [30],ulong *CharacterIDs)

{
    TWriteBuffer *this_00;
    int iVar1;
    ulong uVar2;
    int i;
    TReadBuffer *this_01;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e65b5 to 080e65c9 has its CatchHandler @ 080e6670
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,'2');
                    // try { // try from 080e65d4 to 080e65d8 has its CatchHandler @ 080e66b0
    TWriteBuffer::writeQuad(this_00,MinimumCharacterID);
    iVar1 = executeQuery(this,900,true);
    if (iVar1 == 0) {
        this_01 = &this->ReadBuffer;
                    // try { // try from 080e6616 to 080e661a has its CatchHandler @ 080e66e2
        uVar2 = TReadBuffer::readQuad(this_01);
        iVar1 = 0;
        *NumberOfPlayers = uVar2;
        if (0 < (int)uVar2) {
            do {
                    // try { // try from 080e664f to 080e6653 has its CatchHandler @ 080e670d
                TReadStream::readString(&this_01->super_TReadStream,Names[iVar1],0x1e);
                    // try { // try from 080e6657 to 080e665b has its CatchHandler @ 080e673f
                uVar2 = TReadBuffer::readQuad(this_01);
                CharacterIDs[iVar1] = uVar2;
                iVar1 = iVar1 + 1;
            } while (iVar1 < *NumberOfPlayers);
        }
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::loadPlayers: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int getKeptCharacters(TQueryManagerConnection * this, ulong
// MinimumCharacterID, int * NumberOfPlayers, ulong * CharacterIDs)

int __thiscall
TQueryManagerConnection::getKeptCharacters
          (TQueryManagerConnection *this,ulong MinimumCharacterID,int *NumberOfPlayers,
          ulong *CharacterIDs)

{
    TWriteBuffer *this_00;
    int iVar1;
    ulong uVar2;
    int i;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e6795 to 080e67a9 has its CatchHandler @ 080e6830
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,200);
                    // try { // try from 080e67b4 to 080e67b8 has its CatchHandler @ 080e6870
    TWriteBuffer::writeQuad(this_00,MinimumCharacterID);
    iVar1 = executeQuery(this,0x708,true);
    if (iVar1 == 0) {
                    // try { // try from 080e67f6 to 080e67fa has its CatchHandler @ 080e68a2
        uVar2 = TReadBuffer::readQuad(&this->ReadBuffer);
        iVar1 = 0;
        *NumberOfPlayers = uVar2;
        if (0 < (int)uVar2) {
            do {
                    // try { // try from 080e6813 to 080e6817 has its CatchHandler @ 080e68d0
                uVar2 = TReadBuffer::readQuad(&this->ReadBuffer);
                CharacterIDs[iVar1] = uVar2;
                iVar1 = iVar1 + 1;
            } while (iVar1 < *NumberOfPlayers);
        }
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::getKeptCharacters: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int getDeletedCharacters(TQueryManagerConnection * this, ulong
// MinimumCharacterID, int * NumberOfPlayers, ulong * CharacterIDs)

int __thiscall
TQueryManagerConnection::getDeletedCharacters
          (TQueryManagerConnection *this,ulong MinimumCharacterID,int *NumberOfPlayers,
          ulong *CharacterIDs)

{
    TWriteBuffer *this_00;
    int iVar1;
    ulong uVar2;
    int i;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e6925 to 080e6939 has its CatchHandler @ 080e69c0
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,0xc9);
                    // try { // try from 080e6944 to 080e6948 has its CatchHandler @ 080e6a00
    TWriteBuffer::writeQuad(this_00,MinimumCharacterID);
    iVar1 = executeQuery(this,900,true);
    if (iVar1 == 0) {
                    // try { // try from 080e6986 to 080e698a has its CatchHandler @ 080e6a32
        uVar2 = TReadBuffer::readQuad(&this->ReadBuffer);
        iVar1 = 0;
        *NumberOfPlayers = uVar2;
        if (0 < (int)uVar2) {
            do {
                    // try { // try from 080e69a3 to 080e69a7 has its CatchHandler @ 080e6a60
                uVar2 = TReadBuffer::readQuad(&this->ReadBuffer);
                CharacterIDs[iVar1] = uVar2;
                iVar1 = iVar1 + 1;
            } while (iVar1 < *NumberOfPlayers);
        }
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::getKeptCharacters: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int deleteOldCharacter(TQueryManagerConnection * this, ulong
// CharacterID)

int __thiscall
TQueryManagerConnection::deleteOldCharacter(TQueryManagerConnection *this,ulong CharacterID)

{
    TWriteBuffer *this_00;
    int iVar1;
    
    this_00 = &this->WriteBuffer;
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e6abb to 080e6acf has its CatchHandler @ 080e6b23
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,0xca);
                    // try { // try from 080e6ada to 080e6ade has its CatchHandler @ 080e6b60
    TWriteBuffer::writeQuad(this_00,CharacterID);
    iVar1 = executeQuery(this,0x1e,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::deleteOldCharacter: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int getHiddenCharacters(TQueryManagerConnection * this, ulong
// MinimumCharacterID, int * NumberOfPlayers, ulong * CharacterIDs)

int __thiscall
TQueryManagerConnection::getHiddenCharacters
          (TQueryManagerConnection *this,ulong MinimumCharacterID,int *NumberOfPlayers,
          ulong *CharacterIDs)

{
    TWriteBuffer *this_00;
    int iVar1;
    ulong uVar2;
    int i;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e6bb5 to 080e6bc9 has its CatchHandler @ 080e6c50
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,0xcb);
                    // try { // try from 080e6bd4 to 080e6bd8 has its CatchHandler @ 080e6c90
    TWriteBuffer::writeQuad(this_00,MinimumCharacterID);
    iVar1 = executeQuery(this,0x78,true);
    if (iVar1 == 0) {
                    // try { // try from 080e6c16 to 080e6c1a has its CatchHandler @ 080e6cc2
        uVar2 = TReadBuffer::readQuad(&this->ReadBuffer);
        iVar1 = 0;
        *NumberOfPlayers = uVar2;
        if (0 < (int)uVar2) {
            do {
                    // try { // try from 080e6c33 to 080e6c37 has its CatchHandler @ 080e6cf0
                uVar2 = TReadBuffer::readQuad(&this->ReadBuffer);
                CharacterIDs[iVar1] = uVar2;
                iVar1 = iVar1 + 1;
            } while (iVar1 < *NumberOfPlayers);
        }
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::getHiddenCharacters: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// WARNING: Variable defined which should be unmapped: i
// DWARF original prototype: int createHighscores(TQueryManagerConnection * this, int
// NumberOfPlayers, ulong * CharacterIDs, int * ExpPoints, int * ExpLevel, int * Fist, int * Club,
// int * Axe, int * Sword, int * Distance, int * Shielding, int * Magic, int * Fishing)

int __thiscall
TQueryManagerConnection::createHighscores
          (TQueryManagerConnection *this,int NumberOfPlayers,ulong *CharacterIDs,int *ExpPoints,
          int *ExpLevel,int *Fist,int *Club,int *Axe,int *Sword,int *Distance,int *Shielding,
          int *Magic,int *Fishing)

{
    TWriteBuffer *this_00;
    int iVar1;
    int local_14;
    int i;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e6d45 to 080e6d59 has its CatchHandler @ 080e6eb5
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,0xcc);
                    // try { // try from 080e6d64 to 080e6d68 has its CatchHandler @ 080e6ef0
    TWriteBuffer::writeQuad(this_00,NumberOfPlayers);
    local_14 = 0;
    if (0 < NumberOfPlayers) {
        do {
                    // try { // try from 080e6dc1 to 080e6dc5 has its CatchHandler @ 080e6f22
            TWriteBuffer::writeQuad(this_00,CharacterIDs[local_14]);
                    // try { // try from 080e6dd6 to 080e6dda has its CatchHandler @ 080e6f4f
            TWriteBuffer::writeQuad(this_00,ExpPoints[local_14]);
                    // try { // try from 080e6dec to 080e6df0 has its CatchHandler @ 080e6f7f
            TWriteBuffer::writeWord(this_00,*(ushort *)(ExpLevel + local_14));
                    // try { // try from 080e6e02 to 080e6e06 has its CatchHandler @ 080e6fb3
            TWriteBuffer::writeWord(this_00,*(ushort *)(Fist + local_14));
                    // try { // try from 080e6e18 to 080e6e1c has its CatchHandler @ 080e6fe7
            TWriteBuffer::writeWord(this_00,*(ushort *)(Club + local_14));
                    // try { // try from 080e6e2e to 080e6e32 has its CatchHandler @ 080e701b
            TWriteBuffer::writeWord(this_00,*(ushort *)(Axe + local_14));
                    // try { // try from 080e6e44 to 080e6e48 has its CatchHandler @ 080e704f
            TWriteBuffer::writeWord(this_00,*(ushort *)(Sword + local_14));
                    // try { // try from 080e6e5a to 080e6e5e has its CatchHandler @ 080e7083
            TWriteBuffer::writeWord(this_00,*(ushort *)(Distance + local_14));
                    // try { // try from 080e6e70 to 080e6e74 has its CatchHandler @ 080e70b7
            TWriteBuffer::writeWord(this_00,*(ushort *)(Shielding + local_14));
                    // try { // try from 080e6e86 to 080e6e8a has its CatchHandler @ 080e70eb
            TWriteBuffer::writeWord(this_00,*(ushort *)(Magic + local_14));
                    // try { // try from 080e6e9c to 080e6ea0 has its CatchHandler @ 080e711f
            TWriteBuffer::writeWord(this_00,*(ushort *)(Fishing + local_14));
            local_14 = local_14 + 1;
        } while (local_14 < NumberOfPlayers);
    }
    iVar1 = executeQuery(this,0x78,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::createHighscores: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int createCensus(TQueryManagerConnection * this)

int __thiscall TQueryManagerConnection::createCensus(TQueryManagerConnection *this)

{
    int iVar1;
    
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e7178 to 080e718c has its CatchHandler @ 080e71c8
    TWriteBuffer::writeWord(&this->WriteBuffer,0);
    TWriteBuffer::writeByte(&this->WriteBuffer,0xcd);
    iVar1 = executeQuery(this,600,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::createCensus: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int createKillStatistics(TQueryManagerConnection * this)

int __thiscall TQueryManagerConnection::createKillStatistics(TQueryManagerConnection *this)

{
    int iVar1;
    
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e7228 to 080e723c has its CatchHandler @ 080e7278
    TWriteBuffer::writeWord(&this->WriteBuffer,0);
    TWriteBuffer::writeByte(&this->WriteBuffer,0xce);
    iVar1 = executeQuery(this,300,true);
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        error("TQueryManagerConnection::createKillStatistics: Anfrage fehlgeschlagen.\n");
        iVar1 = -1;
    }
    return iVar1;
}



// DWARF original prototype: int getPlayersOnline(TQueryManagerConnection * this, int *
// NumberOfWorlds, char[30] * Names, ushort * Players)

int __thiscall
TQueryManagerConnection::getPlayersOnline
          (TQueryManagerConnection *this,int *NumberOfWorlds,char (*Names) [30],ushort *Players)

{
    TReadBuffer *this_00;
    byte bVar1;
    ushort uVar2;
    int iVar3;
    
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e72d5 to 080e72e9 has its CatchHandler @ 080e7380
    TWriteBuffer::writeWord(&this->WriteBuffer,0);
    TWriteBuffer::writeByte(&this->WriteBuffer,0xcf);
    iVar3 = executeQuery(this,0x78,true);
    if (iVar3 == 0) {
        this_00 = &this->ReadBuffer;
                    // try { // try from 080e7326 to 080e732a has its CatchHandler @ 080e73c0
        bVar1 = TReadBuffer::readByte(this_00);
        iVar3 = 0;
        *NumberOfWorlds = (uint)bVar1;
        if (bVar1 != 0) {
            do {
                    // try { // try from 080e735f to 080e7363 has its CatchHandler @ 080e73eb
                TReadStream::readString(&this_00->super_TReadStream,Names[iVar3],0x1e);
                    // try { // try from 080e7367 to 080e736b has its CatchHandler @ 080e741a
                uVar2 = TReadBuffer::readWord(this_00);
                Players[iVar3] = uVar2;
                iVar3 = iVar3 + 1;
            } while (iVar3 < *NumberOfWorlds);
        }
        iVar3 = 0;
    }
    else {
        error("TQueryManagerConnection::getPlayersOnline: Anfrage fehlgeschlagen.\n");
        iVar3 = -1;
    }
    return iVar3;
}



// DWARF original prototype: int getWorlds(TQueryManagerConnection * this, int * NumberOfWorlds,
// char[30] * Names)

int __thiscall
TQueryManagerConnection::getWorlds
          (TQueryManagerConnection *this,int *NumberOfWorlds,char (*Names) [30])

{
    byte bVar1;
    int iVar2;
    
    this->QueryOk = true;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e7475 to 080e7489 has its CatchHandler @ 080e7511
    TWriteBuffer::writeWord(&this->WriteBuffer,0);
    TWriteBuffer::writeByte(&this->WriteBuffer,0xd0);
    iVar2 = executeQuery(this,0x78,true);
    if (iVar2 == 0) {
                    // try { // try from 080e74c9 to 080e74cd has its CatchHandler @ 080e7550
        bVar1 = TReadBuffer::readByte(&this->ReadBuffer);
        iVar2 = 0;
        *NumberOfWorlds = (uint)bVar1;
        if (bVar1 != 0) {
            do {
                    // try { // try from 080e7502 to 080e7506 has its CatchHandler @ 080e7580
                TReadStream::readString(&(this->ReadBuffer).super_TReadStream,Names[iVar2],0x1e);
                iVar2 = iVar2 + 1;
            } while (iVar2 < *NumberOfWorlds);
        }
        iVar2 = 0;
    }
    else {
        error("TQueryManagerConnection::getWorlds: Anfrage fehlgeschlagen.\n");
        iVar2 = -1;
    }
    return iVar2;
}



// DWARF original prototype: int getServerLoad(TQueryManagerConnection * this, char * World, int
// Period, int * Data)

int __thiscall
TQueryManagerConnection::getServerLoad
          (TQueryManagerConnection *this,char *World,int Period,int *Data)

{
    TWriteBuffer *this_00;
    ushort uVar1;
    int iVar2;
    uint uVar3;
    ushort Value;
    int i;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e75d5 to 080e75e9 has its CatchHandler @ 080e7673
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,0xd1);
                    // try { // try from 080e75f4 to 080e75f8 has its CatchHandler @ 080e76b0
    TWriteStream::writeString(&this_00->super_TWriteStream,World);
                    // try { // try from 080e7607 to 080e760b has its CatchHandler @ 080e76e2
    TWriteBuffer::writeByte(this_00,(uchar)Period);
    iVar2 = executeQuery(this,0x168,true);
    if (iVar2 == 0) {
        iVar2 = 0;
        do {
                    // try { // try from 080e7630 to 080e7634 has its CatchHandler @ 080e7712
            uVar1 = TReadBuffer::readWord(&this->ReadBuffer);
            uVar3 = 0xffffffff;
            if (uVar1 != 0xffff) {
                uVar3 = (uint)uVar1;
            }
            Data[iVar2] = uVar3;
            iVar2 = iVar2 + 1;
        } while (iVar2 < 600);
        iVar2 = 0;
    }
    else {
        error("TQueryManagerConnection::getServerLoad: Anfrage fehlgeschlagen.\n");
        iVar2 = -1;
    }
    return iVar2;
}



// DWARF original prototype: int insertPaymentDataOld(TQueryManagerConnection * this, ulong
// PurchaseNr, ulong ReferenceNr, char * FirstName, char * LastName, char * Company, char * Street,
// char * Zip, char * City, char * Country, char * State, char * Phone, char * Fax, char * EMail,
// char * PaymentMethod, ulong ProductID, char * Registrant, ulong AccountID, ulong * PaymentID)

int __thiscall TQueryManagerConnection::insertPaymentDataOld(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    byte bVar1;
    int iVar2;
    ulong uVar3;
    ulong in_stack_00000008;
    ulong in_stack_0000000c;
    char *in_stack_00000010;
    char *in_stack_00000014;
    char *in_stack_00000018;
    char *in_stack_0000001c;
    char *in_stack_00000020;
    char *in_stack_00000024;
    char *in_stack_00000028;
    char *in_stack_0000002c;
    char *in_stack_00000030;
    char *in_stack_00000034;
    char *in_stack_00000038;
    char *in_stack_0000003c;
    ulong in_stack_00000040;
    char *in_stack_00000044;
    ulong in_stack_00000048;
    ulong *in_stack_0000004c;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e7765 to 080e7779 has its CatchHandler @ 080e78f6
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,0xd2);
                    // try { // try from 080e7784 to 080e7788 has its CatchHandler @ 080e7930
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e7793 to 080e7797 has its CatchHandler @ 080e7962
    TWriteBuffer::writeQuad(this_00,in_stack_0000000c);
                    // try { // try from 080e77a2 to 080e77a6 has its CatchHandler @ 080e7992
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000010);
                    // try { // try from 080e77b1 to 080e77b5 has its CatchHandler @ 080e79c2
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000014);
                    // try { // try from 080e77c0 to 080e77c4 has its CatchHandler @ 080e79f6
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000018);
                    // try { // try from 080e77cf to 080e77d3 has its CatchHandler @ 080e7a2a
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_0000001c);
                    // try { // try from 080e77de to 080e77e2 has its CatchHandler @ 080e7a5e
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000020);
                    // try { // try from 080e77ed to 080e77f1 has its CatchHandler @ 080e7a92
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000024);
                    // try { // try from 080e77fc to 080e7800 has its CatchHandler @ 080e7ac6
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000028);
                    // try { // try from 080e780b to 080e780f has its CatchHandler @ 080e7afa
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_0000002c);
                    // try { // try from 080e781a to 080e781e has its CatchHandler @ 080e7b2e
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000030);
                    // try { // try from 080e7829 to 080e782d has its CatchHandler @ 080e7b62
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000034);
                    // try { // try from 080e7838 to 080e783c has its CatchHandler @ 080e7b96
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000038);
                    // try { // try from 080e7847 to 080e784b has its CatchHandler @ 080e7bca
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_0000003c);
                    // try { // try from 080e7856 to 080e785a has its CatchHandler @ 080e7bfe
    TWriteBuffer::writeQuad(this_00,in_stack_00000040);
                    // try { // try from 080e7865 to 080e7869 has its CatchHandler @ 080e7c32
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000044);
                    // try { // try from 080e7874 to 080e7878 has its CatchHandler @ 080e7c66
    TWriteBuffer::writeQuad(this_00,in_stack_00000048);
    iVar2 = executeQuery(this,0x168,true);
    if (iVar2 == 0) {
                    // try { // try from 080e78e6 to 080e78ea has its CatchHandler @ 080e7c9a
        uVar3 = TReadBuffer::readQuad(&this->ReadBuffer);
        *in_stack_0000004c = uVar3;
        iVar2 = 0;
    }
    else {
        if (iVar2 == 1) {
                    // try { // try from 080e78b7 to 080e78bb has its CatchHandler @ 080e7ccc
            bVar1 = TReadBuffer::readByte(&this->ReadBuffer);
            if (bVar1 == 1) {
                return 1;
            }
            error(&DAT_0810f520,(uint)bVar1);
        }
        else {
            error("TQueryManagerConnection::insertPaymentDataOld: Anfrage fehlgeschlagen.\n");
        }
        iVar2 = -1;
    }
    return iVar2;
}



// DWARF original prototype: int addPaymentOld(TQueryManagerConnection * this, ulong AccountID, char
// * Description, ulong PaymentID, int Days, int * ActionTaken)

int __thiscall TQueryManagerConnection::addPaymentOld(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    bool bVar1;
    byte bVar2;
    int iVar3;
    int FailureCode;
    uint uVar4;
    ulong in_stack_00000008;
    char *in_stack_0000000c;
    ulong in_stack_00000010;
    ushort in_stack_00000014;
    uint *in_stack_00000018;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e7d25 to 080e7d39 has its CatchHandler @ 080e7e06
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,0xd3);
                    // try { // try from 080e7d44 to 080e7d48 has its CatchHandler @ 080e7e40
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e7d53 to 080e7d57 has its CatchHandler @ 080e7e72
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_0000000c);
                    // try { // try from 080e7d62 to 080e7d66 has its CatchHandler @ 080e7ea2
    TWriteBuffer::writeQuad(this_00,in_stack_00000010);
                    // try { // try from 080e7d72 to 080e7d76 has its CatchHandler @ 080e7ed2
    TWriteBuffer::writeWord(this_00,in_stack_00000014);
    iVar3 = executeQuery(this,0x168,true);
    if (iVar3 == 0) {
                    // try { // try from 080e7df2 to 080e7df6 has its CatchHandler @ 080e7f06
        bVar2 = TReadBuffer::readByte(&this->ReadBuffer);
        *in_stack_00000018 = (uint)bVar2;
        iVar3 = 0;
    }
    else {
        if (iVar3 == 1) {
                    // try { // try from 080e7db5 to 080e7db9 has its CatchHandler @ 080e7f38
            bVar2 = TReadBuffer::readByte(&this->ReadBuffer);
            uVar4 = (uint)bVar2;
            bVar1 = false;
            if ((uVar4 != 0) && (uVar4 < 3)) {
                bVar1 = true;
            }
            if (bVar1) {
                return uVar4;
            }
            error(&DAT_0810f5e0,uVar4);
        }
        else {
            error("TQueryManagerConnection::addPaymentOld: Anfrage fehlgeschlagen.\n");
        }
        iVar3 = -1;
    }
    return iVar3;
}



// DWARF original prototype: int cancelPaymentOld(TQueryManagerConnection * this, ulong PurchaseNr,
// ulong ReferenceNr, ulong AccountID, bool * IllegalUse, char * EMailAddress)

int __thiscall TQueryManagerConnection::cancelPaymentOld(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    bool bVar1;
    byte bVar2;
    uchar uVar3;
    int iVar4;
    int FailureCode;
    uint uVar5;
    ulong in_stack_00000008;
    ulong in_stack_0000000c;
    ulong in_stack_00000010;
    undefined4 in_stack_00000014;
    char *in_stack_00000018;
    char *Text;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e7f95 to 080e7fa9 has its CatchHandler @ 080e8072
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,0xd4);
                    // try { // try from 080e7fb4 to 080e7fb8 has its CatchHandler @ 080e80b0
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e7fc3 to 080e7fc7 has its CatchHandler @ 080e80e2
    TWriteBuffer::writeQuad(this_00,in_stack_0000000c);
                    // try { // try from 080e7fd2 to 080e7fd6 has its CatchHandler @ 080e8112
    TWriteBuffer::writeQuad(this_00,in_stack_00000010);
    iVar4 = executeQuery(this,0x168,true);
    if (iVar4 == 0) {
                    // try { // try from 080e8047 to 080e804b has its CatchHandler @ 080e8142
        uVar3 = TReadBuffer::readByte(&this->ReadBuffer);
        *(bool *)in_stack_00000014 = uVar3 != '\0';
                    // try { // try from 080e8069 to 080e806d has its CatchHandler @ 080e8174
        TReadStream::readString(&(this->ReadBuffer).super_TReadStream,in_stack_00000018,0x32);
        iVar4 = 0;
    }
    else {
        if (iVar4 == 1) {
                    // try { // try from 080e8015 to 080e8019 has its CatchHandler @ 080e81aa
            bVar2 = TReadBuffer::readByte(&this->ReadBuffer);
            uVar5 = (uint)bVar2;
            bVar1 = false;
            if ((uVar5 != 0) && (uVar5 < 3)) {
                bVar1 = true;
            }
            if (bVar1) {
                return uVar5;
            }
            Text = &DAT_0810f6a0;
        }
        else {
            Text = "TQueryManagerConnection::cancelPaymentOld: Anfrage fehlgeschlagen.\n";
        }
        error(Text);
        iVar4 = -1;
    }
    return iVar4;
}



// DWARF original prototype: int insertPaymentDataNew(TQueryManagerConnection * this, ulong
// PurchaseNr, ulong ReferenceNr, char * FirstName, char * LastName, char * Company, char * Street,
// char * Zip, char * City, char * Country, char * State, char * Phone, char * Fax, char * EMail,
// char * PaymentMethod, ulong ProductID, char * Registrant, char * PaymentKey, ulong * PaymentID)

int __thiscall TQueryManagerConnection::insertPaymentDataNew(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    byte bVar1;
    int iVar2;
    ulong uVar3;
    ulong in_stack_00000008;
    ulong in_stack_0000000c;
    char *in_stack_00000010;
    char *in_stack_00000014;
    char *in_stack_00000018;
    char *in_stack_0000001c;
    char *in_stack_00000020;
    char *in_stack_00000024;
    char *in_stack_00000028;
    char *in_stack_0000002c;
    char *in_stack_00000030;
    char *in_stack_00000034;
    char *in_stack_00000038;
    char *in_stack_0000003c;
    ulong in_stack_00000040;
    char *in_stack_00000044;
    char *in_stack_00000048;
    ulong *in_stack_0000004c;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e8205 to 080e8219 has its CatchHandler @ 080e8396
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,0xd5);
                    // try { // try from 080e8224 to 080e8228 has its CatchHandler @ 080e83d0
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e8233 to 080e8237 has its CatchHandler @ 080e8402
    TWriteBuffer::writeQuad(this_00,in_stack_0000000c);
                    // try { // try from 080e8242 to 080e8246 has its CatchHandler @ 080e8432
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000010);
                    // try { // try from 080e8251 to 080e8255 has its CatchHandler @ 080e8462
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000014);
                    // try { // try from 080e8260 to 080e8264 has its CatchHandler @ 080e8496
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000018);
                    // try { // try from 080e826f to 080e8273 has its CatchHandler @ 080e84ca
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_0000001c);
                    // try { // try from 080e827e to 080e8282 has its CatchHandler @ 080e84fe
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000020);
                    // try { // try from 080e828d to 080e8291 has its CatchHandler @ 080e8532
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000024);
                    // try { // try from 080e829c to 080e82a0 has its CatchHandler @ 080e8566
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000028);
                    // try { // try from 080e82ab to 080e82af has its CatchHandler @ 080e859a
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_0000002c);
                    // try { // try from 080e82ba to 080e82be has its CatchHandler @ 080e85ce
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000030);
                    // try { // try from 080e82c9 to 080e82cd has its CatchHandler @ 080e8602
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000034);
                    // try { // try from 080e82d8 to 080e82dc has its CatchHandler @ 080e8636
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000038);
                    // try { // try from 080e82e7 to 080e82eb has its CatchHandler @ 080e866a
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_0000003c);
                    // try { // try from 080e82f6 to 080e82fa has its CatchHandler @ 080e869e
    TWriteBuffer::writeQuad(this_00,in_stack_00000040);
                    // try { // try from 080e8305 to 080e8309 has its CatchHandler @ 080e86d2
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000044);
                    // try { // try from 080e8314 to 080e8318 has its CatchHandler @ 080e8706
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000048);
    iVar2 = executeQuery(this,0x168,true);
    if (iVar2 == 0) {
                    // try { // try from 080e8386 to 080e838a has its CatchHandler @ 080e873a
        uVar3 = TReadBuffer::readQuad(&this->ReadBuffer);
        *in_stack_0000004c = uVar3;
        iVar2 = 0;
    }
    else {
        if (iVar2 == 1) {
                    // try { // try from 080e8357 to 080e835b has its CatchHandler @ 080e876c
            bVar1 = TReadBuffer::readByte(&this->ReadBuffer);
            if (bVar1 == 1) {
                return 1;
            }
            error(&DAT_0810f760,(uint)bVar1);
        }
        else {
            error("TQueryManagerConnection::insertPaymentDataNew: Anfrage fehlgeschlagen.\n");
        }
        iVar2 = -1;
    }
    return iVar2;
}



// DWARF original prototype: int addPaymentNew(TQueryManagerConnection * this, char * PaymentKey,
// ulong PaymentID, int * ActionTaken, char * EMailReceiver)

int __thiscall TQueryManagerConnection::addPaymentNew(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    bool bVar1;
    byte bVar2;
    int iVar3;
    int FailureCode;
    uint uVar4;
    char *in_stack_00000008;
    ulong in_stack_0000000c;
    uint *in_stack_00000010;
    char *in_stack_00000014;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e87c5 to 080e87d9 has its CatchHandler @ 080e88a8
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,0xd6);
                    // try { // try from 080e87e4 to 080e87e8 has its CatchHandler @ 080e88e2
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000008);
                    // try { // try from 080e87f3 to 080e87f7 has its CatchHandler @ 080e8912
    TWriteBuffer::writeQuad(this_00,in_stack_0000000c);
    iVar3 = executeQuery(this,0x168,true);
    if (iVar3 == 0) {
                    // try { // try from 080e8876 to 080e887a has its CatchHandler @ 080e8942
        bVar2 = TReadBuffer::readByte(&this->ReadBuffer);
        *in_stack_00000010 = (uint)bVar2;
        if (bVar2 == 5) {
                    // try { // try from 080e88a1 to 080e88a5 has its CatchHandler @ 080e8970
            TReadStream::readString(&(this->ReadBuffer).super_TReadStream,in_stack_00000014,100);
        }
        iVar3 = 0;
    }
    else {
        if (iVar3 == 1) {
                    // try { // try from 080e8836 to 080e883a has its CatchHandler @ 080e89a6
            bVar2 = TReadBuffer::readByte(&this->ReadBuffer);
            uVar4 = (uint)bVar2;
            bVar1 = false;
            if ((uVar4 != 0) && (uVar4 < 4)) {
                bVar1 = true;
            }
            if (bVar1) {
                return uVar4;
            }
            error(&DAT_0810f820,uVar4);
        }
        else {
            error("TQueryManagerConnection::addPaymentNew: Anfrage fehlgeschlagen.\n");
        }
        iVar3 = -1;
    }
    return iVar3;
}



// DWARF original prototype: int cancelPaymentNew(TQueryManagerConnection * this, ulong PurchaseNr,
// ulong ReferenceNr, char * PaymentKey, bool * IllegalUse, bool * Present, char * EMailAddress)

int __thiscall TQueryManagerConnection::cancelPaymentNew(TQueryManagerConnection *this)

{
    TWriteBuffer *this_00;
    TReadBuffer *this_01;
    uchar uVar1;
    int iVar2;
    ulong in_stack_00000008;
    ulong in_stack_0000000c;
    char *in_stack_00000010;
    undefined4 in_stack_00000014;
    undefined4 in_stack_00000018;
    char *in_stack_0000001c;
    char *Text;
    
    this->QueryOk = true;
    this_00 = &this->WriteBuffer;
    (this->WriteBuffer).Position = 0;
                    // try { // try from 080e8a05 to 080e8a19 has its CatchHandler @ 080e8ae4
    TWriteBuffer::writeWord(this_00,0);
    TWriteBuffer::writeByte(this_00,0xd7);
                    // try { // try from 080e8a24 to 080e8a28 has its CatchHandler @ 080e8b20
    TWriteBuffer::writeQuad(this_00,in_stack_00000008);
                    // try { // try from 080e8a33 to 080e8a37 has its CatchHandler @ 080e8b52
    TWriteBuffer::writeQuad(this_00,in_stack_0000000c);
                    // try { // try from 080e8a42 to 080e8a46 has its CatchHandler @ 080e8b82
    TWriteStream::writeString(&this_00->super_TWriteStream,in_stack_00000010);
    iVar2 = executeQuery(this,0x168,true);
    if (iVar2 == 0) {
        this_01 = &this->ReadBuffer;
                    // try { // try from 080e8aa6 to 080e8aaa has its CatchHandler @ 080e8bb2
        uVar1 = TReadBuffer::readByte(this_01);
        *(bool *)in_stack_00000014 = uVar1 != '\0';
                    // try { // try from 080e8ab9 to 080e8abd has its CatchHandler @ 080e8be4
        uVar1 = TReadBuffer::readByte(this_01);
        *(bool *)in_stack_00000018 = uVar1 != '\0';
                    // try { // try from 080e8adb to 080e8adf has its CatchHandler @ 080e8c16
        TReadStream::readString(&this_01->super_TReadStream,in_stack_0000001c,0x32);
        iVar2 = 0;
    }
    else {
        if (iVar2 == 1) {
                    // try { // try from 080e8a85 to 080e8a89 has its CatchHandler @ 080e8c4c
            uVar1 = TReadBuffer::readByte(&this->ReadBuffer);
            if (uVar1 == '\x01') {
                return 1;
            }
            Text = &DAT_0810f8e0;
        }
        else {
            Text = "TQueryManagerConnection::cancelPaymentNew: Anfrage fehlgeschlagen.\n";
        }
        error(Text);
        iVar2 = -1;
    }
    return iVar2;
}



// DWARF original prototype: void TQueryManagerConnectionPool(TQueryManagerConnectionPool * this,
// int Connections)

void __thiscall
TQueryManagerConnectionPool::TQueryManagerConnectionPool
          (TQueryManagerConnectionPool *this,int Connections)

{
    Semaphore::Semaphore(&this->FreeQueryManagerConnections,Connections);
                    // try { // try from 080e8cb5 to 080e8cb9 has its CatchHandler @ 080e8d00
    Semaphore::Semaphore(&this->QueryManagerConnectionMutex,1);
    if (Connections < 1) {
                    // try { // try from 080e8ce6 to 080e8cea has its CatchHandler @ 080e8cf2
        error(&DAT_0810f940,Connections);
        Connections = 1;
    }
    this->NumberOfConnections = Connections;
    this->QueryManagerConnection = (TQueryManagerConnection *)0x0;
    this->QueryManagerConnectionFree = (bool *)0x0;
    return;
}



// DWARF original prototype: void TQueryManagerConnectionPool(TQueryManagerConnectionPool * this,
// int Connections)

void __thiscall
TQueryManagerConnectionPool::TQueryManagerConnectionPool
          (TQueryManagerConnectionPool *this,int Connections)

{
    Semaphore::Semaphore(&this->FreeQueryManagerConnections,Connections);
                    // try { // try from 080e8d55 to 080e8d59 has its CatchHandler @ 080e8da0
    Semaphore::Semaphore(&this->QueryManagerConnectionMutex,1);
    if (Connections < 1) {
                    // try { // try from 080e8d86 to 080e8d8a has its CatchHandler @ 080e8d92
        error(&DAT_0810f940,Connections);
        Connections = 1;
    }
    this->NumberOfConnections = Connections;
    this->QueryManagerConnection = (TQueryManagerConnection *)0x0;
    this->QueryManagerConnectionFree = (bool *)0x0;
    return;
}



// DWARF original prototype: void init(TQueryManagerConnectionPool * this)

int __thiscall
TQueryManagerConnectionPool::init(TQueryManagerConnectionPool *this,EVP_PKEY_CTX *ctx)

{
    int *piVar1;
    bool *pbVar2;
    undefined4 *puVar3;
    int iVar4;
    int i;
    int iVar5;
    TQueryManagerConnection *this_00;
    
    iVar5 = this->NumberOfConnections;
                    // try { // try from 080e8dda to 080e8dde has its CatchHandler @ 080e8f00
    piVar1 = (int *)operator_new__(iVar5 * 0x30 + 4);
    *piVar1 = iVar5;
    this_00 = (TQueryManagerConnection *)(piVar1 + 1);
    while (iVar5 = iVar5 + -1, iVar5 != -1) {
                    // try { // try from 080e8dfc to 080e8e00 has its CatchHandler @ 080e8ea4
        TQueryManagerConnection::TQueryManagerConnection(this_00,0x4000);
        this_00 = this_00 + 1;
    }
    this->QueryManagerConnection = (TQueryManagerConnection *)(piVar1 + 1);
                    // try { // try from 080e8e1e to 080e8ea3 has its CatchHandler @ 080e8f00
    pbVar2 = (bool *)operator_new__(this->NumberOfConnections);
    iVar5 = 0;
    this->QueryManagerConnectionFree = pbVar2;
    if (0 < this->NumberOfConnections) {
        iVar4 = 0;
        do {
            if (*(int *)((int)&this->QueryManagerConnection->Socket + iVar4) < 0) {
                error("TQueryManagerConnectionPool::init: Kann nicht zum Query-Manager verbinden.\n"
                     );
                puVar3 = (undefined4 *)__cxa_allocate_exception(4);
                *puVar3 = "cannot connect to query manager";
                    // WARNING: Subroutine does not return
                __cxa_throw(puVar3,char_const*::typeinfo,0);
            }
            iVar4 = iVar4 + 0x30;
            pbVar2 = this->QueryManagerConnectionFree;
            pbVar2[iVar5] = true;
            iVar5 = iVar5 + 1;
        } while (iVar5 < this->NumberOfConnections);
    }
    return (int)pbVar2;
}



// DWARF original prototype: void exit(TQueryManagerConnectionPool * this)

void __thiscall TQueryManagerConnectionPool::exit(TQueryManagerConnectionPool *this,int __status)

{
    TQueryManagerConnection *pTVar1;
    int i;
    int iVar2;
    TQueryManagerConnection *this_00;
    int in_stack_ffffffe8;
    
    iVar2 = 0;
    if (0 < this->NumberOfConnections) {
        do {
            iVar2 = iVar2 + 1;
            Semaphore::down(&this->FreeQueryManagerConnections);
        } while (iVar2 < this->NumberOfConnections);
    }
    pTVar1 = this->QueryManagerConnection;
    if (pTVar1 != (TQueryManagerConnection *)0x0) {
        this_00 = pTVar1 + *(int *)&pTVar1[-1].QueryOk;
        while (pTVar1 != this_00) {
            this_00 = this_00 + -1;
            TQueryManagerConnection::~TQueryManagerConnection(this_00,in_stack_ffffffe8);
            pTVar1 = this->QueryManagerConnection;
        }
        operator_delete__(&pTVar1[-1].QueryOk);
    }
    if (this->QueryManagerConnectionFree == (bool *)0x0) {
        return;
    }
    operator_delete__(this->QueryManagerConnectionFree);
    return;
}



// DWARF original prototype: TQueryManagerConnection * getConnection(TQueryManagerConnectionPool *
// this)

TQueryManagerConnection * __thiscall
TQueryManagerConnectionPool::getConnection(TQueryManagerConnectionPool *this)

{
    Semaphore *this_00;
    int i;
    int iVar1;
    int iVar2;
    
    this_00 = &this->QueryManagerConnectionMutex;
    Semaphore::down(&this->FreeQueryManagerConnections);
    Semaphore::down(this_00);
    iVar1 = 0;
    if (0 < this->NumberOfConnections) {
        iVar2 = 0;
        do {
            if (this->QueryManagerConnectionFree[iVar1] != false) {
                this->QueryManagerConnectionFree[iVar1] = false;
                Semaphore::up(this_00);
                return (TQueryManagerConnection *)
                       ((int)&this->QueryManagerConnection->BufferSize + iVar2);
            }
            iVar1 = iVar1 + 1;
            iVar2 = iVar2 + 0x30;
        } while (iVar1 < this->NumberOfConnections);
    }
    error("TQueryManagerConnectionPool::getConnection: Keine freie Verbindung gefunden.\n");
    Semaphore::up(this_00);
    return (TQueryManagerConnection *)0x0;
}



// DWARF original prototype: void releaseConnection(TQueryManagerConnectionPool * this,
// TQueryManagerConnection * Connection)

void __thiscall
TQueryManagerConnectionPool::releaseConnection
          (TQueryManagerConnectionPool *this,TQueryManagerConnection *Connection)

{
    TQueryManagerConnection *pTVar1;
    int i;
    int iVar2;
    
    iVar2 = 0;
    if (0 < this->NumberOfConnections) {
        pTVar1 = this->QueryManagerConnection;
        do {
            if (pTVar1 == Connection) {
                this->QueryManagerConnectionFree[iVar2] = true;
                Semaphore::up(&this->FreeQueryManagerConnections);
                return;
            }
            iVar2 = iVar2 + 1;
            pTVar1 = pTVar1 + 1;
        } while (iVar2 < this->NumberOfConnections);
    }
    error("TQueryManagerConnectionPool::releaseConnection: Verbindung nicht gefunden.\n");
    return;
}



// DWARF original prototype: void TQueryManagerPoolConnection(TQueryManagerPoolConnection * this,
// TQueryManagerConnectionPool * Pool)

void __thiscall
TQueryManagerPoolConnection::TQueryManagerPoolConnection
          (TQueryManagerPoolConnection *this,TQueryManagerConnectionPool *Pool)

{
    TQueryManagerConnection *pTVar1;
    int i;
    int iVar2;
    
    if (Pool == (TQueryManagerConnectionPool *)0x0) {
        error("TQueryManagerPoolConnection::TQueryManagerPoolConnection: Pool ist NULL.\n");
        this->QueryManagerConnection = (TQueryManagerConnection *)0x0;
    }
    else {
        this->QueryManagerConnectionPool = Pool;
        iVar2 = 0;
        Semaphore::down(&Pool->FreeQueryManagerConnections);
        Semaphore::down(&Pool->QueryManagerConnectionMutex);
        if (0 < Pool->NumberOfConnections) {
            do {
                if (Pool->QueryManagerConnectionFree[iVar2] != false) {
                    Pool->QueryManagerConnectionFree[iVar2] = false;
                    Semaphore::up(&Pool->QueryManagerConnectionMutex);
                    pTVar1 = Pool->QueryManagerConnection + iVar2;
                    goto LAB_080e90b4;
                }
                iVar2 = iVar2 + 1;
            } while (iVar2 < Pool->NumberOfConnections);
        }
        error("TQueryManagerConnectionPool::getConnection: Keine freie Verbindung gefunden.\n");
        Semaphore::up(&Pool->QueryManagerConnectionMutex);
        pTVar1 = (TQueryManagerConnection *)0x0;
LAB_080e90b4:
        this->QueryManagerConnection = pTVar1;
    }
    return;
}



// DWARF original prototype: void TQueryManagerPoolConnection(TQueryManagerPoolConnection * this,
// TQueryManagerConnectionPool * Pool)

void __thiscall
TQueryManagerPoolConnection::TQueryManagerPoolConnection
          (TQueryManagerPoolConnection *this,TQueryManagerConnectionPool *Pool)

{
    TQueryManagerConnection *pTVar1;
    int i;
    int iVar2;
    
    if (Pool == (TQueryManagerConnectionPool *)0x0) {
        error("TQueryManagerPoolConnection::TQueryManagerPoolConnection: Pool ist NULL.\n");
        this->QueryManagerConnection = (TQueryManagerConnection *)0x0;
    }
    else {
        this->QueryManagerConnectionPool = Pool;
        iVar2 = 0;
        Semaphore::down(&Pool->FreeQueryManagerConnections);
        Semaphore::down(&Pool->QueryManagerConnectionMutex);
        if (0 < Pool->NumberOfConnections) {
            do {
                if (Pool->QueryManagerConnectionFree[iVar2] != false) {
                    Pool->QueryManagerConnectionFree[iVar2] = false;
                    Semaphore::up(&Pool->QueryManagerConnectionMutex);
                    pTVar1 = Pool->QueryManagerConnection + iVar2;
                    goto LAB_080e9154;
                }
                iVar2 = iVar2 + 1;
            } while (iVar2 < Pool->NumberOfConnections);
        }
        error("TQueryManagerConnectionPool::getConnection: Keine freie Verbindung gefunden.\n");
        Semaphore::up(&Pool->QueryManagerConnectionMutex);
        pTVar1 = (TQueryManagerConnection *)0x0;
LAB_080e9154:
        this->QueryManagerConnection = pTVar1;
    }
    return;
}



// DWARF original prototype: void ~TQueryManagerPoolConnection(TQueryManagerPoolConnection * this,
// int __in_chrg)

void __thiscall
TQueryManagerPoolConnection::~TQueryManagerPoolConnection
          (TQueryManagerPoolConnection *this,int __in_chrg)

{
    TQueryManagerConnectionPool *pTVar1;
    TQueryManagerConnection *pTVar2;
    int i;
    int iVar3;
    
    if (this->QueryManagerConnection == (TQueryManagerConnection *)0x0) {
        return;
    }
    pTVar1 = this->QueryManagerConnectionPool;
    iVar3 = 0;
    if (0 < pTVar1->NumberOfConnections) {
        pTVar2 = pTVar1->QueryManagerConnection;
        do {
            if (pTVar2 == this->QueryManagerConnection) {
                pTVar1->QueryManagerConnectionFree[iVar3] = true;
                Semaphore::up(&pTVar1->FreeQueryManagerConnections);
                return;
            }
            iVar3 = iVar3 + 1;
            pTVar2 = pTVar2 + 1;
        } while (iVar3 < pTVar1->NumberOfConnections);
    }
    error("TQueryManagerConnectionPool::releaseConnection: Verbindung nicht gefunden.\n");
    return;
}



// DWARF original prototype: void ~TQueryManagerPoolConnection(TQueryManagerPoolConnection * this,
// int __in_chrg)

void __thiscall
TQueryManagerPoolConnection::~TQueryManagerPoolConnection
          (TQueryManagerPoolConnection *this,int __in_chrg)

{
    TQueryManagerConnectionPool *pTVar1;
    TQueryManagerConnection *pTVar2;
    int i;
    int iVar3;
    
    if (this->QueryManagerConnection == (TQueryManagerConnection *)0x0) {
        return;
    }
    pTVar1 = this->QueryManagerConnectionPool;
    iVar3 = 0;
    if (0 < pTVar1->NumberOfConnections) {
        pTVar2 = pTVar1->QueryManagerConnection;
        do {
            if (pTVar2 == this->QueryManagerConnection) {
                pTVar1->QueryManagerConnectionFree[iVar3] = true;
                Semaphore::up(&pTVar1->FreeQueryManagerConnections);
                return;
            }
            iVar3 = iVar3 + 1;
            pTVar2 = pTVar2 + 1;
        } while (iVar3 < pTVar1->NumberOfConnections);
    }
    error("TQueryManagerConnectionPool::releaseConnection: Verbindung nicht gefunden.\n");
    return;
}

// CRYPTO.CC
//==============================================================================

// WARNING: Unknown calling convention -- yet parameter storage is locked

void _GLOBAL__I_ApplicationType(void)

{
    __static_initialization_and_destruction_0(1,0xffff);
    return;
}



void __static_initialization_and_destruction_0(int __initialize_p,int __priority)

{
    if ((__priority == 0xffff) && (__initialize_p == 1)) {
        vlong::vlong(&RSA_EXPONENT,0x10001);
        __cxa_atexit(__tcf_0,0,&__dso_handle);
    }
    return;
}



// DWARF original prototype: void TRSAPrivateKey(TRSAPrivateKey * this)

void __thiscall TRSAPrivateKey::TRSAPrivateKey(TRSAPrivateKey *this)

{
    this->_vptr_TRSAPrivateKey = (_func_int_varargs **)&PTR__TRSAPrivateKey_081283e0;
    vlong::vlong(&this->m_PrimeP,0);
                    // try { // try from 080e9359 to 080e935d has its CatchHandler @ 080e93d8
    vlong::vlong(&this->m_PrimeQ,0);
                    // try { // try from 080e936f to 080e9373 has its CatchHandler @ 080e93d4
    vlong::vlong(&this->m_U,0);
                    // try { // try from 080e9385 to 080e9389 has its CatchHandler @ 080e93d0
    vlong::vlong(&this->m_DP,0);
                    // try { // try from 080e9398 to 080e939c has its CatchHandler @ 080e93aa
    vlong::vlong(&this->m_DQ,0);
    return;
}



// DWARF original prototype: void TRSAPrivateKey(TRSAPrivateKey * this)

void __thiscall TRSAPrivateKey::TRSAPrivateKey(TRSAPrivateKey *this)

{
    this->_vptr_TRSAPrivateKey = (_func_int_varargs **)&PTR__TRSAPrivateKey_081283e0;
    vlong::vlong(&this->m_PrimeP,0);
                    // try { // try from 080e9429 to 080e942d has its CatchHandler @ 080e94a8
    vlong::vlong(&this->m_PrimeQ,0);
                    // try { // try from 080e943f to 080e9443 has its CatchHandler @ 080e94a4
    vlong::vlong(&this->m_U,0);
                    // try { // try from 080e9455 to 080e9459 has its CatchHandler @ 080e94a0
    vlong::vlong(&this->m_DP,0);
                    // try { // try from 080e9468 to 080e946c has its CatchHandler @ 080e947a
    vlong::vlong(&this->m_DQ,0);
    return;
}



// DWARF original prototype: void ~TRSAPrivateKey(TRSAPrivateKey * this, int __in_chrg)

void __thiscall TRSAPrivateKey::~TRSAPrivateKey(TRSAPrivateKey *this,int __in_chrg)

{
    int unaff_EBX;
    
    this->_vptr_TRSAPrivateKey = (_func_int_varargs **)&PTR__TRSAPrivateKey_081283e0;
    vlong::~vlong(&this->m_DQ,unaff_EBX);
    vlong::~vlong(&this->m_DP,unaff_EBX);
    vlong::~vlong(&this->m_U,unaff_EBX);
    vlong::~vlong(&this->m_PrimeQ,unaff_EBX);
    vlong::~vlong(&this->m_PrimeP,unaff_EBX);
    return;
}



// DWARF original prototype: void ~TRSAPrivateKey(TRSAPrivateKey * this, int __in_chrg)

void __thiscall TRSAPrivateKey::~TRSAPrivateKey(TRSAPrivateKey *this,int __in_chrg)

{
    int unaff_EBX;
    
    this->_vptr_TRSAPrivateKey = (_func_int_varargs **)&PTR__TRSAPrivateKey_081283e0;
    vlong::~vlong(&this->m_DQ,unaff_EBX);
    vlong::~vlong(&this->m_DP,unaff_EBX);
    vlong::~vlong(&this->m_U,unaff_EBX);
    vlong::~vlong(&this->m_PrimeQ,unaff_EBX);
    vlong::~vlong(&this->m_PrimeP,unaff_EBX);
    return;
}



// DWARF original prototype: void ~TRSAPrivateKey(TRSAPrivateKey * this, int __in_chrg)

void __thiscall TRSAPrivateKey::~TRSAPrivateKey(TRSAPrivateKey *this,int __in_chrg)

{
    int unaff_EBX;
    
    this->_vptr_TRSAPrivateKey = (_func_int_varargs **)&PTR__TRSAPrivateKey_081283e0;
    vlong::~vlong(&this->m_DQ,unaff_EBX);
    vlong::~vlong(&this->m_DP,unaff_EBX);
    vlong::~vlong(&this->m_U,unaff_EBX);
    vlong::~vlong(&this->m_PrimeQ,unaff_EBX);
    vlong::~vlong(&this->m_PrimeP,unaff_EBX);
    operator_delete(this);
    return;
}



// DWARF original prototype: void init(TRSAPrivateKey * this, char * PrimeP, char * PrimeQ)

int __thiscall TRSAPrivateKey::init(TRSAPrivateKey *this,EVP_PKEY_CTX *ctx)

{
    vlong *pvVar1;
    undefined4 *puVar2;
    size_t __len;
    char *in_stack_0000000c;
    vlong *pvVar3;
    vlong *Number;
    vlong local_6c;
    vlong local_5c;
    undefined1 local_4c [4];
    vlong d;
    vlong local_3c;
    string Error;
    
    if (ctx == (EVP_PKEY_CTX *)0x0) {
        puVar2 = (undefined4 *)__cxa_allocate_exception(4);
        *puVar2 = "PrimeP pointer is NULL";
    }
    else {
        if (in_stack_0000000c != (char *)0x0) {
            pvVar1 = &this->m_PrimeP;
                    // try { // try from 080e95ec to 080e95f0 has its CatchHandler @ 080e9850
            vlong::convert((char *)ctx,pvVar1);
            Number = &this->m_PrimeQ;
                    // try { // try from 080e95fe to 080e9602 has its CatchHandler @ 080e9980
            vlong::convert(in_stack_0000000c,Number);
            vlong::vlong(&local_5c,1);
            pvVar3 = Number;
                    // try { // try from 080e9627 to 080e962b has its CatchHandler @ 080e9aa1
            operator-((vlong *)&stack0xffffffd4,Number);
            vlong::~vlong(&local_5c,(int)pvVar3);
                    // try { // try from 080e9648 to 080e964c has its CatchHandler @ 080e9adb
            vlong::vlong(&local_6c,1);
            pvVar3 = pvVar1;
                    // try { // try from 080e965e to 080e9662 has its CatchHandler @ 080e9ab3
            operator-(&local_5c,pvVar1);
                    // try { // try from 080e966c to 080e9670 has its CatchHandler @ 080e9adb
            vlong::~vlong(&local_6c,(int)pvVar3);
                    // try { // try from 080e9685 to 080e9689 has its CatchHandler @ 080e9ad7
            operator*(&local_3c,&local_5c);
            pvVar3 = &RSA_EXPONENT;
                    // try { // try from 080e96a2 to 080e96a6 has its CatchHandler @ 080e9ac5
            vlong::modinv((vlong *)local_4c,&RSA_EXPONENT,&local_3c);
                    // try { // try from 080e96b0 to 080e96b4 has its CatchHandler @ 080e9ad7
            vlong::~vlong(&local_3c,(int)pvVar3);
                    // try { // try from 080e96bb to 080e96bf has its CatchHandler @ 080e9adb
            vlong::~vlong(&local_5c,(int)pvVar3);
            vlong::~vlong((vlong *)&stack0xffffffd4,(int)pvVar3);
                    // try { // try from 080e96d9 to 080e96dd has its CatchHandler @ 080e9b10
            vlong::modinv(&local_6c,pvVar1,Number);
            pvVar3 = &local_6c;
                    // try { // try from 080e96f1 to 080e96f5 has its CatchHandler @ 080e9adf
            vlong::operator=(&this->m_U,pvVar3);
                    // try { // try from 080e96fc to 080e9713 has its CatchHandler @ 080e9b10
            vlong::~vlong(&local_6c,(int)pvVar3);
            vlong::vlong(&local_3c,1);
                    // try { // try from 080e9725 to 080e9729 has its CatchHandler @ 080e9af1
            operator-(&local_5c,pvVar1);
                    // try { // try from 080e9733 to 080e9737 has its CatchHandler @ 080e9b10
            vlong::~vlong(&local_3c,(int)pvVar1);
                    // try { // try from 080e974c to 080e9750 has its CatchHandler @ 080e9b0a
            operator%(&local_6c,(vlong *)local_4c);
            pvVar3 = &local_6c;
                    // try { // try from 080e9764 to 080e9768 has its CatchHandler @ 080e9af8
            vlong::operator=(&this->m_DP,pvVar3);
                    // try { // try from 080e976f to 080e9773 has its CatchHandler @ 080e9b0a
            vlong::~vlong(&local_6c,(int)pvVar3);
                    // try { // try from 080e977a to 080e9791 has its CatchHandler @ 080e9b10
            vlong::~vlong(&local_5c,(int)pvVar3);
            vlong::vlong(&local_3c,1);
                    // try { // try from 080e97a3 to 080e97a7 has its CatchHandler @ 080e9af1
            operator-(&local_5c,Number);
                    // try { // try from 080e97b1 to 080e97b5 has its CatchHandler @ 080e9b10
            vlong::~vlong(&local_3c,(int)Number);
                    // try { // try from 080e97ca to 080e97ce has its CatchHandler @ 080e9b0a
            operator%(&local_6c,(vlong *)local_4c);
            pvVar3 = &local_6c;
                    // try { // try from 080e97e2 to 080e97e6 has its CatchHandler @ 080e9af8
            pvVar1 = vlong::operator=(&this->m_DQ,pvVar3);
                    // try { // try from 080e97ed to 080e97f1 has its CatchHandler @ 080e9b0a
            vlong::~vlong(&local_6c,(int)pvVar3);
                    // try { // try from 080e97f8 to 080e97fc has its CatchHandler @ 080e9b10
            vlong::~vlong(&local_5c,(int)pvVar3);
            vlong::~vlong((vlong *)local_4c,(int)pvVar3);
            return (int)pvVar1;
        }
        puVar2 = (undefined4 *)__cxa_allocate_exception(4);
        *puVar2 = "PrimeQ pointer is NULL";
    }
                    // WARNING: Subroutine does not return
    __cxa_throw(puVar2,char_const*::typeinfo,0);
}



// DWARF original prototype: void decrypt(TRSAPrivateKey * this, uchar * Data)

int __thiscall
TRSAPrivateKey::decrypt
          (TRSAPrivateKey *this,EVP_PKEY_CTX *ctx,uchar *out,size_t *outlen,uchar *in,size_t inlen)

{
    size_t __size;
    int iVar1;
    vlong *pvVar2;
    undefined4 *puVar3;
    size_t __len;
    char *pE;
    char *pE_1;
    undefined1 *puVar4;
    string Error_1;
    vlong local_8c;
    vlong local_7c;
    vlong local_6c;
    undefined1 local_5c [4];
    vlong a;
    undefined1 local_4c [4];
    vlong b;
    undefined1 local_3c [4];
    vlong Plain;
    undefined1 local_2c [4];
    vlong Cipher;
    
    if (ctx == (EVP_PKEY_CTX *)0x0) {
        puVar3 = (undefined4 *)__cxa_allocate_exception(4);
        *puVar3 = "Data pointer is NULL";
    }
    else {
        vlong::vlong((vlong *)local_2c,0);
        pvVar2 = (vlong *)local_2c;
                    // try { // try from 080e9b57 to 080e9b5b has its CatchHandler @ 080e9e62
        iVar1 = vlong::cf(&this->m_PrimeP,(vlong *)local_2c);
        vlong::~vlong((vlong *)local_2c,(int)pvVar2);
        if (iVar1 == 0) {
            puVar3 = (undefined4 *)__cxa_allocate_exception(4);
            *puVar3 = "m_PrimeP is null";
        }
        else {
            vlong::vlong((vlong *)local_2c,0);
            pvVar2 = (vlong *)local_2c;
                    // try { // try from 080e9b9c to 080e9ba0 has its CatchHandler @ 080e9e74
            iVar1 = vlong::cf(&this->m_PrimeQ,(vlong *)local_2c);
            vlong::~vlong((vlong *)local_2c,(int)pvVar2);
            if (iVar1 != 0) {
                vlong::vlong((vlong *)local_2c,0);
                    // try { // try from 080e9bda to 080e9bde has its CatchHandler @ 080e9e76
                vlong::convert((uchar *)ctx,0x80,(vlong *)local_2c);
                    // try { // try from 080e9bed to 080e9bf1 has its CatchHandler @ 080e9e62
                operator%((vlong *)local_4c,(vlong *)local_2c);
                puVar4 = local_4c;
                    // try { // try from 080e9c10 to 080e9c14 has its CatchHandler @ 080e9fa0
                vlong::modexp((vlong *)local_5c);
                    // try { // try from 080e9c1e to 080e9c22 has its CatchHandler @ 080e9e62
                vlong::~vlong((vlong *)local_4c,(int)puVar4);
                    // try { // try from 080e9c37 to 080e9c3b has its CatchHandler @ 080ea17e
                operator%((vlong *)local_3c,(vlong *)local_2c);
                puVar4 = local_3c;
                    // try { // try from 080e9c60 to 080e9c64 has its CatchHandler @ 080e9fb2
                vlong::modexp((vlong *)local_4c);
                    // try { // try from 080e9c6e to 080e9c72 has its CatchHandler @ 080ea17e
                vlong::~vlong((vlong *)local_3c,(int)puVar4);
                    // try { // try from 080e9c80 to 080e9c84 has its CatchHandler @ 080ea177
                vlong::vlong((vlong *)local_3c,(vlong *)local_5c);
                pvVar2 = (vlong *)local_3c;
                    // try { // try from 080e9c92 to 080e9c96 has its CatchHandler @ 080e9fc4
                iVar1 = vlong::cf((vlong *)local_4c,pvVar2);
                    // try { // try from 080e9ca2 to 080e9cca has its CatchHandler @ 080ea177
                vlong::~vlong((vlong *)local_3c,(int)pvVar2);
                if (iVar1 < 0) {
                    // try { // try from 080e9e00 to 080e9e04 has its CatchHandler @ 080ea177
                    vlong::operator+=((vlong *)local_4c,&this->m_PrimeQ);
                }
                operator-((vlong *)&stack0xffffff64,(vlong *)local_4c);
                    // try { // try from 080e9ceb to 080e9cef has its CatchHandler @ 080ea012
                operator*(&local_8c,(vlong *)&stack0xffffff64);
                    // try { // try from 080e9d0d to 080e9d11 has its CatchHandler @ 080ea00e
                operator%(&local_7c,&local_8c);
                    // try { // try from 080e9d26 to 080e9d2a has its CatchHandler @ 080ea00a
                operator*(&local_6c,&this->m_PrimeP);
                pvVar2 = (vlong *)local_5c;
                    // try { // try from 080e9d42 to 080e9d46 has its CatchHandler @ 080e9fd9
                operator+((vlong *)local_3c,pvVar2);
                    // try { // try from 080e9d50 to 080e9d54 has its CatchHandler @ 080ea00a
                vlong::~vlong(&local_6c,(int)pvVar2);
                    // try { // try from 080e9d5b to 080e9d5f has its CatchHandler @ 080ea00e
                vlong::~vlong(&local_7c,(int)pvVar2);
                    // try { // try from 080e9d69 to 080e9d6d has its CatchHandler @ 080ea012
                vlong::~vlong(&local_8c,(int)pvVar2);
                    // try { // try from 080e9d77 to 080e9d7b has its CatchHandler @ 080ea177
                vlong::~vlong((vlong *)&stack0xffffff64,(int)pvVar2);
                    // try { // try from 080e9d8c to 080e9d90 has its CatchHandler @ 080ea11f
                vlong::vlong((vlong *)&stack0xffffff64,(vlong *)local_3c);
                    // try { // try from 080e9da9 to 080e9dad has its CatchHandler @ 080ea016
                vlong::convert((vlong *)&stack0xffffff64,(uchar *)ctx,0x80);
                    // try { // try from 080e9db7 to 080e9dbb has its CatchHandler @ 080ea11f
                vlong::~vlong((vlong *)&stack0xffffff64,(int)ctx);
                pvVar2 = (vlong *)local_3c;
                    // try { // try from 080e9dc2 to 080e9dc6 has its CatchHandler @ 080ea177
                vlong::~vlong(pvVar2,(int)ctx);
                    // try { // try from 080e9dcd to 080e9dd1 has its CatchHandler @ 080ea17e
                vlong::~vlong((vlong *)local_4c,(int)ctx);
                    // try { // try from 080e9dd8 to 080e9ddc has its CatchHandler @ 080e9e62
                vlong::~vlong((vlong *)local_5c,(int)ctx);
                vlong::~vlong((vlong *)local_2c,(int)ctx);
                return (int)pvVar2;
            }
            puVar3 = (undefined4 *)__cxa_allocate_exception(4);
            *puVar3 = "m_PrimeQ is null";
        }
    }
                    // WARNING: Subroutine does not return
    __cxa_throw(puVar3,char_const*::typeinfo,0);
}



void __tcf_0(void *param_1)

{
    int in_stack_00000008;
    
    vlong::~vlong(&RSA_EXPONENT,in_stack_00000008);
    return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void _GLOBAL__I__ZN14TRSAPrivateKeyC2Ev(void)

{
    __static_initialization_and_destruction_0(1,0xffff);
    return;
}



// DWARF original name: _M_replace_safe<const char*>
// DWARF original prototype: basic_string<char,std::char_traits<char>,std::allocator<char>_> *
// _M_replace_safe<const_char*>(basic_string<char,std::char_traits<char>,std::allocator<char>_> *
// this, 
// __normal_iterator<char*,std::basic_string<char,_std::char_traits<char>,_std::allocator<char>_>_>
// __i1, 
// __normal_iterator<char*,std::basic_string<char,_std::char_traits<char>,_std::allocator<char>_>_>
// __i2, char * __k1, char * __k2)

basic_string<> * __thiscall
basic_string<>::_M_replace_safe<>
          (basic_string<> *this,__normal_iterator<> __i1,__normal_iterator<> __i2,char *__k1,
          char *__k2)

{
    uint __n;
    uint uVar1;
    
    __n = (int)__k2 - (int)__k1;
    if (std::string::_Rep::_S_max_size <= __n) {
        std::__throw_length_error("basic_string::_M_replace");
    }
    uVar1 = (int)__i1._M_current - (int)(this->_M_dataplus)._M_p;
    std::string::_M_mutate((uint)this,uVar1,(int)__i2._M_current - (int)__i1._M_current);
    if (__n != 0) {
        memcpy((this->_M_dataplus)._M_p + uVar1,__k1,__n);
    }
    return this;
}



// DWARF original prototype: void TXTEASymmetricKey(TXTEASymmetricKey * this)

void __thiscall TXTEASymmetricKey::TXTEASymmetricKey(TXTEASymmetricKey *this)

{
    this->_vptr_TXTEASymmetricKey = (_func_int_varargs **)&PTR__TXTEASymmetricKey_081283f8;
    this->m_SymmetricKey[0] = '\0';
    this->m_SymmetricKey[1] = '\0';
    this->m_SymmetricKey[2] = '\0';
    this->m_SymmetricKey[3] = '\0';
    this->m_SymmetricKey[4] = '\0';
    this->m_SymmetricKey[5] = '\0';
    this->m_SymmetricKey[6] = '\0';
    this->m_SymmetricKey[7] = '\0';
    this->m_SymmetricKey[8] = '\0';
    this->m_SymmetricKey[9] = '\0';
    this->m_SymmetricKey[10] = '\0';
    this->m_SymmetricKey[0xb] = '\0';
    this->m_SymmetricKey[0xc] = '\0';
    this->m_SymmetricKey[0xd] = '\0';
    this->m_SymmetricKey[0xe] = '\0';
    this->m_SymmetricKey[0xf] = '\0';
    return;
}



// DWARF original prototype: void TXTEASymmetricKey(TXTEASymmetricKey * this)

void __thiscall TXTEASymmetricKey::TXTEASymmetricKey(TXTEASymmetricKey *this)

{
    this->_vptr_TXTEASymmetricKey = (_func_int_varargs **)&PTR__TXTEASymmetricKey_081283f8;
    this->m_SymmetricKey[0] = '\0';
    this->m_SymmetricKey[1] = '\0';
    this->m_SymmetricKey[2] = '\0';
    this->m_SymmetricKey[3] = '\0';
    this->m_SymmetricKey[4] = '\0';
    this->m_SymmetricKey[5] = '\0';
    this->m_SymmetricKey[6] = '\0';
    this->m_SymmetricKey[7] = '\0';
    this->m_SymmetricKey[8] = '\0';
    this->m_SymmetricKey[9] = '\0';
    this->m_SymmetricKey[10] = '\0';
    this->m_SymmetricKey[0xb] = '\0';
    this->m_SymmetricKey[0xc] = '\0';
    this->m_SymmetricKey[0xd] = '\0';
    this->m_SymmetricKey[0xe] = '\0';
    this->m_SymmetricKey[0xf] = '\0';
    return;
}



// DWARF original prototype: void ~TXTEASymmetricKey(TXTEASymmetricKey * this, int __in_chrg)

void __thiscall TXTEASymmetricKey::~TXTEASymmetricKey(TXTEASymmetricKey *this,int __in_chrg)

{
    this->_vptr_TXTEASymmetricKey = (_func_int_varargs **)&PTR__TXTEASymmetricKey_081283f8;
    return;
}



// DWARF original prototype: void ~TXTEASymmetricKey(TXTEASymmetricKey * this, int __in_chrg)

void __thiscall TXTEASymmetricKey::~TXTEASymmetricKey(TXTEASymmetricKey *this,int __in_chrg)

{
    this->_vptr_TXTEASymmetricKey = (_func_int_varargs **)&PTR__TXTEASymmetricKey_081283f8;
    return;
}



// DWARF original prototype: void ~TXTEASymmetricKey(TXTEASymmetricKey * this, int __in_chrg)

void __thiscall TXTEASymmetricKey::~TXTEASymmetricKey(TXTEASymmetricKey *this,int __in_chrg)

{
    this->_vptr_TXTEASymmetricKey = (_func_int_varargs **)&PTR__TXTEASymmetricKey_081283f8;
    operator_delete(this);
    return;
}



// DWARF original prototype: void init(TXTEASymmetricKey * this, uchar * SymmetricKey)

int __thiscall TXTEASymmetricKey::init(TXTEASymmetricKey *this,EVP_PKEY_CTX *ctx)

{
    int in_EAX;
    
    if (ctx != (EVP_PKEY_CTX *)0x0) {
        *(undefined4 *)this->m_SymmetricKey = *(undefined4 *)ctx;
        *(undefined4 *)(this->m_SymmetricKey + 4) = *(undefined4 *)(ctx + 4);
        *(undefined4 *)(this->m_SymmetricKey + 8) = *(undefined4 *)(ctx + 8);
        in_EAX = *(int *)(ctx + 0xc);
        *(int *)(this->m_SymmetricKey + 0xc) = in_EAX;
    }
    return in_EAX;
}



// DWARF original prototype: bool isInitialized(TXTEASymmetricKey * this)

bool __thiscall TXTEASymmetricKey::isInitialized(TXTEASymmetricKey *this)

{
    int i;
    int iVar1;
    
    iVar1 = 0;
    do {
        if (this->m_SymmetricKey[iVar1] != '\0') {
            return true;
        }
        iVar1 = iVar1 + 1;
    } while (iVar1 < 0x10);
    return false;
}



// DWARF original prototype: void encrypt(TXTEASymmetricKey * this, uchar * Data)

void __thiscall TXTEASymmetricKey::encrypt(TXTEASymmetricKey *this,char *__block,int __edflag)

{
    uint uVar1;
    ulong v0;
    uint uVar2;
    ulong v1;
    uint uVar3;
    ulong sum;
    uint uVar4;
    ulong i;
    uint uVar5;
    
    if (__block != (char *)0x0) {
        uVar5 = 0;
        uVar2 = *(uint *)__block;
        uVar3 = *(uint *)(__block + 4);
        uVar4 = 0;
        do {
            uVar5 = uVar5 + 1;
            uVar1 = *(int *)(this->m_SymmetricKey + (uVar4 & 3) * 4) + uVar4;
            uVar4 = uVar4 + 0x9e3779b9;
            uVar2 = uVar2 + ((uVar3 << 4 ^ uVar3 >> 5) + uVar3 ^ uVar1);
            uVar3 = uVar3 + ((uVar2 * 0x10 ^ uVar2 >> 5) + uVar2 ^
                            *(int *)(this->m_SymmetricKey + (uVar4 >> 0xb & 3) * 4) + uVar4);
        } while (uVar5 < 0x20);
        *(uint *)__block = uVar2;
        *(uint *)(__block + 4) = uVar3;
    }
    return;
}



// DWARF original prototype: void decrypt(TXTEASymmetricKey * this, uchar * Data)

int __thiscall
TXTEASymmetricKey::decrypt
          (TXTEASymmetricKey *this,EVP_PKEY_CTX *ctx,uchar *out,size_t *outlen,uchar *in,
          size_t inlen)

{
    uint uVar1;
    ulong v0;
    uint uVar2;
    ulong v1;
    uint uVar3;
    ulong sum;
    uint uVar4;
    ulong i;
    uint uVar5;
    
    if (ctx != (EVP_PKEY_CTX *)0x0) {
        uVar5 = 0;
        uVar2 = *(uint *)ctx;
        uVar3 = *(uint *)(ctx + 4);
        uVar4 = 0xc6ef3720;
        do {
            uVar5 = uVar5 + 1;
            uVar1 = *(int *)(this->m_SymmetricKey + (uVar4 >> 0xb & 3) * 4) + uVar4;
            uVar4 = uVar4 + 0x61c88647;
            uVar3 = uVar3 - ((uVar2 << 4 ^ uVar2 >> 5) + uVar2 ^ uVar1);
            uVar2 = uVar2 - ((uVar3 * 0x10 ^ uVar3 >> 5) + uVar3 ^
                            *(int *)(this->m_SymmetricKey + (uVar4 & 3) * 4) + uVar4);
        } while (uVar5 < 0x20);
        *(uint *)ctx = uVar2;
        *(uint *)(ctx + 4) = uVar3;
    }
    return (int)ctx;
}



void TXTEASymmetricKey::encrypt(char *__block,int __edflag)

{
    uint uVar1;
    ulong v0;
    uint uVar2;
    ulong v1;
    uint uVar3;
    ulong sum;
    uint uVar4;
    ulong i;
    uint uVar5;
    
    uVar2 = *(uint *)__block;
    uVar3 = *(uint *)(__block + 4);
    uVar4 = 0;
    uVar5 = 0;
    do {
        uVar5 = uVar5 + 1;
        uVar1 = *(int *)(__edflag + (uVar4 & 3) * 4) + uVar4;
        uVar4 = uVar4 + 0x9e3779b9;
        uVar2 = uVar2 + ((uVar3 << 4 ^ uVar3 >> 5) + uVar3 ^ uVar1);
        uVar3 = uVar3 + ((uVar2 * 0x10 ^ uVar2 >> 5) + uVar2 ^
                        *(int *)(__edflag + (uVar4 >> 0xb & 3) * 4) + uVar4);
    } while (uVar5 < 0x20);
    *(uint *)__block = uVar2;
    *(uint *)(__block + 4) = uVar3;
    return;
}



int TXTEASymmetricKey::decrypt(EVP_PKEY_CTX *ctx,uchar *out,size_t *outlen,uchar *in,size_t inlen)

{
    uint uVar1;
    ulong v0;
    uint uVar2;
    ulong v1;
    uint uVar3;
    ulong sum;
    uint uVar4;
    ulong i;
    uint uVar5;
    
    uVar5 = 0;
    uVar2 = *(uint *)ctx;
    uVar3 = *(uint *)(ctx + 4);
    uVar4 = 0xc6ef3720;
    do {
        uVar5 = uVar5 + 1;
        uVar1 = *(int *)(out + (uVar4 >> 0xb & 3) * 4) + uVar4;
        uVar4 = uVar4 + 0x61c88647;
        uVar3 = uVar3 - ((uVar2 << 4 ^ uVar2 >> 5) + uVar2 ^ uVar1);
        uVar2 = uVar2 - ((uVar3 * 0x10 ^ uVar3 >> 5) + uVar3 ^
                        *(int *)(out + (uVar4 & 3) * 4) + uVar4);
    } while (uVar5 < 0x20);
    *(uint *)ctx = uVar2;
    *(uint *)(ctx + 4) = uVar3;
    return (int)ctx;
}



// DWARF original prototype: void vlong_montgomery(vlong_montgomery * this, vlong * M)

void __thiscall vlong_montgomery::vlong_montgomery(vlong_montgomery *this,vlong *M)

{
    _func_int_varargs **pp_Var1;
    uint *puVar2;
    vlong_value *pvVar3;
    int iVar4;
    uint i;
    int iVar5;
    uint uVar6;
    vlong_montgomery *__in_chrg;
    vlong *a;
    vlong *pvVar7;
    vlong_montgomery *x;
    undefined1 local_3c [4];
    vlong result;
    vlong local_2c [2];
    
    (this->R)._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    pvVar3 = (vlong_value *)operator_new(0x10);
    (pvVar3->super_vlong_flex_unit).z = 0;
    (pvVar3->super_vlong_flex_unit).a = (uint *)0x0;
    pvVar3->share = 0;
    (this->R).value = pvVar3;
    (this->R).negative = 0;
    (pvVar3->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,0);
    (this->R1)._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080ea612 to 080ea656 has its CatchHandler @ 080ead98
    pvVar3 = (vlong_value *)operator_new(0x10);
    (pvVar3->super_vlong_flex_unit).z = 0;
    (pvVar3->super_vlong_flex_unit).a = (uint *)0x0;
    pvVar3->share = 0;
    (this->R1).value = pvVar3;
    (this->R1).negative = 0;
    (pvVar3->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,0);
    pvVar7 = &this->m;
    (this->m)._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080ea66b to 080ea6af has its CatchHandler @ 080ead91
    pvVar3 = (vlong_value *)operator_new(0x10);
    (pvVar3->super_vlong_flex_unit).z = 0;
    (pvVar3->super_vlong_flex_unit).a = (uint *)0x0;
    pvVar3->share = 0;
    (this->m).value = pvVar3;
    (this->m).negative = 0;
    (pvVar3->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,0);
    (this->n1)._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080ea6c4 to 080ea708 has its CatchHandler @ 080ead8a
    pvVar3 = (vlong_value *)operator_new(0x10);
    (pvVar3->super_vlong_flex_unit).z = 0;
    (pvVar3->super_vlong_flex_unit).a = (uint *)0x0;
    pvVar3->share = 0;
    (this->n1).value = pvVar3;
    (this->n1).negative = 0;
    (pvVar3->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,0);
    (this->T)._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080ea71d to 080ea761 has its CatchHandler @ 080ead83
    pvVar3 = (vlong_value *)operator_new(0x10);
    (pvVar3->super_vlong_flex_unit).z = 0;
    (pvVar3->super_vlong_flex_unit).a = (uint *)0x0;
    pvVar3->share = 0;
    (this->T).value = pvVar3;
    (this->T).negative = 0;
    (pvVar3->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,0);
    (this->k)._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080ea776 to 080ea7ba has its CatchHandler @ 080ead7c
    pvVar3 = (vlong_value *)operator_new(0x10);
    (pvVar3->super_vlong_flex_unit).z = 0;
    (pvVar3->super_vlong_flex_unit).a = (uint *)0x0;
    pvVar3->share = 0;
    (this->k).value = pvVar3;
    (this->k).negative = 0;
    (pvVar3->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,0);
    pvVar3 = (this->m).value;
    if (pvVar3->share == 0) {
        if (pvVar3 != (vlong_value *)0x0) {
            uVar6 = (pvVar3->super_vlong_flex_unit).z;
            while (uVar6 != 0) {
                uVar6 = uVar6 - 1;
                (pvVar3->super_vlong_flex_unit).a[uVar6] = 0;
            }
            puVar2 = (pvVar3->super_vlong_flex_unit).a;
            if (puVar2 != (uint *)0x0) {
                operator_delete__(puVar2);
            }
            operator_delete(pvVar3);
        }
    }
    else {
        pvVar3->share = pvVar3->share - 1;
    }
    pvVar3 = M->value;
    (this->m).value = pvVar3;
    puVar2 = &pvVar3->share;
    *puVar2 = *puVar2 + 1;
    (this->m).negative = M->negative;
    this->N = 0;
    local_2c[0]._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080ea7f7 to 080ea8f5 has its CatchHandler @ 080ead75
    local_2c[0].value = (vlong_value *)operator_new(0x10);
    ((local_2c[0].value)->super_vlong_flex_unit).z = 0;
    ((local_2c[0].value)->super_vlong_flex_unit).a = (uint *)0x0;
    (local_2c[0].value)->share = 0;
    local_2c[0].negative = 0;
    ((local_2c[0].value)->super_vlong_flex_unit).n = 0;
    __in_chrg = (vlong_montgomery *)0x0;
    vlong_flex_unit::set((vlong_flex_unit *)local_2c[0].value,0,1);
    pvVar3 = (this->R).value;
    if (pvVar3->share == 0) {
        if (pvVar3 != (vlong_value *)0x0) {
            uVar6 = (pvVar3->super_vlong_flex_unit).z;
            while (uVar6 != 0) {
                uVar6 = uVar6 - 1;
                (pvVar3->super_vlong_flex_unit).a[uVar6] = 0;
            }
            puVar2 = (pvVar3->super_vlong_flex_unit).a;
            if (puVar2 != (uint *)0x0) {
                operator_delete__(puVar2);
            }
            operator_delete(pvVar3);
        }
    }
    else {
        pvVar3->share = pvVar3->share - 1;
    }
    pvVar3 = local_2c[0].value;
    (this->R).value = local_2c[0].value;
    (local_2c[0].value)->share = (local_2c[0].value)->share + 1;
    local_2c[0]._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    (this->R).negative = local_2c[0].negative;
    if ((local_2c[0].value)->share == 0) {
        if (local_2c[0].value != (vlong_value *)0x0) {
                    // try { // try from 080eab2d to 080eab31 has its CatchHandler @ 080ead75
            vlong_flex_unit::~vlong_flex_unit
                      (&(local_2c[0].value)->super_vlong_flex_unit,(int)__in_chrg);
            operator_delete(pvVar3);
        }
    }
    else {
        (local_2c[0].value)->share = (local_2c[0].value)->share - 1;
    }
    while( true ) {
        local_2c[0]._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
        local_2c[0].negative = M->negative;
        iVar4 = 0;
        x = (vlong_montgomery *)M->value;
        (x->R1)._vptr_vlong = (_func_int_varargs **)((int)(x->R1)._vptr_vlong + 1);
        if (((this->R).negative != 0) && ((((this->R).value)->super_vlong_flex_unit).n != 0)) {
            iVar4 = 1;
        }
        iVar5 = 0;
        if ((local_2c[0].negative != 0) && ((x->R)._vptr_vlong != (_func_int_varargs **)0x0)) {
            iVar5 = 1;
        }
        local_2c[0].value = (vlong_value *)x;
        if (iVar4 == iVar5) {
                    // try { // try from 080eab18 to 080eab1c has its CatchHandler @ 080eabbc
            iVar4 = vlong_value::cf((this->R).value,(vlong_value *)x);
            __in_chrg = x;
        }
        else {
            iVar4 = (uint)(iVar4 == 0) * 2 + -1;
        }
        vlong::~vlong(local_2c,(int)__in_chrg);
        if (-1 < iVar4) break;
        __in_chrg = this;
        vlong::operator+=(&this->R,&this->R);
        this->N = this->N + 1;
    }
    local_3c = (undefined1  [4])&PTR__vlong_08128410;
    result.value = (vlong_value *)(this->R).negative;
    result._vptr_vlong = (_func_int_varargs **)(this->R).value;
    ((vlong_value *)result._vptr_vlong)->share = ((vlong_value *)result._vptr_vlong)->share + 1;
                    // try { // try from 080ea921 to 080ea925 has its CatchHandler @ 080ead15
    vlong::operator-=((vlong *)local_3c,pvVar7);
    a = (vlong *)local_3c;
                    // try { // try from 080ea93a to 080ea93e has its CatchHandler @ 080ead1f
    vlong::modinv(local_2c,a,pvVar7);
    pvVar3 = (this->R1).value;
    if (pvVar3->share == 0) {
        if (pvVar3 != (vlong_value *)0x0) {
            uVar6 = (pvVar3->super_vlong_flex_unit).z;
            while (uVar6 != 0) {
                uVar6 = uVar6 - 1;
                (pvVar3->super_vlong_flex_unit).a[uVar6] = 0;
            }
            puVar2 = (pvVar3->super_vlong_flex_unit).a;
            if (puVar2 != (uint *)0x0) {
                operator_delete__(puVar2);
            }
            operator_delete(pvVar3);
        }
    }
    else {
        pvVar3->share = pvVar3->share - 1;
    }
    pvVar3 = local_2c[0].value;
    (this->R1).value = local_2c[0].value;
    ((vlong *)&(local_2c[0].value)->share)->_vptr_vlong =
         (_func_int_varargs **)((int)((vlong *)&(local_2c[0].value)->share)->_vptr_vlong + 1);
    local_2c[0]._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    (this->R1).negative = local_2c[0].negative;
    pp_Var1 = ((vlong *)&(local_2c[0].value)->share)->_vptr_vlong;
    if (pp_Var1 == (_func_int_varargs **)0x0) {
        if ((vlong_montgomery *)local_2c[0].value != (vlong_montgomery *)0x0) {
                    // try { // try from 080eaabd to 080eaac1 has its CatchHandler @ 080ead1f
            vlong_flex_unit::~vlong_flex_unit(&(local_2c[0].value)->super_vlong_flex_unit,(int)a);
            operator_delete(pvVar3);
        }
    }
    else {
        ((vlong *)&(local_2c[0].value)->share)->_vptr_vlong =
             (_func_int_varargs **)((int)pp_Var1 - 1);
    }
    pp_Var1 = result._vptr_vlong;
    local_3c = (undefined1  [4])&PTR__vlong_08128410;
    if (result._vptr_vlong[3] == (_func_int_varargs *)0x0) {
        if (result._vptr_vlong != (_func_int_varargs **)0x0) {
                    // try { // try from 080eaaa0 to 080eaaa4 has its CatchHandler @ 080ead75
            vlong_flex_unit::~vlong_flex_unit((vlong_flex_unit *)result._vptr_vlong,(int)a);
            operator_delete(pp_Var1);
        }
    }
    else {
        result._vptr_vlong[3] = result._vptr_vlong[3] + -1;
    }
                    // try { // try from 080ea9ac to 080ea9b0 has its CatchHandler @ 080ead75
    vlong::modinv(local_2c,pvVar7,&this->R);
    local_3c = (undefined1  [4])&PTR__vlong_08128410;
    result.value = (vlong_value *)(this->R).negative;
    pvVar7 = local_2c;
    result._vptr_vlong = (_func_int_varargs **)(this->R).value;
    ((vlong_value *)result._vptr_vlong)->share = ((vlong_value *)result._vptr_vlong)->share + 1;
                    // try { // try from 080ea9d7 to 080ea9db has its CatchHandler @ 080ead58
    vlong::operator-=((vlong *)local_3c,pvVar7);
    pvVar3 = (this->n1).value;
    if (pvVar3->share == 0) {
        if (pvVar3 != (vlong_value *)0x0) {
            uVar6 = (pvVar3->super_vlong_flex_unit).z;
            while (uVar6 != 0) {
                uVar6 = uVar6 - 1;
                (pvVar3->super_vlong_flex_unit).a[uVar6] = 0;
            }
            puVar2 = (pvVar3->super_vlong_flex_unit).a;
            if (puVar2 != (uint *)0x0) {
                operator_delete__(puVar2);
            }
            operator_delete(pvVar3);
        }
    }
    else {
        pvVar3->share = pvVar3->share - 1;
    }
    pp_Var1 = result._vptr_vlong;
    (this->n1).value = (vlong_value *)result._vptr_vlong;
    result._vptr_vlong[3] = result._vptr_vlong[3] + 1;
    local_3c = (undefined1  [4])&PTR__vlong_08128410;
    (this->n1).negative = (int)result.value;
    if (result._vptr_vlong[3] == (_func_int_varargs *)0x0) {
        if (result._vptr_vlong != (_func_int_varargs **)0x0) {
                    // try { // try from 080eaa4e to 080eaa52 has its CatchHandler @ 080ead71
            vlong_flex_unit::~vlong_flex_unit((vlong_flex_unit *)result._vptr_vlong,(int)pvVar7);
            operator_delete(pp_Var1);
        }
    }
    else {
        result._vptr_vlong[3] = result._vptr_vlong[3] + -1;
    }
    pvVar3 = local_2c[0].value;
    local_2c[0]._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    pp_Var1 = ((vlong *)&(local_2c[0].value)->share)->_vptr_vlong;
    if (pp_Var1 == (_func_int_varargs **)0x0) {
        if ((vlong_montgomery *)local_2c[0].value != (vlong_montgomery *)0x0) {
                    // try { // try from 080eaa38 to 080eaa3c has its CatchHandler @ 080ead75
            vlong_flex_unit::~vlong_flex_unit
                      (&(local_2c[0].value)->super_vlong_flex_unit,(int)pvVar7);
            operator_delete(pvVar3);
        }
    }
    else {
        ((vlong *)&(local_2c[0].value)->share)->_vptr_vlong =
             (_func_int_varargs **)((int)pp_Var1 - 1);
    }
    return;
}



// DWARF original prototype: void vlong_montgomery(vlong_montgomery * this, vlong * M)

void __thiscall vlong_montgomery::vlong_montgomery(vlong_montgomery *this,vlong *M)

{
    _func_int_varargs **pp_Var1;
    uint *puVar2;
    vlong_value *pvVar3;
    int iVar4;
    uint i;
    int iVar5;
    uint uVar6;
    vlong_montgomery *__in_chrg;
    vlong *a;
    vlong *pvVar7;
    vlong_montgomery *x;
    undefined1 local_3c [4];
    vlong result;
    vlong local_2c [2];
    
    (this->R)._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    pvVar3 = (vlong_value *)operator_new(0x10);
    (pvVar3->super_vlong_flex_unit).z = 0;
    (pvVar3->super_vlong_flex_unit).a = (uint *)0x0;
    pvVar3->share = 0;
    (this->R).value = pvVar3;
    (this->R).negative = 0;
    (pvVar3->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,0);
    (this->R1)._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080eae12 to 080eae56 has its CatchHandler @ 080eb598
    pvVar3 = (vlong_value *)operator_new(0x10);
    (pvVar3->super_vlong_flex_unit).z = 0;
    (pvVar3->super_vlong_flex_unit).a = (uint *)0x0;
    pvVar3->share = 0;
    (this->R1).value = pvVar3;
    (this->R1).negative = 0;
    (pvVar3->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,0);
    pvVar7 = &this->m;
    (this->m)._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080eae6b to 080eaeaf has its CatchHandler @ 080eb591
    pvVar3 = (vlong_value *)operator_new(0x10);
    (pvVar3->super_vlong_flex_unit).z = 0;
    (pvVar3->super_vlong_flex_unit).a = (uint *)0x0;
    pvVar3->share = 0;
    (this->m).value = pvVar3;
    (this->m).negative = 0;
    (pvVar3->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,0);
    (this->n1)._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080eaec4 to 080eaf08 has its CatchHandler @ 080eb58a
    pvVar3 = (vlong_value *)operator_new(0x10);
    (pvVar3->super_vlong_flex_unit).z = 0;
    (pvVar3->super_vlong_flex_unit).a = (uint *)0x0;
    pvVar3->share = 0;
    (this->n1).value = pvVar3;
    (this->n1).negative = 0;
    (pvVar3->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,0);
    (this->T)._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080eaf1d to 080eaf61 has its CatchHandler @ 080eb583
    pvVar3 = (vlong_value *)operator_new(0x10);
    (pvVar3->super_vlong_flex_unit).z = 0;
    (pvVar3->super_vlong_flex_unit).a = (uint *)0x0;
    pvVar3->share = 0;
    (this->T).value = pvVar3;
    (this->T).negative = 0;
    (pvVar3->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,0);
    (this->k)._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080eaf76 to 080eafba has its CatchHandler @ 080eb57c
    pvVar3 = (vlong_value *)operator_new(0x10);
    (pvVar3->super_vlong_flex_unit).z = 0;
    (pvVar3->super_vlong_flex_unit).a = (uint *)0x0;
    pvVar3->share = 0;
    (this->k).value = pvVar3;
    (this->k).negative = 0;
    (pvVar3->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,0);
    pvVar3 = (this->m).value;
    if (pvVar3->share == 0) {
        if (pvVar3 != (vlong_value *)0x0) {
            uVar6 = (pvVar3->super_vlong_flex_unit).z;
            while (uVar6 != 0) {
                uVar6 = uVar6 - 1;
                (pvVar3->super_vlong_flex_unit).a[uVar6] = 0;
            }
            puVar2 = (pvVar3->super_vlong_flex_unit).a;
            if (puVar2 != (uint *)0x0) {
                operator_delete__(puVar2);
            }
            operator_delete(pvVar3);
        }
    }
    else {
        pvVar3->share = pvVar3->share - 1;
    }
    pvVar3 = M->value;
    (this->m).value = pvVar3;
    puVar2 = &pvVar3->share;
    *puVar2 = *puVar2 + 1;
    (this->m).negative = M->negative;
    this->N = 0;
    local_2c[0]._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080eaff7 to 080eb0f5 has its CatchHandler @ 080eb575
    local_2c[0].value = (vlong_value *)operator_new(0x10);
    ((local_2c[0].value)->super_vlong_flex_unit).z = 0;
    ((local_2c[0].value)->super_vlong_flex_unit).a = (uint *)0x0;
    (local_2c[0].value)->share = 0;
    local_2c[0].negative = 0;
    ((local_2c[0].value)->super_vlong_flex_unit).n = 0;
    __in_chrg = (vlong_montgomery *)0x0;
    vlong_flex_unit::set((vlong_flex_unit *)local_2c[0].value,0,1);
    pvVar3 = (this->R).value;
    if (pvVar3->share == 0) {
        if (pvVar3 != (vlong_value *)0x0) {
            uVar6 = (pvVar3->super_vlong_flex_unit).z;
            while (uVar6 != 0) {
                uVar6 = uVar6 - 1;
                (pvVar3->super_vlong_flex_unit).a[uVar6] = 0;
            }
            puVar2 = (pvVar3->super_vlong_flex_unit).a;
            if (puVar2 != (uint *)0x0) {
                operator_delete__(puVar2);
            }
            operator_delete(pvVar3);
        }
    }
    else {
        pvVar3->share = pvVar3->share - 1;
    }
    pvVar3 = local_2c[0].value;
    (this->R).value = local_2c[0].value;
    (local_2c[0].value)->share = (local_2c[0].value)->share + 1;
    local_2c[0]._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    (this->R).negative = local_2c[0].negative;
    if ((local_2c[0].value)->share == 0) {
        if (local_2c[0].value != (vlong_value *)0x0) {
                    // try { // try from 080eb32d to 080eb331 has its CatchHandler @ 080eb575
            vlong_flex_unit::~vlong_flex_unit
                      (&(local_2c[0].value)->super_vlong_flex_unit,(int)__in_chrg);
            operator_delete(pvVar3);
        }
    }
    else {
        (local_2c[0].value)->share = (local_2c[0].value)->share - 1;
    }
    while( true ) {
        local_2c[0]._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
        local_2c[0].negative = M->negative;
        iVar4 = 0;
        x = (vlong_montgomery *)M->value;
        (x->R1)._vptr_vlong = (_func_int_varargs **)((int)(x->R1)._vptr_vlong + 1);
        if (((this->R).negative != 0) && ((((this->R).value)->super_vlong_flex_unit).n != 0)) {
            iVar4 = 1;
        }
        iVar5 = 0;
        if ((local_2c[0].negative != 0) && ((x->R)._vptr_vlong != (_func_int_varargs **)0x0)) {
            iVar5 = 1;
        }
        local_2c[0].value = (vlong_value *)x;
        if (iVar4 == iVar5) {
                    // try { // try from 080eb318 to 080eb31c has its CatchHandler @ 080eb3bc
            iVar4 = vlong_value::cf((this->R).value,(vlong_value *)x);
            __in_chrg = x;
        }
        else {
            iVar4 = (uint)(iVar4 == 0) * 2 + -1;
        }
        vlong::~vlong(local_2c,(int)__in_chrg);
        if (-1 < iVar4) break;
        __in_chrg = this;
        vlong::operator+=(&this->R,&this->R);
        this->N = this->N + 1;
    }
    local_3c = (undefined1  [4])&PTR__vlong_08128410;
    result.value = (vlong_value *)(this->R).negative;
    result._vptr_vlong = (_func_int_varargs **)(this->R).value;
    ((vlong_value *)result._vptr_vlong)->share = ((vlong_value *)result._vptr_vlong)->share + 1;
                    // try { // try from 080eb121 to 080eb125 has its CatchHandler @ 080eb515
    vlong::operator-=((vlong *)local_3c,pvVar7);
    a = (vlong *)local_3c;
                    // try { // try from 080eb13a to 080eb13e has its CatchHandler @ 080eb51f
    vlong::modinv(local_2c,a,pvVar7);
    pvVar3 = (this->R1).value;
    if (pvVar3->share == 0) {
        if (pvVar3 != (vlong_value *)0x0) {
            uVar6 = (pvVar3->super_vlong_flex_unit).z;
            while (uVar6 != 0) {
                uVar6 = uVar6 - 1;
                (pvVar3->super_vlong_flex_unit).a[uVar6] = 0;
            }
            puVar2 = (pvVar3->super_vlong_flex_unit).a;
            if (puVar2 != (uint *)0x0) {
                operator_delete__(puVar2);
            }
            operator_delete(pvVar3);
        }
    }
    else {
        pvVar3->share = pvVar3->share - 1;
    }
    pvVar3 = local_2c[0].value;
    (this->R1).value = local_2c[0].value;
    ((vlong *)&(local_2c[0].value)->share)->_vptr_vlong =
         (_func_int_varargs **)((int)((vlong *)&(local_2c[0].value)->share)->_vptr_vlong + 1);
    local_2c[0]._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    (this->R1).negative = local_2c[0].negative;
    pp_Var1 = ((vlong *)&(local_2c[0].value)->share)->_vptr_vlong;
    if (pp_Var1 == (_func_int_varargs **)0x0) {
        if ((vlong_montgomery *)local_2c[0].value != (vlong_montgomery *)0x0) {
                    // try { // try from 080eb2bd to 080eb2c1 has its CatchHandler @ 080eb51f
            vlong_flex_unit::~vlong_flex_unit(&(local_2c[0].value)->super_vlong_flex_unit,(int)a);
            operator_delete(pvVar3);
        }
    }
    else {
        ((vlong *)&(local_2c[0].value)->share)->_vptr_vlong =
             (_func_int_varargs **)((int)pp_Var1 - 1);
    }
    pp_Var1 = result._vptr_vlong;
    local_3c = (undefined1  [4])&PTR__vlong_08128410;
    if (result._vptr_vlong[3] == (_func_int_varargs *)0x0) {
        if (result._vptr_vlong != (_func_int_varargs **)0x0) {
                    // try { // try from 080eb2a0 to 080eb2a4 has its CatchHandler @ 080eb575
            vlong_flex_unit::~vlong_flex_unit((vlong_flex_unit *)result._vptr_vlong,(int)a);
            operator_delete(pp_Var1);
        }
    }
    else {
        result._vptr_vlong[3] = result._vptr_vlong[3] + -1;
    }
                    // try { // try from 080eb1ac to 080eb1b0 has its CatchHandler @ 080eb575
    vlong::modinv(local_2c,pvVar7,&this->R);
    local_3c = (undefined1  [4])&PTR__vlong_08128410;
    result.value = (vlong_value *)(this->R).negative;
    pvVar7 = local_2c;
    result._vptr_vlong = (_func_int_varargs **)(this->R).value;
    ((vlong_value *)result._vptr_vlong)->share = ((vlong_value *)result._vptr_vlong)->share + 1;
                    // try { // try from 080eb1d7 to 080eb1db has its CatchHandler @ 080eb558
    vlong::operator-=((vlong *)local_3c,pvVar7);
    pvVar3 = (this->n1).value;
    if (pvVar3->share == 0) {
        if (pvVar3 != (vlong_value *)0x0) {
            uVar6 = (pvVar3->super_vlong_flex_unit).z;
            while (uVar6 != 0) {
                uVar6 = uVar6 - 1;
                (pvVar3->super_vlong_flex_unit).a[uVar6] = 0;
            }
            puVar2 = (pvVar3->super_vlong_flex_unit).a;
            if (puVar2 != (uint *)0x0) {
                operator_delete__(puVar2);
            }
            operator_delete(pvVar3);
        }
    }
    else {
        pvVar3->share = pvVar3->share - 1;
    }
    pp_Var1 = result._vptr_vlong;
    (this->n1).value = (vlong_value *)result._vptr_vlong;
    result._vptr_vlong[3] = result._vptr_vlong[3] + 1;
    local_3c = (undefined1  [4])&PTR__vlong_08128410;
    (this->n1).negative = (int)result.value;
    if (result._vptr_vlong[3] == (_func_int_varargs *)0x0) {
        if (result._vptr_vlong != (_func_int_varargs **)0x0) {
                    // try { // try from 080eb24e to 080eb252 has its CatchHandler @ 080eb571
            vlong_flex_unit::~vlong_flex_unit((vlong_flex_unit *)result._vptr_vlong,(int)pvVar7);
            operator_delete(pp_Var1);
        }
    }
    else {
        result._vptr_vlong[3] = result._vptr_vlong[3] + -1;
    }
    pvVar3 = local_2c[0].value;
    local_2c[0]._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    pp_Var1 = ((vlong *)&(local_2c[0].value)->share)->_vptr_vlong;
    if (pp_Var1 == (_func_int_varargs **)0x0) {
        if ((vlong_montgomery *)local_2c[0].value != (vlong_montgomery *)0x0) {
                    // try { // try from 080eb238 to 080eb23c has its CatchHandler @ 080eb575
            vlong_flex_unit::~vlong_flex_unit
                      (&(local_2c[0].value)->super_vlong_flex_unit,(int)pvVar7);
            operator_delete(pvVar3);
        }
    }
    else {
        ((vlong *)&(local_2c[0].value)->share)->_vptr_vlong =
             (_func_int_varargs **)((int)pp_Var1 - 1);
    }
    return;
}



// DWARF original prototype: void mul(vlong_montgomery * this, vlong * x, vlong * y)

void __thiscall vlong_montgomery::mul(vlong_montgomery *this,vlong *x,vlong *y)

{
    uint uVar1;
    sbyte sVar2;
    vlong_value *pvVar3;
    uint uVar4;
    int iVar5;
    uint u;
    uint x_00;
    int iVar6;
    uint i;
    vlong_value *i_00;
    vlong_value *pvVar7;
    uint uVar8;
    vlong_value *pvVar9;
    vlong_value *pvVar10;
    byte local_34;
    uint delta;
    vlong_value *local_28;
    int local_24;
    
    vlong_flex_unit::fast_mul
              (&((this->T).value)->super_vlong_flex_unit,&x->value->super_vlong_flex_unit,
               &y->value->super_vlong_flex_unit,this->N * 2);
    vlong_flex_unit::fast_mul
              (&((this->k).value)->super_vlong_flex_unit,&((this->T).value)->super_vlong_flex_unit,
               &((this->n1).value)->super_vlong_flex_unit,this->N);
    vlong_flex_unit::fast_mul
              (&x->value->super_vlong_flex_unit,&((this->k).value)->super_vlong_flex_unit,
               &((this->m).value)->super_vlong_flex_unit,this->N * 2);
    pvVar9 = (vlong_value *)&this->T;
    vlong::operator+=(x,(vlong *)pvVar9);
    pvVar10 = x->value;
    uVar1 = this->N;
    pvVar3 = (vlong_value *)(pvVar10->super_vlong_flex_unit).n;
    uVar8 = uVar1 & 0x1f;
    if (pvVar3 != (vlong_value *)0x0) {
        sVar2 = (sbyte)uVar8;
        local_34 = 0x20 - sVar2;
        i_00 = (vlong_value *)0x0;
        do {
            x_00 = 0;
            pvVar9 = (vlong_value *)((int)&(i_00->super_vlong_flex_unit).n + (uVar1 >> 5));
            if (pvVar9 < pvVar3) {
                x_00 = (pvVar10->super_vlong_flex_unit).a[(int)pvVar9];
            }
            if (uVar8 != 0) {
                uVar4 = 0;
                pvVar9 = (vlong_value *)((int)&(pvVar9->super_vlong_flex_unit).n + 1);
                if (pvVar9 < pvVar3) {
                    uVar4 = (pvVar10->super_vlong_flex_unit).a[(int)pvVar9];
                }
                x_00 = (x_00 >> sVar2) + (uVar4 << (local_34 & 0x1f));
            }
            pvVar7 = (vlong_value *)((int)&(i_00->super_vlong_flex_unit).n + 1);
            pvVar9 = i_00;
            vlong_flex_unit::set(&pvVar10->super_vlong_flex_unit,(uint)i_00,x_00);
            pvVar3 = (vlong_value *)(pvVar10->super_vlong_flex_unit).n;
            i_00 = pvVar7;
        } while (pvVar7 < pvVar3);
    }
    delta = (uint)&PTR__vlong_08128410;
    iVar5 = 0;
    local_24 = (this->m).negative;
    pvVar10 = (this->m).value;
    pvVar10->share = pvVar10->share + 1;
    if ((x->negative != 0) && ((x->value->super_vlong_flex_unit).n != 0)) {
        iVar5 = 1;
    }
    iVar6 = 0;
    if ((local_24 != 0) && ((pvVar10->super_vlong_flex_unit).n != 0)) {
        iVar6 = 1;
    }
    local_28 = pvVar10;
    if (iVar5 == iVar6) {
                    // try { // try from 080eb76a to 080eb76e has its CatchHandler @ 080eb771
        iVar5 = vlong_value::cf(x->value,pvVar10);
        pvVar9 = pvVar10;
    }
    else {
        iVar5 = (uint)(iVar5 == 0) * 2 + -1;
    }
    vlong::~vlong((vlong *)&delta,(int)pvVar9);
    if (-1 < iVar5) {
        vlong::operator-=(x,&this->m);
    }
    return;
}



// DWARF original prototype: vlong exp(vlong_montgomery * this, vlong * x, vlong * e)

double __thiscall vlong_montgomery::exp(vlong_montgomery *this,double __x)

{
    vlong_flex_unit *x;
    _func_int_varargs **pp_Var1;
    vlong_value *x_00;
    uint uVar2;
    uint uVar3;
    vlong_value *pvVar4;
    uint x_2;
    uint i;
    uint uVar5;
    longdouble extraout_ST0;
    longdouble lVar6;
    longdouble extraout_ST0_00;
    longdouble extraout_ST0_01;
    longdouble extraout_ST0_02;
    vlong *result_5;
    int in_stack_00000010;
    uint uVar7;
    uint local_84;
    uint *local_80;
    undefined1 local_5c [4];
    vlong_value divide;
    vlong result_2;
    undefined1 local_3c [4];
    vlong t;
    undefined1 local_2c [4];
    vlong result;
    
    local_2c = (undefined1  [4])&PTR__vlong_08128410;
    result.value = (vlong_value *)result_5->negative;
    result._vptr_vlong = (_func_int_varargs **)result_5->value;
    ((vlong_value *)result._vptr_vlong)->share = ((vlong_value *)result._vptr_vlong)->share + 1;
                    // try { // try from 080eb7cb to 080eb7cf has its CatchHandler @ 080ebce1
    vlong::operator-=((vlong *)local_2c,result_5 + 2);
    divide.share = (uint)&PTR__vlong_08128410;
                    // try { // try from 080eb7de to 080eb81f has its CatchHandler @ 080ebdf0
    x_00 = (vlong_value *)operator_new(0x10);
    (x_00->super_vlong_flex_unit).z = 0;
    (x_00->super_vlong_flex_unit).a = (uint *)0x0;
    x_00->share = 0;
    (x_00->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)x_00,0,0);
    x = *(vlong_flex_unit **)(__x._4_4_ + 4);
    pvVar4 = result_5->value;
    uVar5 = x->n << 5;
    do {
        uVar7 = uVar5;
        if (uVar7 == 0) break;
        uVar5 = uVar7 - 1;
        if (uVar5 >> 5 < x->n) {
            uVar2 = x->a[uVar5 >> 5];
        }
        else {
            uVar2 = 0;
        }
    } while ((1 << ((byte)uVar5 & 0x1f) & uVar2) == 0);
    uVar5 = (pvVar4->super_vlong_flex_unit).n << 5;
    do {
        uVar2 = uVar5;
        if (uVar2 == 0) break;
        uVar5 = uVar2 - 1;
        if (uVar5 >> 5 < (pvVar4->super_vlong_flex_unit).n) {
            uVar3 = (pvVar4->super_vlong_flex_unit).a[uVar5 >> 5];
        }
        else {
            uVar3 = 0;
        }
    } while ((1 << ((byte)uVar5 & 0x1f) & uVar3) == 0);
                    // try { // try from 080eb8c8 to 080eb8cc has its CatchHandler @ 080ebcf0
    vlong_flex_unit::fast_mul
              ((vlong_flex_unit *)x_00,x,&pvVar4->super_vlong_flex_unit,uVar2 + uVar7);
    local_3c = (undefined1  [4])&PTR__vlong_08128410;
    uVar5 = result_5->negative;
    uVar7 = *(uint *)(__x._4_4_ + 8);
                    // try { // try from 080eb8ec to 080eb92d has its CatchHandler @ 080ebd80
    t._vptr_vlong = (_func_int_varargs **)operator_new(0x10);
    ((vlong_flex_unit *)t._vptr_vlong)->z = 0;
    ((vlong_flex_unit *)t._vptr_vlong)->a = (uint *)0x0;
    ((vlong_flex_unit *)((int)t._vptr_vlong + 0xc))->n = 0;
    t.value = (vlong_value *)0x0;
    ((vlong_flex_unit *)t._vptr_vlong)->n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)t._vptr_vlong,0,0);
    divide.super_vlong_flex_unit.a = (uint *)0x0;
    divide.super_vlong_flex_unit.n = 0;
    local_5c = (undefined1  [4])0x0;
    divide.super_vlong_flex_unit.z = 0;
    pvVar4 = x_00;
                    // try { // try from 080eb968 to 080eb96c has its CatchHandler @ 080ebd33
    vlong_value::divide((vlong_value *)local_5c,x_00,result_5[2].value,(vlong_value *)t._vptr_vlong)
    ;
    t.value = (vlong_value *)(uVar5 ^ uVar7);
                    // try { // try from 080eb979 to 080eb97d has its CatchHandler @ 080ebd78
    vlong_flex_unit::~vlong_flex_unit((vlong_flex_unit *)local_5c,(int)pvVar4);
    divide.share = (uint)&PTR__vlong_08128410;
    if (x_00->share == 0) {
        if (x_00 != (vlong_value *)0x0) {
                    // try { // try from 080ebcb3 to 080ebcb7 has its CatchHandler @ 080ebdf0
            vlong_flex_unit::~vlong_flex_unit((vlong_flex_unit *)x_00,(int)pvVar4);
            operator_delete(x_00);
        }
    }
    else {
        x_00->share = x_00->share - 1;
    }
    local_80 = *(uint **)(in_stack_00000010 + 4);
    uVar5 = *local_80 << 5;
    do {
        uVar7 = uVar5;
        if (uVar7 == 0) break;
        uVar5 = uVar7 - 1;
        if (uVar5 >> 5 < *local_80) {
            uVar2 = *(uint *)(local_80[1] + (uVar5 >> 5) * 4);
        }
        else {
            uVar2 = 0;
        }
    } while ((1 << ((byte)uVar5 & 0x1f) & uVar2) == 0);
    uVar5 = 0;
    while( true ) {
        if (uVar5 >> 5 < *local_80) {
            uVar2 = *(uint *)(local_80[1] + (uVar5 >> 5) * 4);
        }
        else {
            uVar2 = 0;
        }
        if ((1 << ((byte)uVar5 & 0x1f) & uVar2) != 0) {
            mul((vlong_montgomery *)result_5,(vlong *)local_2c,(vlong *)local_3c);
        }
        uVar5 = uVar5 + 1;
        if (uVar5 == uVar7) break;
                    // try { // try from 080eba0f to 080eba77 has its CatchHandler @ 080ebde7
        mul((vlong_montgomery *)result_5,(vlong *)local_3c,(vlong *)local_3c);
        local_80 = *(uint **)(in_stack_00000010 + 4);
    }
    local_5c = (undefined1  [4])&PTR__vlong_08128410;
    divide.super_vlong_flex_unit.n = (uint)operator_new(0x10);
    ((vlong_flex_unit *)divide.super_vlong_flex_unit.n)->z = 0;
    ((vlong_flex_unit *)divide.super_vlong_flex_unit.n)->a = (uint *)0x0;
    ((vlong_flex_unit *)(divide.super_vlong_flex_unit.n + 0xc))->n = 0;
    divide.super_vlong_flex_unit.a = (uint *)0x0;
    ((vlong_flex_unit *)divide.super_vlong_flex_unit.n)->n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)divide.super_vlong_flex_unit.n,0,0);
    pvVar4 = result_5[1].value;
    uVar5 = (int)*result._vptr_vlong << 5;
    do {
        uVar7 = uVar5;
        if (uVar7 == 0) break;
        uVar5 = uVar7 - 1;
        if ((_func_int_varargs *)(uVar5 >> 5) < *result._vptr_vlong) {
            uVar2 = *(uint *)(result._vptr_vlong[1] + (int)(uVar5 >> 5) * 4);
        }
        else {
            uVar2 = 0;
        }
    } while ((1 << ((byte)uVar5 & 0x1f) & uVar2) == 0);
    uVar5 = (pvVar4->super_vlong_flex_unit).n << 5;
    do {
        uVar2 = uVar5;
        if (uVar2 == 0) break;
        uVar5 = uVar2 - 1;
        if (uVar5 >> 5 < (pvVar4->super_vlong_flex_unit).n) {
            local_84 = (pvVar4->super_vlong_flex_unit).a[uVar5 >> 5];
        }
        else {
            local_84 = 0;
        }
    } while ((local_84 & 1 << ((byte)uVar5 & 0x1f)) == 0);
                    // try { // try from 080ebb09 to 080ebb0d has its CatchHandler @ 080ebd84
    vlong_flex_unit::fast_mul
              ((vlong_flex_unit *)divide.super_vlong_flex_unit.n,
               (vlong_flex_unit *)result._vptr_vlong,&pvVar4->super_vlong_flex_unit,uVar2 + uVar7);
    divide.super_vlong_flex_unit.a = (uint *)(result_5[1].negative ^ (uint)result.value);
    (this->R)._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080ebb2c to 080ebb70 has its CatchHandler @ 080ebde3
    pvVar4 = (vlong_value *)operator_new(0x10);
    (pvVar4->super_vlong_flex_unit).z = 0;
    (pvVar4->super_vlong_flex_unit).a = (uint *)0x0;
    pvVar4->share = 0;
    (this->R).value = pvVar4;
    (this->R).negative = 0;
    (pvVar4->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar4,0,0);
    divide.share = 0;
    uVar7 = divide.super_vlong_flex_unit.n;
                    // try { // try from 080ebbae to 080ebbb2 has its CatchHandler @ 080ebd9d
    vlong_value::divide((vlong_value *)&divide.share,(vlong_value *)divide.super_vlong_flex_unit.n,
                        result_5[2].value,(this->R).value);
    (this->R).negative = (int)divide.super_vlong_flex_unit.a;
                    // try { // try from 080ebbbf to 080ebbc3 has its CatchHandler @ 080ebddf
    vlong_flex_unit::~vlong_flex_unit((vlong_flex_unit *)&divide.share,uVar7);
    uVar5 = divide.super_vlong_flex_unit.n;
    local_5c = (undefined1  [4])&PTR__vlong_08128410;
    lVar6 = extraout_ST0;
    if (*(int *)(divide.super_vlong_flex_unit.n + 0xc) == 0) {
        if (divide.super_vlong_flex_unit.n != 0) {
                    // try { // try from 080ebc46 to 080ebc89 has its CatchHandler @ 080ebde7
            vlong_flex_unit::~vlong_flex_unit
                      ((vlong_flex_unit *)divide.super_vlong_flex_unit.n,uVar7);
            operator_delete((void *)uVar5);
            lVar6 = extraout_ST0_02;
        }
    }
    else {
        *(int *)(divide.super_vlong_flex_unit.n + 0xc) =
             *(int *)(divide.super_vlong_flex_unit.n + 0xc) + -1;
    }
    pp_Var1 = t._vptr_vlong;
    local_3c = (undefined1  [4])&PTR__vlong_08128410;
    if (t._vptr_vlong[3] == (_func_int_varargs *)0x0) {
        if (t._vptr_vlong != (_func_int_varargs **)0x0) {
                    // try { // try from 080ebc30 to 080ebc34 has its CatchHandler @ 080ebdf0
            vlong_flex_unit::~vlong_flex_unit((vlong_flex_unit *)t._vptr_vlong,uVar7);
            operator_delete(pp_Var1);
            lVar6 = extraout_ST0_01;
        }
    }
    else {
        t._vptr_vlong[3] = t._vptr_vlong[3] + -1;
    }
    pp_Var1 = result._vptr_vlong;
    local_2c = (undefined1  [4])&PTR__vlong_08128410;
    if (result._vptr_vlong[3] == (_func_int_varargs *)0x0) {
        if (result._vptr_vlong != (_func_int_varargs **)0x0) {
            vlong_flex_unit::~vlong_flex_unit((vlong_flex_unit *)result._vptr_vlong,uVar7);
            operator_delete(pp_Var1);
            lVar6 = extraout_ST0_00;
        }
    }
    else {
        result._vptr_vlong[3] = result._vptr_vlong[3] + -1;
    }
    return (double)lVar6;
}



// DWARF original prototype: void vlong_flex_unit(vlong_flex_unit * this)

void __thiscall vlong_flex_unit::vlong_flex_unit(vlong_flex_unit *this)

{
    this->z = 0;
    this->a = (uint *)0x0;
    this->n = 0;
    return;
}



// DWARF original prototype: void vlong_flex_unit(vlong_flex_unit * this)

void __thiscall vlong_flex_unit::vlong_flex_unit(vlong_flex_unit *this)

{
    this->z = 0;
    this->a = (uint *)0x0;
    this->n = 0;
    return;
}



// DWARF original prototype: void ~vlong_flex_unit(vlong_flex_unit * this, int __in_chrg)

void __thiscall vlong_flex_unit::~vlong_flex_unit(vlong_flex_unit *this,int __in_chrg)

{
    uint i;
    uint uVar1;
    
    uVar1 = this->z;
    while (uVar1 != 0) {
        uVar1 = uVar1 - 1;
        this->a[uVar1] = 0;
    }
    if (this->a == (uint *)0x0) {
        return;
    }
    operator_delete__(this->a);
    return;
}



// DWARF original prototype: void ~vlong_flex_unit(vlong_flex_unit * this, int __in_chrg)

void __thiscall vlong_flex_unit::~vlong_flex_unit(vlong_flex_unit *this,int __in_chrg)

{
    uint i;
    uint uVar1;
    
    uVar1 = this->z;
    while (uVar1 != 0) {
        uVar1 = uVar1 - 1;
        this->a[uVar1] = 0;
    }
    if (this->a == (uint *)0x0) {
        return;
    }
    operator_delete__(this->a);
    return;
}



// DWARF original prototype: uint get(vlong_flex_unit * this, uint i)

uint __thiscall vlong_flex_unit::get(vlong_flex_unit *this,uint i)

{
    uint uVar1;
    
    if (i < this->n) {
        uVar1 = this->a[i];
    }
    else {
        uVar1 = 0;
    }
    return uVar1;
}



// DWARF original prototype: void clear(vlong_flex_unit * this)

void __thiscall vlong_flex_unit::clear(vlong_flex_unit *this)

{
    this->n = 0;
    return;
}



// DWARF original prototype: void reserve(vlong_flex_unit * this, uint x)

void __thiscall vlong_flex_unit::reserve(vlong_flex_unit *this,uint x)

{
    uint *puVar1;
    uint i;
    uint uVar2;
    uint *na;
    
    if (this->z < x) {
        puVar1 = (uint *)operator_new__(x * 4);
        uVar2 = 0;
        if (this->n != 0) {
            do {
                puVar1[uVar2] = this->a[uVar2];
                uVar2 = uVar2 + 1;
            } while (uVar2 < this->n);
        }
        if (this->a != (uint *)0x0) {
            operator_delete__(this->a);
        }
        this->a = puVar1;
        this->z = x;
    }
    return;
}



// DWARF original prototype: void set(vlong_flex_unit * this, uint i, uint x)

void __thiscall vlong_flex_unit::set(vlong_flex_unit *this,uint i,uint x)

{
    uint uVar1;
    uint uVar2;
    uint *puVar3;
    uint j;
    uint uVar4;
    uint *na;
    
    uVar4 = this->n;
    if (i < uVar4) {
        this->a[i] = x;
        if ((x == 0) && (uVar4 = this->n, uVar4 != 0)) {
            uVar1 = this->a[uVar4 - 1];
            while (uVar1 == 0) {
                uVar2 = uVar4 - 1;
                this->n = uVar2;
                if (uVar2 == 0) {
                    return;
                }
                uVar1 = this->a[uVar4 - 2];
                uVar4 = uVar2;
            }
        }
    }
    else if (x != 0) {
        uVar1 = i + 1;
        if (this->z < uVar1) {
            puVar3 = (uint *)operator_new__(uVar1 * 4);
            uVar2 = 0;
            uVar4 = this->n;
            if (uVar4 != 0) {
                do {
                    puVar3[uVar2] = this->a[uVar2];
                    uVar2 = uVar2 + 1;
                    uVar4 = this->n;
                } while (uVar2 < uVar4);
            }
            if (this->a != (uint *)0x0) {
                operator_delete__(this->a);
                uVar4 = this->n;
            }
            this->a = puVar3;
            this->z = uVar1;
        }
        for (; uVar4 < i; uVar4 = uVar4 + 1) {
            this->a[uVar4] = 0;
        }
        this->a[i] = x;
        this->n = uVar1;
    }
    return;
}



// WARNING: Variable defined which should be unmapped: i
// DWARF original prototype: void fast_mul(vlong_flex_unit * this, vlong_flex_unit * x,
// vlong_flex_unit * y, uint keep)

void __thiscall
vlong_flex_unit::fast_mul(vlong_flex_unit *this,vlong_flex_unit *x,vlong_flex_unit *y,uint keep)

{
    uint *puVar1;
    uint m;
    uint *puVar2;
    uint p;
    uint v;
    uint uVar3;
    uint uVar4;
    uint uVar5;
    uint *na;
    uint uVar6;
    uint c;
    uint uVar7;
    int iVar8;
    uint w;
    uint uVar9;
    uint uVar10;
    uint uVar11;
    uint local_24;
    uint j;
    uint min_1;
    uint min;
    uint limit;
    uint i;
    
    min = keep + 0x1f >> 5;
    if (this->z < min) {
        puVar2 = (uint *)operator_new__(min << 2);
        uVar5 = 0;
        if (this->n != 0) {
            do {
                puVar2[uVar5] = this->a[uVar5];
                uVar5 = uVar5 + 1;
            } while (uVar5 < this->n);
        }
        if (this->a != (uint *)0x0) {
            operator_delete__(this->a);
        }
        this->a = puVar2;
        this->z = min;
    }
    limit = 0;
    if (min != 0) {
        do {
            this->a[limit] = 0;
            limit = limit + 1;
        } while (limit < min);
    }
    limit = 0;
    uVar5 = min;
    if (x->n <= min) {
        uVar5 = x->n;
    }
    if (uVar5 != 0) {
        do {
            uVar7 = 0;
            uVar9 = limit + y->n;
            uVar3 = min;
            if (uVar9 <= min) {
                uVar3 = uVar9;
            }
            local_24 = limit;
            if (limit < uVar3) {
                uVar10 = x->a[limit] & 0xffff;
                uVar9 = x->a[limit] >> 0x10;
                uVar7 = 0;
                do {
                    uVar4 = this->a[local_24] + uVar7;
                    uVar7 = (uint)(uVar4 < uVar7);
                    uVar6 = y->a[local_24 - limit] & 0xffff;
                    uVar11 = uVar10 * uVar6;
                    uVar4 = uVar4 + uVar11;
                    if (uVar4 < uVar11) {
                        uVar7 = uVar7 + 1;
                    }
                    uVar6 = uVar9 * uVar6;
                    uVar11 = uVar6 * 0x10000;
                    uVar4 = uVar4 + uVar11;
                    iVar8 = uVar7 + (uVar6 >> 0x10);
                    if (uVar4 < uVar11) {
                        iVar8 = iVar8 + 1;
                    }
                    uVar7 = y->a[local_24 - limit] >> 0x10;
                    uVar6 = uVar10 * uVar7;
                    uVar11 = uVar6 * 0x10000;
                    iVar8 = iVar8 + (uVar6 >> 0x10);
                    uVar4 = uVar4 + uVar11;
                    if (uVar4 < uVar11) {
                        iVar8 = iVar8 + 1;
                    }
                    uVar7 = iVar8 + uVar7 * uVar9;
                    this->a[local_24] = uVar4;
                    local_24 = local_24 + 1;
                } while (local_24 < uVar3);
            }
            if ((uVar7 != 0) && (local_24 < min)) {
                puVar2 = this->a;
                do {
                    puVar2[local_24] = puVar2[local_24] + uVar7;
                    puVar2 = this->a;
                    puVar1 = puVar2 + local_24;
                    local_24 = local_24 + 1;
                    uVar7 = (uint)(*puVar1 < uVar7);
                    if (uVar7 == 0) break;
                } while (local_24 < min);
            }
            limit = limit + 1;
        } while (limit < uVar5);
    }
    if ((keep & 0x1f) != 0) {
        this->a[min - 1] = this->a[min - 1] & (1 << (sbyte)(keep & 0x1f)) - 1U;
    }
    if ((min != 0) && (this->a[min - 1] == 0)) {
        puVar2 = this->a + (min - 1);
        do {
            puVar2 = puVar2 + -1;
            min = min - 1;
            if (min == 0) break;
        } while (*puVar2 == 0);
    }
    this->n = min;
    return;
}



// DWARF original name: operator unsigned int
// DWARF original prototype: uint operator_unsigned_int(vlong_value * this)

uint __thiscall vlong_value::operator_unsigned_int(vlong_value *this)

{
    uint uVar1;
    
    if ((this->super_vlong_flex_unit).n == 0) {
        uVar1 = 0;
    }
    else {
        uVar1 = *(this->super_vlong_flex_unit).a;
    }
    return uVar1;
}



// DWARF original prototype: int is_zero(vlong_value * this)

int __thiscall vlong_value::is_zero(vlong_value *this)

{
    return (uint)((this->super_vlong_flex_unit).n == 0);
}



// DWARF original prototype: int test(vlong_value * this, uint i)

int __thiscall vlong_value::test(vlong_value *this,uint i)

{
    uint uVar1;
    
    if (i >> 5 < (this->super_vlong_flex_unit).n) {
        uVar1 = (this->super_vlong_flex_unit).a[i >> 5];
    }
    else {
        uVar1 = 0;
    }
    return (uint)((1 << ((byte)i & 0x1f) & uVar1) != 0);
}



// DWARF original prototype: uint bits(vlong_value * this)

uint __thiscall vlong_value::bits(vlong_value *this)

{
    uint uVar1;
    uint uVar2;
    uint x;
    uint uVar3;
    
    uVar1 = (this->super_vlong_flex_unit).n << 5;
    do {
        uVar3 = uVar1;
        if (uVar3 == 0) {
            return 0;
        }
        uVar1 = uVar3 - 1;
        if (uVar1 >> 5 < (this->super_vlong_flex_unit).n) {
            uVar2 = (this->super_vlong_flex_unit).a[uVar1 >> 5];
        }
        else {
            uVar2 = 0;
        }
    } while ((1 << ((byte)uVar1 & 0x1f) & uVar2) == 0);
    return uVar3;
}



// DWARF original prototype: int cf(vlong_value * this, vlong_value * x)

int __thiscall vlong_value::cf(vlong_value *this,vlong_value *x)

{
    uint uVar1;
    uint uVar2;
    int iVar3;
    uint uVar4;
    uint i;
    uint uVar5;
    uint uVar6;
    
    uVar1 = (this->super_vlong_flex_unit).n;
    uVar2 = (x->super_vlong_flex_unit).n;
    iVar3 = 1;
    if ((uVar1 <= uVar2) && (iVar3 = -1, uVar5 = uVar1, uVar2 <= uVar1)) {
        do {
            if (uVar5 == 0) {
                return 0;
            }
            uVar5 = uVar5 - 1;
            uVar6 = 0;
            if (uVar5 < uVar1) {
                uVar6 = (this->super_vlong_flex_unit).a[uVar5];
            }
            uVar4 = 0;
            if (uVar5 < uVar2) {
                uVar4 = (x->super_vlong_flex_unit).a[uVar5];
            }
            if (uVar4 < uVar6) {
                return 1;
            }
            if (uVar5 < (this->super_vlong_flex_unit).n) {
                uVar6 = (this->super_vlong_flex_unit).a[uVar5];
            }
            else {
                uVar6 = 0;
            }
            if (uVar5 < (x->super_vlong_flex_unit).n) {
                uVar4 = (x->super_vlong_flex_unit).a[uVar5];
            }
            else {
                uVar4 = 0;
            }
            iVar3 = -1;
        } while (uVar4 <= uVar6);
    }
    return iVar3;
}



// WARNING: Variable defined which should be unmapped: N
// DWARF original prototype: void shl(vlong_value * this)

void __thiscall vlong_value::shl(vlong_value *this)

{
    uint uVar1;
    uint uVar2;
    uint carry;
    uint uVar3;
    uint uVar4;
    uint i;
    uint i_00;
    uint N;
    
    uVar3 = 0;
    uVar1 = (this->super_vlong_flex_unit).n;
    uVar2 = uVar1;
    i_00 = 0;
    while( true ) {
        uVar4 = 0;
        if (i_00 < uVar2) {
            uVar4 = (this->super_vlong_flex_unit).a[i_00];
        }
        vlong_flex_unit::set(&this->super_vlong_flex_unit,i_00,uVar3 + uVar4 * 2);
        uVar3 = uVar4 >> 0x1f;
        if (uVar1 < i_00 + 1) break;
        uVar2 = (this->super_vlong_flex_unit).n;
        i_00 = i_00 + 1;
    }
    return;
}



// DWARF original prototype: void shr(vlong_value * this)

void __thiscall vlong_value::shr(vlong_value *this)

{
    uint uVar1;
    uint carry;
    int iVar2;
    uint i;
    uint i_00;
    uint uVar3;
    
    iVar2 = 0;
    uVar1 = (this->super_vlong_flex_unit).n;
    i_00 = uVar1;
    if (uVar1 != 0) {
        while( true ) {
            i_00 = i_00 - 1;
            uVar3 = 0;
            if (i_00 < uVar1) {
                uVar3 = (this->super_vlong_flex_unit).a[i_00];
            }
            vlong_flex_unit::set(&this->super_vlong_flex_unit,i_00,(uVar3 >> 1) + iVar2);
            iVar2 = uVar3 << 0x1f;
            if (i_00 == 0) break;
            uVar1 = (this->super_vlong_flex_unit).n;
        }
    }
    return;
}



// WARNING: Variable defined which should be unmapped: delta
// DWARF original prototype: void shr(vlong_value * this, uint x)

void __thiscall vlong_value::shr(vlong_value *this,uint x)

{
    sbyte sVar1;
    uint uVar2;
    uint uVar3;
    uint uVar4;
    uint u;
    uint x_00;
    uint i;
    uint i_00;
    byte local_18;
    uint delta;
    
    uVar2 = (this->super_vlong_flex_unit).n;
    if (uVar2 != 0) {
        sVar1 = (sbyte)(x & 0x1f);
        local_18 = 0x20 - sVar1;
        i_00 = 0;
        do {
            x_00 = 0;
            uVar4 = (x >> 5) + i_00;
            if (uVar4 < uVar2) {
                x_00 = (this->super_vlong_flex_unit).a[uVar4];
            }
            if ((x & 0x1f) != 0) {
                uVar3 = 0;
                if (uVar4 + 1 < uVar2) {
                    uVar3 = (this->super_vlong_flex_unit).a[uVar4 + 1];
                }
                x_00 = (x_00 >> sVar1) + (uVar3 << (local_18 & 0x1f));
            }
            uVar4 = i_00 + 1;
            vlong_flex_unit::set(&this->super_vlong_flex_unit,i_00,x_00);
            uVar2 = (this->super_vlong_flex_unit).n;
            i_00 = uVar4;
        } while (uVar4 < uVar2);
    }
    return;
}



// DWARF original prototype: void add(vlong_value * this, vlong_value * x)

void __thiscall vlong_value::add(vlong_value *this,vlong_value *x)

{
    uint *puVar1;
    uint uVar2;
    uint *puVar3;
    uint uVar4;
    uint u;
    uint i;
    uint uVar5;
    uint uVar6;
    uint carry;
    uint uVar7;
    uint max;
    uint uVar8;
    
    uVar8 = (this->super_vlong_flex_unit).n;
    uVar5 = (x->super_vlong_flex_unit).n;
    if (uVar8 < uVar5) {
        uVar8 = uVar5;
    }
    if ((this->super_vlong_flex_unit).z < uVar8) {
        puVar3 = (uint *)operator_new__(uVar8 * 4);
        uVar5 = 0;
        if ((this->super_vlong_flex_unit).n != 0) {
            do {
                puVar3[uVar5] = (this->super_vlong_flex_unit).a[uVar5];
                uVar5 = uVar5 + 1;
            } while (uVar5 < (this->super_vlong_flex_unit).n);
        }
        puVar1 = (this->super_vlong_flex_unit).a;
        if (puVar1 != (uint *)0x0) {
            operator_delete__(puVar1);
        }
        (this->super_vlong_flex_unit).a = puVar3;
        (this->super_vlong_flex_unit).z = uVar8;
    }
    uVar7 = 0;
    uVar5 = 0;
    if (uVar8 != 0xffffffff) {
        do {
            if (uVar5 < (this->super_vlong_flex_unit).n) {
                uVar2 = (this->super_vlong_flex_unit).a[uVar5];
            }
            else {
                uVar2 = 0;
            }
            uVar6 = (uint)(uVar7 + uVar2 < uVar7);
            if (uVar5 < (x->super_vlong_flex_unit).n) {
                uVar4 = (x->super_vlong_flex_unit).a[uVar5];
            }
            else {
                uVar4 = 0;
            }
            uVar2 = uVar7 + uVar2 + uVar4;
            uVar7 = uVar6;
            if (uVar2 < uVar4) {
                uVar7 = uVar6 + 1;
            }
            uVar6 = uVar5 + 1;
            vlong_flex_unit::set(&this->super_vlong_flex_unit,uVar5,uVar2);
            uVar5 = uVar6;
        } while (uVar6 < uVar8 + 1);
    }
    return;
}



// WARNING: Variable defined which should be unmapped: N
// DWARF original prototype: void subtract(vlong_value * this, vlong_value * x)

void __thiscall vlong_value::subtract(vlong_value *this,vlong_value *x)

{
    uint uVar1;
    uint uVar2;
    uint uVar3;
    uint ux;
    uint nu;
    uint i;
    uint i_00;
    uint carry;
    uint uVar4;
    uint N;
    
    i_00 = 0;
    uVar1 = (this->super_vlong_flex_unit).n;
    if (uVar1 != 0) {
        uVar4 = 0;
        do {
            if (i_00 < (x->super_vlong_flex_unit).n) {
                uVar2 = (x->super_vlong_flex_unit).a[i_00];
            }
            else {
                uVar2 = 0;
            }
            if (uVar4 <= uVar4 + uVar2) {
                if (i_00 < (this->super_vlong_flex_unit).n) {
                    uVar3 = (this->super_vlong_flex_unit).a[i_00];
                }
                else {
                    uVar3 = 0;
                }
                uVar2 = uVar3 - (uVar4 + uVar2);
                uVar4 = (uint)(uVar3 < uVar2);
                vlong_flex_unit::set(&this->super_vlong_flex_unit,i_00,uVar2);
            }
            i_00 = i_00 + 1;
        } while (i_00 < uVar1);
    }
    return;
}



// DWARF original prototype: void init(vlong_value * this, uint x)

int __thiscall vlong_value::init(vlong_value *this,EVP_PKEY_CTX *ctx)

{
    (this->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set(&this->super_vlong_flex_unit,0,(uint)ctx);
    return (int)ctx;
}



// DWARF original prototype: void copy(vlong_value * this, vlong_value * x)

int __thiscall vlong_value::copy(vlong_value *this,EVP_PKEY_CTX *dst,EVP_PKEY_CTX *src)

{
    uint in_EAX;
    uint i;
    uint i_00;
    
    (this->super_vlong_flex_unit).n = 0;
    i_00 = *(uint *)dst;
    while (i_00 != 0) {
        i_00 = i_00 - 1;
        if (i_00 < *(uint *)dst) {
            in_EAX = *(uint *)(*(int *)(dst + 4) + i_00 * 4);
        }
        else {
            in_EAX = 0;
        }
        vlong_flex_unit::set(&this->super_vlong_flex_unit,i_00,in_EAX);
    }
    return in_EAX;
}



// DWARF original prototype: void vlong_value(vlong_value * this)

void __thiscall vlong_value::vlong_value(vlong_value *this)

{
    (this->super_vlong_flex_unit).z = 0;
    (this->super_vlong_flex_unit).a = (uint *)0x0;
    (this->super_vlong_flex_unit).n = 0;
    this->share = 0;
    return;
}



// DWARF original prototype: void vlong_value(vlong_value * this)

void __thiscall vlong_value::vlong_value(vlong_value *this)

{
    (this->super_vlong_flex_unit).z = 0;
    (this->super_vlong_flex_unit).a = (uint *)0x0;
    (this->super_vlong_flex_unit).n = 0;
    this->share = 0;
    return;
}



// DWARF original prototype: void mul(vlong_value * this, vlong_value * x, vlong_value * y)

void __thiscall vlong_value::mul(vlong_value *this,vlong_value *x,vlong_value *y)

{
    uint uVar1;
    uint uVar2;
    
    uVar1 = bits(x);
    uVar2 = bits(y);
    vlong_flex_unit::fast_mul
              (&this->super_vlong_flex_unit,&x->super_vlong_flex_unit,&y->super_vlong_flex_unit,
               uVar2 + uVar1);
    return;
}



// DWARF original prototype: void divide(vlong_value * this, vlong_value * x, vlong_value * y,
// vlong_value * rem)

void __thiscall
vlong_value::divide(vlong_value *this,vlong_value *x,vlong_value *y,vlong_value *rem)

{
    uint *this_00;
    uint i_00;
    uint i_01;
    int iVar1;
    uint carry;
    EVP_PKEY_CTX *src;
    uint local_4c;
    uint i_1;
    uint i;
    vlong_value s;
    vlong_value m;
    
    this_00 = &s.share;
    (this->super_vlong_flex_unit).n = 0;
    src = (EVP_PKEY_CTX *)0x0;
    vlong_flex_unit::set(&this->super_vlong_flex_unit,0,0);
    copy(rem,(EVP_PKEY_CTX *)x,src);
    vlong_value((vlong_value *)this_00);
    vlong_value((vlong_value *)&i);
                    // try { // try from 080ec83c to 080ec97b has its CatchHandler @ 080ec9d7
    copy((vlong_value *)this_00,(EVP_PKEY_CTX *)y,src);
    i = 0;
    vlong_flex_unit::set((vlong_flex_unit *)&i,0,1);
    while (iVar1 = cf(rem,(vlong_value *)this_00), 0 < iVar1) {
        shl((vlong_value *)this_00);
        shl((vlong_value *)&i);
    }
    while (iVar1 = cf(rem,y), -1 < iVar1) {
        while (iVar1 = cf(rem,(vlong_value *)this_00), iVar1 < 0) {
            iVar1 = 0;
            i_01 = i;
            i_00 = s.share;
            while (i = i_01, i_00 != 0) {
                i_00 = i_00 - 1;
                if (i_00 < s.share) {
                    i_1 = *(uint *)(m.super_vlong_flex_unit.n + i_00 * 4);
                }
                else {
                    i_1 = 0;
                }
                vlong_flex_unit::set((vlong_flex_unit *)this_00,i_00,(i_1 >> 1) + iVar1);
                iVar1 = i_1 << 0x1f;
                i_01 = i;
            }
            iVar1 = 0;
            while (i_01 != 0) {
                i_01 = i_01 - 1;
                if (i_01 < i) {
                    local_4c = *(uint *)(s.super_vlong_flex_unit.n + i_01 * 4);
                }
                else {
                    local_4c = 0;
                }
                vlong_flex_unit::set((vlong_flex_unit *)&i,i_01,(local_4c >> 1) + iVar1);
                iVar1 = local_4c << 0x1f;
            }
        }
        subtract(rem,(vlong_value *)this_00);
        add(this,(vlong_value *)&i);
    }
    while (s.super_vlong_flex_unit.a != (uint *)0x0) {
        s.super_vlong_flex_unit.a = (uint *)((int)s.super_vlong_flex_unit.a + -1);
        *(undefined4 *)(s.super_vlong_flex_unit.n + (int)s.super_vlong_flex_unit.a * 4) = 0;
    }
    if (s.super_vlong_flex_unit.n != 0) {
        operator_delete__((void *)s.super_vlong_flex_unit.n);
    }
    if (m.super_vlong_flex_unit.a != (uint *)0x0) {
        do {
            m.super_vlong_flex_unit.a = (uint *)((int)m.super_vlong_flex_unit.a + -1);
            *(undefined4 *)(m.super_vlong_flex_unit.n + (int)m.super_vlong_flex_unit.a * 4) = 0;
        } while (m.super_vlong_flex_unit.a != (uint *)0x0);
    }
    if (m.super_vlong_flex_unit.n != 0) {
        operator_delete__((void *)m.super_vlong_flex_unit.n);
    }
    return;
}



// DWARF original prototype: void vlong(vlong * this, uint x)

void __thiscall vlong::vlong(vlong *this,uint x)

{
    vlong_value *this_00;
    
    this->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    this_00 = (vlong_value *)operator_new(0x10);
    vlong_value::vlong_value(this_00);
    this->negative = 0;
    this->value = this_00;
    (this_00->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)this_00,0,x);
    return;
}



// DWARF original prototype: void vlong(vlong * this, uint x)

void __thiscall vlong::vlong(vlong *this,uint x)

{
    vlong_value *this_00;
    
    this->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    this_00 = (vlong_value *)operator_new(0x10);
    vlong_value::vlong_value(this_00);
    this->negative = 0;
    this->value = this_00;
    (this_00->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)this_00,0,x);
    return;
}



// DWARF original prototype: void vlong(vlong * this, vlong * x)

void __thiscall vlong::vlong(vlong *this,vlong *x)

{
    uint *puVar1;
    int iVar2;
    vlong_value *pvVar3;
    
    iVar2 = x->negative;
    this->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    this->negative = iVar2;
    pvVar3 = x->value;
    this->value = pvVar3;
    puVar1 = &pvVar3->share;
    *puVar1 = *puVar1 + 1;
    return;
}



// DWARF original prototype: void vlong(vlong * this, vlong * x)

void __thiscall vlong::vlong(vlong *this,vlong *x)

{
    uint *puVar1;
    int iVar2;
    vlong_value *pvVar3;
    
    iVar2 = x->negative;
    this->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    this->negative = iVar2;
    pvVar3 = x->value;
    this->value = pvVar3;
    puVar1 = &pvVar3->share;
    *puVar1 = *puVar1 + 1;
    return;
}



// DWARF original prototype: void ~vlong(vlong * this, int __in_chrg)

void __thiscall vlong::~vlong(vlong *this,int __in_chrg)

{
    vlong_value *pvVar1;
    uint *puVar2;
    uint i;
    uint uVar3;
    
    pvVar1 = this->value;
    this->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    if (pvVar1->share == 0) {
        if (pvVar1 != (vlong_value *)0x0) {
            uVar3 = (pvVar1->super_vlong_flex_unit).z;
            while (uVar3 != 0) {
                uVar3 = uVar3 - 1;
                (pvVar1->super_vlong_flex_unit).a[uVar3] = 0;
            }
            puVar2 = (pvVar1->super_vlong_flex_unit).a;
            if (puVar2 != (uint *)0x0) {
                operator_delete__(puVar2);
            }
            operator_delete(pvVar1);
        }
    }
    else {
        pvVar1->share = pvVar1->share - 1;
    }
    return;
}



// DWARF original prototype: void ~vlong(vlong * this, int __in_chrg)

void __thiscall vlong::~vlong(vlong *this,int __in_chrg)

{
    vlong_value *pvVar1;
    uint *puVar2;
    uint i;
    uint uVar3;
    
    pvVar1 = this->value;
    this->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    if (pvVar1->share == 0) {
        if (pvVar1 != (vlong_value *)0x0) {
            uVar3 = (pvVar1->super_vlong_flex_unit).z;
            while (uVar3 != 0) {
                uVar3 = uVar3 - 1;
                (pvVar1->super_vlong_flex_unit).a[uVar3] = 0;
            }
            puVar2 = (pvVar1->super_vlong_flex_unit).a;
            if (puVar2 != (uint *)0x0) {
                operator_delete__(puVar2);
            }
            operator_delete(pvVar1);
        }
    }
    else {
        pvVar1->share = pvVar1->share - 1;
    }
    return;
}



// DWARF original prototype: void ~vlong(vlong * this, int __in_chrg)

void __thiscall vlong::~vlong(vlong *this,int __in_chrg)

{
    vlong_value *pvVar1;
    uint *puVar2;
    uint i;
    uint uVar3;
    
    pvVar1 = this->value;
    this->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    if (pvVar1->share == 0) {
        if (pvVar1 != (vlong_value *)0x0) {
            uVar3 = (pvVar1->super_vlong_flex_unit).z;
            while (uVar3 != 0) {
                uVar3 = uVar3 - 1;
                (pvVar1->super_vlong_flex_unit).a[uVar3] = 0;
            }
            puVar2 = (pvVar1->super_vlong_flex_unit).a;
            if (puVar2 != (uint *)0x0) {
                operator_delete__(puVar2);
            }
            operator_delete(pvVar1);
        }
    }
    else {
        pvVar1->share = pvVar1->share - 1;
    }
    operator_delete(this);
    return;
}



// DWARF original prototype: void docopy(vlong * this)

void __thiscall vlong::docopy(vlong *this)

{
    uint uVar1;
    vlong_value *this_00;
    EVP_PKEY_CTX *in_stack_ffffffec;
    
    uVar1 = this->value->share;
    if (uVar1 != 0) {
        this->value->share = uVar1 - 1;
        this_00 = (vlong_value *)operator_new(0x10);
        vlong_value::vlong_value(this_00);
        vlong_value::copy(this_00,(EVP_PKEY_CTX *)this->value,in_stack_ffffffec);
        this->value = this_00;
    }
    return;
}



// DWARF original prototype: int cf(vlong * this, vlong * x)

int __thiscall vlong::cf(vlong *this,vlong *x)

{
    int iVar1;
    int iVar2;
    
    iVar2 = 0;
    if ((this->negative != 0) && ((this->value->super_vlong_flex_unit).n != 0)) {
        iVar2 = 1;
    }
    iVar1 = 0;
    if ((x->negative != 0) && ((x->value->super_vlong_flex_unit).n != 0)) {
        iVar1 = 1;
    }
    if (iVar2 != iVar1) {
        return (uint)(iVar2 == 0) * 2 + -1;
    }
    iVar2 = vlong_value::cf(this->value,x->value);
    return iVar2;
}



// DWARF original prototype: vlong * operator=(vlong * this, vlong * x)

vlong * __thiscall vlong::operator=(vlong *this,vlong *x)

{
    vlong_value *pvVar1;
    uint *puVar2;
    uint i;
    uint uVar3;
    
    pvVar1 = this->value;
    if (pvVar1->share == 0) {
        if (pvVar1 != (vlong_value *)0x0) {
            uVar3 = (pvVar1->super_vlong_flex_unit).z;
            while (uVar3 != 0) {
                uVar3 = uVar3 - 1;
                (pvVar1->super_vlong_flex_unit).a[uVar3] = 0;
            }
            puVar2 = (pvVar1->super_vlong_flex_unit).a;
            if (puVar2 != (uint *)0x0) {
                operator_delete__(puVar2);
            }
            operator_delete(pvVar1);
        }
    }
    else {
        pvVar1->share = pvVar1->share - 1;
    }
    pvVar1 = x->value;
    this->value = pvVar1;
    puVar2 = &pvVar1->share;
    *puVar2 = *puVar2 + 1;
    this->negative = x->negative;
    return this;
}



// DWARF original name: operator unsigned int
// DWARF original prototype: uint operator_unsigned_int(vlong * this)

uint __thiscall vlong::operator_unsigned_int(vlong *this)

{
    uint uVar1;
    
    if ((this->value->super_vlong_flex_unit).n == 0) {
        uVar1 = 0;
    }
    else {
        uVar1 = *(this->value->super_vlong_flex_unit).a;
    }
    return uVar1;
}



// DWARF original prototype: vlong * operator+=(vlong * this, vlong * x)

vlong * __thiscall vlong::operator+=(vlong *this,vlong *x)

{
    int iVar1;
    vlong_value *pvVar2;
    vlong *x_00;
    EVP_PKEY_CTX *in_stack_ffffffcc;
    undefined1 local_2c [4];
    vlong tmp;
    
    if (this->negative == x->negative) {
        pvVar2 = this->value;
        if (pvVar2->share != 0) {
            pvVar2->share = pvVar2->share - 1;
            pvVar2 = (vlong_value *)operator_new(0x10);
            vlong_value::vlong_value(pvVar2);
            vlong_value::copy(pvVar2,(EVP_PKEY_CTX *)this->value,in_stack_ffffffcc);
            this->value = pvVar2;
        }
        vlong_value::add(pvVar2,x->value);
    }
    else {
        iVar1 = vlong_value::cf(this->value,x->value);
        if (iVar1 < 0) {
            local_2c = (undefined1  [4])&PTR__vlong_08128410;
            tmp.value = (vlong_value *)this->negative;
            tmp._vptr_vlong = (_func_int_varargs **)this->value;
            ((vlong_value *)tmp._vptr_vlong)->share = ((vlong_value *)tmp._vptr_vlong)->share + 1;
            operator=(this,x);
            x_00 = (vlong *)local_2c;
                    // try { // try from 080ecedc to 080ecee0 has its CatchHandler @ 080ecf40
            operator+=(this,x_00);
            ~vlong((vlong *)local_2c,(int)x_00);
        }
        else {
            pvVar2 = this->value;
            if (pvVar2->share != 0) {
                pvVar2->share = pvVar2->share - 1;
                pvVar2 = (vlong_value *)operator_new(0x10);
                vlong_value::vlong_value(pvVar2);
                vlong_value::copy(pvVar2,(EVP_PKEY_CTX *)this->value,in_stack_ffffffcc);
                this->value = pvVar2;
            }
            vlong_value::subtract(pvVar2,x->value);
        }
    }
    return this;
}



// DWARF original prototype: vlong * operator-=(vlong * this, vlong * x)

vlong * __thiscall vlong::operator-=(vlong *this,vlong *x)

{
    vlong_value *pvVar1;
    int iVar2;
    vlong *x_00;
    EVP_PKEY_CTX *in_stack_ffffffcc;
    undefined1 local_2c [4];
    vlong tmp;
    
    if (this->negative == x->negative) {
        iVar2 = vlong_value::cf(this->value,x->value);
        if (iVar2 < 0) {
            local_2c = (undefined1  [4])&PTR__vlong_08128410;
            tmp.value = (vlong_value *)this->negative;
            tmp._vptr_vlong = (_func_int_varargs **)this->value;
            ((vlong_value *)tmp._vptr_vlong)->share = ((vlong_value *)tmp._vptr_vlong)->share + 1;
            operator=(this,x);
            x_00 = (vlong *)local_2c;
                    // try { // try from 080ed062 to 080ed066 has its CatchHandler @ 080ed084
            operator-=(this,x_00);
            this->negative = 1 - this->negative;
            ~vlong((vlong *)local_2c,(int)x_00);
        }
        else {
            pvVar1 = this->value;
            if (pvVar1->share != 0) {
                pvVar1->share = pvVar1->share - 1;
                pvVar1 = (vlong_value *)operator_new(0x10);
                vlong_value::vlong_value(pvVar1);
                vlong_value::copy(pvVar1,(EVP_PKEY_CTX *)this->value,in_stack_ffffffcc);
                this->value = pvVar1;
            }
            vlong_value::subtract(pvVar1,x->value);
        }
    }
    else {
        pvVar1 = this->value;
        if (pvVar1->share != 0) {
            pvVar1->share = pvVar1->share - 1;
            pvVar1 = (vlong_value *)operator_new(0x10);
            vlong_value::vlong_value(pvVar1);
            vlong_value::copy(pvVar1,(EVP_PKEY_CTX *)this->value,in_stack_ffffffcc);
            this->value = pvVar1;
        }
        vlong_value::add(pvVar1,x->value);
    }
    return this;
}



vlong * operator+(vlong *x,vlong *y)

{
    uint *puVar1;
    int iVar2;
    vlong_value *pvVar3;
    vlong *result;
    vlong *in_stack_0000000c;
    
    iVar2 = y->negative;
    x->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    x->negative = iVar2;
    pvVar3 = y->value;
    x->value = pvVar3;
    puVar1 = &pvVar3->share;
    *puVar1 = *puVar1 + 1;
                    // try { // try from 080ed0d1 to 080ed0d5 has its CatchHandler @ 080ed0e4
    vlong::operator+=(x,in_stack_0000000c);
    return x;
}



vlong * operator-(vlong *x,vlong *y)

{
    uint *puVar1;
    int iVar2;
    vlong_value *pvVar3;
    vlong *result;
    vlong *in_stack_0000000c;
    
    iVar2 = y->negative;
    x->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    x->negative = iVar2;
    pvVar3 = y->value;
    x->value = pvVar3;
    puVar1 = &pvVar3->share;
    *puVar1 = *puVar1 + 1;
                    // try { // try from 080ed131 to 080ed135 has its CatchHandler @ 080ed144
    vlong::operator-=(x,in_stack_0000000c);
    return x;
}



vlong * operator*(vlong *x,vlong *y)

{
    vlong_value *this;
    vlong_value *this_00;
    vlong_value *pvVar1;
    uint uVar2;
    uint uVar3;
    vlong *result;
    int in_stack_0000000c;
    
    x->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    pvVar1 = (vlong_value *)operator_new(0x10);
    vlong_value::vlong_value(pvVar1);
    x->negative = 0;
    x->value = pvVar1;
    (pvVar1->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar1,0,0);
    pvVar1 = x->value;
    this = y->value;
    this_00 = *(vlong_value **)(in_stack_0000000c + 4);
    uVar2 = vlong_value::bits(this);
    uVar3 = vlong_value::bits(this_00);
                    // try { // try from 080ed1ef to 080ed1f3 has its CatchHandler @ 080ed211
    vlong_flex_unit::fast_mul
              (&pvVar1->super_vlong_flex_unit,&this->super_vlong_flex_unit,
               &this_00->super_vlong_flex_unit,uVar3 + uVar2);
    x->negative = *(uint *)(in_stack_0000000c + 8) ^ y->negative;
    return x;
}



vlong * operator/(vlong *x,vlong *y)

{
    vlong_value *this;
    uint i;
    vlong *result;
    int in_stack_0000000c;
    undefined1 local_2c [4];
    vlong_value rem;
    
    x->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    this = (vlong_value *)operator_new(0x10);
    vlong_value::vlong_value(this);
    x->negative = 0;
    x->value = this;
    (this->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)this,0,0);
    vlong_value::vlong_value((vlong_value *)local_2c);
                    // try { // try from 080ed2a9 to 080ed2ad has its CatchHandler @ 080ed300
    vlong_value::divide(x->value,y->value,*(vlong_value **)(in_stack_0000000c + 4),
                        (vlong_value *)local_2c);
    x->negative = *(uint *)(in_stack_0000000c + 8) ^ y->negative;
    while (rem.super_vlong_flex_unit.a != (uint *)0x0) {
        rem.super_vlong_flex_unit.a = (uint *)((int)rem.super_vlong_flex_unit.a + -1);
        *(undefined4 *)(rem.super_vlong_flex_unit.n + (int)rem.super_vlong_flex_unit.a * 4) = 0;
    }
    if (rem.super_vlong_flex_unit.n != 0) {
        operator_delete__((void *)rem.super_vlong_flex_unit.n);
    }
    return x;
}



vlong * operator%(vlong *x,vlong *y)

{
    vlong_value *this;
    uint i;
    vlong *result;
    int in_stack_0000000c;
    undefined1 local_2c [4];
    vlong_value divide;
    
    x->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
    this = (vlong_value *)operator_new(0x10);
    vlong_value::vlong_value(this);
    x->negative = 0;
    x->value = this;
    (this->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)this,0,0);
    vlong_value::vlong_value((vlong_value *)local_2c);
                    // try { // try from 080ed3b9 to 080ed3bd has its CatchHandler @ 080ed400
    vlong_value::divide((vlong_value *)local_2c,y->value,*(vlong_value **)(in_stack_0000000c + 4),
                        x->value);
    x->negative = y->negative;
    while (divide.super_vlong_flex_unit.a != (uint *)0x0) {
        divide.super_vlong_flex_unit.a = (uint *)((int)divide.super_vlong_flex_unit.a + -1);
        *(undefined4 *)(divide.super_vlong_flex_unit.n + (int)divide.super_vlong_flex_unit.a * 4) =
             0;
    }
    if (divide.super_vlong_flex_unit.n != 0) {
        operator_delete__((void *)divide.super_vlong_flex_unit.n);
    }
    return x;
}



vlong * vlong::modexp(vlong *__return_storage_ptr__)

{
    undefined4 in_stack_00000008;
    vlong *in_stack_00000010;
    vlong_montgomery *__in_chrg;
    undefined1 local_5c [4];
    vlong_montgomery me;
    
    vlong_montgomery::vlong_montgomery((vlong_montgomery *)local_5c,in_stack_00000010);
    __in_chrg = (vlong_montgomery *)local_5c;
                    // try { // try from 080ed472 to 080ed476 has its CatchHandler @ 080ed4c4
    vlong_montgomery::exp
              ((vlong_montgomery *)__return_storage_ptr__,
               (double)CONCAT44(in_stack_00000008,(vlong_montgomery *)local_5c));
    ~vlong((vlong *)&me.T.negative,(int)__in_chrg);
    ~vlong((vlong *)&me.n1.negative,(int)__in_chrg);
    ~vlong((vlong *)&me.m.negative,(int)__in_chrg);
    ~vlong((vlong *)&me.R1.negative,(int)__in_chrg);
    ~vlong((vlong *)&me.R.negative,(int)__in_chrg);
    ~vlong((vlong *)local_5c,(int)__in_chrg);
    return __return_storage_ptr__;
}



vlong * __thiscall vlong::gcd(vlong *this,vlong *X,vlong *Y)

{
    vlong_value *pvVar1;
    int iVar2;
    vlong *pvVar3;
    vlong local_4c;
    undefined1 local_3c [4];
    vlong y;
    undefined1 local_2c [4];
    vlong x;
    
    local_2c = (undefined1  [4])&PTR__vlong_08128410;
    x.value = (vlong_value *)X->negative;
    x._vptr_vlong = (_func_int_varargs **)X->value;
    ((vlong_value *)x._vptr_vlong)->share = ((vlong_value *)x._vptr_vlong)->share + 1;
    local_3c = (undefined1  [4])&PTR__vlong_08128410;
    y.value = (vlong_value *)Y->negative;
    y._vptr_vlong = (_func_int_varargs **)Y->value;
    ((vlong_value *)y._vptr_vlong)->share = ((vlong_value *)y._vptr_vlong)->share + 1;
    do {
        local_4c._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080ed55f to 080ed65f has its CatchHandler @ 080ed6c8
        pvVar1 = (vlong_value *)operator_new(0x10);
        vlong_value::vlong_value(pvVar1);
        local_4c.negative = 0;
        (pvVar1->super_vlong_flex_unit).n = 0;
        local_4c.value = pvVar1;
        vlong_flex_unit::set((vlong_flex_unit *)pvVar1,0,0);
        pvVar3 = &local_4c;
        iVar2 = cf((vlong *)local_3c,&local_4c);
        ~vlong(&local_4c,(int)pvVar3);
        if (iVar2 == 0) {
            this->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
            this->negative = (int)x.value;
            pvVar1 = (vlong_value *)x._vptr_vlong;
LAB_080ed68e:
            this->value = pvVar1;
            pvVar1->share = pvVar1->share + 1;
            ~vlong((vlong *)local_3c,(int)pvVar3);
            ~vlong((vlong *)local_2c,(int)pvVar3);
            return this;
        }
        operator%(&local_4c,(vlong *)local_2c);
        pvVar3 = &local_4c;
        operator=((vlong *)local_2c,&local_4c);
        ~vlong(&local_4c,(int)pvVar3);
        local_4c._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
        pvVar1 = (vlong_value *)operator_new(0x10);
        vlong_value::vlong_value(pvVar1);
        local_4c.negative = 0;
        (pvVar1->super_vlong_flex_unit).n = 0;
        local_4c.value = pvVar1;
        vlong_flex_unit::set((vlong_flex_unit *)pvVar1,0,0);
        pvVar3 = &local_4c;
        iVar2 = cf((vlong *)local_2c,&local_4c);
        ~vlong(&local_4c,(int)pvVar3);
        if (iVar2 == 0) {
            this->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
            this->negative = (int)y.value;
            pvVar1 = (vlong_value *)y._vptr_vlong;
            goto LAB_080ed68e;
        }
        operator%(&local_4c,(vlong *)local_3c);
        pvVar3 = &local_4c;
        operator=((vlong *)local_3c,&local_4c);
        ~vlong(&local_4c,(int)pvVar3);
    } while( true );
}



vlong * __thiscall vlong::modinv(vlong *this,vlong *a,vlong *m)

{
    vlong_value *pvVar1;
    int iVar2;
    vlong *i;
    vlong *pvVar3;
    undefined1 local_8c [4];
    vlong result_1;
    undefined1 local_7c [4];
    vlong result;
    undefined1 local_6c [4];
    vlong y;
    undefined1 local_5c [4];
    vlong x;
    undefined1 local_4c [4];
    vlong c;
    undefined1 local_3c [4];
    vlong b;
    undefined1 local_2c [4];
    vlong j;
    
    local_2c = (undefined1  [4])&PTR__vlong_08128410;
    pvVar1 = (vlong_value *)operator_new(0x10);
    vlong_value::vlong_value(pvVar1);
    j.value = (vlong_value *)0x0;
    (pvVar1->super_vlong_flex_unit).n = 0;
    j._vptr_vlong = (_func_int_varargs **)pvVar1;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar1,0,1);
    this->_vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    // try { // try from 080ed754 to 080ed78a has its CatchHandler @ 080edb77
    pvVar1 = (vlong_value *)operator_new(0x10);
    vlong_value::vlong_value(pvVar1);
    this->negative = 0;
    this->value = pvVar1;
    (pvVar1->super_vlong_flex_unit).n = 0;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar1,0,0);
    local_3c = (undefined1  [4])&PTR__vlong_08128410;
    b.value = (vlong_value *)m->negative;
    b._vptr_vlong = (_func_int_varargs **)m->value;
    ((vlong_value *)b._vptr_vlong)->share = ((vlong_value *)b._vptr_vlong)->share + 1;
    local_4c = (undefined1  [4])&PTR__vlong_08128410;
    c.value = (vlong_value *)a->negative;
    c._vptr_vlong = (_func_int_varargs **)a->value;
    ((vlong_value *)c._vptr_vlong)->share = ((vlong_value *)c._vptr_vlong)->share + 1;
    local_5c = (undefined1  [4])&PTR__vlong_08128410;
                    // try { // try from 080ed7c8 to 080ed7fe has its CatchHandler @ 080edb70
    pvVar1 = (vlong_value *)operator_new(0x10);
    vlong_value::vlong_value(pvVar1);
    x.value = (vlong_value *)0x0;
    (pvVar1->super_vlong_flex_unit).n = 0;
    x._vptr_vlong = (_func_int_varargs **)pvVar1;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar1,0,0);
    local_6c = (undefined1  [4])&PTR__vlong_08128410;
                    // try { // try from 080ed80d to 080ed843 has its CatchHandler @ 080edb64
    pvVar1 = (vlong_value *)operator_new(0x10);
    vlong_value::vlong_value(pvVar1);
    y.value = (vlong_value *)0x0;
    (pvVar1->super_vlong_flex_unit).n = 0;
    y._vptr_vlong = (_func_int_varargs **)pvVar1;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar1,0,0);
    while( true ) {
        local_7c = (undefined1  [4])&PTR__vlong_08128410;
                    // try { // try from 080ed855 to 080ed90c has its CatchHandler @ 080edb60
        pvVar1 = (vlong_value *)operator_new(0x10);
        vlong_value::vlong_value(pvVar1);
        result.value = (vlong_value *)0x0;
        (pvVar1->super_vlong_flex_unit).n = 0;
        result._vptr_vlong = (_func_int_varargs **)pvVar1;
        vlong_flex_unit::set((vlong_flex_unit *)pvVar1,0,0);
        pvVar3 = (vlong *)local_7c;
        iVar2 = cf((vlong *)local_4c,pvVar3);
        ~vlong((vlong *)local_7c,(int)pvVar3);
        if (iVar2 == 0) break;
        operator/((vlong *)local_7c,(vlong *)local_3c);
        pvVar3 = (vlong *)local_7c;
        operator=((vlong *)local_5c,pvVar3);
        ~vlong((vlong *)local_7c,(int)pvVar3);
        operator*((vlong *)local_8c,(vlong *)local_5c);
        local_7c = (undefined1  [4])&PTR__vlong_08128410;
        result.value = b.value;
        result._vptr_vlong = b._vptr_vlong;
        b._vptr_vlong[3] = b._vptr_vlong[3] + 1;
                    // try { // try from 080ed936 to 080ed93a has its CatchHandler @ 080edaf0
        operator-=((vlong *)local_7c,(vlong *)local_8c);
        pvVar3 = (vlong *)local_7c;
        operator=((vlong *)local_6c,pvVar3);
        ~vlong((vlong *)local_7c,(int)pvVar3);
        ~vlong((vlong *)local_8c,(int)pvVar3);
        operator=((vlong *)local_3c,(vlong *)local_4c);
        operator=((vlong *)local_4c,(vlong *)local_6c);
        operator=((vlong *)local_6c,(vlong *)local_2c);
                    // try { // try from 080ed9aa to 080ed9ae has its CatchHandler @ 080edb60
        operator*((vlong *)local_7c,(vlong *)local_2c);
        local_8c = (undefined1  [4])&PTR__vlong_08128410;
        result_1.value = (vlong_value *)this->negative;
        result_1._vptr_vlong = (_func_int_varargs **)this->value;
        ((vlong_value *)result_1._vptr_vlong)->share =
             ((vlong_value *)result_1._vptr_vlong)->share + 1;
                    // try { // try from 080ed9df to 080ed9e3 has its CatchHandler @ 080edb41
        operator-=((vlong *)local_8c,(vlong *)local_7c);
        pvVar3 = (vlong *)local_8c;
        operator=((vlong *)local_2c,pvVar3);
        ~vlong((vlong *)local_8c,(int)pvVar3);
        ~vlong((vlong *)local_7c,(int)pvVar3);
        operator=(this,(vlong *)local_6c);
    }
    local_8c = (undefined1  [4])&PTR__vlong_08128410;
                    // try { // try from 080eda35 to 080edae6 has its CatchHandler @ 080edb60
    pvVar1 = (vlong_value *)operator_new(0x10);
    vlong_value::vlong_value(pvVar1);
    result_1.value = (vlong_value *)0x0;
    (pvVar1->super_vlong_flex_unit).n = 0;
    result_1._vptr_vlong = (_func_int_varargs **)pvVar1;
    vlong_flex_unit::set((vlong_flex_unit *)pvVar1,0,0);
    pvVar3 = (vlong *)local_8c;
    iVar2 = cf(this,pvVar3);
    ~vlong((vlong *)local_8c,(int)pvVar3);
    if (iVar2 < 0) {
        operator+=(this,m);
        pvVar3 = m;
    }
    ~vlong((vlong *)local_6c,(int)pvVar3);
    ~vlong((vlong *)local_5c,(int)pvVar3);
    ~vlong((vlong *)local_4c,(int)pvVar3);
    ~vlong((vlong *)local_3c,(int)pvVar3);
    ~vlong((vlong *)local_2c,(int)pvVar3);
    return this;
}



void vlong::convert(char *pDecimal,vlong *Number)

{
    char cVar1;
    ushort uVar2;
    vlong_value *pvVar3;
    uint x;
    undefined4 *puVar4;
    vlong *pvVar5;
    undefined2 local_4e;
    undefined1 auStack_4c [2];
    char Buffer [2];
    vlong local_3c;
    undefined1 local_2c [4];
    vlong result;
    
    if (pDecimal == (char *)0x0) {
        puVar4 = (undefined4 *)__cxa_allocate_exception(4);
        *puVar4 = "pDecimal pointer is NULL";
    }
    else {
        local_2c = (undefined1  [4])&PTR__vlong_08128410;
        local_4e = 0x100;
        pvVar3 = (vlong_value *)operator_new(0x10);
        vlong_value::vlong_value(pvVar3);
        result.value = (vlong_value *)0x0;
        (pvVar3->super_vlong_flex_unit).n = 0;
        result._vptr_vlong = (_func_int_varargs **)pvVar3;
        vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,0);
        pvVar5 = (vlong *)local_2c;
        operator=(Number,(vlong *)local_2c);
        ~vlong((vlong *)local_2c,(int)pvVar5);
        cVar1 = *pDecimal;
        while( true ) {
            if (cVar1 == '\0') {
                return;
            }
            uVar2 = (ushort)local_4e >> 8;
            local_4e = CONCAT11((char)uVar2,cVar1);
            x = __strtol_internal(&local_4e,0,10,0);
            if (9 < x) break;
            _auStack_4c = &PTR__vlong_08128410;
            pvVar3 = (vlong_value *)operator_new(0x10);
            vlong_value::vlong_value(pvVar3);
            (pvVar3->super_vlong_flex_unit).n = 0;
            vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,10);
            pvVar5 = Number;
                    // try { // try from 080edcae to 080edcb2 has its CatchHandler @ 080edda6
            operator*(&local_3c,Number);
            ~vlong((vlong *)auStack_4c,(int)pvVar5);
            _auStack_4c = &PTR__vlong_08128410;
                    // try { // try from 080edccf to 080edd04 has its CatchHandler @ 080edde3
            pvVar3 = (vlong_value *)operator_new(0x10);
            vlong_value::vlong_value(pvVar3);
            (pvVar3->super_vlong_flex_unit).n = 0;
            vlong_flex_unit::set((vlong_flex_unit *)pvVar3,0,x);
            local_2c = (undefined1  [4])&PTR__vlong_08128410;
            pvVar5 = (vlong *)auStack_4c;
            result.value = (vlong_value *)local_3c.negative;
            result._vptr_vlong = (_func_int_varargs **)local_3c.value;
            (local_3c.value)->share = (local_3c.value)->share + 1;
                    // try { // try from 080edd25 to 080edd29 has its CatchHandler @ 080eddad
            operator+=((vlong *)local_2c,pvVar5);
            pDecimal = pDecimal + 1;
            ~vlong((vlong *)auStack_4c,(int)pvVar5);
            pvVar5 = (vlong *)local_2c;
            operator=(Number,(vlong *)local_2c);
            ~vlong((vlong *)local_2c,(int)pvVar5);
            ~vlong(&local_3c,(int)pvVar5);
            cVar1 = *pDecimal;
        }
        puVar4 = (undefined4 *)__cxa_allocate_exception(4);
        *puVar4 = "digit out of range";
    }
                    // WARNING: Subroutine does not return
    __cxa_throw(puVar4,char_const*::typeinfo,0);
}



void vlong::convert(vlong *Number,char *pDecimal,ushort DecimalLength)

{
    char cVar1;
    vlong_value *pvVar2;
    int iVar3;
    undefined4 *puVar4;
    vlong *pvVar5;
    uint local_48;
    int local_44;
    int i;
    undefined1 auStack_3c [2];
    char Buffer [2];
    vlong local_2c [2];
    
    if (pDecimal == (char *)0x0) {
        puVar4 = (undefined4 *)__cxa_allocate_exception(4);
        *puVar4 = "pDecimal pointer is NULL";
    }
    else {
        if (DecimalLength != 0) {
            memset(pDecimal,0,(uint)DecimalLength);
            local_2c[0]._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
            pvVar2 = (vlong_value *)operator_new(0x10);
            vlong_value::vlong_value(pvVar2);
            local_2c[0].negative = 0;
            (pvVar2->super_vlong_flex_unit).n = 0;
            local_2c[0].value = pvVar2;
            vlong_flex_unit::set((vlong_flex_unit *)pvVar2,0,0);
            pvVar5 = local_2c;
            iVar3 = cf(Number,local_2c);
            ~vlong(local_2c,(int)pvVar5);
            if (iVar3 == 0) {
                *pDecimal = '0';
            }
            else {
                local_44 = 0;
                i._2_2_ = 0x100;
                while( true ) {
                    local_2c[0]._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    pvVar2 = (vlong_value *)operator_new(0x10);
                    vlong_value::vlong_value(pvVar2);
                    local_2c[0].negative = 0;
                    (pvVar2->super_vlong_flex_unit).n = 0;
                    local_2c[0].value = pvVar2;
                    vlong_flex_unit::set((vlong_flex_unit *)pvVar2,0,0);
                    pvVar5 = local_2c;
                    iVar3 = cf(Number,local_2c);
                    ~vlong(local_2c,(int)pvVar5);
                    if (iVar3 < 1) break;
                    if ((int)(uint)DecimalLength <= local_44) {
                        puVar4 = (undefined4 *)__cxa_allocate_exception(4);
                        *puVar4 = 
                        "large number does not fit in char array (DecimalLength too small)";
                        goto LAB_080ee083;
                    }
                    _auStack_3c = &PTR__vlong_08128410;
                    pvVar2 = (vlong_value *)operator_new(0x10);
                    vlong_value::vlong_value(pvVar2);
                    (pvVar2->super_vlong_flex_unit).n = 0;
                    vlong_flex_unit::set((vlong_flex_unit *)pvVar2,0,10);
                    pvVar5 = Number;
                    // try { // try from 080edf82 to 080edf86 has its CatchHandler @ 080ee0f0
                    operator%(local_2c,Number);
                    ~vlong((vlong *)auStack_3c,(int)pvVar5);
                    if (((local_2c[0].value)->super_vlong_flex_unit).n == 0) {
                        local_48 = 0;
                    }
                    else {
                        local_48 = *((local_2c[0].value)->super_vlong_flex_unit).a;
                    }
                    ~vlong(local_2c,(int)pvVar5);
                    local_2c[0]._vptr_vlong = (_func_int_varargs **)&PTR__vlong_08128410;
                    pvVar2 = (vlong_value *)operator_new(0x10);
                    vlong_value::vlong_value(pvVar2);
                    local_2c[0].negative = 0;
                    (pvVar2->super_vlong_flex_unit).n = 0;
                    local_2c[0].value = pvVar2;
                    vlong_flex_unit::set((vlong_flex_unit *)pvVar2,0,10);
                    pvVar5 = Number;
                    // try { // try from 080ee007 to 080ee00b has its CatchHandler @ 080ee100
                    operator/((vlong *)auStack_3c,Number);
                    ~vlong(local_2c,(int)pvVar5);
                    pvVar5 = (vlong *)auStack_3c;
                    operator=(Number,pvVar5);
                    ~vlong((vlong *)auStack_3c,(int)pvVar5);
                    sprintf((char *)((int)&i + 2),"%d",local_48);
                    pDecimal[local_44] = i._2_1_;
                    local_44 = local_44 + 1;
                }
                local_44 = local_44 + -1;
                while (0 < local_44) {
                    cVar1 = *pDecimal;
                    *pDecimal = pDecimal[local_44];
                    pDecimal[local_44] = cVar1;
                }
            }
            return;
        }
        puVar4 = (undefined4 *)__cxa_allocate_exception(4);
        *puVar4 = "DecimalLength = 0";
    }
LAB_080ee083:
                    // WARNING: Subroutine does not return
    __cxa_throw(puVar4,char_const*::typeinfo,0);
}



void vlong::convert(uchar *pData,ushort DataLength,vlong *Number)

{
    undefined4 *puVar1;
    byte bVar2;
    int j;
    uint uVar3;
    int iVar4;
    int i;
    int iVar5;
    uint x;
    uint x_00;
    ushort DataLength_local;
    
    uVar3 = (uint)DataLength;
    if (pData == (uchar *)0x0) {
        puVar1 = (undefined4 *)__cxa_allocate_exception(4);
        *puVar1 = "pData pointer is NULL";
                    // WARNING: Subroutine does not return
        __cxa_throw(puVar1,char_const*::typeinfo,0);
    }
    iVar5 = 0;
    (Number->value->super_vlong_flex_unit).n = 0;
    if (uVar3 != 0) {
        do {
            x_00 = 0;
            iVar4 = 0;
            bVar2 = 0x18;
            do {
                if (iVar4 + iVar5 < (int)uVar3) {
                    x_00 = x_00 + ((uint)pData[iVar4 + iVar5] << (bVar2 & 0x1f));
                }
                iVar4 = iVar4 + 1;
                bVar2 = bVar2 - 8;
            } while (iVar4 < 4);
            iVar4 = iVar5 + 4;
            vlong_flex_unit::set
                      (&Number->value->super_vlong_flex_unit,((uVar3 + 3) / 4 - iVar5 / 4) - 1,x_00)
            ;
            iVar5 = iVar4;
        } while (iVar4 < (int)uVar3);
    }
    return;
}



void vlong::convert(vlong *Number,uchar *pData,ushort DataLength)

{
    undefined4 *puVar1;
    byte bVar2;
    uint __n;
    uint uVar3;
    int j;
    int iVar4;
    int i;
    int iVar5;
    ushort DataLength_local;
    uint local_1c;
    
    __n = (uint)DataLength;
    if (pData != (uchar *)0x0) {
        memset(pData,0,__n);
        if (__n != 0) {
            iVar5 = 0;
            do {
                uVar3 = ((__n + 3) / 4 - iVar5 / 4) - 1;
                if (uVar3 < (Number->value->super_vlong_flex_unit).n) {
                    local_1c = (Number->value->super_vlong_flex_unit).a[uVar3];
                }
                else {
                    local_1c = 0;
                }
                iVar4 = 0;
                bVar2 = 0x18;
                do {
                    if (iVar4 + iVar5 < (int)__n) {
                        pData[iVar4 + iVar5] = (uchar)(local_1c >> (bVar2 & 0x1f));
                    }
                    iVar4 = iVar4 + 1;
                    bVar2 = bVar2 - 8;
                } while (iVar4 < 4);
                iVar5 = iVar5 + 4;
            } while (iVar5 < (int)__n);
        }
        return;
    }
    puVar1 = (undefined4 *)__cxa_allocate_exception(4);
    *puVar1 = "pData pointer is NULL";
                    // WARNING: Subroutine does not return
    __cxa_throw(puVar1,char_const*::typeinfo,0);
}
