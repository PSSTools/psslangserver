/*
 * WorkspaceFolderInfo.h
 *
 *  Created on: Oct 8, 2020
 *      Author: ballance
 */

#pragma once
#include <map>
#include <memory>
#include <string>
#include "FileInfo.h"

namespace pls {

class WorkspaceFolderInfo {
public:
	WorkspaceFolderInfo(const std::string &uri);

	virtual ~WorkspaceFolderInfo();

	const std::string &uri() const { return m_uri; }


	void addFile(FileInfo *file);

	void removeFile(const std::string &uri);

	const std::map<std::string,FileInfoUP> &files() const {
		return m_files;
	}

private:
	std::string							m_uri;
	std::map<std::string, FileInfoUP>	m_files;
};

typedef std::unique_ptr<WorkspaceFolderInfo> WorkspaceFolderInfoUP;

} /* namespace pls */

