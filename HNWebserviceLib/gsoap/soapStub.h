/* soapStub.h
   Generated by gSOAP 2.8.21 from DataInterface.h

Copyright(C) 2000-2014, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapStub_H
#define soapStub_H
#include <vector>
#define SOAP_NAMESPACE_OF_ns1	"http://tempuri.org/"
#include "stdsoap2.h"
#if GSOAP_VERSION != 20821
# error "GSOAP VERSION 20821 MISMATCH IN GENERATED CODE VERSUS LIBRARY CODE: PLEASE REINSTALL PACKAGE"
#endif


/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#ifndef SOAP_TYPE__ns1__Common
#define SOAP_TYPE__ns1__Common (8)
/* ns1:Common */
class SOAP_CMAC _ns1__Common
{
public:
	std::wstring *methodName;	/* optional element of type xsd:string */
	std::wstring *jsonParam;	/* optional element of type xsd:string */
	std::wstring *ResultData;	/* optional element of type xsd:string */
	std::wstring *SourceData;	/* optional element of type xsd:string */
	std::wstring *SourceDataMS;	/* optional element of type xsd:string */
	std::wstring *OBDSourceData;	/* optional element of type xsd:string */
	std::wstring *userName;	/* optional element of type xsd:string */
	std::wstring *passWord;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 8; } /* = unique type id SOAP_TYPE__ns1__Common */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__Common() { _ns1__Common::soap_default(NULL); }
	virtual ~_ns1__Common() { }
};
#endif

#ifndef SOAP_TYPE__ns1__CommonResponse
#define SOAP_TYPE__ns1__CommonResponse (9)
/* ns1:CommonResponse */
class SOAP_CMAC _ns1__CommonResponse
{
public:
	std::wstring *CommonResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 9; } /* = unique type id SOAP_TYPE__ns1__CommonResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__CommonResponse() { _ns1__CommonResponse::soap_default(NULL); }
	virtual ~_ns1__CommonResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__UploadAppearanceData
#define SOAP_TYPE__ns1__UploadAppearanceData (10)
/* ns1:UploadAppearanceData */
class SOAP_CMAC _ns1__UploadAppearanceData
{
public:
	std::wstring *jsonParam;	/* optional element of type xsd:string */
	std::wstring *userName;	/* optional element of type xsd:string */
	std::wstring *passWord;	/* optional element of type xsd:string */
	std::wstring *appearancePicture;	/* optional element of type xsd:string */
	std::wstring *pic2;	/* optional element of type xsd:string */
	std::wstring *pic3;	/* optional element of type xsd:string */
	std::wstring *pic4;	/* optional element of type xsd:string */
	std::wstring *pic5;	/* optional element of type xsd:string */
	std::wstring *pic6;	/* optional element of type xsd:string */
	std::wstring *pic7;	/* optional element of type xsd:string */
	std::wstring *pic8;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 10; } /* = unique type id SOAP_TYPE__ns1__UploadAppearanceData */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__UploadAppearanceData() { _ns1__UploadAppearanceData::soap_default(NULL); }
	virtual ~_ns1__UploadAppearanceData() { }
};
#endif

#ifndef SOAP_TYPE__ns1__UploadAppearanceDataResponse
#define SOAP_TYPE__ns1__UploadAppearanceDataResponse (11)
/* ns1:UploadAppearanceDataResponse */
class SOAP_CMAC _ns1__UploadAppearanceDataResponse
{
public:
	std::wstring *UploadAppearanceDataResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 11; } /* = unique type id SOAP_TYPE__ns1__UploadAppearanceDataResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__UploadAppearanceDataResponse() { _ns1__UploadAppearanceDataResponse::soap_default(NULL); }
	virtual ~_ns1__UploadAppearanceDataResponse() { }
};
#endif

#ifndef SOAP_TYPE___ns1__Common
#define SOAP_TYPE___ns1__Common (18)
/* Operation wrapper: */
struct __ns1__Common
{
public:
	_ns1__Common *ns1__Common;	/* optional element of type ns1:Common */
public:
	int soap_type() const { return 18; } /* = unique type id SOAP_TYPE___ns1__Common */
};
#endif

#ifndef SOAP_TYPE___ns1__UploadAppearanceData
#define SOAP_TYPE___ns1__UploadAppearanceData (22)
/* Operation wrapper: */
struct __ns1__UploadAppearanceData
{
public:
	_ns1__UploadAppearanceData *ns1__UploadAppearanceData;	/* optional element of type ns1:UploadAppearanceData */
public:
	int soap_type() const { return 22; } /* = unique type id SOAP_TYPE___ns1__UploadAppearanceData */
};
#endif

#ifndef SOAP_TYPE___ns1__Common_
#define SOAP_TYPE___ns1__Common_ (24)
/* Operation wrapper: */
struct __ns1__Common_
{
public:
	_ns1__Common *ns1__Common;	/* optional element of type ns1:Common */
public:
	int soap_type() const { return 24; } /* = unique type id SOAP_TYPE___ns1__Common_ */
};
#endif

#ifndef SOAP_TYPE___ns1__UploadAppearanceData_
#define SOAP_TYPE___ns1__UploadAppearanceData_ (26)
/* Operation wrapper: */
struct __ns1__UploadAppearanceData_
{
public:
	_ns1__UploadAppearanceData *ns1__UploadAppearanceData;	/* optional element of type ns1:UploadAppearanceData */
public:
	int soap_type() const { return 26; } /* = unique type id SOAP_TYPE___ns1__UploadAppearanceData_ */
};
#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (27)
/* SOAP Header: */
struct SOAP_ENV__Header
{
public:
	int soap_type() const { return 27; } /* = unique type id SOAP_TYPE_SOAP_ENV__Header */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (28)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
public:
	char *SOAP_ENV__Value;	/* optional element of type xsd:QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
public:
	int soap_type() const { return 28; } /* = unique type id SOAP_TYPE_SOAP_ENV__Code */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (30)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
public:
	char *__any;
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
public:
	int soap_type() const { return 30; } /* = unique type id SOAP_TYPE_SOAP_ENV__Detail */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (33)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
public:
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
public:
	int soap_type() const { return 33; } /* = unique type id SOAP_TYPE_SOAP_ENV__Reason */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (34)
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
public:
	char *faultcode;	/* optional element of type xsd:QName */
	char *faultstring;	/* optional element of type xsd:string */
	char *faultactor;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *detail;	/* optional element of type SOAP-ENV:Detail */
	struct SOAP_ENV__Code *SOAP_ENV__Code;	/* optional element of type SOAP-ENV:Code */
	struct SOAP_ENV__Reason *SOAP_ENV__Reason;	/* optional element of type SOAP-ENV:Reason */
	char *SOAP_ENV__Node;	/* optional element of type xsd:string */
	char *SOAP_ENV__Role;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *SOAP_ENV__Detail;	/* optional element of type SOAP-ENV:Detail */
public:
	int soap_type() const { return 34; } /* = unique type id SOAP_TYPE_SOAP_ENV__Fault */
};
#endif

#endif

/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
typedef char *_QName;
#endif

#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (6)
typedef char *_XML;
#endif


/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


#endif

/* End of soapStub.h */
