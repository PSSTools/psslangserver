/*
 * FileManager.h
 *
 *  Created on: Oct 7, 2020
 *      Author: ballance
 */

#pragma once
#include <string>
#include <vector>
#include "IIndexManagerListener.h"
#include "OpenFileInfo.h"
#include "WorkspaceFolderInfo.h"

namespace pls {

class IndexManager {
public:
	IndexManager();

	virtual ~IndexManager();

	void addListener(IIndexManagerListener *l) {
		m_listeners.push_back(l);
	}

	WorkspaceFolderInfo *addWorkspaceFolder(const std::string &uri);

	OpenFileInfo *openFile(
			const std::string 	&uri,
			const std::string	&content);

	OpenFileInfo *openFileChanged(
			const std::string 	&uri,
			const std::string	&content);

	void closeFile(
			const std::string 	&uri);

private:
	void discoverSource(
			WorkspaceFolderInfo 	*folder,
			const std::string		&path);

	void addSourceFile(
			WorkspaceFolderInfo 	*folder,
			const std::string		&path);

	void parseFile(
			FileInfo		*info,
			std::istream	*in);


private:
	std::vector<IIndexManagerListener *>			m_listeners;
	std::map<std::string, WorkspaceFolderInfoUP>	m_workspaceFolders;
	std::map<std::string, OpenFileInfoUP>			m_openFiles;
};

typedef std::unique_ptr<IndexManager> IndexManagerUP;

} /* namespace pls */

