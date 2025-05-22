#ifndef TIBIA_SCRIPT_HH_
#define TIBIA_SCRIPT_HH_ 1

#include "common.hh"
#include "enums.hh"

struct TReadScriptFile {
	// REGULAR FUNCTIONS
	// =========================================================================
	TReadScriptFile(void);
	~TReadScriptFile(void);
	void open(const char *FileName);
	void close(void);
	void error(const char *Text);
	void nextToken(void);
	char *getIdentifier(void);
	int getNumber(void);
	char *getString(void);
	uint8 *getBytesequence(void);
	void getCoordinate(int *x, int *y, int *z);
	char getSpecial(void);

	char *readIdentifier(void){
		this->nextToken();
		return this->getIdentifier();
	}

	int readNumber(void){
		this->nextToken();

		int Sign = 1;
		if(this->Token == SPECIAL && this->Special == '-'){
			Sign = -1;
			this->nextToken();
		}

		return Sign * this->getNumber();
	}

	char *readString(void){
		this->nextToken();
		return this->getString();
	}

	uint8 *readBytesequence(void){
		this->nextToken();
		return this->getBytesequence();
	}

	void readCoordinate(int *x, int *y, int *z){
		this->nextToken();
		this->getCoordinate(x, y, z);
	}

	char readSpecial(void){
		this->nextToken();
		return this->getSpecial();
	}

	void TReadScriptFile::readSymbol(char Symbol){
		if(this->readSpecial() != Symbol){
			this->error("symbol mismatch");
		}
	}

	// DATA
	// =========================================================================
	TOKEN Token;
	FILE *File[3];
	char Filename[3][4096];
	int Line[3];
	char String[4000];
	int RecursionDepth;
	uint8 *Bytes;
	int Number;
	int CoordX;
	int CoordY;
	int CoordZ;
	char Special;
};

struct TWriteScriptFile {
	// REGULAR FUNCTIONS
	// =========================================================================
	TWriteScriptFile(void);
	~TWriteScriptFile(void);
	void open(char *FileName);
	void close(void);
	void error(char *Text);
	void writeLn(void);
	void writeText(char *Text);
	void writeNumber(int Number);
	void writeString(char *Text);
	void writeCoordinate(int x ,int y ,int z);
	void writeBytesequence(uint8 *Sequence, int Length);

	// DATA
	// =========================================================================
	FILE *File;
	char Filename[4096];
	int Line;
};

struct TReadBinaryFile: TReadStream {
	// REGULAR FUNCTIONS
	// =========================================================================
	TReadBinaryFile(void);
	void open(char *FileName);
	int close(void);
	void error(char *Text);
	int getPosition(void);
	int getSize(void);
	void seek(int Offset);

	// VIRTUAL FUNCTIONS
	// =========================================================================
	// TODO(fusion): `TReadStream` itself doesn't have a destructor on its VTABLE
	// which makes me think there is something else going on here or the compiler
	// optimized it away because it was never used but it seems problematic.
	virtual ~TReadBinaryFile(void) override;											// VTABLE[8]
	// Duplicate destructor that also calls operator delete.							// VTABLE[9]

	uint8 readByte(void) override;
	void readBytes(uint8 *Buffer, int Count) override;
	bool eof(TReadBinaryFile *this) override;
	void skip(int Count) override;

	// DATA
	// =========================================================================
	FILE *File;
	char Filename[4096];
	int FileSize;
};

#endif //TIBIA_SCRIPT_HH_
