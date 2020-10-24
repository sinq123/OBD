/* soapGasServiceNewSoapProxy.h
   Generated by gSOAP 2.8.21 from GasServiceNew.h

Copyright(C) 2000-2014, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapGasServiceNewSoapProxy_H
#define soapGasServiceNewSoapProxy_H
#include "soapH.h"

class SOAP_CMAC GasServiceNewSoapProxy : public soap
{ public:
	/// Endpoint URL of service 'GasServiceNewSoapProxy' (change as needed)
	const char *soap_endpoint;
	/// Variables globally declared in GasServiceNew.h (non-static)
	/// Constructor
	GasServiceNewSoapProxy();
	/// Construct from another engine state
	GasServiceNewSoapProxy(const struct soap&);
	/// Constructor with endpoint URL
	GasServiceNewSoapProxy(const char *url);
	/// Constructor with engine input+output mode control
	GasServiceNewSoapProxy(soap_mode iomode);
	/// Constructor with URL and input+output mode control
	GasServiceNewSoapProxy(const char *url, soap_mode iomode);
	/// Constructor with engine input and output mode control
	GasServiceNewSoapProxy(soap_mode imode, soap_mode omode);
	/// Destructor frees deserialized data
	virtual	~GasServiceNewSoapProxy();
	/// Initializer used by constructors
	virtual	void GasServiceNewSoapProxy_init(soap_mode imode, soap_mode omode);
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

	/// Web service operation 'VehicleRegJudge' (returns error code or SOAP_OK)
	virtual	int VehicleRegJudge(_ns1__VehicleRegJudge *ns1__VehicleRegJudge, _ns1__VehicleRegJudgeResponse &ns1__VehicleRegJudgeResponse) { return this->VehicleRegJudge(NULL, NULL, ns1__VehicleRegJudge, ns1__VehicleRegJudgeResponse); }
	virtual	int VehicleRegJudge(const char *endpoint, const char *soap_action, _ns1__VehicleRegJudge *ns1__VehicleRegJudge, _ns1__VehicleRegJudgeResponse &ns1__VehicleRegJudgeResponse);

	/// Web service operation 'OnlineJudge' (returns error code or SOAP_OK)
	virtual	int OnlineJudge(_ns1__OnlineJudge *ns1__OnlineJudge, _ns1__OnlineJudgeResponse &ns1__OnlineJudgeResponse) { return this->OnlineJudge(NULL, NULL, ns1__OnlineJudge, ns1__OnlineJudgeResponse); }
	virtual	int OnlineJudge(const char *endpoint, const char *soap_action, _ns1__OnlineJudge *ns1__OnlineJudge, _ns1__OnlineJudgeResponse &ns1__OnlineJudgeResponse);

	/// Web service operation 'AddCheckData' (returns error code or SOAP_OK)
	virtual	int AddCheckData(_ns1__AddCheckData *ns1__AddCheckData, _ns1__AddCheckDataResponse &ns1__AddCheckDataResponse) { return this->AddCheckData(NULL, NULL, ns1__AddCheckData, ns1__AddCheckDataResponse); }
	virtual	int AddCheckData(const char *endpoint, const char *soap_action, _ns1__AddCheckData *ns1__AddCheckData, _ns1__AddCheckDataResponse &ns1__AddCheckDataResponse);

	/// Web service operation 'AddObdData' (returns error code or SOAP_OK)
	virtual	int AddObdData(_ns1__AddObdData *ns1__AddObdData, _ns1__AddObdDataResponse &ns1__AddObdDataResponse) { return this->AddObdData(NULL, NULL, ns1__AddObdData, ns1__AddObdDataResponse); }
	virtual	int AddObdData(const char *endpoint, const char *soap_action, _ns1__AddObdData *ns1__AddObdData, _ns1__AddObdDataResponse &ns1__AddObdDataResponse);

	/// Web service operation 'AddIuprData' (returns error code or SOAP_OK)
	virtual	int AddIuprData(_ns1__AddIuprData *ns1__AddIuprData, _ns1__AddIuprDataResponse &ns1__AddIuprDataResponse) { return this->AddIuprData(NULL, NULL, ns1__AddIuprData, ns1__AddIuprDataResponse); }
	virtual	int AddIuprData(const char *endpoint, const char *soap_action, _ns1__AddIuprData *ns1__AddIuprData, _ns1__AddIuprDataResponse &ns1__AddIuprDataResponse);

	/// Web service operation 'RyzfControl' (returns error code or SOAP_OK)
	virtual	int RyzfControl(_ns1__RyzfControl *ns1__RyzfControl, _ns1__RyzfControlResponse &ns1__RyzfControlResponse) { return this->RyzfControl(NULL, NULL, ns1__RyzfControl, ns1__RyzfControlResponse); }
	virtual	int RyzfControl(const char *endpoint, const char *soap_action, _ns1__RyzfControl *ns1__RyzfControl, _ns1__RyzfControlResponse &ns1__RyzfControlResponse);

	/// Web service operation 'AddVerify' (returns error code or SOAP_OK)
	virtual	int AddVerify(_ns1__AddVerify *ns1__AddVerify, _ns1__AddVerifyResponse &ns1__AddVerifyResponse) { return this->AddVerify(NULL, NULL, ns1__AddVerify, ns1__AddVerifyResponse); }
	virtual	int AddVerify(const char *endpoint, const char *soap_action, _ns1__AddVerify *ns1__AddVerify, _ns1__AddVerifyResponse &ns1__AddVerifyResponse);

	/// Web service operation 'PrintJudge' (returns error code or SOAP_OK)
	virtual	int PrintJudge(_ns1__PrintJudge *ns1__PrintJudge, _ns1__PrintJudgeResponse &ns1__PrintJudgeResponse) { return this->PrintJudge(NULL, NULL, ns1__PrintJudge, ns1__PrintJudgeResponse); }
	virtual	int PrintJudge(const char *endpoint, const char *soap_action, _ns1__PrintJudge *ns1__PrintJudge, _ns1__PrintJudgeResponse &ns1__PrintJudgeResponse);

	/// Web service operation 'PrintUp' (returns error code or SOAP_OK)
	virtual	int PrintUp(_ns1__PrintUp *ns1__PrintUp, _ns1__PrintUpResponse &ns1__PrintUpResponse) { return this->PrintUp(NULL, NULL, ns1__PrintUp, ns1__PrintUpResponse); }
	virtual	int PrintUp(const char *endpoint, const char *soap_action, _ns1__PrintUp *ns1__PrintUp, _ns1__PrintUpResponse &ns1__PrintUpResponse);

	/// Web service operation 'LokeJudge' (returns error code or SOAP_OK)
	virtual	int LokeJudge(_ns1__LokeJudge *ns1__LokeJudge, _ns1__LokeJudgeResponse &ns1__LokeJudgeResponse) { return this->LokeJudge(NULL, NULL, ns1__LokeJudge, ns1__LokeJudgeResponse); }
	virtual	int LokeJudge(const char *endpoint, const char *soap_action, _ns1__LokeJudge *ns1__LokeJudge, _ns1__LokeJudgeResponse &ns1__LokeJudgeResponse);

	/// Web service operation 'AddWGJCData' (returns error code or SOAP_OK)
	virtual	int AddWGJCData(_ns1__AddWGJCData *ns1__AddWGJCData, _ns1__AddWGJCDataResponse &ns1__AddWGJCDataResponse) { return this->AddWGJCData(NULL, NULL, ns1__AddWGJCData, ns1__AddWGJCDataResponse); }
	virtual	int AddWGJCData(const char *endpoint, const char *soap_action, _ns1__AddWGJCData *ns1__AddWGJCData, _ns1__AddWGJCDataResponse &ns1__AddWGJCDataResponse);

	/// Web service operation 'AddDeviceCheckData' (returns error code or SOAP_OK)
	virtual	int AddDeviceCheckData(_ns1__AddDeviceCheckData *ns1__AddDeviceCheckData, _ns1__AddDeviceCheckDataResponse &ns1__AddDeviceCheckDataResponse) { return this->AddDeviceCheckData(NULL, NULL, ns1__AddDeviceCheckData, ns1__AddDeviceCheckDataResponse); }
	virtual	int AddDeviceCheckData(const char *endpoint, const char *soap_action, _ns1__AddDeviceCheckData *ns1__AddDeviceCheckData, _ns1__AddDeviceCheckDataResponse &ns1__AddDeviceCheckDataResponse);

	/// Web service operation 'VehicleRegJudge' (returns error code or SOAP_OK)
	virtual	int VehicleRegJudge_(_ns1__VehicleRegJudge *ns1__VehicleRegJudge, _ns1__VehicleRegJudgeResponse &ns1__VehicleRegJudgeResponse) { return this->VehicleRegJudge_(NULL, NULL, ns1__VehicleRegJudge, ns1__VehicleRegJudgeResponse); }
	virtual	int VehicleRegJudge_(const char *endpoint, const char *soap_action, _ns1__VehicleRegJudge *ns1__VehicleRegJudge, _ns1__VehicleRegJudgeResponse &ns1__VehicleRegJudgeResponse);

	/// Web service operation 'OnlineJudge' (returns error code or SOAP_OK)
	virtual	int OnlineJudge_(_ns1__OnlineJudge *ns1__OnlineJudge, _ns1__OnlineJudgeResponse &ns1__OnlineJudgeResponse) { return this->OnlineJudge_(NULL, NULL, ns1__OnlineJudge, ns1__OnlineJudgeResponse); }
	virtual	int OnlineJudge_(const char *endpoint, const char *soap_action, _ns1__OnlineJudge *ns1__OnlineJudge, _ns1__OnlineJudgeResponse &ns1__OnlineJudgeResponse);

	/// Web service operation 'AddCheckData' (returns error code or SOAP_OK)
	virtual	int AddCheckData_(_ns1__AddCheckData *ns1__AddCheckData, _ns1__AddCheckDataResponse &ns1__AddCheckDataResponse) { return this->AddCheckData_(NULL, NULL, ns1__AddCheckData, ns1__AddCheckDataResponse); }
	virtual	int AddCheckData_(const char *endpoint, const char *soap_action, _ns1__AddCheckData *ns1__AddCheckData, _ns1__AddCheckDataResponse &ns1__AddCheckDataResponse);

	/// Web service operation 'AddObdData' (returns error code or SOAP_OK)
	virtual	int AddObdData_(_ns1__AddObdData *ns1__AddObdData, _ns1__AddObdDataResponse &ns1__AddObdDataResponse) { return this->AddObdData_(NULL, NULL, ns1__AddObdData, ns1__AddObdDataResponse); }
	virtual	int AddObdData_(const char *endpoint, const char *soap_action, _ns1__AddObdData *ns1__AddObdData, _ns1__AddObdDataResponse &ns1__AddObdDataResponse);

	/// Web service operation 'AddIuprData' (returns error code or SOAP_OK)
	virtual	int AddIuprData_(_ns1__AddIuprData *ns1__AddIuprData, _ns1__AddIuprDataResponse &ns1__AddIuprDataResponse) { return this->AddIuprData_(NULL, NULL, ns1__AddIuprData, ns1__AddIuprDataResponse); }
	virtual	int AddIuprData_(const char *endpoint, const char *soap_action, _ns1__AddIuprData *ns1__AddIuprData, _ns1__AddIuprDataResponse &ns1__AddIuprDataResponse);

	/// Web service operation 'RyzfControl' (returns error code or SOAP_OK)
	virtual	int RyzfControl_(_ns1__RyzfControl *ns1__RyzfControl, _ns1__RyzfControlResponse &ns1__RyzfControlResponse) { return this->RyzfControl_(NULL, NULL, ns1__RyzfControl, ns1__RyzfControlResponse); }
	virtual	int RyzfControl_(const char *endpoint, const char *soap_action, _ns1__RyzfControl *ns1__RyzfControl, _ns1__RyzfControlResponse &ns1__RyzfControlResponse);

	/// Web service operation 'AddVerify' (returns error code or SOAP_OK)
	virtual	int AddVerify_(_ns1__AddVerify *ns1__AddVerify, _ns1__AddVerifyResponse &ns1__AddVerifyResponse) { return this->AddVerify_(NULL, NULL, ns1__AddVerify, ns1__AddVerifyResponse); }
	virtual	int AddVerify_(const char *endpoint, const char *soap_action, _ns1__AddVerify *ns1__AddVerify, _ns1__AddVerifyResponse &ns1__AddVerifyResponse);

	/// Web service operation 'PrintJudge' (returns error code or SOAP_OK)
	virtual	int PrintJudge_(_ns1__PrintJudge *ns1__PrintJudge, _ns1__PrintJudgeResponse &ns1__PrintJudgeResponse) { return this->PrintJudge_(NULL, NULL, ns1__PrintJudge, ns1__PrintJudgeResponse); }
	virtual	int PrintJudge_(const char *endpoint, const char *soap_action, _ns1__PrintJudge *ns1__PrintJudge, _ns1__PrintJudgeResponse &ns1__PrintJudgeResponse);

	/// Web service operation 'PrintUp' (returns error code or SOAP_OK)
	virtual	int PrintUp_(_ns1__PrintUp *ns1__PrintUp, _ns1__PrintUpResponse &ns1__PrintUpResponse) { return this->PrintUp_(NULL, NULL, ns1__PrintUp, ns1__PrintUpResponse); }
	virtual	int PrintUp_(const char *endpoint, const char *soap_action, _ns1__PrintUp *ns1__PrintUp, _ns1__PrintUpResponse &ns1__PrintUpResponse);

	/// Web service operation 'LokeJudge' (returns error code or SOAP_OK)
	virtual	int LokeJudge_(_ns1__LokeJudge *ns1__LokeJudge, _ns1__LokeJudgeResponse &ns1__LokeJudgeResponse) { return this->LokeJudge_(NULL, NULL, ns1__LokeJudge, ns1__LokeJudgeResponse); }
	virtual	int LokeJudge_(const char *endpoint, const char *soap_action, _ns1__LokeJudge *ns1__LokeJudge, _ns1__LokeJudgeResponse &ns1__LokeJudgeResponse);

	/// Web service operation 'AddWGJCData' (returns error code or SOAP_OK)
	virtual	int AddWGJCData_(_ns1__AddWGJCData *ns1__AddWGJCData, _ns1__AddWGJCDataResponse &ns1__AddWGJCDataResponse) { return this->AddWGJCData_(NULL, NULL, ns1__AddWGJCData, ns1__AddWGJCDataResponse); }
	virtual	int AddWGJCData_(const char *endpoint, const char *soap_action, _ns1__AddWGJCData *ns1__AddWGJCData, _ns1__AddWGJCDataResponse &ns1__AddWGJCDataResponse);

	/// Web service operation 'AddDeviceCheckData' (returns error code or SOAP_OK)
	virtual	int AddDeviceCheckData_(_ns1__AddDeviceCheckData *ns1__AddDeviceCheckData, _ns1__AddDeviceCheckDataResponse &ns1__AddDeviceCheckDataResponse) { return this->AddDeviceCheckData_(NULL, NULL, ns1__AddDeviceCheckData, ns1__AddDeviceCheckDataResponse); }
	virtual	int AddDeviceCheckData_(const char *endpoint, const char *soap_action, _ns1__AddDeviceCheckData *ns1__AddDeviceCheckData, _ns1__AddDeviceCheckDataResponse &ns1__AddDeviceCheckDataResponse);
};
#endif
