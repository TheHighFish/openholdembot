#ifndef INC_GLOBAL_H
#define INC_GLOBAL_H

extern class CGlobal 
{
public:
	CGlobal(void);
	~CGlobal(void);

	// Startup path
	char			startup_path[MAX_PATH];

} global;

#endif /* INC_GLOBAL_H */