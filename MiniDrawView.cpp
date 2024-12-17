﻿
// MiniDrawView.cpp: реализация класса CMiniDrawView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MiniDraw.h"
#endif

#include "MiniDrawDoc.h"
#include "MiniDrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMiniDrawView

IMPLEMENT_DYNCREATE(CMiniDrawView, CView)

BEGIN_MESSAGE_MAP(CMiniDrawView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMiniDrawView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CMiniDrawView

CMiniDrawView::CMiniDrawView() noexcept
{
	// TODO: добавьте код создания
	m_Dragging = 0;
	m_HCross = AfxGetApp() -> LoadStandardCursor(IDC_CROSS);
}

CMiniDrawView::~CMiniDrawView()
{
}

BOOL CMiniDrawView::PreCreateWindow(CREATESTRUCT&  cs)
{
	// TODO: Здесь измените класс или стили окна
	// модифицируя поля структуры cs
	m_ClassName = AfxRegisterWndClass(
		CS_HREDRAW | CS_VREDRAW, // стили окна
		0, // без указателя;
		(HBRUSH)::GetStockObject(WHITE_BRUSH),

		// задать чисто белый фон;

		0); //без значка
	cs.lpszClass = m_ClassName;
	return CView::PreCreateWindow(cs);
}

// Рисование CMiniDrawView

void CMiniDrawView::OnDraw(CDC* pDC)
{
	CMiniDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	int Index = pDoc->GetNumLines();
	while (Index--)
		pDoc->GetLine(Index)->Draw(pDC);

}


// Печать CMiniDrawView


void CMiniDrawView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMiniDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CMiniDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CMiniDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}

void CMiniDrawView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMiniDrawView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Диагностика CMiniDrawView

#ifdef _DEBUG
void CMiniDrawView::AssertValid() const
{
	CView::AssertValid();
}

void CMiniDrawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMiniDrawDoc* CMiniDrawView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMiniDrawDoc)));
	return (CMiniDrawDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CMiniDrawView


void CMiniDrawView::OnLButtonDown(UINT nFlags, CPoint
	point)
{
	// TODO: Здесь добавьте собственный код обработчика
	// и/или вызов стандартного обработчика
	m_PointOld = point;
	m_PointOrigin = point;
	SetCapture();
	m_Dragging = 1;
	RECT Rect;
	GetClientRect(&Rect);
	ClientToScreen(&Rect);
	::ClipCursor(&Rect);
	CView::OnLButtonDown(nFlags, point);
}

void CMiniDrawView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Здесь добавьте собственный код обработчика
	// и/или вызов стандартного обработчика
	::SetCursor(m_HCross);
	if (m_Dragging)
	{
		CClientDC ClientDC(this);
		ClientDC.SetROP2(R2_NOT);
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(m_PointOld);
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(point);
		m_PointOld = point;
	}
	CView::OnMouseMove(nFlags, point);
}

void CMiniDrawView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Здесь добавьте код собственного обработчика
	// и/или вызов стандартного обработчика
	if (m_Dragging)
	{
		m_Dragging = 0;
		::ReleaseCapture();
		::ClipCursor(NULL);
		CClientDC ClientDC(this);
		ClientDC.SetROP2(R2_NOT);
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(m_PointOld);
		ClientDC.SetROP2(R2_COPYPEN);
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(point);
		CMiniDrawDoc* pDoc = GetDocument();
		pDoc -> AddLine(m_PointOrigin.x, m_PointOrigin.y,

			point.x, point.y);
	}
	CView::OnLButtonUp(nFlags, point);
}

// Отображение данных класса CMiniDrawView
