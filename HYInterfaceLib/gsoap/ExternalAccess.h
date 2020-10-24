/* ExternalAccess.h
   Generated by wsdl2h 2.8.21 from ExternalAccess.wsdl and typemap.dat
   2019-11-27 05:00:05 GMT

   DO NOT INCLUDE THIS FILE DIRECTLY INTO YOUR PROJECT BUILDS
   USE THE soapcpp2-GENERATED SOURCE CODE FILES FOR YOUR PROJECT BUILDS

   gSOAP XML Web services tools.
   Copyright (C) 2000-2014 Robert van Engelen, Genivia Inc. All Rights Reserved.
   Part of this software is released under one of the following licenses:
   GPL or Genivia's license for commercial use.
*/

/** @page page_notes Notes

@note HINTS:
 - Run soapcpp2 on ExternalAccess.h to generate the SOAP/XML processing logic.
   Use soapcpp2 -I to specify paths for #import
   To build with STL, 'stlvector.h' is imported from 'import' dir in package.
   Use soapcpp2 -j to generate improved proxy and server classes.
 - Use wsdl2h -c and -s to generate pure C code or C++ code without STL.
 - Use 'typemap.dat' to control namespace bindings and type mappings.
   It is strongly recommended to customize the names of the namespace prefixes
   generated by wsdl2h. To do so, modify the prefix bindings in the Namespaces
   section below and add the modified lines to 'typemap.dat' to rerun wsdl2h.
 - Use Doxygen (www.doxygen.org) on this file to generate documentation.
 - Use wsdl2h -R to generate REST operations.
 - Use wsdl2h -nname to use name as the base namespace prefix instead of 'ns'.
 - Use wsdl2h -Nname for service prefix and produce multiple service bindings
 - Use wsdl2h -d to enable DOM support for xsd:anyType.
 - Use wsdl2h -g to auto-generate readers and writers for root elements.
 - Use wsdl2h -b to auto-generate bi-directional operations (duplex ops).
 - Struct/class members serialized as XML attributes are annotated with a '@'.
 - Struct/class members that have a special role are annotated with a '$'.

@warning
   DO NOT INCLUDE THIS ANNOTATED FILE DIRECTLY IN YOUR PROJECT SOURCE CODE.
   USE THE FILES GENERATED BY soapcpp2 FOR YOUR PROJECT'S SOURCE CODE:
   THE soapStub.h FILE CONTAINS THIS CONTENT WITHOUT ANNOTATIONS.

@copyright LICENSE:
@verbatim
--------------------------------------------------------------------------------
gSOAP XML Web services tools
Copyright (C) 2000-2014, Robert van Engelen, Genivia Inc. All Rights Reserved.

This software is released under one of the following licenses:
GPL or Genivia's license for commercial use.
--------------------------------------------------------------------------------
GPL license.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Author contact information:
engelen@genivia.com / engelen@acm.org

This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial-use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------
@endverbatim

*/


//gsoapopt w

/******************************************************************************\
 *                                                                            *
 * Definitions                                                                *
 *   http://tempuri.org/                                                      *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Import                                                                     *
 *                                                                            *
\******************************************************************************/


// STL vector containers (use option -s to remove STL dependency)
#import "stlvector.h"

/******************************************************************************\
 *                                                                            *
 * Schema Namespaces                                                          *
 *                                                                            *
\******************************************************************************/


/* NOTE:

It is strongly recommended to customize the names of the namespace prefixes
generated by wsdl2h. To do so, modify the prefix bindings below and add the
modified lines to typemap.dat to rerun wsdl2h:

ns1 = "http://tempuri.org/"

*/

#define SOAP_NAMESPACE_OF_ns1	"http://tempuri.org/"
//gsoap ns1   schema namespace:	http://tempuri.org/
//gsoap ns1   schema elementForm:	qualified
//gsoap ns1   schema attributeForm:	unqualified

