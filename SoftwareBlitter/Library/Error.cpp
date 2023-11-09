#include "Error.h"
#include <assert.h>
#include <windows.h>
#include <stdio.h> 

/*****************************************************************************************/
void DisplayGameError(const char* Message)
{
	MessageBox( 0, Message, "Error", MB_OK );
}

/*****************************************************************************************/
void DebugPrint(const char* chString, ...)
{
	static char	buffer[1024];
	char *args;

	args = (char*)&chString + sizeof(chString);
	vsprintf_s(buffer, 1024, chString, args);

	OutputDebugString(buffer);
}