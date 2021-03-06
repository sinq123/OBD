/* soapStub.h
   Generated by gSOAP 2.8.21 from GasServiceNew.h

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

#ifndef SOAP_TYPE__ns1__VehicleRegJudge
#define SOAP_TYPE__ns1__VehicleRegJudge (8)
/* ns1:VehicleRegJudge */
class SOAP_CMAC _ns1__VehicleRegJudge
{
public:
	std::wstring *JczId;	/* optional element of type xsd:string */
	std::wstring *ZcTime;	/* optional element of type xsd:string */
	std::wstring *CarItems;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 8; } /* = unique type id SOAP_TYPE__ns1__VehicleRegJudge */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__VehicleRegJudge() { _ns1__VehicleRegJudge::soap_default(NULL); }
	virtual ~_ns1__VehicleRegJudge() { }
};
#endif

#ifndef SOAP_TYPE__ns1__VehicleRegJudgeResponse
#define SOAP_TYPE__ns1__VehicleRegJudgeResponse (9)
/* ns1:VehicleRegJudgeResponse */
class SOAP_CMAC _ns1__VehicleRegJudgeResponse
{
public:
	std::wstring *VehicleRegJudgeResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 9; } /* = unique type id SOAP_TYPE__ns1__VehicleRegJudgeResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__VehicleRegJudgeResponse() { _ns1__VehicleRegJudgeResponse::soap_default(NULL); }
	virtual ~_ns1__VehicleRegJudgeResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__OnlineJudge
#define SOAP_TYPE__ns1__OnlineJudge (10)
/* ns1:OnlineJudge */
class SOAP_CMAC _ns1__OnlineJudge
{
public:
	std::wstring *jczNo;	/* optional element of type xsd:string */
	std::wstring *jcxNo;	/* optional element of type xsd:string */
	std::wstring *bdrq;	/* optional element of type xsd:string */
	std::wstring *CarPZ;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 10; } /* = unique type id SOAP_TYPE__ns1__OnlineJudge */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__OnlineJudge() { _ns1__OnlineJudge::soap_default(NULL); }
	virtual ~_ns1__OnlineJudge() { }
};
#endif

#ifndef SOAP_TYPE__ns1__OnlineJudgeResponse
#define SOAP_TYPE__ns1__OnlineJudgeResponse (11)
/* ns1:OnlineJudgeResponse */
class SOAP_CMAC _ns1__OnlineJudgeResponse
{
public:
	std::wstring *OnlineJudgeResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 11; } /* = unique type id SOAP_TYPE__ns1__OnlineJudgeResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__OnlineJudgeResponse() { _ns1__OnlineJudgeResponse::soap_default(NULL); }
	virtual ~_ns1__OnlineJudgeResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__AddCheckData
#define SOAP_TYPE__ns1__AddCheckData (12)
/* ns1:AddCheckData */
class SOAP_CMAC _ns1__AddCheckData
{
public:
	std::wstring *CheckDataItems;	/* optional element of type xsd:string */
	std::wstring *CheckEndDataItems;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 12; } /* = unique type id SOAP_TYPE__ns1__AddCheckData */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__AddCheckData() { _ns1__AddCheckData::soap_default(NULL); }
	virtual ~_ns1__AddCheckData() { }
};
#endif

#ifndef SOAP_TYPE__ns1__AddCheckDataResponse
#define SOAP_TYPE__ns1__AddCheckDataResponse (13)
/* ns1:AddCheckDataResponse */
class SOAP_CMAC _ns1__AddCheckDataResponse
{
public:
	std::wstring *AddCheckDataResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 13; } /* = unique type id SOAP_TYPE__ns1__AddCheckDataResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__AddCheckDataResponse() { _ns1__AddCheckDataResponse::soap_default(NULL); }
	virtual ~_ns1__AddCheckDataResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__AddObdData
#define SOAP_TYPE__ns1__AddObdData (14)
/* ns1:AddObdData */
class SOAP_CMAC _ns1__AddObdData
{
public:
	std::wstring *ObdDataItems;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 14; } /* = unique type id SOAP_TYPE__ns1__AddObdData */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__AddObdData() { _ns1__AddObdData::soap_default(NULL); }
	virtual ~_ns1__AddObdData() { }
};
#endif

