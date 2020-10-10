/*
 * OpenFileInfo.h
 *
 *  Created on: Oct 10, 2020
 *      Author: ballance
 */

#pragma once
#include "FileInfo.h"

namespace pls {

class OpenFileInfo : public FileInfo {
public:
	OpenFileInfo(
			const std::string 	&uri,
			const std::string	&content);

	virtual ~OpenFileInfo();

	const std::string &content() const { return m_content; }

private:
	std::string				m_content;

};

typedef std::unique_ptr<OpenFileInfo> OpenFileInfoUP;

} /* namespace pls */

