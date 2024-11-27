#pragma once

#include <memory>
#include "Common-cpp/inc/Common.h"

class UIListener
{
public:
	virtual ~UIListener(void) {};
	virtual void write(const ExitGames::Common::JString& str) = 0;
	virtual void writeLine(const ExitGames::Common::JString& str) = 0;
	virtual bool anyKeyPressed(void) const = 0;
	virtual int getKeyPressed(void) const = 0;
	virtual void onLibClosed(void) = 0;
};