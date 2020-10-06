/*
 * PSSLangServer.h
 *
 *  Created on: Oct 4, 2020
 *      Author: ballance
 */

#pragma once
#include "BaseLangServer.h"

class PSSLangServer : public lls::BaseLangServer {
public:
	PSSLangServer();

	virtual ~PSSLangServer();

	virtual lls::ServerCapabilitiesSP initialize(
			lls::InitializeParamsSP params) override;

};

