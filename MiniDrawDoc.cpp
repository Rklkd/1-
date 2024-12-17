
// MiniDrawDoc.cpp: реализация класса CMiniDrawDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MiniDraw.h"
#endif

#include "MiniDrawDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMiniDrawDoc

IMPLEMENT_DYNCREATE(CMiniDrawDoc, CDocument)

BEGIN_MESSAGE_MAP(CMiniDrawDoc, CDocument)
	ON_COMMAND(ID_EDIT_CLEAR_ALL, &CMiniDrawDoc::OnEditClearAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, &CMiniDrawDoc::OnUpdateEditClearAll)
	ON_COMMAND(ID_EDIT_UNDO, &CMiniDrawDoc::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CMiniDrawDoc::OnUpdateEditUndo)
END_MESSAGE_MAP()


// Создание или уничтожение CMiniDrawDoc

CMiniDrawDoc::CMiniDrawDoc() noexcept
{
	// TODO: добавьте код для одноразового вызова конструктора

}

CMiniDrawDoc::~CMiniDrawDoc()
{
}

BOOL CMiniDrawDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}




// Сериализация CMiniDrawDoc

void CMiniDrawDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		// TODO: добавьте код загрузки
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CMiniDrawDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CMiniDrawDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CMiniDrawDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CMiniDrawDoc

#ifdef _DEBUG
void CMiniDrawDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMiniDrawDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CMiniDrawDoc


void CLine::Draw(CDC* PDC)
{
	PDC -> MoveTo(m_X1, m_Y1);


	PDC -> LineTo(m_X2, m_Y2);
}


void CMiniDrawDoc::AddLine(int X1, int Y1, int X2, int Y2)
{
	CLine* pLine = new CLine(X1, Y1, X2, Y2);
	m_LineArray.Add(pLine);
}
CLine* CMiniDrawDoc::GetLine(int Index)
{
	if (Index< 0 || Index> m_LineArray.GetUpperBound())
		return 0;
	return m_LineArray.GetAt(Index);
}
int CMiniDrawDoc::GetNumLines()
{
	return (int)m_LineArray.GetSize();
}

void CMiniDrawDoc::DeleteContents()


{
	// TODO: Здесь добавьте собственный код обработчика
	// и/или вызов базового класса
	int Index = (int)m_LineArray.GetSize();
	while (Index--)
		delete m_LineArray.GetAt(Index);
	m_LineArray.RemoveAll();

	CDocument::DeleteContents();
}

void CMiniDrawDoc::OnEditClearAll()
{
	// TODO: Здесь добавьте собственный код обработчика
	DeleteContents();

	UpdateAllViews(0);
}

void CMiniDrawDoc::OnUpdateEditClearAll(CCmdUI* pCmdUI)
{
	// TODO: Здесь добавьте собственный код обработчика
	pCmdUI -> Enable((int)m_LineArray.GetSize());
}

void CMiniDrawDoc::OnEditUndo()
{
	// TODO: Здесь добавьте собственный код обработчика
	int Index = (int)m_LineArray.GetUpperBound();
	if (Index>-1)
	{
		delete m_LineArray.GetAt(Index);
		m_LineArray.RemoveAt(Index);
	}
	UpdateAllViews(0);
}

void CMiniDrawDoc::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	// TODO: Здесь добавьте собственный код обработчика
	pCmdUI -> Enable((int)m_LineArray.GetSize());
}