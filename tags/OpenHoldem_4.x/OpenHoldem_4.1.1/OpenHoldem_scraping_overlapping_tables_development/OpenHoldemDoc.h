#ifndef INC_OPENHOLDEMDOC_H
#define INC_OPENHOLDEMDOC_H

// OpenHoldemDoc.h : interface of the COpenHoldemDoc class
//
class COpenHoldemDoc : public CDocument 
{
protected:
	COpenHoldemDoc();
	DECLARE_DYNCREATE(COpenHoldemDoc)
	DECLARE_MESSAGE_MAP()

public:
	static COpenHoldemDoc* GetDocument();
    BOOL SaveModified();
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual ~COpenHoldemDoc();

private:
	void ReadFormula(CArchive& ar);
};

#endif /* INC_OPENHOLDEMDOC_H */