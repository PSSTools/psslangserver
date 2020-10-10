/*
 * FileInfo.h
 *
 *  Created on: Oct 8, 2020
 *      Author: ballance
 */

#pragma once
#include <string>
#include <vector>

#include "Marker.h"

namespace pls {

class FileInfo {
public:
	FileInfo(const std::string &uri);

	virtual ~FileInfo();

	const std::string &uri() const { return m_uri; }

	void addMarker(const pssp::Marker &m);

	void clearMarkers();

	const std::vector<pssp::Marker> &markers() const {
		return m_markers;
	}

	uint32_t n_error() const { return m_n_error; }

	uint32_t n_warn() const { return m_n_warn; }

	uint32_t n_info() const { return m_n_info; }

	uint32_t n_hint() const { return m_n_hint; }


private:
	std::string					m_uri;
	std::vector<pssp::Marker>	m_markers;
	uint32_t					m_n_error;
	uint32_t					m_n_warn;
	uint32_t					m_n_info;
	uint32_t					m_n_hint;

};

typedef std::unique_ptr<FileInfo> FileInfoUP;

} /* namespace pls */

