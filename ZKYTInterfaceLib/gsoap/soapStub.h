/* soapStub.h
   Generated by gSOAP 2.8.21 from deviceSwap.h

Copyright(C) 2000-2014, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapStub_H
#define soapStub_H
#include <vector>
#define SOAP_NAMESPACE_OF_ns1	"http://iface.server.webservice.zkyt.com/"
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

#ifndef SOAP_TYPE_ns1__getSqm
#define SOAP_TYPE_ns1__getSqm (8)
/* ns1:getSqm */
class SOAP_CMAC ns1__getSqm
{
public:
	std::wstring *jcxbh;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 8; } /* = unique type id SOAP_TYPE_ns1__getSqm */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         ns1__getSqm() { ns1__getSqm::soap_default(NULL); }
	virtual ~ns1__getSqm() { }
};
#endif

#ifndef SOAP_TYPE_ns1__getSqmResponse
#define SOAP_TYPE_ns1__getSqmResponse (9)
/* ns1:getSqmResponse */
class SOAP_CMAC ns1__getSqmResponse
{
public:
	std::wstring *return_;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 9; } /* = unique type id SOAP_TYPE_ns1__getSqmResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         ns1__getSqmResponse() { ns1__getSqmResponse::soap_default(NULL); }
	virtual ~ns1__getSqmResponse() { }
};
#endif

#ifndef SOAP_TYPE_ns1__jdcjk
#define SOAP_TYPE_ns1__jdcjk (10)
/* ns1:jdcjk */
class SOAP_CMAC ns1__jdcjk
{
public:
	std::wstring *sqm;	/* optional element of type xsd:string */
	std::wstring *jklx;	/* optional element of type xsd:string */
	std::wstring *jhsj;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 10; } /* = unique type id SOAP_TYPE_ns1__jdcjk */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         ns1__jdcjk() { ns1__jdcjk::soap_default(NULL); }
	virtual ~ns1__jdcjk() { }
};
#endif

#ifndef SOAP_TYPE_ns1__jdcjkResponse
#define SOAP_TYPE_ns1__jdcjkResponse (11)
/* ns1:jdcjkResponse */
class SOAP_CMAC ns1__jdcjkResponse
{
public:
	std::wstring *return_;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 11; } /* = unique type id SOAP_TYPE_ns1__jdcjkResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         ns1__jdcjkResponse() { ns1__jdcjkResponse::soap_default(NULL); }
	virtual ~ns1__jdcjkResponse() { }
};
#endif

#ifndef SOAP_TYPE___ns1__getSqm
#define SOAP_TYPE___ns1__getSqm (18)
/* Operation wrapper: */
struct __ns1__getSqm
{
public:
	ns1__getSqm *ns1__getSqm_;	/* optional element of type ns1:getSqm */
public:
	int soap_type() const { return 18; } /* = unique type id SOAP_TYPE___ns1__getSqm */
};
#endif

#ifndef SOAP_TYPE___ns1__jdcjk
#define SOAP_TYPE___ns1__jdcjk (22)
/* Operation wrapper: */
struct __ns1__jdcjk
{
public:
	ns1__jdcjk *ns1__jdcjk_;	/* optional element of type ns1:jdcjk */
public:
	int soap_type() const { return 22; } /* = unique type id SOAP_TYPE___ns1__jdcjk */
};
#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (23)
/* SOAP Header: */
struct SOAP_ENV__Header
{
public:
	int soap_type() const { return 23; } /* = unique type id SOAP_TYPE_SOAP_ENV__Header */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (24)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
public:
	char *SOAP_ENV__Value;	/* optional element of type xsd:QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
public:
	int soap_type() const { return 24; } /* = unique type id SOAP_TYPE_SOAP_ENV__Code */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (26)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
public:
	char *__any;
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
public:
	int soap_type() const { return 26; } /* = unique type id SOAP_TYPE_SOAP_ENV__Detail */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (29)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
public:
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
public:
	int soap_type() const { return 29; } /* = unique type id SOAP_TYPE_SOAP_ENV__Reason */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (30)
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
	int soap_type() const { return 30; } /* = unique type id SOAP_TYPE_SOAP_ENV__Fault */
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