/******************************************************************************\
 *                                                                            *
 * Built-in Schema Types and Top-Level Elements and Attributes                *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Forward Declarations                                                       *
 *                                                                            *
\******************************************************************************/


//  Forward declaration of class _ns1__wsInvokeInterface.
class _ns1__wsInvokeInterface;

//  Forward declaration of class _ns1__wsInvokeInterfaceResponse.
class _ns1__wsInvokeInterfaceResponse;

//  Forward declaration of class _ns1__GetAssemblyInfo.
class _ns1__GetAssemblyInfo;

//  Forward declaration of class _ns1__GetAssemblyInfoResponse.
class _ns1__GetAssemblyInfoResponse;


/******************************************************************************\
 *                                                                            *
 * Schema Types and Top-Level Elements and Attributes                         *
 *   http://tempuri.org/                                                      *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Schema Complex Types and Top-Level Elements                                *
 *   http://tempuri.org/                                                      *
 *                                                                            *
\******************************************************************************/

/// @brief Top-level root element "http://tempuri.org/":wsInvokeInterface
/// @brief "http://tempuri.org/":wsInvokeInterface is a complexType.
///
/// class _ns1__wsInvokeInterface operations:
/// - soap_new__ns1__wsInvokeInterface(soap*) allocate
/// - soap_new__ns1__wsInvokeInterface(soap*, int num) allocate array
/// - soap_new_req__ns1__wsInvokeInterface(soap*, ...) allocate, set required members
/// - soap_new_set__ns1__wsInvokeInterface(soap*, ...) allocate, set all public members
/// - int soap_read__ns1__wsInvokeInterface(soap*, _ns1__wsInvokeInterface*) deserialize from a stream
/// - int soap_write__ns1__wsInvokeInterface(soap, _ns1__wsInvokeInterface*) serialize to a stream
class _ns1__wsInvokeInterface
{ public:
/// Element "KEY" of XSD type xs:string.
    std::wstring*                        KEY                            0;	///< Optional element.
/// Element "JKID" of XSD type xs:string.
    std::wstring*                        JKID                           0;	///< Optional element.
/// Element "XML" of XSD type xs:string.
    std::wstring*                        XML_                           0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set).
    struct soap                         *soap                          ;
};

/// @brief Top-level root element "http://tempuri.org/":wsInvokeInterfaceResponse
/// @brief "http://tempuri.org/":wsInvokeInterfaceResponse is a complexType.
///
/// class _ns1__wsInvokeInterfaceResponse operations:
/// - soap_new__ns1__wsInvokeInterfaceResponse(soap*) allocate
/// - soap_new__ns1__wsInvokeInterfaceResponse(soap*, int num) allocate array
/// - soap_new_req__ns1__wsInvokeInterfaceResponse(soap*, ...) allocate, set required members
/// - soap_new_set__ns1__wsInvokeInterfaceResponse(soap*, ...) allocate, set all public members
/// - int soap_read__ns1__wsInvokeInterfaceResponse(soap*, _ns1__wsInvokeInterfaceResponse*) deserialize from a stream
/// - int soap_write__ns1__wsInvokeInterfaceResponse(soap, _ns1__wsInvokeInterfaceResponse*) serialize to a stream
class _ns1__wsInvokeInterfaceResponse
{ public:
/// Element "wsInvokeInterfaceResult" of XSD type xs:string.
    std::wstring*                        wsInvokeInterfaceResult        0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set).
    struct soap                         *soap                          ;
};

