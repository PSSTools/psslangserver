/*
 * OpenFileInfo.cpp
 *
 *  Created on: Oct 10, 2020
 *      Author: ballance
 */

#include "OpenFileInfo.h"

namespace pls {

OpenFileInfo::OpenFileInfo(
		const std::string		&uri,
		const std::string		&content) : FileInfo(uri), m_content(content) {
	// TODO Auto-generated constructor stub

}

OpenFileInfo::~OpenFileInfo() {
	// TODO Auto-generated destructor stub
}

} /* namespace pls */
