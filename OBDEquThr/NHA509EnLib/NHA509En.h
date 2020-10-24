#ifndef NHA509En_H
#define NHA509En_H

#ifdef __cplusplus
extern "C" {
#endif

BYTE GetAuthenticationKey(BYTE bTable, BYTE bSeed, LPCSTR szSignature);

#ifdef __cplusplus
}
#endif

#endif