/// @brief Top-level root element "http://tempuri.org/":GetAssemblyInfo
/// @brief "http://tempuri.org/":GetAssemblyInfo is a complexType.
///
/// class _ns1__GetAssemblyInfo operations:
/// - soap_new__ns1__GetAssemblyInfo(soap*) allocate
/// - soap_new__ns1__GetAssemblyInfo(soap*, int num) allocate array
/// - soap_new_req__ns1__GetAssemblyInfo(soap*, ...) allocate, set required members
/// - soap_new_set__ns1__GetAssemblyInfo(soap*, ...) allocate, set all public members
/// - int soap_read__ns1__GetAssemblyInfo(soap*, _ns1__GetAssemblyInfo*) deserialize from a stream
/// - int soap_write__ns1__GetAssemblyInfo(soap, _ns1__GetAssemblyInfo*) serialize to a stream
class _ns1__GetAssemblyInfo
{ public:
/// A handle to the soap struct that manages this instance (automatically set).
    struct soap                         *soap                          ;
};

/// @brief Top-level root element "http://tempuri.org/":GetAssemblyInfoResponse
/// @brief "http://tempuri.org/":GetAssemblyInfoResponse is a complexType.
///
/// class _ns1__GetAssemblyInfoResponse operations:
/// - soap_new__ns1__GetAssemblyInfoResponse(soap*) allocate
/// - soap_new__ns1__GetAssemblyInfoResponse(soap*, int num) allocate array
/// - soap_new_req__ns1__GetAssemblyInfoResponse(soap*, ...) allocate, set required members
/// - soap_new_set__ns1__GetAssemblyInfoResponse(soap*, ...) allocate, set all public members
/// - int soap_read__ns1__GetAssemblyInfoResponse(soap*, _ns1__GetAssemblyInfoResponse*) deserialize from a stream
/// - int soap_write__ns1__GetAssemblyInfoResponse(soap, _ns1__GetAssemblyInfoResponse*) serialize to a stream
class _ns1__GetAssemblyInfoResponse
{ public:
/// Element "GetAssemblyInfoResult" of XSD type xs:string.
    std::wstring*                        GetAssemblyInfoResult          0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set).
    struct soap                         *soap                          ;
};


/******************************************************************************\
 *                                                                            *
 * Additional Top-Level Elements                                              *
 *   http://tempuri.org/                                                      *
 *                                                                            *
\******************************************************************************/

/// @brief Top-level root element "http://tempuri.org/":string of XSD type xs:string.
/// @note Use wsdl2h option -g to auto-generate a top-level root element declaration and processing code.


/******************************************************************************\
 *                                                                            *
 * Additional Top-Level Attributes                                            *
 *   http://tempuri.org/                                                      *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Services                                                                   *
 *                                                                            *
\******************************************************************************/

// This service supports SOAP 1.2 namespaces:
#import "soap12.h"

//gsoap ns1  service name:	ExternalAccessSoap 
//gsoap ns1  service type:	ExternalAccessSoap 
//gsoap ns1  service port:	http://192.168.10.150/jcz_jk/ExternalAccess.asmx 
//gsoap ns1  service namespace:	http://tempuri.org/ 
//gsoap ns1  service transport:	http://schemas.xmlsoap.org/soap/http 

/** @mainpage Service Definitions

@section Service_bindings Service Bindings

  - @ref ExternalAccessSoap

@section Service_more More Information

  - @ref page_notes "Notes"

  - @ref page_XMLDataBinding "XML Data Binding"

  - @ref SOAP_ENV__Header "SOAP Header Content" (when applicable)

  - @ref SOAP_ENV__Detail "SOAP Fault Detail Content" (when applicable)


*/

/**

@page ExternalAccessSoap Binding "ExternalAccessSoap"

@section ExternalAccessSoap_operations Operations of Binding "ExternalAccessSoap"

  - @ref __ns1__wsInvokeInterface

  - @ref __ns1__GetAssemblyInfo

  - @ref __ns1__wsInvokeInterface_

  - @ref __ns1__GetAssemblyInfo_

@section ExternalAccessSoap_ports Endpoints of Binding "ExternalAccessSoap"

  - http://192.168.10.150/jcz_jk/ExternalAccess.asmx

@note Multiple service bindings collected as one, use wsdl2h option -Nname to produce a separate service for each binding


*/

