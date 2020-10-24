/*
 * FileInfoMarkerCollector.cpp
 *
 *  Created on: Oct 10, 2020
 *      Author: ballance
 */

#include <string.h>
#include "FileInfoMarkerCollector.h"

namespace pls {

FileInfoMarkerCollector::FileInfoMarkerCollector(FileInfo *info) : m_info(info) {
	// TODO Auto-generated constructor stub
	memset(m_counts, 0, sizeof(m_counts));
}

FileInfoMarkerCollector::~FileInfoMarkerCollector() {
	// TODO Auto-generated destructor stub
}

void FileInfoMarkerCollector::marker(const pssp::Marker &m) {
	m_info->addMarker(m);
}

bool FileInfoMarkerCollector::hasSeverity(pssp::MarkerSeverityE s) {
	return m_counts[s];
}

} /* namespace pls */
