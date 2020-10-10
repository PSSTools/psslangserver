/*
 * URI.h
 *
 *  Created on: Oct 9, 2020
 *      Author: ballance
 */

#pragma once
#include <string>

namespace pls {

class URI {
public:
	URI();
	virtual ~URI();

	static std::string path(const std::string &uri);
};

} /* namespace pls */

