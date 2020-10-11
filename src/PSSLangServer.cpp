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

#undef EN_DEBUG_PSS_LANGSERVER

#ifdef EN_DEBUG_INDEX_MANAGER
#define DEBUG_ENTER(fmt, ...) \
	fprintf(stdout, "--> PSSLangServer::" fmt, ##__VA_ARGS__); \
	fprintf(stdout, "\n"); \
	fflush(stdout);
#define DEBUG_LEAVE(fmt, ...) \
	fprintf(stdout, "<-- PSSLangServer::" fmt, ##__VA_ARGS__); \
	fprintf(stdout, "\n"); \
	fflush(stdout);
#define DEBUG_MSG(fmt, ...) \
	fprintf(stdout, "PSSLangServer:: " fmt, ##__VA_ARGS__); \
	fprintf(stdout, "\n"); \
	fflush(stdout);
#else
#define DEBUG_ENTER(fmt, ...)
#define DEBUG_LEAVE(fmt, ...)
#define DEBUG_MSG(fmt, ...)
#endif

namespace pls {

PSSLangServer::PSSLangServer() : m_connection(0),
		m_index_mgr(new IndexManager()) {
	m_index_mgr->addListener(this);
	// TODO Auto-generated constructor stub

}

PSSLangServer::~PSSLangServer() {
	// TODO Auto-generated destructor stub
}

lls::ServerCapabilitiesSP PSSLangServer::initialize(
		lls::IClientConnection		*connection,
		lls::InitializeParamsSP 	params) {
	DEBUG_ENTER("initialize");
	m_connection = connection;
	lls::ServerCapabilitiesSP capabilities = lls::ServerCapabilities::mk();

	lls::TextDocumentSyncOptionsSP textDocumentSync =
			lls::TextDocumentSyncOptions::mk(
					lls::ValBool::true_v,
					lls::TextDocumentSyncKind::Full);
	capabilities->textDocumentSync(textDocumentSync);

	WorkspaceFolderInfo *folder = m_index_mgr->addWorkspaceFolder(
			params->rootUri()->val());

	DEBUG_LEAVE("initialize");
	return capabilities;
}

void PSSLangServer::didChangeTextDocument(
			lls::DidChangeTextDocumentParamsSP params) {
	DEBUG_ENTER("didChangeTextDocument");

	lls::ValVector<lls::TextDocumentContentChangeEvent>::SP changes =
			params->contentChanges();

	OpenFileInfo *info = m_index_mgr->openFileChanged(
			params->textDocument()->uri()->val(),
			changes->children(0)->text()->val());

	DEBUG_LEAVE("didChangeTextDocument");
}

void PSSLangServer::didOpenTextDocument(
			lls::DidOpenTextDocumentParamsSP params) {
	DEBUG_ENTER("didOpenTextDocument");

	m_index_mgr->openFile(
			params->textDocument()->uri()->val(),
			params->textDocument()->text()->val());

	DEBUG_LEAVE("didOpenTextDocument");
}

void PSSLangServer::fileParsed(FileInfo *info) {
	DEBUG_ENTER("fileParsed");
	lls::ValVector<lls::Diagnostic>::SP diagnostics =
			lls::ValVector<lls::Diagnostic>::mk();

	for (std::vector<pssp::Marker>::const_iterator
			it=info->markers().begin(); it!=info->markers().end(); it++) {
		// Note: ANTLR line numbers are 1-offset while LSP line numbers
		// are 0-offset
		diagnostics->push_back(lls::Diagnostic::mk(
				lls::Range::mk(
						lls::Position::mk(
								lls::ValInt::mk(it->loc().lineno()-1),
								lls::ValInt::mk(it->loc().linepos())),
						lls::Position::mk(
								lls::ValInt::mk(it->loc().lineno()-1),
								lls::ValInt::mk(it->loc().linepos()))),
				lls::ValStr::mk(it->msg())));
	}

	lls::PublishDiagnosticsParamsSP diagnostic_p =
			lls::PublishDiagnosticsParams::mk(
					lls::ValStr::mk(info->uri()),
					diagnostics);
	m_connection->publishDiagnostics(diagnostic_p);
	DEBUG_LEAVE("fileParsed");
}

}