#ifndef SOAP_TYPE__ns1__AddObdDataResponse
#define SOAP_TYPE__ns1__AddObdDataResponse (15)
/* ns1:AddObdDataResponse */
class SOAP_CMAC _ns1__AddObdDataResponse
{
public:
	std::wstring *AddObdDataResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 15; } /* = unique type id SOAP_TYPE__ns1__AddObdDataResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__AddObdDataResponse() { _ns1__AddObdDataResponse::soap_default(NULL); }
	virtual ~_ns1__AddObdDataResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__AddIuprData
#define SOAP_TYPE__ns1__AddIuprData (16)
/* ns1:AddIuprData */
class SOAP_CMAC _ns1__AddIuprData
{
public:
	std::wstring *IuprDataItems;	/* optional element of type xsd:string */
	std::wstring *ObdType;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 16; } /* = unique type id SOAP_TYPE__ns1__AddIuprData */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__AddIuprData() { _ns1__AddIuprData::soap_default(NULL); }
	virtual ~_ns1__AddIuprData() { }
};
#endif

#ifndef SOAP_TYPE__ns1__AddIuprDataResponse
#define SOAP_TYPE__ns1__AddIuprDataResponse (17)
/* ns1:AddIuprDataResponse */
class SOAP_CMAC _ns1__AddIuprDataResponse
{
public:
	std::wstring *AddIuprDataResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 17; } /* = unique type id SOAP_TYPE__ns1__AddIuprDataResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__AddIuprDataResponse() { _ns1__AddIuprDataResponse::soap_default(NULL); }
	virtual ~_ns1__AddIuprDataResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__RyzfControl
#define SOAP_TYPE__ns1__RyzfControl (18)
/* ns1:RyzfControl */
class SOAP_CMAC _ns1__RyzfControl
{
public:
	std::wstring *rykzItems;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 18; } /* = unique type id SOAP_TYPE__ns1__RyzfControl */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__RyzfControl() { _ns1__RyzfControl::soap_default(NULL); }
	virtual ~_ns1__RyzfControl() { }
};
#endif

#ifndef SOAP_TYPE__ns1__RyzfControlResponse
#define SOAP_TYPE__ns1__RyzfControlResponse (19)
/* ns1:RyzfControlResponse */
class SOAP_CMAC _ns1__RyzfControlResponse
{
public:
	std::wstring *RyzfControlResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 19; } /* = unique type id SOAP_TYPE__ns1__RyzfControlResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__RyzfControlResponse() { _ns1__RyzfControlResponse::soap_default(NULL); }
	virtual ~_ns1__RyzfControlResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__AddVerify
#define SOAP_TYPE__ns1__AddVerify (20)
/* ns1:AddVerify */
class SOAP_CMAC _ns1__AddVerify
{
public:
	std::wstring *DeviceType;	/* optional element of type xsd:string */
	std::wstring *Items;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 20; } /* = unique type id SOAP_TYPE__ns1__AddVerify */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__AddVerify() { _ns1__AddVerify::soap_default(NULL); }
	virtual ~_ns1__AddVerify() { }
};
#endif

#ifndef SOAP_TYPE__ns1__AddVerifyResponse
#define SOAP_TYPE__ns1__AddVerifyResponse (21)
/* ns1:AddVerifyResponse */
class SOAP_CMAC _ns1__AddVerifyResponse
{
public:
	std::wstring *AddVerifyResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 21; } /* = unique type id SOAP_TYPE__ns1__AddVerifyResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__AddVerifyResponse() { _ns1__AddVerifyResponse::soap_default(NULL); }
	virtual ~_ns1__AddVerifyResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__PrintJudge
#define SOAP_TYPE__ns1__PrintJudge (22)
/* ns1:PrintJudge */
class SOAP_CMAC _ns1__PrintJudge
{
public:
	std::wstring *lsbh;	/* optional element of type xsd:string */
	std::wstring *jczNo;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 22; } /* = unique type id SOAP_TYPE__ns1__PrintJudge */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__PrintJudge() { _ns1__PrintJudge::soap_default(NULL); }
	virtual ~_ns1__PrintJudge() { }
};
#endif

