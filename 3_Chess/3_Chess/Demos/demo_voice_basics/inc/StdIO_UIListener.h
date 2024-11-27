#include "UIListener.h"

class StdIO_UIListener: public UIListener
{
private:
	virtual void write(const ExitGames::Common::JString& str);
	virtual void writeLine(const ExitGames::Common::JString& str);
	virtual bool anyKeyPressed(void) const;
	virtual int getKeyPressed(void) const;
	virtual void onLibClosed(void);
};