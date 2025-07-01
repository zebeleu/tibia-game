#ifndef TIBIA_READER_HH_
#define TIBIA_READER_HH_ 1

#include "common.hh"

enum TReaderThreadOrderType: int {
	READER_ORDER_TERMINATE		= 0,
	READER_ORDER_LOADSECTOR		= 1,
	READER_ORDER_LOADCHARACTER	= 2,
};

enum TReaderThreadReplyType: int {
	READER_REPLY_SECTORDATA		= 0,
	READER_REPLY_CHARACTERDATA	= 1,
};

#endif //TIBIA_READER_HH
