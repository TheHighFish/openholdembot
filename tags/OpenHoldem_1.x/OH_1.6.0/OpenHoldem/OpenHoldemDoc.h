#ifndef INC_OPENHOLDEMDOC_H
#define INC_OPENHOLDEMDOC_H

// OpenHoldemDoc.h : interface of the COpenHoldemDoc class
//

#include "structs_defines.h"

class COpenHoldemDoc : public CDocument {
protected:
	COpenHoldemDoc();
	DECLARE_DYNCREATE(COpenHoldemDoc)
	DECLARE_MESSAGE_MAP()

public:
	static COpenHoldemDoc* GetDocument();

	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual ~COpenHoldemDoc();
	void WriteFormula(SFormula *f, CArchive& ar);
	void ReadFormula(SFormula *f, CArchive& ar);
};


#endif /* INC_OPENHOLDEMDOC_H */