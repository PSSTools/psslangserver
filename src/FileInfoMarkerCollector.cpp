/*
 * FileInfoMarkerCollector.cpp
 *
 *  Created on: Oct 10, 2020
 *      Author: ballance
 */

#include "FileInfoMarkerCollector.h"

namespace pls {

FileInfoMarkerCollector::FileInfoMarkerCollector(FileInfo *info) : m_info(info) {
	// TODO Auto-generated constructor stub

}

FileInfoMarkerCollector::~FileInfoMarkerCollector() {
	// TODO Auto-generated destructor stub
}

void FileInfoMarkerCollector::marker(const pssp::Marker &m) {
	m_info->addMarker(m);
}

} /* namespace pls */