/******************************************************************************\
 *                                                                            *
 * Service Binding                                                            *
 *   ExternalAccessSoap                                                       *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Service Operation                                                          *
 *   __ns1__wsInvokeInterface                                                 *
 *                                                                            *
\******************************************************************************/


/** Operation "__ns1__wsInvokeInterface" of service binding "ExternalAccessSoap".

  - SOAP document/literal style messaging

  - Action: "http://tempuri.org/wsInvokeInterface"

  - Addressing input action: "http://tempuri.org/wsInvokeInterface"

  - Addressing output action: "http://tempuri.org/wsInvokeInterfaceResponse"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__wsInvokeInterface(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // input parameters:
    _ns1__wsInvokeInterface*            ns1__wsInvokeInterface,
    // output parameters:
    _ns1__wsInvokeInterfaceResponse    &ns1__wsInvokeInterfaceResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__wsInvokeInterface(
    struct soap *soap,
    // input parameters:
    _ns1__wsInvokeInterface*            ns1__wsInvokeInterface,
    // output parameters:
    _ns1__wsInvokeInterfaceResponse    &ns1__wsInvokeInterfaceResponse
  );
@endcode

C++ proxy class (defined in soapExternalAccessSoapProxy.h):
@code
  class ExternalAccessSoapProxy;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate improved and easy-to-use proxy classes;

C++ service class (defined in soapExternalAccessSoapService.h):
@code
  class ExternalAccessSoapService;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate improved and easy-to-use service classes;

*/

//gsoap ns1  service method-protocol:	wsInvokeInterface SOAP
//gsoap ns1  service method-style:	wsInvokeInterface document
//gsoap ns1  service method-encoding:	wsInvokeInterface literal
//gsoap ns1  service method-input-action:	wsInvokeInterface http://tempuri.org/wsInvokeInterface
//gsoap ns1  service method-output-action:	wsInvokeInterface http://tempuri.org/wsInvokeInterfaceResponse
int __ns1__wsInvokeInterface(
    _ns1__wsInvokeInterface*            ns1__wsInvokeInterface,	///< Input parameter
    _ns1__wsInvokeInterfaceResponse    &ns1__wsInvokeInterfaceResponse	///< Output parameter
);

/******************************************************************************\
 *                                                                            *
 * Service Operation                                                          *
 *   __ns1__GetAssemblyInfo                                                   *
 *                                                                            *
\******************************************************************************/


/** Operation "__ns1__GetAssemblyInfo" of service binding "ExternalAccessSoap".

  - SOAP document/literal style messaging

  - Action: "http://tempuri.org/GetAssemblyInfo"

  - Addressing input action: "http://tempuri.org/GetAssemblyInfo"

  - Addressing output action: "http://tempuri.org/GetAssemblyInfoResponse"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__GetAssemblyInfo(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // input parameters:
    _ns1__GetAssemblyInfo*              ns1__GetAssemblyInfo,
    // output parameters:
    _ns1__GetAssemblyInfoResponse      &ns1__GetAssemblyInfoResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__GetAssemblyInfo(
    struct soap *soap,
    // input parameters:
    _ns1__GetAssemblyInfo*              ns1__GetAssemblyInfo,
    // output parameters:
    _ns1__GetAssemblyInfoResponse      &ns1__GetAssemblyInfoResponse
  );
@endcode

C++ proxy class (defined in soapExternalAccessSoapProxy.h):
@code
  class ExternalAccessSoapProxy;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate improved and easy-to-use proxy classes;

C++ service class (defined in soapExternalAccessSoapService.h):
@code
  class ExternalAccessSoapService;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate improved and easy-to-use service classes;

*/

