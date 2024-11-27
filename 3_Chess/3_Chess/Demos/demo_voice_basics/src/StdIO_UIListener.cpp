#include "StdIO_UIListener.h"
#include <iostream>

int EG_kbhit();

using namespace ExitGames::Common;

#ifdef _EG_WINDOWS_PLATFORM
#include <windows.h>
#include <conio.h>

int EG_kbhit()
{
	int res = _kbhit();
	if(res)
		return _getch();
	return res;
}

#else
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>

int EG_kbhit()
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF)
		return ch;

	return 0;
}
#endif

void StdIO_UIListener::write(const JString& str)
{
	wprintf(str.cstr());
#if defined _EG_MICROSOFT_PLATFORM
	OutputDebugStringW((str).cstr());
#endif
}

void StdIO_UIListener::writeLine(const JString& str)
{
	write(str + L"\n");
}

bool StdIO_UIListener::anyKeyPressed(void) const
{
	return !!EG_kbhit();
}

int StdIO_UIListener::getKeyPressed(void) const
{
	return EG_kbhit();
}

void StdIO_UIListener::onLibClosed(void)
{
	writeLine(L"- -Press any key to quit- ------------------\n");
	while(!anyKeyPressed());
}