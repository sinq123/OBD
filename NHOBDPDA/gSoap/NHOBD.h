//gsoap ns service name: NHOBD
//gsoap ns service protocol:    SOAP
//gsoap ns service style:    rpc
//gsoap ns service encoding:    encoded
//gsoap ns schema namespace:    urn:NHOBD

int ns__queryObjectOut(
		std::wstring _jkid,					///< Input parameter
		std::wstring _QueryXmlDoc,			///< Input parameter
		std::wstring &_queryObjectOutReturn	///< Output parameter
		);
