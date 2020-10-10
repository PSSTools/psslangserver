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

	for (std::map<std::string,FileInfoUP>::const_iterator
			it=folder_i->files().begin(); it!=folder_i->files().end(); it++) {
		fprintf(stdout, "File: %s\n", it->first.c_str());
	}

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
			fprintf(stdout, "fullpath: %s\n", fullpath.c_str());
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
	FileInfoMarkerCollector collector(info);
	pssp::AstBuilder ast_builder(&collector);


	ast_builder.build(in);

	// Trigger a file-parsed notification
	for (std::vector<IIndexManagerListener *>::const_iterator
			it=m_listeners.begin(); it!=m_listeners.end(); it++) {
		(*it)->fileParsed(info);
	}

	DEBUG_LEAVE("parseFile: %s", info->uri().c_str());
}

} /* namespace pls */
