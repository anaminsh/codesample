class CNatPersonDoc : public CListData{

 

private:

       CNatPersonDoc(const CNatPersonDoc &);

 

protected:

       bool                       isDelete;

 

       int                               m_PersonId;

       int                               m_DocId;

       int                               m_DocKindId;

       CString                           m_strDocKind;

       CString                           m_strDocSerial;

       CString                           m_strDocNumber;

       CString                           m_strIssueOrg;

       CString                           m_strFirstName;

       CString                           m_strLastName;

       CString                           m_strPatrName;

       TIMESTAMP_STRUCT    m_IssueDate;

       TIMESTAMP_STRUCT    m_BeginDate;

       CString                           m_OKSMcode;

 

public:

       CNatPersonDoc() :

             isDelete(false),

             m_PersonId(-1),

             m_DocId(-1),

             m_DocKindId(-1),

             m_strDocKind(""),

             m_strDocSerial(""),

             m_strDocNumber(""),

             m_strIssueOrg(""),

             m_strFirstName(""),

             m_strPatrName(""),

             m_strLastName(""),

             m_OKSMcode("")

       {

             GetCurrentTimeStamp(m_IssueDate);

             GetCurrentTimeStamp(m_BeginDate);

       };

       virtual             ~CNatPersonDoc(void){};

 

       void         operator<<(CRecordset& rs);

 

       INT_PTR             ShowEditDlg(CDialog*);

       bool         SaveRecord(CDialog * _dlg);

       bool         Delete(void){ isDelete = true; return true; };

       bool         SaveToDB(SQLHSTMT &hstmt, int NatPersonID);

      

       bool                              GetIsDelete(void)                 { return isDelete; };

       int                                     GetPersonId(void) const           { return m_PersonId; };

       int                                     GetDocId(void) const       { return m_DocId; };

       int                                     GetId(void) const                 { return m_DocId; };

       int                                     GetDocKindId(void) const   { return m_DocKindId; };

       const CString              GetDocKind(void) const            { return m_strDocKind; };

       const CString              GetDocSerial(void) const   { return m_strDocSerial; };

       const CString              GetDocNumber(void) const   { return m_strDocNumber; };

       const CString              GetIssueOrg(void) const           { return m_strIssueOrg; };

       const CString              GetFirstName(void) const   { return m_strFirstName; };

       const CString              GetLastName(void) const           { return m_strLastName; };

       const CString              GetPatrName(void) const           { return m_strPatrName; };

       const TIMESTAMP_STRUCT     GetIssueDate(void) const   { return m_IssueDate; };

       const TIMESTAMP_STRUCT     GetBeginDate(void) const   { return m_BeginDate; };

       const CString              GetOKSMcode(void) const           { return m_OKSMcode; };

 

};

 

INT_PTR CNatPersonDoc::ShowEditDlg(CDialog* Dlg)

{

       ASSERT(Dlg);

       static_cast<CNatPersonDocEdit*>(Dlg)->SetEditData(this);

       return Dlg->DoModal();

}

 

bool  CNatPersonDoc::SaveRecord(CDialog * _Dlg){

       if (_Dlg){

             CNatPersonDocEdit * Dlg = (CNatPersonDocEdit *)_Dlg;

             m_PersonId = Dlg->m_NatPersonId;

             m_DocId = Dlg->m_DocId;

             m_DocKindId = Dlg->m_DocKindId;

             m_strDocKind = Dlg->m_DocKindName;

             m_strDocSerial = Dlg->m_DocSerial;

             m_strDocNumber = Dlg->m_DocNumber;

             m_strIssueOrg = Dlg->m_IssueOrg;

             m_strFirstName = Dlg->m_FirstName;

             m_strLastName = Dlg->m_LastName;

             m_strPatrName = Dlg->m_PatrName;

             m_IssueDate = Dlg->m_IssueDateTS;

             m_BeginDate = Dlg->m_BeginDateTS;

             m_OKSMcode = Dlg->m_OKSMCode;

             return true;

       }

       return false;

}