#ifndef SOAP_TYPE__ns1__PrintJudgeResponse
#define SOAP_TYPE__ns1__PrintJudgeResponse (23)
/* ns1:PrintJudgeResponse */
class SOAP_CMAC _ns1__PrintJudgeResponse
{
public:
	std::wstring *PrintJudgeResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 23; } /* = unique type id SOAP_TYPE__ns1__PrintJudgeResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__PrintJudgeResponse() { _ns1__PrintJudgeResponse::soap_default(NULL); }
	virtual ~_ns1__PrintJudgeResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__PrintUp
#define SOAP_TYPE__ns1__PrintUp (24)
/* ns1:PrintUp */
class SOAP_CMAC _ns1__PrintUp
{
public:
	std::wstring *lsbh;	/* optional element of type xsd:string */
	std::wstring *encodeReport;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 24; } /* = unique type id SOAP_TYPE__ns1__PrintUp */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__PrintUp() { _ns1__PrintUp::soap_default(NULL); }
	virtual ~_ns1__PrintUp() { }
};
#endif

#ifndef SOAP_TYPE__ns1__PrintUpResponse
#define SOAP_TYPE__ns1__PrintUpResponse (25)
/* ns1:PrintUpResponse */
class SOAP_CMAC _ns1__PrintUpResponse
{
public:
	std::wstring *PrintUpResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 25; } /* = unique type id SOAP_TYPE__ns1__PrintUpResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__PrintUpResponse() { _ns1__PrintUpResponse::soap_default(NULL); }
	virtual ~_ns1__PrintUpResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__LokeJudge
#define SOAP_TYPE__ns1__LokeJudge (26)
/* ns1:LokeJudge */
class SOAP_CMAC _ns1__LokeJudge
{
public:
	std::wstring *jczNo;	/* optional element of type xsd:string */
	std::wstring *devBm;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 26; } /* = unique type id SOAP_TYPE__ns1__LokeJudge */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__LokeJudge() { _ns1__LokeJudge::soap_default(NULL); }
	virtual ~_ns1__LokeJudge() { }
};
#endif

#ifndef SOAP_TYPE__ns1__LokeJudgeResponse
#define SOAP_TYPE__ns1__LokeJudgeResponse (27)
/* ns1:LokeJudgeResponse */
class SOAP_CMAC _ns1__LokeJudgeResponse
{
public:
	std::wstring *LokeJudgeResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 27; } /* = unique type id SOAP_TYPE__ns1__LokeJudgeResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__LokeJudgeResponse() { _ns1__LokeJudgeResponse::soap_default(NULL); }
	virtual ~_ns1__LokeJudgeResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__AddWGJCData
#define SOAP_TYPE__ns1__AddWGJCData (28)
/* ns1:AddWGJCData */
class SOAP_CMAC _ns1__AddWGJCData
{
public:
	std::wstring *WgjcDataItems;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 28; } /* = unique type id SOAP_TYPE__ns1__AddWGJCData */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__AddWGJCData() { _ns1__AddWGJCData::soap_default(NULL); }
	virtual ~_ns1__AddWGJCData() { }
};
#endif

#ifndef SOAP_TYPE__ns1__AddWGJCDataResponse
#define SOAP_TYPE__ns1__AddWGJCDataResponse (29)
/* ns1:AddWGJCDataResponse */
class SOAP_CMAC _ns1__AddWGJCDataResponse
{
public:
	std::wstring *AddWGJCDataResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 29; } /* = unique type id SOAP_TYPE__ns1__AddWGJCDataResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__AddWGJCDataResponse() { _ns1__AddWGJCDataResponse::soap_default(NULL); }
	virtual ~_ns1__AddWGJCDataResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__AddDeviceCheckData
#define SOAP_TYPE__ns1__AddDeviceCheckData (30)
/* ns1:AddDeviceCheckData */
class SOAP_CMAC _ns1__AddDeviceCheckData
{
public:
	std::wstring *addDeviceCheckData;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 30; } /* = unique type id SOAP_TYPE__ns1__AddDeviceCheckData */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__AddDeviceCheckData() { _ns1__AddDeviceCheckData::soap_default(NULL); }
	virtual ~_ns1__AddDeviceCheckData() { }
};
#endif

