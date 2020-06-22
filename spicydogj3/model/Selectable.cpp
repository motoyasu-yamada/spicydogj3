#include "stdafx.h"
#include "Selectable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Selectable::Selectable()
:	m_selected(FALSE) {
}

void Selectable::CopyFrom(const Selectable& src) {
	m_selected = src.m_selected;
}

void Selectable::Unselect() {
	m_selected = FALSE;
}

void Selectable::Select() {
	m_selected = TRUE;
}

void Selectable::SelectToggle() {
	if (IsSelected()) {
		Unselect();
	} else {
		Select();
	}
}

BOOL Selectable::IsSelected() const {
	return m_selected;
}

Selectable::~Selectable() {
}


