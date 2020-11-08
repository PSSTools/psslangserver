/*
 * FileManager.cpp
 *
 *  Created on: Oct 7, 2020
 *      Author: ballance
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "IndexManager.h"
#include "FileInfoMarkerCollector.h"
#include "AstBuilder.h"
#include "Path.h"
#include "URI.h"

#define EN_DEBUG_INDEX_MANAGER

#ifdef EN_DEBUG_INDEX_MANAGER
#define DEBUG_ENTER(fmt, ...) \
	fprintf(stdout, "--> IndexManager::" fmt, ##__VA_ARGS__); \
	fprintf(stdout, "\n"); \
	fflush(stdout);
#define DEBUG_LEAVE(fmt, ...) \
	fprintf(stdout, "<-- IndexManager::" fmt, ##__VA_ARGS__); \
	fprintf(stdout, "\n"); \
	fflush(stdout);
#define DEBUG_MSG(fmt, ...) \
	fprintf(stdout, "IndexManager:: " fmt, ##__VA_ARGS__); \
	fprintf(stdout, "\n"); \
	fflush(stdout);
#else
#define DEBUG_ENTER(fmt, ...)
#define DEBUG_LEAVE(fmt, ...)
#define DEBUG_MSG(fmt, ...)
#endif

namespace pls {

IndexManager::IndexManager() {
	// TODO Auto-generated constructor stub

}

IndexManager::~IndexManager() {
	// TODO Auto-generated destructor stub
}

WorkspaceFolderInfo *IndexManager::addWorkspaceFolder(const std::string &uri) {
	WorkspaceFolderInfo *folder_i = new WorkspaceFolderInfo(uri);

	m_workspaceFolders.insert({uri, WorkspaceFolderInfoUP(folder_i)});
	discoverSource(folder_i, URI::path(uri));

	return folder_i;
}

OpenFileInfo *IndexManager::openFile(
		const std::string		&uri,
		const std::string		&content) {
	OpenFileInfo *info = new OpenFileInfo(uri, content);
	std::stringstream in(content);

	m_openFiles.insert({uri, OpenFileInfoUP(info)});
	parseFile(info, &in);

	return info;
}

OpenFileInfo *IndexManager::openFileChanged(
			const std::string 	&uri,
			const std::string	&content) {
	std::map<std::string,OpenFileInfoUP>::iterator it;

	if ((it=m_openFiles.find(uri)) != m_openFiles.end()) {
		it->second.get()->content(content);
		std::stringstream in(content);
		parseFile(
				it->second.get(),
				&in);

		return it->second.get();
	} else {
		return 0;
	}
}

void IndexManager::discoverSource(
		WorkspaceFolderInfo 	*folder,
		const std::string		&path) {
	DEBUG_ENTER("discoverSource: %s", path.c_str());

	DEBUG_MSG("path %s", path.c_str());

	std::vector<std::string> l = Path::list(path);
	for (std::vector<std::string>::const_iterator
			f_it=l.begin(); f_it!=l.end(); f_it++) {
		DEBUG_MSG("file: %s", f_it->c_str());
		if (*f_it != "." && *f_it != "..") {
			std::string fullpath = Path::join(path, *f_it);
			if (Path::is_dir(fullpath)) {
				DEBUG_MSG("directory %s", fullpath.c_str());
				discoverSource(folder, fullpath);
			} else if (Path::ext(*f_it) == ".pss") {
				addSourceFile(folder, fullpath);
			}
		}
	}

	DEBUG_LEAVE("discoverSource: %s", path.c_str());
}

void IndexManager::addSourceFile(
			WorkspaceFolderInfo 	*folder,
			const std::string		&path) {
	DEBUG_ENTER("addSourceFile: %s", path.c_str());
	FileInfo *info = new FileInfo("file://" + path);

	std::ifstream in;

	in.open(path, std::ifstream::in);
	parseFile(info, &in);


	in.close();


	DEBUG_LEAVE("addSourceFile: %s", path.c_str());
}

void IndexManager::parseFile(
			FileInfo		*info,
			std::istream	*in) {
	DEBUG_ENTER("parseFile: %s", info->uri().c_str());
	pssp::GlobalScopeUP global(new pssp::GlobalScope(0));

	info->clearMarkers();

	FileInfoMarkerCollector collector(info);
	pssp::AstBuilder ast_builder(&collector);


	ast_builder.build(global.get(), in);

	// Trigger a file-parsed notification
	for (std::vector<IIndexManagerListener *>::const_iterator
			it=m_listeners.begin(); it!=m_listeners.end(); it++) {
		(*it)->fileParsed(info);
	}

	DEBUG_LEAVE("parseFile: %s", info->uri().c_str());
}

} /* namespace pls */
