#pragma once 
#include "dxstdafx.h"

//find resources
bool FindMediaSearchTypicalDirs( WCHAR* strSearchPath, int cchSearch, LPCWSTR strLeaf, WCHAR* strExePath, WCHAR* strExeName );
bool FindMediaSearchParentDirs( WCHAR* strSearchPath, int cchSearch, WCHAR* strStartAt, WCHAR* strLeafName );
WCHAR* MediaSearchPath();
HRESULT FindDXSDKMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename );

