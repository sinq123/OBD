/* VeptsOutAccess.h
   Generated by wsdl2h 2.8.21 from VeptsOutAccess.wsdl and typemap.dat
   2021-01-18 14:38:43 GMT

   DO NOT INCLUDE THIS FILE DIRECTLY INTO YOUR PROJECT BUILDS
   USE THE soapcpp2-GENERATED SOURCE CODE FILES FOR YOUR PROJECT BUILDS

   gSOAP XML Web services tools.
   Copyright (C) 2000-2014 Robert van Engelen, Genivia Inc. All Rights Reserved.
   Part of this software is released under one of the following licenses:
   GPL or Genivia's license for commercial use.
*/

/** @page page_notes Notes

@note HINTS:
 - Run soapcpp2 on VeptsOutAccess.h to generate the SOAP/XML processing logic.
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
 *   http://www.jc-jcode.com                                                  *
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

ns1 = "http://www.jc-jcode.com"

*/

#define SOAP_NAMESPACE_OF_ns1	"http://www.jc-jcode.com"
//gsoap ns1   schema namespace:	http://www.jc-jcode.com
//gsoap ns1   schema form:	unqualified

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


//  Forward declaration of class ns1__write.
class ns1__write;

//  Forward declaration of class ns1__writeResponse.
class ns1__writeResponse;

//  Forward declaration of class ns1__query.
class ns1__query;

//  Forward declaration of class ns1__queryResponse.
class ns1__queryResponse;


/******************************************************************************\
 *                                                                            *
 * Schema Types and Top-Level Elements and Attributes                         *
 *   http://www.jc-jcode.com                                                  *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Schema Complex Types and Top-Level Elements                                *
 *   http://www.jc-jcode.com                                                  *
 *                                                                            *
\******************************************************************************/

/// @brief "http://www.jc-jcode.com":write is a complexType.
///
/// class ns1__write operations:
/// - soap_new_ns1__write(soap*) allocate
/// - soap_new_ns1__write(soap*, int num) allocate array
/// - soap_new_req_ns1__write(soap*, ...) allocate, set required members
/// - soap_new_set_ns1__write(soap*, ...) allocate, set all public members
/// - int soap_read_ns1__write(soap*, ns1__write*) deserialize from a stream
/// - int soap_write_ns1__write(soap, ns1__write*) serialize to a stream
class ns1__write
{ public:
/// Element "xmlDoc" of XSD type xs:string.
    std::wstring*                        xmlDoc                         0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set).
    struct soap                         *soap                          ;
};

/// @brief "http://www.jc-jcode.com":writeResponse is a complexType.
///
/// class ns1__writeResponse operations:
/// - soap_new_ns1__writeResponse(soap*) allocate
/// - soap_new_ns1__writeResponse(soap*, int num) allocate array
/// - soap_new_req_ns1__writeResponse(soap*, ...) allocate, set required members
/// - soap_new_set_ns1__writeResponse(soap*, ...) allocate, set all public members
/// - int soap_read_ns1__writeResponse(soap*, ns1__writeResponse*) deserialize from a stream
/// - int soap_write_ns1__writeResponse(soap, ns1__writeResponse*) serialize to a stream
class ns1__writeResponse
{ public:
/// Element "return" of XSD type xs:string.
    std::wstring*                        return_                        0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set).
    struct soap                         *soap                          ;
};

/// @brief "http://www.jc-jcode.com":query is a complexType.
///
/// class ns1__query operations:
/// - soap_new_ns1__query(soap*) allocate
/// - soap_new_ns1__query(soap*, int num) allocate array
/// - soap_new_req_ns1__query(soap*, ...) allocate, set required members
/// - soap_new_set_ns1__query(soap*, ...) allocate, set all public members
/// - int soap_read_ns1__query(soap*, ns1__query*) deserialize from a stream
/// - int soap_write_ns1__query(soap, ns1__query*) serialize to a stream
class ns1__query
{ public:
/// Element "xmlDoc" of XSD type xs:string.
    std::wstring*                        xmlDoc                         0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set).
    struct soap                         *soap                          ;
};

/// @brief "http://www.jc-jcode.com":queryResponse is a complexType.
///
/// class ns1__queryResponse operations:
/// - soap_new_ns1__queryResponse(soap*) allocate
/// - soap_new_ns1__queryResponse(soap*, int num) allocate array
/// - soap_new_req_ns1__queryResponse(soap*, ...) allocate, set required members
/// - soap_new_set_ns1__queryResponse(soap*, ...) allocate, set all public members
/// - int soap_read_ns1__queryResponse(soap*, ns1__queryResponse*) deserialize from a stream
/// - int soap_write_ns1__queryResponse(soap, ns1__queryResponse*) serialize to a stream
class ns1__queryResponse
{ public:
/// Element "return" of XSD type xs:string.
    std::wstring*                        return_                        0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set).
    struct soap                         *soap                          ;
};


/******************************************************************************\
 *                                                                            *
 * Additional Top-Level Elements                                              *
 *   http://www.jc-jcode.com                                                  *
 *                                                                            *
\******************************************************************************/

/// @brief Top-level root element "http://www.jc-jcode.com":query of XSD type "http://www.jc-jcode.com":query.
/// @note Use wsdl2h option -g to auto-generate a top-level root element declaration and processing code.

/// @brief Top-level root element "http://www.jc-jcode.com":queryResponse of XSD type "http://www.jc-jcode.com":queryResponse.
/// @note Use wsdl2h option -g to auto-generate a top-level root element declaration and processing code.

/// @brief Top-level root element "http://www.jc-jcode.com":write of XSD type "http://www.jc-jcode.com":write.
/// @note Use wsdl2h option -g to auto-generate a top-level root element declaration and processing code.

