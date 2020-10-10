/*
 * IFileManagerListener.h
 *
 *  Created on: Oct 7, 2020
 *      Author: ballance
 */

#pragma once
#include <memory>
#include <string>
#include "FileInfo.h"

namespace pls {

class IIndexManagerListener {
public:

	virtual ~IIndexManagerListener() { }

	/**
	 * Notification allows clients to query markers, etc
	 */
	virtual void fileParsed(FileInfo *info) = 0;


};

typedef std::shared_ptr<IIndexManagerListener> IFileManagerListenerSP;

}
