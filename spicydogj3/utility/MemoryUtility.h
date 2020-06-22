#pragma once

#include <AFXMT.h>
#pragma warning(push)
#pragma warning(disable : 4100)
#include <memory>


/**
	�V���O���g���|�C���^��p�̃X�}�[�g�|�C���^
*/
template <typename _VT>
class TSingletonPtr {
private:
	/**
		�}���`�X���b�h�r���h���ɃV���O���g���C���X�^���X�̐�����
		�X���b�h�Z�[�t�ɂ���B
		@see get
	*/
#if defined(_MT)
	CCriticalSection m_cs;
#endif
	std::auto_ptr<_VT> m_p;
public:
	/**
		�V���O���g���̎擾
		MFC - �����ł��Ȃ������ꍇ��CMemoryException��O����������
		���̃��\�b�h�̓X���b�h�Z�[�t�ł�
		@return �V���O���g���C���X�^���X
		@see m_cs;
		@see m_p;
	*/
	_VT* get() {
		// ���ɐ�������Ă��邱�Ƃ������I�ȏꍇ��
		// �N���e�B�J���Z�N�V�����ɂ�郍�b�N���X�L�b�v
		_VT* p = m_p.get();
		if (p != NULL)
			return p;
		
		// �܂��������̉\�������邱�Ƃ����������ꍇ
		// �N���e�B�J���Z�N�V�����ɂ�胍�b�N���|���A
		// �m���ȏ������s��
#if defined(_MT)
		CSingleLock lock(&m_cs,TRUE);
		if ((p = m_p.get()) != NULL)
			return p;
#endif
		return (m_p = std::auto_ptr<_VT>(new _VT)).get();
	}
	
	_VT* operator -> () { return get(); }
}; // class TSingletonPtr

/**
	C++�z����J�v�Z��
*/
template <typename _T>
class TCppArray {
private:
	_T* m_p;
	int m_nSize;

public:
	TCppArray() {
		Init();
	}

	TCppArray(int nSize){
		Init();
		NewArray(nSize);
	}
	
	~TCppArray() {
		if (m_p != NULL)
			DeleteArray();
	}

	void NewArray(int nSize) {
		ASSERT(m_p == NULL);
		ASSERT(0 < nSize);
		m_nSize = nSize;
		m_p = new _T[m_nSize];
	}
	
	void DeleteArray() {
		ASSERT(m_p != NULL);
		delete[] ptr();
		m_p = NULL;
	}
	
	_T* ptr() { 
		ASSERT(m_p != NULL);
		ASSERT(AfxIsValidAddress(m_p,length(),TRUE));
		ASSERT(0 < m_nSize);
		return m_p; 
	}
	const _T* ptr() const { 
		ASSERT(m_p == NULL || AfxIsValidAddress(m_p,length(),TRUE) && 0 < m_nSize);
		return m_p; 
	}

	int length() const { return m_nSize * sizeof(_T); }
	
	_T& operator [] (int n) { 
		ASSERT(m_p != NULL);
		ASSERT(AfxIsValidAddress(m_p,length(),TRUE));
		ASSERT(0 < m_nSize);
		ASSERT(0 <=n && n < m_nSize);
		return m_p[n];
	}
	const _T& operator [](int n) const {
		ASSERT(m_p != NULL);
		ASSERT(AfxIsValidAddress(m_p,length(),TRUE));
		ASSERT(0 < m_nSize);
		ASSERT(0 <=n && n < m_nSize);
		return m_p[n];
	}

private:
	void Init() {
		m_p = NULL;
		m_nSize = 0;
	}
};

//	GlobalAllocator�̃J�v�Z���N���X
class GlobalAllocator {
private:
	HGLOBAL m_hg;
	UINT    m_nStyle;
//	�C���^�[�t�F�[�X
public:
	GlobalAllocator();
	GlobalAllocator(DWORD dwSize,UINT nStyle = GHND);
	GlobalAllocator(const GlobalAllocator&);
	GlobalAllocator& operator = (const GlobalAllocator&);
	virtual ~GlobalAllocator();
	void Alloc(DWORD dwSize,UINT nStyle = GHND);
	void Free();
	void Realloc(DWORD dwSize,UINT nStyle = GMEM_MODIFY | GMEM_ZEROINIT);
	void Attach(HGLOBAL);
	HGLOBAL Detach();
	UINT GetSize() const;
	HGLOBAL GetHandle() const;
protected:
	void CopyFrom(const GlobalAllocator&);
}; // GlobalAllocator


//	GlobalAllocator�Ŋl�����������������b�N���A
//	�X�R�[�v���O�ꂽ�烍�b�N����������N���X
class GlobalAllocatorPtr {
private:
	ULONG	m_nLocked;
	HGLOBAL m_hg;
	LPVOID	m_pv;

//	�C���^�[�t�F�[�X
public:
	GlobalAllocatorPtr(HGLOBAL);
	GlobalAllocatorPtr(GlobalAllocator&);
	LPVOID Lock(HGLOBAL);
	void Unlock();

	operator LPVOID();
	operator LPCVOID () const;

//	�C���v�������g
public:
	virtual ~GlobalAllocatorPtr();
};

#pragma warning(push)
#pragma warning(disable : 4284)
template <typename _T>
class TGlobalAllocatorTypedPtr : public GlobalAllocatorPtr {
public:
	TGlobalAllocatorTypedPtr(HGLOBAL h)
	:	GlobalAllocatorPtr(h) {
	}
	TGlobalAllocatorTypedPtr(GlobalAllocator& a)
	:	GlobalAllocatorPtr(a) {
	}
	inline operator _T*() { return get(); }
	inline operator const _T*() const { return get(); }
	inline _T* operator->() { return get(); }
	inline const _T* operator->() const { return get(); }
	inline _T& operator*() { return *get(); }
	inline const _T& operator* () const { return *get();}

private:
	inline _T* get() { return (_T*)(LPVOID)(*this); }
	inline const _T* get() const { return (const _T *)(LPCVOID)(*this);}
};
#pragma warning(pop)

#pragma warning(pop)
