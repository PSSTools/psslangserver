/*
 * PSSLangServer.cpp
 *
 *  Created on: Oct 4, 2020
 *      Author: ballance
 */

#include "PSSLangServer.h"
#include "TextDocumentSyncOptions.h"

PSSLangServer::PSSLangServer() {
	// TODO Auto-generated constructor stub

}

PSSLangServer::~PSSLangServer() {
	// TODO Auto-generated destructor stub
}

lls::ServerCapabilitiesSP PSSLangServer::initialize(
			lls::InitializeParamsSP params) {
	lls::ServerCapabilitiesSP capabilities = lls::ServerCapabilities::mk();

	lls::TextDocumentSyncOptionsSP textDocumentSync =
			lls::TextDocumentSyncOptions::mk(
					lls::ValBool::true_v,
					lls::TextDocumentSyncKind::Full);
	capabilities->textDocumentSync(textDocumentSync);

	return capabilities;
}