bool CNatPersonDoc::SaveToDB(SQLHSTMT &hstmt, int NatPersonID){

       SQLRETURN retcode = SQL_ERROR;

       CString query = "";

 

       if (isDelete)

       {

             query.Format("DELETE FROM dbo.tbl_DrivingLicences WHERE ID_document = %d ", m_DocId);

             retcode = theApp.db.SQLExecDirect(hstmt, (SQLCHAR*)(LPCSTR)query, SQL_NTS);

             query.Format("DELETE FROM dbo.tbl_Documents WHERE ID_document = %d", m_DocId);

             retcode = theApp.db.SQLExecDirect(hstmt, (SQLCHAR*)(LPCSTR)query, SQL_NTS);

       }

       else

       {

             retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &NatPersonID, 0, NULL); //m_PersonId

             retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, 0, 0, (SQLCHAR*)(LPCSTR)m_strFirstName, 0, NULL);

             retcode = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, 0, 0, (SQLCHAR*)(LPCSTR)m_strLastName, 0, NULL);

             retcode = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, 0, 0, (SQLCHAR*)(LPCSTR)m_strPatrName, 0, NULL);

 

             retcode = SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &m_DocKindId, 0, NULL);

             retcode = SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &m_DocId, 0, NULL);

             retcode = SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, 0, 0, (SQLCHAR*)(LPCSTR)m_strDocSerial, 0, NULL);

             retcode = SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, 0, 0, (SQLCHAR*)(LPCSTR)m_strDocNumber, 0, NULL);

 

             retcode = SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, 0, 0, (SQLCHAR*)(LPCSTR)m_strIssueOrg, 0, NULL);

             retcode = SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_TIMESTAMP, SQL_C_TIMESTAMP, 23, 0, &m_IssueDate, 0, NULL);

             retcode = SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_TIMESTAMP, SQL_C_TIMESTAMP, 23, 0, &m_BeginDate, 0, NULL);

 

             retcode = SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, 0, 0, (SQLCHAR*)(LPCSTR)m_OKSMcode, 0, NULL);

 

             retcode = theApp.db.SQLExecDirect(hstmt, (SQLCHAR*)(LPCSTR)"{call dbo.prc_Add_NatPersonDoc(?,?,?,?,?,?,?,?,?,?,?,? )}", SQL_NTS);

       }

 

       if (retcode == SQL_ERROR)

             theApp.DumpError(SQL_HANDLE_STMT, hstmt);

       return (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO);

}

 

class CNatPersonDocList : public CBaseList{

private:

       CNatPersonDocList(const CNatPersonDocList &){};

public:

       CNatPersonDocList(ListType value) :CBaseList(value){};

       CNatPersonDocList() :CBaseList(person_docs){};

       virtual ~CNatPersonDocList(){};

 

       void         operator>>(CListCtrlEx& view);

       void         operator>>(CComboBoxAC& view);

       bool         Load(int);

 

       bool         AddNewItem(CPerson* person);

       bool         EditItem(CListData*);

       bool         DeleteItem(CListData*);

       bool         SaveToDB(SQLHSTMT &hstmt, int NatPersonID);

};

 

bool CNatPersonDocList::Load(int persn_id){

       if (theApp.db.IsOpen())

       {

             theApp.DoWaitCursor(1);

             CRecordset rs(&theApp.db);

 

             try

             {

                    CString str;

                    str.Format(sqlGetNatPersonDoc, persn_id);

                    rs.Open(CRecordset::forwardOnly, str, CRecordset::readOnly);

             }

             catch (CDBException* pException)

             {

                    AfxMessageBox(pException->m_strError, MB_ICONEXCLAMATION);

                    pException->Delete();

                    rs.Close();

                    return FALSE;

             }

             pStatus->SetPaneText(4, "Загрузка справочника документов");

             Sleep(100);

 

             if (!lstData.empty())

             {

                    for_each(lstData.begin(), lstData.end(), DeleteObj());

                    lstData.erase(lstData.begin(), lstData.end());

             }

 

             while (!rs.IsEOF())

             {

                    CNatPersonDoc* DocData = new CNatPersonDoc();

                    *DocData << rs;

                    ASSERT(DocData);

                    lstData.push_back(DocData);

                    rs.MoveNext();

             }

             rs.Close();

       }

       else

             return false;

       return true;

}

 