//gsoap ns1  service method-protocol:	GetAssemblyInfo SOAP
//gsoap ns1  service method-style:	GetAssemblyInfo document
//gsoap ns1  service method-encoding:	GetAssemblyInfo literal
//gsoap ns1  service method-input-action:	GetAssemblyInfo http://tempuri.org/GetAssemblyInfo
//gsoap ns1  service method-output-action:	GetAssemblyInfo http://tempuri.org/GetAssemblyInfoResponse
int __ns1__GetAssemblyInfo(
    _ns1__GetAssemblyInfo*              ns1__GetAssemblyInfo,	///< Input parameter
    _ns1__GetAssemblyInfoResponse      &ns1__GetAssemblyInfoResponse	///< Output parameter
);

/******************************************************************************\
 *                                                                            *
 * Service Operation                                                          *
 *   __ns1__wsInvokeInterface_                                                *
 *                                                                            *
\******************************************************************************/


/** Operation "__ns1__wsInvokeInterface_" of service binding "ExternalAccessSoap".

  - SOAP document/literal style messaging

  - Action: "http://tempuri.org/wsInvokeInterface"

  - Addressing input action: "http://tempuri.org/wsInvokeInterface"

  - Addressing output action: "http://tempuri.org/wsInvokeInterfaceResponse"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__wsInvokeInterface_(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // input parameters:
    _ns1__wsInvokeInterface*            ns1__wsInvokeInterface,
    // output parameters:
    _ns1__wsInvokeInterfaceResponse    &ns1__wsInvokeInterfaceResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__wsInvokeInterface_(
    struct soap *soap,
    // input parameters:
    _ns1__wsInvokeInterface*            ns1__wsInvokeInterface,
    // output parameters:
    _ns1__wsInvokeInterfaceResponse    &ns1__wsInvokeInterfaceResponse
  );
@endcode

C++ proxy class (defined in soapExternalAccessSoapProxy.h):
@code
  class ExternalAccessSoapProxy;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate improved and easy-to-use proxy classes;

C++ service class (defined in soapExternalAccessSoapService.h):
@code
  class ExternalAccessSoapService;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate improved and easy-to-use service classes;

*/

//gsoap ns1  service method-protocol:	wsInvokeInterface_ SOAP
//gsoap ns1  service method-style:	wsInvokeInterface_ document
//gsoap ns1  service method-encoding:	wsInvokeInterface_ literal
//gsoap ns1  service method-input-action:	wsInvokeInterface_ http://tempuri.org/wsInvokeInterface
//gsoap ns1  service method-output-action:	wsInvokeInterface_ http://tempuri.org/wsInvokeInterfaceResponse
int __ns1__wsInvokeInterface_(
    _ns1__wsInvokeInterface*            ns1__wsInvokeInterface,	///< Input parameter
    _ns1__wsInvokeInterfaceResponse    &ns1__wsInvokeInterfaceResponse	///< Output parameter
);

/******************************************************************************\
 *                                                                            *
 * Service Operation                                                          *
 *   __ns1__GetAssemblyInfo_                                                  *
 *                                                                            *
\******************************************************************************/


/** Operation "__ns1__GetAssemblyInfo_" of service binding "ExternalAccessSoap".

  - SOAP document/literal style messaging

  - Action: "http://tempuri.org/GetAssemblyInfo"

  - Addressing input action: "http://tempuri.org/GetAssemblyInfo"

  - Addressing output action: "http://tempuri.org/GetAssemblyInfoResponse"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__GetAssemblyInfo_(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // input parameters:
    _ns1__GetAssemblyInfo*              ns1__GetAssemblyInfo,
    // output parameters:
    _ns1__GetAssemblyInfoResponse      &ns1__GetAssemblyInfoResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__GetAssemblyInfo_(
    struct soap *soap,
    // input parameters:
    _ns1__GetAssemblyInfo*              ns1__GetAssemblyInfo,
    // output parameters:
    _ns1__GetAssemblyInfoResponse      &ns1__GetAssemblyInfoResponse
  );
@endcode

C++ proxy class (defined in soapExternalAccessSoapProxy.h):
@code
  class ExternalAccessSoapProxy;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate improved and easy-to-use proxy classes;

C++ service class (defined in soapExternalAccessSoapService.h):
@code
  class ExternalAccessSoapService;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate improved and easy-to-use service classes;

*/

