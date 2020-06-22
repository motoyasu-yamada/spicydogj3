#pragma once

CString QueryRegValue(HKEY hKey,LPCTSTR ptszKeyPath,LPCTSTR ptszValueName,LPCTSTR ptszDefaultValue);
void WriteRegKeyValue(HKEY hKey,LPCTSTR ptszKeyPath,LPCTSTR ptszValue,LPCTSTR ptszValueName);

