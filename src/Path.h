/*
 * Path.h
 *
 *  Created on: Oct 9, 2020
 *      Author: ballance
 */

#pragma once
#include <string>
#include <vector>

namespace pls {

class Path {
public:
	Path();

	virtual ~Path();

	static bool is_dir(const std::string &path);

	static bool is_file(const std::string &path);

	static std::string ext(const std::string &path);

	static std::string join(const std::string &root, const std::string &leaf);

	static std::vector<std::string> list(const std::string &path);
};

} /* namespace pls */