//gsoap ns1  service method-protocol:	GetAssemblyInfo_ SOAP
//gsoap ns1  service method-style:	GetAssemblyInfo_ document
//gsoap ns1  service method-encoding:	GetAssemblyInfo_ literal
//gsoap ns1  service method-input-action:	GetAssemblyInfo_ http://tempuri.org/GetAssemblyInfo
//gsoap ns1  service method-output-action:	GetAssemblyInfo_ http://tempuri.org/GetAssemblyInfoResponse
int __ns1__GetAssemblyInfo_(
    _ns1__GetAssemblyInfo*              ns1__GetAssemblyInfo,	///< Input parameter
    _ns1__GetAssemblyInfoResponse      &ns1__GetAssemblyInfoResponse	///< Output parameter
);

/**

@page ExternalAccessSoap Binding "ExternalAccessSoap"

@section ExternalAccessSoap_policy_enablers Policy Enablers of Binding "ExternalAccessSoap"

Based on policies, this service imports

  - WS-Policy reminders and enablers:
    - WS-Addressing 1.0 (2005/08, accepts 2004/08):
	@code
	#import "wsa5.h" // to be added to this header file for the soapcpp2 build step
	@endcode
	@code
	#include "plugin/wsaapi.h"
	soap_register_plugin(soap, soap_wsa); // register the wsa plugin in your code
	// See the user guide gsoap/doc/wsa/html/index.html
	@endcode
    - WS-Addressing (2004/08):
	@code
	#import "wsa.h" // to be added to this header file for the soapcpp2 build step
	@endcode
	@code
	#include "plugin/wsaapi.h"
	soap_register_plugin(soap, soap_wsa); // register the wsa plugin in your code
	// See the user guide gsoap/doc/wsa/html/index.html
	@endcode
    - WS-ReliableMessaging 1.0:
	@code
	#import "wsrm5.h" // to be added to this header file for the soapcpp2 build step
	@endcode
	@code
	#include "plugin/wsrmapi.h"
	soap_register_plugin(soap, soap_wsa); // register the wsa plugin in your code
	soap_register_plugin(soap, soap_wsrm); // register the wsrm plugin in your code
	// See the user guide gsoap/doc/wsrm/html/index.html
	@endcode
    - WS-ReliableMessaging 1.1:
	@code
	#import "wsrm.h" // to be added to this header file for the soapcpp2 build step
	@endcode
	@code
	#include "plugin/wsrmapi.h"
	soap_register_plugin(soap, soap_wsa); // register the wsa plugin in your code
	soap_register_plugin(soap, soap_wsrm); // register the wsrm plugin in your code
	// See the user guide gsoap/doc/wsrm/html/index.html
	@endcode
    - WS-Security (SOAP Message Security) 1.0 (accepts 1.1):
	@code
	#import "wsse.h" // to be added to this header file for the soapcpp2 build step
	@endcode
	@code
	#include "plugin/wsseapi.h"
	soap_register_plugin(soap, soap_wsse); // register the wsse plugin in your code
	// See the user guide gsoap/doc/wsse/html/index.html
	@endcode
    - WS-Security (SOAP Message Security) 1.1 (accepts 1.0):
	@code
	#import "wsse11.h" // to be added to this header file for the soapcpp2 build step
	@endcode
	@code
	#include "plugin/wsseapi.h"
	soap_register_plugin(soap, soap_wsse); // register the wsse plugin in your code
	// See the user guide gsoap/doc/wsse/html/index.html
	@endcode
    - HTTP Digest Authentication:
	@code
	#include "plugin/httpda.h"
	soap_register_plugin(soap, soap_http_da); // register the HTTP DA plugin in your code
	// See the user guide gsoap/doc/httpda/html/index.html
	@endcode
*/