#ifndef SOAP_TYPE__ns1__AddDeviceCheckDataResponse
#define SOAP_TYPE__ns1__AddDeviceCheckDataResponse (31)
/* ns1:AddDeviceCheckDataResponse */
class SOAP_CMAC _ns1__AddDeviceCheckDataResponse
{
public:
	std::wstring *AddDeviceCheckDataResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 31; } /* = unique type id SOAP_TYPE__ns1__AddDeviceCheckDataResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__AddDeviceCheckDataResponse() { _ns1__AddDeviceCheckDataResponse::soap_default(NULL); }
	virtual ~_ns1__AddDeviceCheckDataResponse() { }
};
#endif

#ifndef SOAP_TYPE___ns1__VehicleRegJudge
#define SOAP_TYPE___ns1__VehicleRegJudge (38)
/* Operation wrapper: */
struct __ns1__VehicleRegJudge
{
public:
	_ns1__VehicleRegJudge *ns1__VehicleRegJudge;	/* optional element of type ns1:VehicleRegJudge */
public:
	int soap_type() const { return 38; } /* = unique type id SOAP_TYPE___ns1__VehicleRegJudge */
};
#endif

#ifndef SOAP_TYPE___ns1__OnlineJudge
#define SOAP_TYPE___ns1__OnlineJudge (42)
/* Operation wrapper: */
struct __ns1__OnlineJudge
{
public:
	_ns1__OnlineJudge *ns1__OnlineJudge;	/* optional element of type ns1:OnlineJudge */
public:
	int soap_type() const { return 42; } /* = unique type id SOAP_TYPE___ns1__OnlineJudge */
};
#endif

#ifndef SOAP_TYPE___ns1__AddCheckData
#define SOAP_TYPE___ns1__AddCheckData (46)
/* Operation wrapper: */
struct __ns1__AddCheckData
{
public:
	_ns1__AddCheckData *ns1__AddCheckData;	/* optional element of type ns1:AddCheckData */
public:
	int soap_type() const { return 46; } /* = unique type id SOAP_TYPE___ns1__AddCheckData */
};
#endif

#ifndef SOAP_TYPE___ns1__AddObdData
#define SOAP_TYPE___ns1__AddObdData (50)
/* Operation wrapper: */
struct __ns1__AddObdData
{
public:
	_ns1__AddObdData *ns1__AddObdData;	/* optional element of type ns1:AddObdData */
public:
	int soap_type() const { return 50; } /* = unique type id SOAP_TYPE___ns1__AddObdData */
};
#endif

#ifndef SOAP_TYPE___ns1__AddIuprData
#define SOAP_TYPE___ns1__AddIuprData (54)
/* Operation wrapper: */
struct __ns1__AddIuprData
{
public:
	_ns1__AddIuprData *ns1__AddIuprData;	/* optional element of type ns1:AddIuprData */
public:
	int soap_type() const { return 54; } /* = unique type id SOAP_TYPE___ns1__AddIuprData */
};
#endif

#ifndef SOAP_TYPE___ns1__RyzfControl
#define SOAP_TYPE___ns1__RyzfControl (58)
/* Operation wrapper: */
struct __ns1__RyzfControl
{
public:
	_ns1__RyzfControl *ns1__RyzfControl;	/* optional element of type ns1:RyzfControl */
public:
	int soap_type() const { return 58; } /* = unique type id SOAP_TYPE___ns1__RyzfControl */
};
#endif

#ifndef SOAP_TYPE___ns1__AddVerify
#define SOAP_TYPE___ns1__AddVerify (62)
/* Operation wrapper: */
struct __ns1__AddVerify
{
public:
	_ns1__AddVerify *ns1__AddVerify;	/* optional element of type ns1:AddVerify */
public:
	int soap_type() const { return 62; } /* = unique type id SOAP_TYPE___ns1__AddVerify */
};
#endif

#ifndef SOAP_TYPE___ns1__PrintJudge
#define SOAP_TYPE___ns1__PrintJudge (66)
/* Operation wrapper: */
struct __ns1__PrintJudge
{
public:
	_ns1__PrintJudge *ns1__PrintJudge;	/* optional element of type ns1:PrintJudge */
public:
	int soap_type() const { return 66; } /* = unique type id SOAP_TYPE___ns1__PrintJudge */
};
#endif