/// @brief Top-level root element "http://www.jc-jcode.com":writeResponse of XSD type "http://www.jc-jcode.com":writeResponse.
/// @note Use wsdl2h option -g to auto-generate a top-level root element declaration and processing code.


/******************************************************************************\
 *                                                                            *
 * Additional Top-Level Attributes                                            *
 *   http://www.jc-jcode.com                                                  *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Services                                                                   *
 *                                                                            *
\******************************************************************************/


//gsoap ns1  service name:	WSServiceImplServiceSoapBinding 
//gsoap ns1  service type:	WSService 
//gsoap ns1  service port:	http://172.26.163.44:18081/car-apiService/services/VeptsOutAccess.asmx 
//gsoap ns1  service namespace:	http://www.jc-jcode.com 
//gsoap ns1  service transport:	http://schemas.xmlsoap.org/soap/http 

/** @mainpage WSServiceImplService Definitions

@section WSServiceImplService_bindings Service Bindings

  - @ref WSServiceImplServiceSoapBinding

@section WSServiceImplService_more More Information

  - @ref page_notes "Notes"

  - @ref page_XMLDataBinding "XML Data Binding"

  - @ref SOAP_ENV__Header "SOAP Header Content" (when applicable)

  - @ref SOAP_ENV__Detail "SOAP Fault Detail Content" (when applicable)


*/

/**

@page WSServiceImplServiceSoapBinding Binding "WSServiceImplServiceSoapBinding"

@section WSServiceImplServiceSoapBinding_operations Operations of Binding "WSServiceImplServiceSoapBinding"

  - @ref __ns1__write

  - @ref __ns1__query

@section WSServiceImplServiceSoapBinding_ports Endpoints of Binding "WSServiceImplServiceSoapBinding"

  - http://172.26.163.44:18081/car-apiService/services/VeptsOutAccess.asmx

@note Use wsdl2h option -Nname to change the service binding prefix name


*/

/******************************************************************************\
 *                                                                            *
 * Service Binding                                                            *
 *   WSServiceImplServiceSoapBinding                                          *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Service Operation                                                          *
 *   __ns1__write                                                             *
 *                                                                            *
\******************************************************************************/


/** Operation "__ns1__write" of service binding "WSServiceImplServiceSoapBinding".

  - SOAP document/literal style messaging

  - Addressing input action: ""

  - Addressing output action: "Response"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__write(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // input parameters:
    ns1__write*                         ns1__write_,
    // output parameters:
    ns1__writeResponse                 &ns1__writeResponse_
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__write(
    struct soap *soap,
    // input parameters:
    ns1__write*                         ns1__write_,
    // output parameters:
    ns1__writeResponse                 &ns1__writeResponse_
  );
@endcode

C++ proxy class (defined in soapWSServiceImplServiceSoapBindingProxy.h):
@code
  class WSServiceImplServiceSoapBindingProxy;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate improved and easy-to-use proxy classes;

C++ service class (defined in soapWSServiceImplServiceSoapBindingService.h):
@code
  class WSServiceImplServiceSoapBindingService;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate improved and easy-to-use service classes;

*/

//gsoap ns1  service method-protocol:	write SOAP
//gsoap ns1  service method-style:	write document
//gsoap ns1  service method-encoding:	write literal
//gsoap ns1  service method-action:	write ""
//gsoap ns1  service method-output-action:	write Response
int __ns1__write(
    ns1__write*                         ns1__write_,	///< Input parameter
    ns1__writeResponse                 &ns1__writeResponse_	///< Output parameter
);

/******************************************************************************\
 *                                                                            *
 * Service Operation                                                          *
 *   __ns1__query                                                             *
 *                                                                            *
\******************************************************************************/


/** Operation "__ns1__query" of service binding "WSServiceImplServiceSoapBinding".

  - SOAP document/literal style messaging

  - Addressing input action: ""

  - Addressing output action: "Response"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__query(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // input parameters:
    ns1__query*                         ns1__query_,
    // output parameters:
    ns1__queryResponse                 &ns1__queryResponse_
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__query(
    struct soap *soap,
    // input parameters:
    ns1__query*                         ns1__query_,
    // output parameters:
    ns1__queryResponse                 &ns1__queryResponse_
  );
@endcode

C++ proxy class (defined in soapWSServiceImplServiceSoapBindingProxy.h):
@code
  class WSServiceImplServiceSoapBindingProxy;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate improved and easy-to-use proxy classes;

C++ service class (defined in soapWSServiceImplServiceSoapBindingService.h):
@code
  class WSServiceImplServiceSoapBindingService;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate improved and easy-to-use service classes;

*/

//gsoap ns1  service method-protocol:	query SOAP
//gsoap ns1  service method-style:	query document
//gsoap ns1  service method-encoding:	query literal
//gsoap ns1  service method-action:	query ""
//gsoap ns1  service method-output-action:	query Response
int __ns1__query(
    ns1__query*                         ns1__query_,	///< Input parameter
    ns1__queryResponse                 &ns1__queryResponse_	///< Output parameter
);

/**

@page WSServiceImplServiceSoapBinding Binding "WSServiceImplServiceSoapBinding"

@section WSServiceImplServiceSoapBinding_policy_enablers Policy Enablers of Binding "WSServiceImplServiceSoapBinding"

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


@section ns1 Top-level root elements of schema "http://www.jc-jcode.com"

  - <query> (use wsdl2h option -g to auto-generate)

  - <queryResponse> (use wsdl2h option -g to auto-generate)

  - <write> (use wsdl2h option -g to auto-generate)

  - <writeResponse> (use wsdl2h option -g to auto-generate)

*/

/* End of VeptsOutAccess.h */
