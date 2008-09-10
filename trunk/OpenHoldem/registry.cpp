#include "stdafx.h"
#include "registry.h"

#define OH_SUBKEY  "Software\\OpenHoldem\\OpenHoldem"

Registry::Registry()
{
	// Create a registry key for OH,
	// if it does not exist
	
	DWORD		dwDisp;
    RegCreateKeyEx(HKEY_CURRENT_USER, OH_SUBKEY, 0, NULL,
                   REG_OPTION_NON_VOLATILE, 
				   (KEY_WRITE | KEY_QUERY_VALUE | KEY_SET_VALUE),  
				   NULL, &hKey, &dwDisp);
}

Registry::~Registry()
{
    RegCloseKey(hKey);
}



void Registry::Read_OH_RegistryKey(LPCTSTR RegistryKey, int *RegistryValue)
{
    __SEH_HEADER

    DWORD		dwType, cbData;
    char		str[256];
    LONG		hkResult;

    cbData = sizeof(str);
    if ( (hkResult = RegQueryValueEx(hKey, RegistryKey, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
        *RegistryValue = atoi(str);
    //  Otherwise: Keep default value.

    __SEH_LOGFATAL("Registry::Read_OH_RegistryKey : \n");
}


void Registry::Read_OH_RegistryKey(LPCTSTR RegistryKey, bool *RegistryValue)
{
    __SEH_HEADER

    DWORD		dwType, cbData;
    char		str[256];
    LONG		hkResult;

    cbData = sizeof(str);
    if ( (hkResult = RegQueryValueEx(hKey, RegistryKey, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
        *RegistryValue = atoi(str);
    //  Otherwise: Keep default value.

    __SEH_LOGFATAL("Registry::Read_OH_RegistryKey : \n");
}


void Registry::Read_OH_RegistryKey(LPCTSTR RegistryKey, unsigned int *RegistryValue)
{
    __SEH_HEADER

    DWORD		dwType, cbData;
    char		str[256];
    LONG		hkResult;

    cbData = sizeof(str);
    if ( (hkResult = RegQueryValueEx(hKey, RegistryKey, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
        *RegistryValue = atoi(str);
    //  Otherwise: Keep default value.

    __SEH_LOGFATAL("Registry::Read_OH_RegistryKey : \n");
}


void Registry::Read_OH_RegistryKey(LPCTSTR RegistryKey,  CString *RegistryValue)
{
    __SEH_HEADER

    DWORD		dwType, cbData;
    char		str[256];
    LONG		hkResult;

    cbData = sizeof(str);
    if ( (hkResult = RegQueryValueEx(hKey, RegistryKey, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
        *RegistryValue = str;
    //  Otherwise: Keep default value.

    __SEH_LOGFATAL("Registry::Read_OH_RegistryKey : \n");
}


void Registry::Read_OH_RegistryKey(LPCTSTR RegistryKey, double *RegistryValue)
{
    __SEH_HEADER

    DWORD		dwType, cbData;
    char		str[256];
    LONG		hkResult;

    cbData = sizeof(str);
    if ( (hkResult = RegQueryValueEx(hKey, RegistryKey, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
        *RegistryValue = atof(str);
    //  Otherwise: Keep default value.

    __SEH_LOGFATAL("Registry::Read_OH_RegistryKey : \n");
}


void Registry::Write_OH_RegistryKey(LPCTSTR RegistryKey, int RegistryValue)
//  This method is used to write unsigned ints and booleans, too.
{
    __SEH_HEADER

    char		str[256];

    sprintf_s(str, 256, "%d", RegistryValue);
    RegSetValueEx(hKey, RegistryKey, 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

    __SEH_LOGFATAL("Registry::Write_OH_RegistryKey : \n");
}


void Registry::Write_OH_RegistryKey(LPCTSTR RegistryKey, CString RegistryValue)
{
    __SEH_HEADER

    char		str[256];

    sprintf_s(str, 256, "%s", RegistryValue);
    RegSetValueEx(hKey, RegistryKey, 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

    __SEH_LOGFATAL("Registry::Write_OH_RegistryKey : \n");
}


void Registry::Write_OH_RegistryKey(LPCTSTR RegistryKey, double RegistryValue)
{
    __SEH_HEADER

    char		str[256];

    sprintf_s(str, 256, "%f", RegistryValue);
    RegSetValueEx(hKey, RegistryKey, 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

    __SEH_LOGFATAL("Registry::Write_OH_RegistryKey : \n");
}

bool Registry::Get_Profile_Font(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font, CDC* pDC)
{
    __SEH_HEADER

    CWinApp *pApp = AfxGetApp();
    ASSERT_VALID(pApp);
    CString s = pApp->GetProfileString(lpszKey, lpszVal);
    if (s.IsEmpty())
        return FALSE;

    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));
    lf.lfCharSet = DEFAULT_CHARSET;
    int bItalic;
    int iPtSize;

	if (sscanf_s(s.GetString(), "%[a-zA-Z ],%d,%d,%d",
               lf.lfFaceName, 32, &iPtSize, &lf.lfWeight, &bItalic) != 4)
        return FALSE;
    lf.lfHeight = MulDiv(-iPtSize, 	// convert ptsize to logical units
                         ::GetDeviceCaps(pDC ? pDC->m_hDC : ::GetDC(NULL), LOGPIXELSY), 72);
    lf.lfItalic = bItalic;	// because lf.lfItalic is a BYTE
    font.DeleteObject();		// bye
    return font.CreateFontIndirect(&lf)!=0;

    __SEH_LOGFATAL("Registry::GetProfileFont : \n");
}


bool Registry::Write_Profile_Font(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font, CDC* pDC)
{
    __SEH_HEADER

    CWinApp *pApp = AfxGetApp();
    ASSERT_VALID(pApp);
    LOGFONT lf;
    font.GetLogFont(&lf);
    int iPtSize = MulDiv(-lf.lfHeight, 72,
                         ::GetDeviceCaps(pDC ? pDC->m_hDC : ::GetDC(NULL), LOGPIXELSY));
    CString s;
    s.Format("%s,%d,%d,%d", lf.lfFaceName, iPtSize, lf.lfWeight, lf.lfItalic);
    return pApp->WriteProfileString(lpszKey, lpszVal, s)!=0;

    __SEH_LOGFATAL("Registry::WriteProfileFont : \n");
}

void Registry::Read_Reg_String(CString RegistryKey, char* RegistryValue)
{
    __SEH_HEADER

    HKEY				hKey;
    LONG				result;
    DWORD				strSize = MAX_PATH;

    result = RegOpenKeyEx(HKEY_CURRENT_USER, OH_SUBKEY, 0, KEY_READ, &hKey);
    if (result==ERROR_SUCCESS){
        RegQueryValueEx(hKey,RegistryKey,NULL,NULL,(LPBYTE)RegistryValue,&strSize);
    }
    RegCloseKey(hKey);

    __SEH_LOGFATAL("Registry::Read_Reg_String :\n");
}

void Registry::Write_Reg_String(CString RegistryKey, CString RegistryValue)
{
    __SEH_HEADER

    HKEY				hKey;
    LONG				result;
    char				str[MAX_PATH];

    result = RegCreateKeyEx(HKEY_CURRENT_USER, OH_SUBKEY, 0, NULL,
                            REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
    if (result==ERROR_SUCCESS){
        sprintf_s(str, 256, "%s", RegistryValue);
        RegSetValueEx(hKey, RegistryKey, 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
    }
    RegCloseKey(hKey);

    __SEH_LOGFATAL("Registry::Write_Reg_String :\n");
}
