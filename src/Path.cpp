/*
 * Path.cpp
 *
 *  Created on: Oct 9, 2020
 *      Author: ballance
 */

#include "Path.h"
#ifndef _WIN32
#include <dirent.h>
#else
#include <windows.h>
#endif
#include <stdio.h>
#include <sys/stat.h>

namespace pls {

Path::Path() {
	// TODO Auto-generated constructor stub

}

Path::~Path() {
	// TODO Auto-generated destructor stub
}

bool Path::is_dir(const std::string &path) {
	struct stat st;
	int ret = stat(path.c_str(), &st);

	return (ret == 0 && ((st.st_mode & S_IFMT) == S_IFDIR));
}

bool Path::is_file(const std::string &path) {
	struct stat st;
	return  (stat(path.c_str(), &st) == 0 && ((st.st_mode & S_IFMT) == S_IFREG));
}

std::string Path::ext(const std::string &path) {
	int32_t idx;
	if ((idx=path.rfind('.')) != std::string::npos && idx != 0) {
		return path.substr(idx);
	} else {
		return "";
	}
}

std::string Path::join(const std::string &root, const std::string &leaf) {
	return root + "/" + leaf;
}

std::vector<std::string> Path::list(const std::string &path) {
	std::vector<std::string> ret;
#ifndef _WIN32
	DIR *d;
	struct dirent *dir;
	d = opendir(path.c_str());
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			ret.push_back(dir->d_name);
		}
		closedir(d);
	}
#else
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(path.c_str(), &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			ret.push_back(FindFileData.cFileName);
		} while (FindNextFile(hFind, &FindFileData));
	}
#endif
	return ret;
}

} /* namespace pls */
