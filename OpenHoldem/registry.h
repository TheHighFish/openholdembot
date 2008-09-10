#ifndef OPENHOLDEM_REGISTRY_H_
#define OPENHOLDEM_REGISTRY_H_

class Registry {
	//
	// Constructor and destructor
	//
public:
	Registry();
	~Registry();

	//
	// Public methods to access the profile fonts
	//
public:
	bool static Write_Profile_Font(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font, CDC* pDC=NULL);
	bool static Get_Profile_Font(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font, CDC* pDC=NULL);

	//
	// Public methods to read registry keys
	//
public:
	void Read_OH_RegistryKey(LPCTSTR RegistryKey, int *RegistryValue);
	void Read_OH_RegistryKey(LPCTSTR RegistryKey, bool *RegistryValue);
	void Read_OH_RegistryKey(LPCTSTR RegistryKey, unsigned int *RegistryValue);
	void Read_OH_RegistryKey(LPCTSTR RegistryKey, CString *RegistryValue);
	void Read_OH_RegistryKey(LPCTSTR RegistryKey, double *RegistryValue);

	//
	// Public methods to write registry keys
	//
public:
	void Write_OH_RegistryKey(LPCTSTR RegistryKey, int RegistryValue);
	void Write_OH_RegistryKey(LPCTSTR RegistryKey, CString RegistryValue);
	void Write_OH_RegistryKey(LPCTSTR RegistryKey, double RegistryValue);
	
	//
	// Public method to read registry keys.
	// Used e.g. to get the path for Open-File-Dialog.
	//
public:
	void static Read_Reg_String(CString RegistryKey, char* RegistryValue);
	void static Write_Reg_String(CString RegistryKey, CString RegistryValue);

private:
	HKEY hKey;
};

#endif // OPENHOLDEM_REGISTRY_H_