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
