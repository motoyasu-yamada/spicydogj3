#include "stdafx.h"
#include "accel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



int GetAcceleratorTableSize(HACCEL haccel) {
	ASSERT(haccel != NULL);
	return ::CopyAcceleratorTable(haccel,NULL,0);
}

Accel::Accel() : m_haccel(NULL) {
}

Accel::~Accel() {
	DeleteAccel();
}

BOOL Accel::Attach(HACCEL ha) {
	ASSERT(m_haccel == NULL);
	m_haccel = ha;
	if (ha == NULL) {
		return FALSE;
	}
	return TRUE;
}

HACCEL Accel::Detach() {
	ASSERT(m_haccel != NULL);
	HACCEL ha = m_haccel;
	m_haccel = NULL;
	return ha;
}

HACCEL Accel::GetAccel() const {
	return m_haccel;
}

BOOL Accel::DeleteAccel() {
	if (m_haccel == NULL)
		return FALSE;
	HACCEL ha = m_haccel;
	m_haccel = NULL;
	return ::DestroyAcceleratorTable(ha);
}

MultiAccel::MultiAccel() 
:	m_bToMerge(FALSE) {
}

void MultiAccel::SetAccel(int nIndex,HACCEL haNew) {
	const int num = m_accelMap.GetSize();
	if (!(nIndex < num)) {
		m_accelMap.SetSize(nIndex+1);
		m_bToMerge = TRUE;
	}
	for (int n = num;n <= nIndex;n++) {
		m_accelMap[n] = NULL;
	}
	
	HACCEL haOld = m_accelMap[nIndex];
	if (haOld != haNew) {
		m_bToMerge = TRUE;
		m_accelMap[nIndex] = haNew;
	}
}

HACCEL MultiAccel::GetMergedAccel() {
	if (m_bToMerge) {
		CreateAccel();
		m_bToMerge = FALSE;
	}
	
	return m_accelMerged.GetAccel();
}

void MultiAccel::CreateAccel() {
	m_accelMerged.DeleteAccel();
	
	const int numMap = m_accelMap.GetSize();
	CArray<ACCEL,ACCEL&> accels;
	for (int n = 0;n < numMap;n++) {
		HACCEL ha = m_accelMap[n];
		if (ha == NULL)
			continue;

		const int num = GetAcceleratorTableSize(ha);
		const int pos = accels.GetSize();
		ASSERT(num != NULL);
		accels.SetSize(pos + num);
		ACCEL* p = accels.GetData() + pos;
		ASSERT(AfxIsValidAddress(p,sizeof ACCEL * num));
		VERIFY(::CopyAcceleratorTable(ha,p,num) == num);
	}
	m_accelMerged.Attach(::CreateAcceleratorTable(accels.GetData(),accels.GetSize()));
}


AccelOwner::AccelOwner()
:	m_haccel(NULL) {
}

AccelOwner::~AccelOwner() {
}

HACCEL AccelOwner::GetDefaultAccelerator() {
	return m_haccel;
}

void AccelOwner::SetAccelerators(UINT n) {
	SetAccelerators(::LoadAccelerators(AfxGetResourceHandle(),MAKEINTRESOURCE(n)));
}

void AccelOwner::SetAccelerators(HACCEL ha) {
	ASSERT(m_haccel == NULL);
	m_haccel = ha;
}