void CNatPersonDocList::operator>>(CListCtrlEx& view){

 

       view.DeleteAllItems();

       for (int i = view.GetColumnCount() - 1; i >= 0; i--) view.DeleteColumn(i);

       view.InsertColumn(0, "Тип документа", LVCFMT_LEFT, 100);

       view.InsertColumn(1, "Серия", LVCFMT_LEFT, 20);

       view.InsertColumn(2, "Номер", LVCFMT_LEFT, 20);

      

       int nIndex = 0, nItem;

       for (list <CListData*>::iterator itDoc = lstData.begin(); itDoc != lstData.end(); itDoc++)

       {

             if (!((CNatPersonDoc*)(*itDoc))->GetIsDelete()){

                    nItem = view.InsertItem(nIndex, ((CNatPersonDoc*)(*itDoc))->GetDocKind());

                    view.SetItemText(nItem, 1, ((CNatPersonDoc*)(*itDoc))->GetDocSerial());

                    view.SetItemText(nItem, 2, ((CNatPersonDoc*)(*itDoc))->GetDocNumber());

                    view.SetItemData(nItem, reinterpret_cast<DWORD_PTR>(*itDoc));

                    nIndex++;

             }

            

       }

       view.AdjustColumnWidth();

}

void CNatPersonDocList::operator>>(CComboBoxAC& view)

{

       short nIndex = 0, Index = 0;

       for (list < CListData * > ::iterator itData = lstData.begin(); itData != lstData.end(); itData++)

       {

             CString strDocKind = ((CNatPersonDoc*)(*itData))->GetDocKind();

             CString strDocSerial = ((CNatPersonDoc*)(*itData))->GetDocSerial();

             CString strDocNumber = ((CNatPersonDoc*)(*itData))->GetDocNumber();

             nIndex = view.AddString(strDocKind +" ( "+ strDocSerial + " / " + strDocNumber + " )");

             view.SetItemData(nIndex, reinterpret_cast<DWORD_PTR>(*itData));

       }

}

//

 

bool CNatPersonDocList::AddNewItem(CPerson* person)

{

       CNatPersonDoc* NewDoc = new CNatPersonDoc();

       CNatPersonDocEdit Dlg("Добавление документа физ.лица");

       Dlg.SetPerson(person);

 

       if (IDOK == NewDoc->ShowEditDlg(&Dlg) && NewDoc->SaveRecord(&Dlg))

       {

             lstData.push_back(NewDoc);

             return true;

       }

       else

       {

             delete NewDoc;

             NewDoc = NULL;

       }

       return false;

}

 

bool CNatPersonDocList::EditItem(CListData * Data)

{

       CNatPersonDoc* Doc = (CNatPersonDoc*)Data;

       ASSERT(Doc);

       CNatPersonDocEdit Dlg("Редактирование документа физ.лица");

       return (Doc->ShowEditDlg(&Dlg) == IDOK && Doc->SaveRecord(&Dlg));

}

 

bool CNatPersonDocList::DeleteItem(CListData * Data){

       if (((CNatPersonDoc*)Data)->GetDocId() > -1)

             return ((CNatPersonDoc*)Data)->Delete();

       else{

             for (list <CListData*>::iterator itData = lstData.begin(); itData != lstData.end(); itData++){

                    if (*itData == Data){

                           delete Data;

                           lstData.erase(itData);

                           return true;

                    }

             }

       }

       return false;

}

bool CNatPersonDocList::SaveToDB(SQLHSTMT &hstmt, int NatPersonID){

       list < CListData* > ::iterator itData;

       for (itData = lstData.begin(); itData != lstData.end(); itData++){

             if (!((CNatPersonDoc*)(*itData))->SaveToDB(hstmt, NatPersonID))

                    return false;

       }

       return true;

}