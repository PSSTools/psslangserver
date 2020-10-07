/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/*
 * PSSLangServer.cpp
 *
 *  Created on: Oct 4, 2020
 *      Author: ballance
 */

#include "PSSLangServer.h"
#include "nlohmann/json.hpp"
#include "TextDocumentSyncOptions.h"
#include "Position.h"
#include "ValInt.h"
#include "ValStr.h"

PSSLangServer::PSSLangServer() : m_connection(0) {
	// TODO Auto-generated constructor stub

}

PSSLangServer::~PSSLangServer() {
	// TODO Auto-generated destructor stub
}

lls::ServerCapabilitiesSP PSSLangServer::initialize(
		lls::IClientConnection		*connection,
		lls::InitializeParamsSP 	params) {
	m_connection = connection;
	lls::ServerCapabilitiesSP capabilities = lls::ServerCapabilities::mk();

	lls::TextDocumentSyncOptionsSP textDocumentSync =
			lls::TextDocumentSyncOptions::mk(
					lls::ValBool::true_v,
					lls::TextDocumentSyncKind::Full);
	capabilities->textDocumentSync(textDocumentSync);

	return capabilities;
}

void PSSLangServer::didChangeTextDocument(
			lls::DidChangeTextDocumentParamsSP params) {
	fprintf(stdout, "didChangeTextDocument: %s\n", params->textDocument()->uri()->val().c_str());
	fflush(stdout);

	lls::ValVector<lls::Diagnostic>::SP diagnostics =
			lls::ValVector<lls::Diagnostic>::mk();
	fprintf(stdout, "create diagnostics\n");
	fflush(stdout);
	diagnostics->push_back(lls::Diagnostic::mk(
			lls::Range::mk(
					lls::Position::mk(lls::ValInt::mk(4), lls::ValInt::mk(2)),
					lls::Position::mk(lls::ValInt::mk(4), lls::ValInt::mk(8))),
			lls::ValStr::mk(std::string("Error here"))));
	fprintf(stdout, "create diagnostics (2)\n");
	fflush(stdout);
	lls::PublishDiagnosticsParamsSP diagnostic_p =
			lls::PublishDiagnosticsParams::mk(
					params->textDocument()->uri(),
					diagnostics);

	fprintf(stdout, "diagnostic_p: %s\n", diagnostic_p->dump().dump().c_str());

	fprintf(stdout, "connection=%p\n", m_connection);
	fflush(stdout);
	m_connection->publishDiagnostics(diagnostic_p);
	fprintf(stdout, "<-- didChangeTextDocument: %s\n", params->textDocument()->uri()->val().c_str());
	fflush(stdout);
}

void PSSLangServer::didOpenTextDocument(
			lls::DidOpenTextDocumentParamsSP params) {
	fprintf(stdout, "--> didOpenTextDocument: %s\n",
			params->textDocument()->uri()->val().c_str());
	fflush(stdout);
	lls::DocumentSP doc = m_docmgr.openDocument(
			params->textDocument()->uri()->val(),
			params->textDocument()->text()->val());
	fprintf(stdout, "<-- didOpenTextDocument: %s\n",
			params->textDocument()->uri()->val().c_str());
	fflush(stdout);

}