#ifndef SOAP_TYPE___ns1__PrintUp
#define SOAP_TYPE___ns1__PrintUp (70)
/* Operation wrapper: */
struct __ns1__PrintUp
{
public:
	_ns1__PrintUp *ns1__PrintUp;	/* optional element of type ns1:PrintUp */
public:
	int soap_type() const { return 70; } /* = unique type id SOAP_TYPE___ns1__PrintUp */
};
#endif

#ifndef SOAP_TYPE___ns1__LokeJudge
#define SOAP_TYPE___ns1__LokeJudge (74)
/* Operation wrapper: */
struct __ns1__LokeJudge
{
public:
	_ns1__LokeJudge *ns1__LokeJudge;	/* optional element of type ns1:LokeJudge */
public:
	int soap_type() const { return 74; } /* = unique type id SOAP_TYPE___ns1__LokeJudge */
};
#endif

#ifndef SOAP_TYPE___ns1__AddWGJCData
#define SOAP_TYPE___ns1__AddWGJCData (78)
/* Operation wrapper: */
struct __ns1__AddWGJCData
{
public:
	_ns1__AddWGJCData *ns1__AddWGJCData;	/* optional element of type ns1:AddWGJCData */
public:
	int soap_type() const { return 78; } /* = unique type id SOAP_TYPE___ns1__AddWGJCData */
};
#endif

#ifndef SOAP_TYPE___ns1__AddDeviceCheckData
#define SOAP_TYPE___ns1__AddDeviceCheckData (82)
/* Operation wrapper: */
struct __ns1__AddDeviceCheckData
{
public:
	_ns1__AddDeviceCheckData *ns1__AddDeviceCheckData;	/* optional element of type ns1:AddDeviceCheckData */
public:
	int soap_type() const { return 82; } /* = unique type id SOAP_TYPE___ns1__AddDeviceCheckData */
};
#endif

#ifndef SOAP_TYPE___ns1__VehicleRegJudge_
#define SOAP_TYPE___ns1__VehicleRegJudge_ (84)
/* Operation wrapper: */
struct __ns1__VehicleRegJudge_
{
public:
	_ns1__VehicleRegJudge *ns1__VehicleRegJudge;	/* optional element of type ns1:VehicleRegJudge */
public:
	int soap_type() const { return 84; } /* = unique type id SOAP_TYPE___ns1__VehicleRegJudge_ */
};
#endif

#ifndef SOAP_TYPE___ns1__OnlineJudge_
#define SOAP_TYPE___ns1__OnlineJudge_ (86)
/* Operation wrapper: */
struct __ns1__OnlineJudge_
{
public:
	_ns1__OnlineJudge *ns1__OnlineJudge;	/* optional element of type ns1:OnlineJudge */
public:
	int soap_type() const { return 86; } /* = unique type id SOAP_TYPE___ns1__OnlineJudge_ */
};
#endif

#ifndef SOAP_TYPE___ns1__AddCheckData_
#define SOAP_TYPE___ns1__AddCheckData_ (88)
/* Operation wrapper: */
struct __ns1__AddCheckData_
{
public:
	_ns1__AddCheckData *ns1__AddCheckData;	/* optional element of type ns1:AddCheckData */
public:
	int soap_type() const { return 88; } /* = unique type id SOAP_TYPE___ns1__AddCheckData_ */
};
#endif

#ifndef SOAP_TYPE___ns1__AddObdData_
#define SOAP_TYPE___ns1__AddObdData_ (90)
/* Operation wrapper: */
struct __ns1__AddObdData_
{
public:
	_ns1__AddObdData *ns1__AddObdData;	/* optional element of type ns1:AddObdData */
public:
	int soap_type() const { return 90; } /* = unique type id SOAP_TYPE___ns1__AddObdData_ */
};
#endif

#ifndef SOAP_TYPE___ns1__AddIuprData_
#define SOAP_TYPE___ns1__AddIuprData_ (92)
/* Operation wrapper: */
struct __ns1__AddIuprData_
{
public:
	_ns1__AddIuprData *ns1__AddIuprData;	/* optional element of type ns1:AddIuprData */
public:
	int soap_type() const { return 92; } /* = unique type id SOAP_TYPE___ns1__AddIuprData_ */
};
#endif

