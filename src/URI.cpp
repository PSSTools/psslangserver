/*
 * URI.cpp
 *
 *  Created on: Oct 9, 2020
 *      Author: ballance
 */

#include "URI.h"
#include <string.h>

namespace pls {

URI::URI() {
	// TODO Auto-generated constructor stub

}

URI::~URI() {
	// TODO Auto-generated destructor stub
}

std::string URI::path(const std::string &uri) {
	if (uri.find("file://") == 0) {
		return uri.substr(7);
	} else {
		return uri;
	}
}

} /* namespace pls */
