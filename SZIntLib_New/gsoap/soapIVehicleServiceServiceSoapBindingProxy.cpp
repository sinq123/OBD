/* soapIVehicleServiceServiceSoapBindingProxy.cpp
   Generated by gSOAP 2.8.21 from SynTocity.h

Copyright(C) 2000-2014, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#include "soapIVehicleServiceServiceSoapBindingProxy.h"

IVehicleServiceServiceSoapBindingProxy::IVehicleServiceServiceSoapBindingProxy()
{	IVehicleServiceServiceSoapBindingProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

IVehicleServiceServiceSoapBindingProxy::IVehicleServiceServiceSoapBindingProxy(const struct soap &_soap) : soap(_soap)
{ }

IVehicleServiceServiceSoapBindingProxy::IVehicleServiceServiceSoapBindingProxy(const char *url)
{	IVehicleServiceServiceSoapBindingProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
	soap_endpoint = url;
}

IVehicleServiceServiceSoapBindingProxy::IVehicleServiceServiceSoapBindingProxy(soap_mode iomode)
{	IVehicleServiceServiceSoapBindingProxy_init(iomode, iomode);
}

IVehicleServiceServiceSoapBindingProxy::IVehicleServiceServiceSoapBindingProxy(const char *url, soap_mode iomode)
{	IVehicleServiceServiceSoapBindingProxy_init(iomode, iomode);
	soap_endpoint = url;
}

IVehicleServiceServiceSoapBindingProxy::IVehicleServiceServiceSoapBindingProxy(soap_mode imode, soap_mode omode)
{	IVehicleServiceServiceSoapBindingProxy_init(imode, omode);
}

IVehicleServiceServiceSoapBindingProxy::~IVehicleServiceServiceSoapBindingProxy()
{ }

void IVehicleServiceServiceSoapBindingProxy::IVehicleServiceServiceSoapBindingProxy_init(soap_mode imode, soap_mode omode)
{	soap_imode(this, imode);
	soap_omode(this, omode);
	soap_endpoint = NULL;
	static const struct Namespace namespaces[] =
{
	{"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
	{"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"ns1", "http://synToCity.synchrodata.daniu.com/", NULL, NULL},
	{NULL, NULL, NULL, NULL}
};
	soap_set_namespaces(this, namespaces);
}

void IVehicleServiceServiceSoapBindingProxy::destroy()
{	soap_destroy(this);
	soap_end(this);
}

void IVehicleServiceServiceSoapBindingProxy::reset()
{	destroy();
	soap_done(this);
	soap_initialize(this);
	IVehicleServiceServiceSoapBindingProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

void IVehicleServiceServiceSoapBindingProxy::soap_noheader()
{	this->header = NULL;
}

const SOAP_ENV__Header *IVehicleServiceServiceSoapBindingProxy::soap_header()
{	return this->header;
}

const SOAP_ENV__Fault *IVehicleServiceServiceSoapBindingProxy::soap_fault()
{	return this->fault;
}

const char *IVehicleServiceServiceSoapBindingProxy::soap_fault_string()
{	return *soap_faultstring(this);
}

const char *IVehicleServiceServiceSoapBindingProxy::soap_fault_detail()
{	return *soap_faultdetail(this);
}

int IVehicleServiceServiceSoapBindingProxy::soap_close_socket()
{	return soap_closesock(this);
}

int IVehicleServiceServiceSoapBindingProxy::soap_force_close_socket()
{	return soap_force_closesock(this);
}

void IVehicleServiceServiceSoapBindingProxy::soap_print_fault(FILE *fd)
{	::soap_print_fault(this, fd);
}

#ifndef WITH_LEAN
#ifndef WITH_COMPAT
void IVehicleServiceServiceSoapBindingProxy::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this, os);
}
#endif

char *IVehicleServiceServiceSoapBindingProxy::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this, buf, len);
}
#endif

int IVehicleServiceServiceSoapBindingProxy::GetYCVehicle(const char *endpoint, const char *soap_action, ns1__GetYCVehicle *ns1__GetYCVehicle_, ns1__GetYCVehicleResponse &ns1__GetYCVehicleResponse_)
{	struct soap *soap = this;
	struct __ns1__GetYCVehicle soap_tmp___ns1__GetYCVehicle;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.141.32.1:9999/synchrodata/webservice//SynTocity";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__GetYCVehicle.ns1__GetYCVehicle_ = ns1__GetYCVehicle_;
	soap_serializeheader(soap);
	soap_serialize___ns1__GetYCVehicle(soap, &soap_tmp___ns1__GetYCVehicle);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__GetYCVehicle(soap, &soap_tmp___ns1__GetYCVehicle, "-ns1:GetYCVehicle", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__GetYCVehicle(soap, &soap_tmp___ns1__GetYCVehicle, "-ns1:GetYCVehicle", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__GetYCVehicleResponse_)
		return soap_closesock(soap);
	ns1__GetYCVehicleResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__GetYCVehicleResponse_.soap_get(soap, "ns1:GetYCVehicleResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int IVehicleServiceServiceSoapBindingProxy::getres(const char *endpoint, const char *soap_action, ns1__getres *ns1__getres_, ns1__getresResponse &ns1__getresResponse_)
{	struct soap *soap = this;
	struct __ns1__getres soap_tmp___ns1__getres;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.141.32.1:9999/synchrodata/webservice//SynTocity";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__getres.ns1__getres_ = ns1__getres_;
	soap_serializeheader(soap);
	soap_serialize___ns1__getres(soap, &soap_tmp___ns1__getres);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__getres(soap, &soap_tmp___ns1__getres, "-ns1:getres", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__getres(soap, &soap_tmp___ns1__getres, "-ns1:getres", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__getresResponse_)
		return soap_closesock(soap);
	ns1__getresResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__getresResponse_.soap_get(soap, "ns1:getresResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int IVehicleServiceServiceSoapBindingProxy::BeginCheck(const char *endpoint, const char *soap_action, ns1__BeginCheck *ns1__BeginCheck_, ns1__BeginCheckResponse &ns1__BeginCheckResponse_)
{	struct soap *soap = this;
	struct __ns1__BeginCheck soap_tmp___ns1__BeginCheck;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.141.32.1:9999/synchrodata/webservice//SynTocity";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__BeginCheck.ns1__BeginCheck_ = ns1__BeginCheck_;
	soap_serializeheader(soap);
	soap_serialize___ns1__BeginCheck(soap, &soap_tmp___ns1__BeginCheck);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__BeginCheck(soap, &soap_tmp___ns1__BeginCheck, "-ns1:BeginCheck", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__BeginCheck(soap, &soap_tmp___ns1__BeginCheck, "-ns1:BeginCheck", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__BeginCheckResponse_)
		return soap_closesock(soap);
	ns1__BeginCheckResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__BeginCheckResponse_.soap_get(soap, "ns1:BeginCheckResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int IVehicleServiceServiceSoapBindingProxy::UploadInspectionResult(const char *endpoint, const char *soap_action, ns1__UploadInspectionResult *ns1__UploadInspectionResult_, ns1__UploadInspectionResultResponse &ns1__UploadInspectionResultResponse_)
{	struct soap *soap = this;
	struct __ns1__UploadInspectionResult soap_tmp___ns1__UploadInspectionResult;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.141.32.1:9999/synchrodata/webservice//SynTocity";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__UploadInspectionResult.ns1__UploadInspectionResult_ = ns1__UploadInspectionResult_;
	soap_serializeheader(soap);
	soap_serialize___ns1__UploadInspectionResult(soap, &soap_tmp___ns1__UploadInspectionResult);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__UploadInspectionResult(soap, &soap_tmp___ns1__UploadInspectionResult, "-ns1:UploadInspectionResult", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__UploadInspectionResult(soap, &soap_tmp___ns1__UploadInspectionResult, "-ns1:UploadInspectionResult", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__UploadInspectionResultResponse_)
		return soap_closesock(soap);
	ns1__UploadInspectionResultResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__UploadInspectionResultResponse_.soap_get(soap, "ns1:UploadInspectionResultResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int IVehicleServiceServiceSoapBindingProxy::GetVehicle(const char *endpoint, const char *soap_action, ns1__GetVehicle *ns1__GetVehicle_, ns1__GetVehicleResponse &ns1__GetVehicleResponse_)
{	struct soap *soap = this;
	struct __ns1__GetVehicle soap_tmp___ns1__GetVehicle;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.141.32.1:9999/synchrodata/webservice//SynTocity";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__GetVehicle.ns1__GetVehicle_ = ns1__GetVehicle_;
	soap_serializeheader(soap);
	soap_serialize___ns1__GetVehicle(soap, &soap_tmp___ns1__GetVehicle);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__GetVehicle(soap, &soap_tmp___ns1__GetVehicle, "-ns1:GetVehicle", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__GetVehicle(soap, &soap_tmp___ns1__GetVehicle, "-ns1:GetVehicle", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__GetVehicleResponse_)
		return soap_closesock(soap);
	ns1__GetVehicleResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__GetVehicleResponse_.soap_get(soap, "ns1:GetVehicleResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int IVehicleServiceServiceSoapBindingProxy::vehiclelogin(const char *endpoint, const char *soap_action, ns1__vehiclelogin *ns1__vehiclelogin_, ns1__vehicleloginResponse &ns1__vehicleloginResponse_)
{	struct soap *soap = this;
	struct __ns1__vehiclelogin soap_tmp___ns1__vehiclelogin;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.141.32.1:9999/synchrodata/webservice//SynTocity";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__vehiclelogin.ns1__vehiclelogin_ = ns1__vehiclelogin_;
	soap_serializeheader(soap);
	soap_serialize___ns1__vehiclelogin(soap, &soap_tmp___ns1__vehiclelogin);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__vehiclelogin(soap, &soap_tmp___ns1__vehiclelogin, "-ns1:vehiclelogin", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__vehiclelogin(soap, &soap_tmp___ns1__vehiclelogin, "-ns1:vehiclelogin", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__vehicleloginResponse_)
		return soap_closesock(soap);
	ns1__vehicleloginResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__vehicleloginResponse_.soap_get(soap, "ns1:vehicleloginResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int IVehicleServiceServiceSoapBindingProxy::GetCheckList(const char *endpoint, const char *soap_action, ns1__GetCheckList *ns1__GetCheckList_, ns1__GetCheckListResponse &ns1__GetCheckListResponse_)
{	struct soap *soap = this;
	struct __ns1__GetCheckList soap_tmp___ns1__GetCheckList;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.141.32.1:9999/synchrodata/webservice//SynTocity";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__GetCheckList.ns1__GetCheckList_ = ns1__GetCheckList_;
	soap_serializeheader(soap);
	soap_serialize___ns1__GetCheckList(soap, &soap_tmp___ns1__GetCheckList);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__GetCheckList(soap, &soap_tmp___ns1__GetCheckList, "-ns1:GetCheckList", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__GetCheckList(soap, &soap_tmp___ns1__GetCheckList, "-ns1:GetCheckList", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__GetCheckListResponse_)
		return soap_closesock(soap);
	ns1__GetCheckListResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__GetCheckListResponse_.soap_get(soap, "ns1:GetCheckListResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int IVehicleServiceServiceSoapBindingProxy::validateJC(const char *endpoint, const char *soap_action, ns1__validateJC *ns1__validateJC_, ns1__validateJCResponse &ns1__validateJCResponse_)
{	struct soap *soap = this;
	struct __ns1__validateJC soap_tmp___ns1__validateJC;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.141.32.1:9999/synchrodata/webservice//SynTocity";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__validateJC.ns1__validateJC_ = ns1__validateJC_;
	soap_serializeheader(soap);
	soap_serialize___ns1__validateJC(soap, &soap_tmp___ns1__validateJC);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__validateJC(soap, &soap_tmp___ns1__validateJC, "-ns1:validateJC", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__validateJC(soap, &soap_tmp___ns1__validateJC, "-ns1:validateJC", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__validateJCResponse_)
		return soap_closesock(soap);
	ns1__validateJCResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__validateJCResponse_.soap_get(soap, "ns1:validateJCResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int IVehicleServiceServiceSoapBindingProxy::LoginServer(const char *endpoint, const char *soap_action, ns1__LoginServer *ns1__LoginServer_, ns1__LoginServerResponse &ns1__LoginServerResponse_)
{	struct soap *soap = this;
	struct __ns1__LoginServer soap_tmp___ns1__LoginServer;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.141.32.1:9999/synchrodata/webservice//SynTocity";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__LoginServer.ns1__LoginServer_ = ns1__LoginServer_;
	soap_serializeheader(soap);
	soap_serialize___ns1__LoginServer(soap, &soap_tmp___ns1__LoginServer);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__LoginServer(soap, &soap_tmp___ns1__LoginServer, "-ns1:LoginServer", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__LoginServer(soap, &soap_tmp___ns1__LoginServer, "-ns1:LoginServer", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__LoginServerResponse_)
		return soap_closesock(soap);
	ns1__LoginServerResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__LoginServerResponse_.soap_get(soap, "ns1:LoginServerResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int IVehicleServiceServiceSoapBindingProxy::GetBaseTypeInfo(const char *endpoint, const char *soap_action, ns1__GetBaseTypeInfo *ns1__GetBaseTypeInfo_, ns1__GetBaseTypeInfoResponse &ns1__GetBaseTypeInfoResponse_)
{	struct soap *soap = this;
	struct __ns1__GetBaseTypeInfo soap_tmp___ns1__GetBaseTypeInfo;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.141.32.1:9999/synchrodata/webservice//SynTocity";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__GetBaseTypeInfo.ns1__GetBaseTypeInfo_ = ns1__GetBaseTypeInfo_;
	soap_serializeheader(soap);
	soap_serialize___ns1__GetBaseTypeInfo(soap, &soap_tmp___ns1__GetBaseTypeInfo);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__GetBaseTypeInfo(soap, &soap_tmp___ns1__GetBaseTypeInfo, "-ns1:GetBaseTypeInfo", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__GetBaseTypeInfo(soap, &soap_tmp___ns1__GetBaseTypeInfo, "-ns1:GetBaseTypeInfo", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__GetBaseTypeInfoResponse_)
		return soap_closesock(soap);
	ns1__GetBaseTypeInfoResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__GetBaseTypeInfoResponse_.soap_get(soap, "ns1:GetBaseTypeInfoResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}
/* End of client proxy code */