#ifndef SOAP_TYPE___ns1__RyzfControl_
#define SOAP_TYPE___ns1__RyzfControl_ (94)
/* Operation wrapper: */
struct __ns1__RyzfControl_
{
public:
	_ns1__RyzfControl *ns1__RyzfControl;	/* optional element of type ns1:RyzfControl */
public:
	int soap_type() const { return 94; } /* = unique type id SOAP_TYPE___ns1__RyzfControl_ */
};
#endif

#ifndef SOAP_TYPE___ns1__AddVerify_
#define SOAP_TYPE___ns1__AddVerify_ (96)
/* Operation wrapper: */
struct __ns1__AddVerify_
{
public:
	_ns1__AddVerify *ns1__AddVerify;	/* optional element of type ns1:AddVerify */
public:
	int soap_type() const { return 96; } /* = unique type id SOAP_TYPE___ns1__AddVerify_ */
};
#endif

#ifndef SOAP_TYPE___ns1__PrintJudge_
#define SOAP_TYPE___ns1__PrintJudge_ (98)
/* Operation wrapper: */
struct __ns1__PrintJudge_
{
public:
	_ns1__PrintJudge *ns1__PrintJudge;	/* optional element of type ns1:PrintJudge */
public:
	int soap_type() const { return 98; } /* = unique type id SOAP_TYPE___ns1__PrintJudge_ */
};
#endif

#ifndef SOAP_TYPE___ns1__PrintUp_
#define SOAP_TYPE___ns1__PrintUp_ (100)
/* Operation wrapper: */
struct __ns1__PrintUp_
{
public:
	_ns1__PrintUp *ns1__PrintUp;	/* optional element of type ns1:PrintUp */
public:
	int soap_type() const { return 100; } /* = unique type id SOAP_TYPE___ns1__PrintUp_ */
};
#endif

#ifndef SOAP_TYPE___ns1__LokeJudge_
#define SOAP_TYPE___ns1__LokeJudge_ (102)
/* Operation wrapper: */
struct __ns1__LokeJudge_
{
public:
	_ns1__LokeJudge *ns1__LokeJudge;	/* optional element of type ns1:LokeJudge */
public:
	int soap_type() const { return 102; } /* = unique type id SOAP_TYPE___ns1__LokeJudge_ */
};
#endif

#ifndef SOAP_TYPE___ns1__AddWGJCData_
#define SOAP_TYPE___ns1__AddWGJCData_ (104)
/* Operation wrapper: */
struct __ns1__AddWGJCData_
{
public:
	_ns1__AddWGJCData *ns1__AddWGJCData;	/* optional element of type ns1:AddWGJCData */
public:
	int soap_type() const { return 104; } /* = unique type id SOAP_TYPE___ns1__AddWGJCData_ */
};
#endif

#ifndef SOAP_TYPE___ns1__AddDeviceCheckData_
#define SOAP_TYPE___ns1__AddDeviceCheckData_ (106)
/* Operation wrapper: */
struct __ns1__AddDeviceCheckData_
{
public:
	_ns1__AddDeviceCheckData *ns1__AddDeviceCheckData;	/* optional element of type ns1:AddDeviceCheckData */
public:
	int soap_type() const { return 106; } /* = unique type id SOAP_TYPE___ns1__AddDeviceCheckData_ */
};
#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (107)
/* SOAP Header: */
struct SOAP_ENV__Header
{
public:
	int soap_type() const { return 107; } /* = unique type id SOAP_TYPE_SOAP_ENV__Header */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (108)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
public:
	char *SOAP_ENV__Value;	/* optional element of type xsd:QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
public:
	int soap_type() const { return 108; } /* = unique type id SOAP_TYPE_SOAP_ENV__Code */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (110)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
public:
	char *__any;
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
public:
	int soap_type() const { return 110; } /* = unique type id SOAP_TYPE_SOAP_ENV__Detail */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (113)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
public:
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
public:
	int soap_type() const { return 113; } /* = unique type id SOAP_TYPE_SOAP_ENV__Reason */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (114)
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
	int soap_type() const { return 114; } /* = unique type id SOAP_TYPE_SOAP_ENV__Fault */
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
