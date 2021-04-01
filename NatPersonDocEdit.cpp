// NatPersonDocEdit.cpp : implementation file
//

#include "stdafx.h"
#include "NatPersonDocEdit.h"
#include "afxdialogex.h"
#include "СNatuPersEdtDlg.h"


// CNatPersonDocEdit dialog

IMPLEMENT_DYNAMIC(CNatPersonDocEdit, CDialog)

CNatPersonDocEdit::CNatPersonDocEdit(CString Title, CWnd* pParent /*=NULL*/)
: CDialog(CNatPersonDocEdit::IDD, pParent)
	, m_NatPersonId(-1)
	, m_LastName(_T(""))
	, m_FirstName(_T(""))
	, m_PatrName(_T(""))
	, m_DocId(-1)
	, m_DocKindId(-1)
	, m_DocKindName(_T(""))
	, m_DocSerial(_T(""))
	, m_DocNumber(_T(""))
	, m_IssueOrg(_T(""))
	, m_OKSMCode(_T(""))
	, m_Title(Title)
{
	TIMESTAMP_STRUCT_EXT empty;
	m_BeginDateTS = empty;
	m_IssueDateTS = empty;
	m_BeginDate.SetDateTime(empty);	
	m_IssueDate.SetDateTime(empty);

}

CNatPersonDocEdit::~CNatPersonDocEdit()
{
}

void CNatPersonDocEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_NAME_LAST, m_LastName); DDV_MaxChars(pDX, m_LastName, 50);
	DDX_Text(pDX, IDC_NAME_FIRST, m_FirstName); DDV_MaxChars(pDX, m_FirstName, 50);
	DDX_Text(pDX, IDC_NAME_PART, m_PatrName); DDV_MaxChars(pDX, m_PatrName, 50);
	
	DDX_Control(pDX, IDC_DOC_KIND, m_DocKind);
	
	DDX_Text(pDX, IDC_DOC_SERIAL, m_DocSerial); DDV_MaxChars(pDX, m_DocSerial, 18);
	DDX_Text(pDX, IDC_DOC_NUMBER, m_DocNumber); DDV_MaxChars(pDX, m_DocNumber, 18);
	

	DDX_Control(pDX, IDC_DATE_ISSUE, m_IssueDate);
	DDX_Control(pDX, IDC_DATE_BEGIN, m_BeginDate);
		
	DDX_Text(pDX, IDC_ISSUE_ORG, m_IssueOrg); DDV_MaxChars(pDX, m_IssueOrg, 120);
	
	DDX_Control(pDX, IDC_COUNTRY, m_OKSM);
}


BEGIN_MESSAGE_MAP(CNatPersonDocEdit, CDialog)

	ON_CBN_SELCHANGE(IDC_DOC_KIND, &CNatPersonDocEdit::OnCbnSelchangeDocKind)
	ON_BN_CLICKED(IDOK, &CNatPersonDocEdit::OnBnClickedOk)

	ON_EN_CHANGE(IDC_DATE_BEGIN, &CNatPersonDocEdit::OnEnChangeDateBegin)
END_MESSAGE_MAP()

BOOL CNatPersonDocEdit::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->SetWindowTextA(m_Title);		
	
	m_NatPersonId = m_CurrDoc->GetPersonId();
	if (!m_NatPersonId == -1)	m_NatPersonId = m_CurrPerson->GetId();

	m_LastName = m_CurrDoc->GetLastName();
	m_FirstName = m_CurrDoc->GetFirstName();
	m_PatrName = m_CurrDoc->GetPatrName();
	if (m_CurrPerson != NULL) {
		if (m_LastName.IsEmpty())	m_LastName = m_CurrPerson->GetLastName();
		if (m_FirstName.IsEmpty())	m_FirstName = m_CurrPerson->GetFirstName();
		if (m_PatrName.IsEmpty())	m_PatrName = m_CurrPerson->GetPatrName();
	}
	
	m_DocId = m_CurrDoc->GetDocId();
	m_DocKindId = m_CurrDoc->GetDocKindId();
	m_DocKindName = m_CurrDoc->GetDocKind();

	m_DocSerial = m_CurrDoc->GetDocSerial();
	m_DocNumber = m_CurrDoc->GetDocNumber();
	m_IssueOrg = m_CurrDoc->GetIssueOrg();

	m_IssueDateTS = (m_CurrDoc->GetIssueDate());
	m_IssueDate.nTypeField = MET_DATE;
	m_IssueDate.SetDateTime(m_CurrDoc->GetIssueDate());
	
	m_BeginDateTS = (m_CurrDoc->GetBeginDate());
	m_BeginDate.nTypeField = MET_DATE;
	m_BeginDate.SetDateTime(m_CurrDoc->GetBeginDate());
	
	if (theApp.m_referens.LoadReference(oksm))
		*theApp.m_referens.GetReference(oksm) >> m_OKSM;
	m_OKSM.SetCBXDroppedWidth();
	m_OKSMCode = m_CurrDoc->GetOKSMcode();
	if (m_OKSMCode.IsEmpty() && m_DocId == -1 && !(m_DocKindId == 12 || m_DocKindId == 17))
		m_OKSMCode = "643";
	m_OKSM.SetCurrSellById(std::atoi(m_OKSMCode));
	m_OKSM.SetCBXDroppedWidth();

	if (theApp.m_referens.LoadReference(person_doc_kinds)) {
		theApp.m_referens.GetReference(person_doc_kinds)->LoadWithFilter((void*)(-1), m_DocKind, m_DocKindId);
	}		
	m_DocKind.SetCBXDroppedWidth();

	if (m_DocKindId > -1)	
		m_DocKind.SetCurrSellById(m_DocKindId);
	if (m_DocKindId != 2 && m_DocKindId != 12 /*&& m_DocKindId != 41 && m_DocKindId != 42*/)
	{
		m_BeginDate.SetWindowText("");
		m_BeginDate.SetReadOnly(true);
	}
	
	UpdateData(FALSE);
	return TRUE;  
	
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CNatPersonDocEdit::OnCbnSelchangeDocKind()
{
	m_DocKindId = ((CNatPersonDocKind*)m_DocKind.GetItemData(m_DocKind.GetCurSel()))->GetId();
	m_DocKindName = ((CNatPersonDocKind*)m_DocKind.GetItemData(m_DocKind.GetCurSel()))->GetKindName();
	
	if (m_BeginDate.IsValid()) m_BeginDateTS = m_BeginDate.GetTIMESTAMP();
	if (m_DocKindId != 2 && m_DocKindId != 12 /*&& m_DocKindId != 41 && m_DocKindId != 42*/)
	{
		m_BeginDate.SetWindowText("");
		m_BeginDate.SetReadOnly(true);	
	}
	else
	{
		m_BeginDate.SetReadOnly(false);
		m_BeginDate.SetDateTime(m_BeginDateTS);
	}		
 }

