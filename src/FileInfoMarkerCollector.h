/*
 * FileInfoMarkerCollector.h
 *
 *  Created on: Oct 10, 2020
 *      Author: ballance
 */

#pragma once
#include "FileInfo.h"
#include "IMarkerListener.h"

namespace pls {

class FileInfoMarkerCollector : public virtual pssp::IMarkerListener {
public:
	FileInfoMarkerCollector(FileInfo *info);

	virtual ~FileInfoMarkerCollector();

	virtual void marker(const pssp::Marker &m) override;

private:
	FileInfo					*m_info;
};

} /* namespace pls */

