/*
 * FileInfoMarkerCollector.h
 *
 *  Created on: Oct 10, 2020
 *      Author: ballance
 */

#pragma once
#include <stdint.h>
#include "FileInfo.h"
#include "IMarkerListener.h"

namespace pls {

class FileInfoMarkerCollector : public virtual pssp::IMarkerListener {
public:
	FileInfoMarkerCollector(FileInfo *info);

	virtual ~FileInfoMarkerCollector();

	virtual void marker(const pssp::Marker &m) override;

	virtual bool hasSeverity(pssp::MarkerSeverityE s) override;

private:
	FileInfo					*m_info;
	uint32_t					m_counts[pssp::Severity_NumLevels];
};

} /* namespace pls */

