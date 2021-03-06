/* soapIVehicleServiceServiceSoapBindingProxy.h
   Generated by gSOAP 2.8.21 from SynTocity.h

Copyright(C) 2000-2014, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapIVehicleServiceServiceSoapBindingProxy_H
#define soapIVehicleServiceServiceSoapBindingProxy_H
#include "soapH.h"

class SOAP_CMAC IVehicleServiceServiceSoapBindingProxy : public soap
{ public:
	/// Endpoint URL of service 'IVehicleServiceServiceSoapBindingProxy' (change as needed)
	const char *soap_endpoint;
	/// Variables globally declared in SynTocity.h (non-static)
	/// Constructor
	IVehicleServiceServiceSoapBindingProxy();
	/// Construct from another engine state
	IVehicleServiceServiceSoapBindingProxy(const struct soap&);
	/// Constructor with endpoint URL
	IVehicleServiceServiceSoapBindingProxy(const char *url);
	/// Constructor with engine input+output mode control
	IVehicleServiceServiceSoapBindingProxy(soap_mode iomode);
	/// Constructor with URL and input+output mode control
	IVehicleServiceServiceSoapBindingProxy(const char *url, soap_mode iomode);
	/// Constructor with engine input and output mode control
	IVehicleServiceServiceSoapBindingProxy(soap_mode imode, soap_mode omode);
	/// Destructor frees deserialized data
	virtual	~IVehicleServiceServiceSoapBindingProxy();
	/// Initializer used by constructors
	virtual	void IVehicleServiceServiceSoapBindingProxy_init(soap_mode imode, soap_mode omode);
	/// Delete all deserialized data (with soap_destroy and soap_end)
	virtual	void destroy();
	/// Delete all deserialized data and reset to default
	virtual	void reset();
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Get SOAP Header structure (NULL when absent)
	virtual	const SOAP_ENV__Header *soap_header();
	/// Get SOAP Fault structure (NULL when absent)
	virtual	const SOAP_ENV__Fault *soap_fault();
	/// Get SOAP Fault string (NULL when absent)
	virtual	const char *soap_fault_string();
	/// Get SOAP Fault detail as string (NULL when absent)
	virtual	const char *soap_fault_detail();
	/// Close connection (normally automatic, except for send_X ops)
	virtual	int soap_close_socket();
	/// Force close connection (can kill a thread blocked on IO)
	virtual	int soap_force_close_socket();
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
#ifndef WITH_COMPAT
	virtual	void soap_stream_fault(std::ostream&);
#endif

	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif

	/// Web service operation 'BeginCheck' (returns error code or SOAP_OK)
	virtual	int BeginCheck(ns1__BeginCheck *ns1__BeginCheck_, ns1__BeginCheckResponse &ns1__BeginCheckResponse_) { return this->BeginCheck(NULL, NULL, ns1__BeginCheck_, ns1__BeginCheckResponse_); }
	virtual	int BeginCheck(const char *endpoint, const char *soap_action, ns1__BeginCheck *ns1__BeginCheck_, ns1__BeginCheckResponse &ns1__BeginCheckResponse_);

	/// Web service operation 'UploadInspectionResult' (returns error code or SOAP_OK)
	virtual	int UploadInspectionResult(ns1__UploadInspectionResult *ns1__UploadInspectionResult_, ns1__UploadInspectionResultResponse &ns1__UploadInspectionResultResponse_) { return this->UploadInspectionResult(NULL, NULL, ns1__UploadInspectionResult_, ns1__UploadInspectionResultResponse_); }
	virtual	int UploadInspectionResult(const char *endpoint, const char *soap_action, ns1__UploadInspectionResult *ns1__UploadInspectionResult_, ns1__UploadInspectionResultResponse &ns1__UploadInspectionResultResponse_);

	/// Web service operation 'GetVehicle' (returns error code or SOAP_OK)
	virtual	int GetVehicle(ns1__GetVehicle *ns1__GetVehicle_, ns1__GetVehicleResponse &ns1__GetVehicleResponse_) { return this->GetVehicle(NULL, NULL, ns1__GetVehicle_, ns1__GetVehicleResponse_); }
	virtual	int GetVehicle(const char *endpoint, const char *soap_action, ns1__GetVehicle *ns1__GetVehicle_, ns1__GetVehicleResponse &ns1__GetVehicleResponse_);

	/// Web service operation 'vehiclelogin' (returns error code or SOAP_OK)
	virtual	int vehiclelogin(ns1__vehiclelogin *ns1__vehiclelogin_, ns1__vehicleloginResponse &ns1__vehicleloginResponse_) { return this->vehiclelogin(NULL, NULL, ns1__vehiclelogin_, ns1__vehicleloginResponse_); }
	virtual	int vehiclelogin(const char *endpoint, const char *soap_action, ns1__vehiclelogin *ns1__vehiclelogin_, ns1__vehicleloginResponse &ns1__vehicleloginResponse_);

	/// Web service operation 'GetCheckList' (returns error code or SOAP_OK)
	virtual	int GetCheckList(ns1__GetCheckList *ns1__GetCheckList_, ns1__GetCheckListResponse &ns1__GetCheckListResponse_) { return this->GetCheckList(NULL, NULL, ns1__GetCheckList_, ns1__GetCheckListResponse_); }
	virtual	int GetCheckList(const char *endpoint, const char *soap_action, ns1__GetCheckList *ns1__GetCheckList_, ns1__GetCheckListResponse &ns1__GetCheckListResponse_);

	/// Web service operation 'LoginServer' (returns error code or SOAP_OK)
	virtual	int LoginServer(ns1__LoginServer *ns1__LoginServer_, ns1__LoginServerResponse &ns1__LoginServerResponse_) { return this->LoginServer(NULL, NULL, ns1__LoginServer_, ns1__LoginServerResponse_); }
	virtual	int LoginServer(const char *endpoint, const char *soap_action, ns1__LoginServer *ns1__LoginServer_, ns1__LoginServerResponse &ns1__LoginServerResponse_);

	/// Web service operation 'GetBaseTypeInfo' (returns error code or SOAP_OK)
	virtual	int GetBaseTypeInfo(ns1__GetBaseTypeInfo *ns1__GetBaseTypeInfo_, ns1__GetBaseTypeInfoResponse &ns1__GetBaseTypeInfoResponse_) { return this->GetBaseTypeInfo(NULL, NULL, ns1__GetBaseTypeInfo_, ns1__GetBaseTypeInfoResponse_); }
	virtual	int GetBaseTypeInfo(const char *endpoint, const char *soap_action, ns1__GetBaseTypeInfo *ns1__GetBaseTypeInfo_, ns1__GetBaseTypeInfoResponse &ns1__GetBaseTypeInfoResponse_);
};
#endif
