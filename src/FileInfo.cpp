/*
 * FileInfo.cpp
 *
 *  Created on: Oct 8, 2020
 *      Author: ballance
 */

#include "FileInfo.h"

namespace pls {

FileInfo::FileInfo(const std::string &uri) : m_uri(uri),
		m_n_error(0), m_n_warn(0), m_n_info(0), m_n_hint(0) {
	// TODO Auto-generated constructor stub

}

FileInfo::~FileInfo() {
	// TODO Auto-generated destructor stub
}

void FileInfo::addMarker(const pssp::Marker &m) {
	m_markers.push_back(m);
	switch (m.severity()) {
	case pssp::Severity_Error: m_n_error++; break;
	case pssp::Severity_Warn: m_n_warn++; break;
	case pssp::Severity_Info: m_n_info++; break;
	case pssp::Severity_Hint: m_n_hint++; break;
	}
}

void FileInfo::clearMarkers() {
	m_n_error = 0;
	m_n_warn = 0;
	m_n_info = 0;
	m_n_hint = 0;
	m_markers.clear();
}

} /* namespace pls */
