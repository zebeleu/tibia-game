#ifndef TIBIA_CRYPTO_HH_
#define TIBIA_CRYPTO_HH_ 1

#include "common.hh"

// TODO(fusion): We might want to scrap this implementation and use OpenSSL instead.

struct vlong_flex_unit{
    uint32 n;
    uint32 *a;
    uint32 z;
};

struct vlong_value: vlong_flex_unit {
    uint32 share;
};

struct vlong{
    vlong_value *value;
    int negative;
};

struct vlong_montgomery{
    vlong R;
    vlong R1;
    vlong m;
    vlong n1;
    vlong T;
    vlong k;
    uint32 N;
};

struct TRSAPrivateKey{
	TRSAPrivateKey(void);
	~TRSAPrivateKey(void);
	void decrypt(uint8 *Data); // single 128 bytes block

	// DATA
	// =================
    vlong m_PrimeP;
    vlong m_PrimeQ;
    vlong m_U;
    vlong m_DP;
    vlong m_DQ;
};

struct TXTEASymmetricKey{
	TXTEASymmetricKey(void);
	~TXTEASymmetricKey(void);
	void encrypt(uint8 *Data); // single 8 bytes block
	void decrypt(uint8 *Data); // single 8 bytes block

	// DATA
	// =================
	uint32 m_SymmetricKey[4];
};

#endif //TIBIA_CRYPTO_HH_
