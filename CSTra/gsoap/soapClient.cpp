/* soapClient.cpp
   Generated by gSOAP 2.8.21 from onvif.h

Copyright(C) 2000-2014, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "soapH.h"

SOAP_SOURCE_STAMP("@(#) soapClient.cpp ver 2.8.21 2020-10-29 12:22:28 GMT")


SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__write(struct soap *soap, const char *soap_endpoint, const char *soap_action, ns1__write *ns1__write_, ns1__writeResponse &ns1__writeResponse_)
{	struct __ns1__write soap_tmp___ns1__write;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.26.163.44:18081/car-apiService/services/VeptsOutAccess.asmx";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__write.ns1__write_ = ns1__write_;
	soap_serializeheader(soap);
	soap_serialize___ns1__write(soap, &soap_tmp___ns1__write);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__write(soap, &soap_tmp___ns1__write, "-ns1:write", NULL)
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
	 || soap_put___ns1__write(soap, &soap_tmp___ns1__write, "-ns1:write", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__writeResponse_)
		return soap_closesock(soap);
	ns1__writeResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__writeResponse_.soap_get(soap, "ns1:writeResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__query(struct soap *soap, const char *soap_endpoint, const char *soap_action, ns1__query *ns1__query_, ns1__queryResponse &ns1__queryResponse_)
{	struct __ns1__query soap_tmp___ns1__query;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.26.163.44:18081/car-apiService/services/VeptsOutAccess.asmx";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__query.ns1__query_ = ns1__query_;
	soap_serializeheader(soap);
	soap_serialize___ns1__query(soap, &soap_tmp___ns1__query);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__query(soap, &soap_tmp___ns1__query, "-ns1:query", NULL)
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
	 || soap_put___ns1__query(soap, &soap_tmp___ns1__query, "-ns1:query", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__queryResponse_)
		return soap_closesock(soap);
	ns1__queryResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__queryResponse_.soap_get(soap, "ns1:queryResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__write_(struct soap *soap, const char *soap_endpoint, const char *soap_action, ns1__write *ns1__write_, ns1__writeResponse &ns1__writeResponse_)
{	struct __ns1__write_ soap_tmp___ns1__write_;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.26.163.44:18081/car-apiService/services/VeptsOutAccess.asmx";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__write_.ns1__write_ = ns1__write_;
	soap_serializeheader(soap);
	soap_serialize___ns1__write_(soap, &soap_tmp___ns1__write_);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__write_(soap, &soap_tmp___ns1__write_, "-ns1:write", NULL)
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
	 || soap_put___ns1__write_(soap, &soap_tmp___ns1__write_, "-ns1:write", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__writeResponse_)
		return soap_closesock(soap);
	ns1__writeResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__writeResponse_.soap_get(soap, "ns1:writeResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__query_(struct soap *soap, const char *soap_endpoint, const char *soap_action, ns1__query *ns1__query_, ns1__queryResponse &ns1__queryResponse_)
{	struct __ns1__query_ soap_tmp___ns1__query_;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://172.26.163.44:18081/car-apiService/services/VeptsOutAccess.asmx";
	if (soap_action == NULL)
		soap_action = "";
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_tmp___ns1__query_.ns1__query_ = ns1__query_;
	soap_serializeheader(soap);
	soap_serialize___ns1__query_(soap, &soap_tmp___ns1__query_);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__query_(soap, &soap_tmp___ns1__query_, "-ns1:query", NULL)
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
	 || soap_put___ns1__query_(soap, &soap_tmp___ns1__query_, "-ns1:query", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&ns1__queryResponse_)
		return soap_closesock(soap);
	ns1__queryResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__queryResponse_.soap_get(soap, "ns1:queryResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

/* End of soapClient.cpp */