/******************************************************************************\
 *                                                                            *
 * XML Data Binding                                                           *
 *                                                                            *
\******************************************************************************/


/**

@page page_XMLDataBinding XML Data Binding

SOAP/XML services use data bindings contractually bound by WSDL and auto-
generated by wsdl2h and soapcpp2 (see Service Bindings). Plain data bindings
are adopted from XML schemas as part of the WSDL types section or when running
wsdl2h on a set of schemas to produce non-SOAP-based XML data bindings.

The following readers and writers are C/C++ data type (de)serializers auto-
generated by wsdl2h and soapcpp2. Run soapcpp2 on this file to generate the
(de)serialization code, which is stored in soapC.c[pp]. Include "soapH.h" in
your code to import these data type and function declarations. Only use the
soapcpp2-generated files in your project build. Do not include the wsdl2h-
generated .h file in your code.

Data can be read in XML and deserialized from:
  - a file descriptor, using soap->recvfd = fd
  - a socket, using soap->socket = ...
  - a C++ stream, using soap->is = ...
  - a buffer, using the soap->frecv() callback

Data can be serialized in XML and written to:
  - a file descriptor, using soap->sendfd = fd
  - a socket, using soap->socket = ...
  - a C++ stream, using soap->os = ...
  - a buffer, using the soap->fsend() callback

The following options are available for (de)serialization control:
  - soap->encodingStyle = NULL; to remove SOAP 1.1/1.2 encodingStyle
  - soap_mode(soap, SOAP_XML_TREE); XML without id-ref (no cycles!)
  - soap_mode(soap, SOAP_XML_GRAPH); XML with id-ref (including cycles)
  - soap_set_namespaces(soap, struct Namespace *nsmap); to set xmlns bindings


@section ns1 Top-level root elements of schema "http://tempuri.org/"

  - <wsInvokeInterface> @ref _ns1__wsInvokeInterface
    @code
    // Reader (returns SOAP_OK on success):
    soap_read__ns1__wsInvokeInterface(struct soap*, _ns1__wsInvokeInterface*);
    // Writer (returns SOAP_OK on success):
    soap_write__ns1__wsInvokeInterface(struct soap*, _ns1__wsInvokeInterface*);
    @endcode

  - <wsInvokeInterfaceResponse> @ref _ns1__wsInvokeInterfaceResponse
    @code
    // Reader (returns SOAP_OK on success):
    soap_read__ns1__wsInvokeInterfaceResponse(struct soap*, _ns1__wsInvokeInterfaceResponse*);
    // Writer (returns SOAP_OK on success):
    soap_write__ns1__wsInvokeInterfaceResponse(struct soap*, _ns1__wsInvokeInterfaceResponse*);
    @endcode

  - <GetAssemblyInfo> @ref _ns1__GetAssemblyInfo
    @code
    // Reader (returns SOAP_OK on success):
    soap_read__ns1__GetAssemblyInfo(struct soap*, _ns1__GetAssemblyInfo*);
    // Writer (returns SOAP_OK on success):
    soap_write__ns1__GetAssemblyInfo(struct soap*, _ns1__GetAssemblyInfo*);
    @endcode

  - <GetAssemblyInfoResponse> @ref _ns1__GetAssemblyInfoResponse
    @code
    // Reader (returns SOAP_OK on success):
    soap_read__ns1__GetAssemblyInfoResponse(struct soap*, _ns1__GetAssemblyInfoResponse*);
    // Writer (returns SOAP_OK on success):
    soap_write__ns1__GetAssemblyInfoResponse(struct soap*, _ns1__GetAssemblyInfoResponse*);
    @endcode

  - <string> (use wsdl2h option -g to auto-generate)

*/

/* End of ExternalAccess.h */