void CNatPersonDocEdit::OnBnClickedOk()
 {

	//CDialog::OnOK();

	UpdateData();

	m_LastName.TrimRight();
	m_FirstName.TrimRight();
	m_PatrName.TrimRight();

	m_DocSerial.TrimRight();
	m_DocNumber.TrimRight();
	m_IssueOrg.TrimRight();

	if (m_OKSM.GetCurSel() > 0)
	{
		m_OKSMCode = ((COKSM*)m_OKSM.GetItemData(m_OKSM.GetCurSel()))->GetOKSMCode();
	}
	else
	{
		m_OKSMCode = "";
	}	
	m_OKSMCode.TrimRight();

	m_IssueDateTS = m_IssueDate.GetTIMESTAMP();
	if (!m_BeginDate.IsEmpty())
		m_BeginDateTS = m_BeginDate.GetTIMESTAMP();

	bool Checked = CheckBeforeSaving();
	if (Checked)
	{
		CDialog::OnOK();
	}
}

bool CNatPersonDocEdit::CheckBeforeSaving(){

	if (m_FirstName.IsEmpty()){
		AfxMessageBox(_T("Не задано имя  физ. лица в документе."));
		GetDlgItem(IDC_NAME_FIRST)->SetFocus();
		return false;
	}
	if (m_LastName.IsEmpty()){
		AfxMessageBox(_T("Не задана фамилия физ. лица в документе."));
		GetDlgItem(IDC_NAME_LAST)->SetFocus();
		return false;
	}
	if (m_DocKindId < 0) {
		AfxMessageBox(_T("Не выбран тип документа."));
		m_DocKind.SetFocus();
		return false;
	}	
	if (m_DocSerial.IsEmpty()) {
		AfxMessageBox(_T("Не задана серия документа."));
		GetDlgItem(IDC_DOC_SERIAL)->SetFocus();
		return false;
	}
	if (m_DocNumber.IsEmpty()) {
		AfxMessageBox(_T("Не задан номер документа."));
		GetDlgItem(IDC_DOC_NUMBER)->SetFocus();
		return false;
	}
	if (m_IssueDate.IsEmpty() || !m_IssueDate.IsValid()){
		AfxMessageBox(_T("Не задана или некорректно задана дата выдачи."));
		m_IssueDate.SetFocus();
		return false;
	}
	if (!m_BeginDate.IsEmpty() && !m_BeginDate.IsValid()){
		AfxMessageBox(_T("Некорректно задана дата начала водительского стажа."));
		m_BeginDate.SetFocus();
		return false;
	}
	/*
	if (m_IssueOrg.IsEmpty()) {
		AfxMessageBox(_T("Не задано место выдачи."));
		GetDlgItem(IDC_ISSUE_ORG)->SetFocus();
		return false;
	}
	*/
	if ((m_DocKindId == 12 || m_DocKindId == 17) && m_OKSMCode.IsEmpty()) {
		AfxMessageBox(_T("Не выбрана страна."));
		m_OKSM.SetFocus();
		return false;
	}
	if (m_OKSM.GetCurSel() > 0 && ((COKSM*)m_OKSM.GetItemData(m_OKSM.GetCurSel()))->GetOKSMCode() == "643" && (m_DocKindId == 12 || m_DocKindId == 17))
	{
		AfxMessageBox(_T("Для этого типа документа страна не может быть 'РОССИЯ'"));
		m_OKSM.SetFocus();
		return false;
	}
	if (m_OKSM.GetCurSel() > 0 && ((COKSM*)m_OKSM.GetItemData(m_OKSM.GetCurSel()))->GetOKSMCode() != "643" && (m_DocKindId != 12 && m_DocKindId != 17))
	{
		AfxMessageBox(_T("Для этого типа документа страна должна быть 'РОССИЯ'"));
		m_OKSMCode = "643";
		m_OKSM.SetCurrSellById(std::atoi(m_OKSMCode));
		m_OKSM.SetFocus();
		return false;
	}
	return true;
}

void CNatPersonDocEdit::OnEnChangeDateBegin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
