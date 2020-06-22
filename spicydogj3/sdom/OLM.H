#pragma once
/**	OLM - Object Lifetime Management.



	@version	1.0		2001/11/12
	@author		m.yamada


	(用語)
	リアルワールド			... 履歴時系列のある時点の静的オブジェクト構造
	アナザーワールド		... 履歴時系列上に存在するトランザクション単位のオブジェクト構造

	リアルオブジェクト		...　履歴時系列のある時点にて実在するオブジェクト
	アナザーオブジェクト	...	 履歴時系列の削除処理後未来、作成処理前過去において、存在する
								 リアルオブジェクトのシャドー

	オーナー/キーパー		オブジェクトの所有権を持つもの
							あるオブジェクトを所有するオーナーまたはキーパーが削除される時に
							自動的にそのオブジェクトも削除される
							オブジェクトの所有者は唯一つのオーナーまたはキーパーのみ。
	
	リファラー				オブジェクトを参照するもの
							オーナー⇒キーパーへ所有権が移転するとリファラーはゴースト状態になる
							キーパー⇒オーナーへ所有権が移転するとリファラーはアライブ状態になる

	(ライフタイム)
		         生成   消滅
		RO:      ●<--->●
		AO: ---->○     ○<---
*/


#include "sdomn.h"
#include "SDOMArchive.h"
#include "OLMAlloc.h"

#include <list>
#include <algorithm>

class COLMObject;
class COLMOwner;
class COLMKeeper;
class COLMReferer;
class COLMRefererBackup;

/** OLMで管理されるオブジェクト */
class COLMObject : public CObject,public SDOMSerializable {
private:
	typedef std::list<COLMReferer*> REFERERS;
	mutable REFERERS m_referers;
	
public:
	COLMObject();
	virtual ~COLMObject();

	int GetReferenceCount() const;

	void AppendReferer(COLMReferer*) const;
	void RemoveReferer(COLMReferer*) const;

	void OnRemovedOLMObject(COLMObject*);
	void OnRevivalOLMObject(COLMObject*);
};

/**
	COLMObjectをラップする各種クラスの基底クラス
*/
class COLMObjectPtr : public CObject {
	friend class COLMOwners;

public:
	BOOL IsNullObject() const;
	COLMObject* GetObject();
	const COLMObject* GetObject() const;
	BOOL operator ==(const CObject*&) const;
	operator const CObject*() const;
	const COLMObject& operator*() const;

protected:
	COLMObjectPtr();
	virtual ~COLMObjectPtr();
	COLMObjectPtr(const COLMObjectPtr&);
	COLMObjectPtr& operator =(const COLMObjectPtr&);
	void SetObject(COLMObject*);
	void DeleteObject();
private:
	COLMObject* m_pObj;
	virtual void AssertValid() const;
};


/**	OLMオブジェクトの所有者 (リアルオブジェクト管理) */
class COLMOwner : public COLMObjectPtr {
	friend class COLMKeeper;
	friend void _OLMConstruct(COLMOwner *_P, const COLMOwner& _V);
public:
	COLMOwner();
	virtual ~COLMOwner();

	virtual void OwnNew(COLMObject*);
	virtual void InheritedFrom(COLMOwner*);

protected:
	/**　指定アロケータのみ複製可能 */
	COLMOwner(const COLMOwner&);
private:
	/** COLMOwnerの複製は禁止されています */
	COLMOwner& operator =(const COLMOwner& k);
};


/** OLMオブジェクトの所有者
	(アナザーオブジェクト) */
class COLMKeeper : public COLMObjectPtr {
	friend void _OLMConstruct(COLMKeeper*,const COLMKeeper&);
public:
	COLMKeeper();
	virtual ~COLMKeeper();

	virtual void OwnNew(COLMObject*);	
	virtual void TransferTo(COLMOwner*);
	virtual void InheritedFrom(COLMOwner*);
protected:	
	/**　指定アロケータのみ複製可能 */
	COLMKeeper(const COLMKeeper&);
private:	
	/** 複製禁止 */
	COLMKeeper& operator =(const COLMKeeper&);
	void Backup(COLMObject* pObj);
	void Restore(COLMObject* pObj);
}; // class COLMKeeper

/**　オブジェクト通知　*/
class COLMRefererListener {
public:
	virtual void OnRemovedOLMObject(COLMObject*) {}
protected:
	virtual ~COLMRefererListener() {}
}; // class COLMKeeperListener

/** OLMオブジェクトの参照者 */
class COLMReferer : public COLMObjectPtr {
private:
	/** リファラの参照先オブジェクトの生/死の輪廻イベントを受け取るリスナ */
	COLMRefererListener* m_pListener;
	/** リアル→アナザへい移動したオブジェクト */
	COLMObject* m_pKept;

public:
	COLMReferer();
	/** 参照のコピー */
	COLMReferer(const COLMReferer&);
	/** 参照のコピー */
	COLMReferer& operator =(const COLMReferer&);
	virtual ~COLMReferer();

	void AppendListener(COLMRefererListener*);
	void RemoveListener(COLMRefererListener*);
	void ReferTo(COLMObject const*);
	void ClearRefererer();
	const COLMObject* GetKeptOLMObject() const;

	/**  COLMObjectからのイベントハンドラ */
	virtual void OnRemovedOLMObject(COLMObject*);
	/** COLMObjectからのイベントハンドラ */
	virtual void OnRevivalOLMObject(COLMObject*);

}; // class COLMReferer 

/**
	OLMテンプレート
*/
template <typename _VT,typename _BVT>
class _TOLM : public _BVT {
public:
	_TOLM() {}
	operator const _VT* () const { return dynamic_cast<const _VT*>(GetObject());}
	operator _VT*() { return dynamic_cast<_VT*>(GetObject());}
	_VT* operator ->() { return dynamic_cast<_VT*>(GetObject());}
	const _VT* operator ->() const { return dynamic_cast<const _VT*>(GetObject());}
	_VT& operator*() { return dynamic_cast<_VT&>(*GetObject());}
	const _VT& operator*() const { return dynamic_cast<_const VT&>(*GetObject());}
protected:
	_TOLM(const _TOLM& src) : _BVT(src) {}
private:
	_TOLM& operator = (const _TOLM& src);
};

template <typename _VT>
class TOLMOwner : public _TOLM<_VT,COLMOwner> {
	friend void _OLMConstruct(TOLMOwner<_VT>*,const TOLMOwner<_VT>&);
	typedef _TOLM<_VT,COLMOwner> _BASE;
public:
	TOLMOwner() {}
private:
	/** コンテナへのアロケーション時のみコピー化 */
	TOLMOwner<_VT>(const TOLMOwner<_VT>& src) : _BASE(src) {}
	/** 代入禁止 */
	TOLMOwner& operator =(const TOLMOwner&);
};

template <typename _VT>
class TOLMKeeper : public _TOLM<_VT,COLMKeeper> {
	friend void _OLMConstruct(TOLMKeeper<_VT>*,const TOLMKeeper<_VT>&);
public:
	TOLMKeeper() {}
private:
	/** コンテナへのアロケーション時のみコピー化 */
	TOLMKeeper (const TOLMKeeper& src) : _TOLM<_VT,COLMKeeper>(src) {};
	/** 代入禁止 */
	TOLMKeeper& operator =(const TOLMKeeper&);
};

template <typename _VT>
class TOLMReferer : public _TOLM<_VT,COLMReferer>  {
public:
	TOLMReferer<_VT>() {
	}

	/** 参照コピー */
	TOLMReferer<_VT>(const TOLMReferer& src) 
	:	_TOLM<_VT,COLMReferer>(src) {
	}

	/** 参照コピー */
	TOLMReferer<_VT>& operator =(const TOLMReferer<_VT>& src) {
		ReferTo(src.GetObject());
		return *this;
	}
};

/** オーナー順序のスナップ */
class COLMOwnersOrderSnap {
private:
	friend class COLMOwners;
	std::list<COLMObject*> m_snap;
}; // COLMOwnersOrderSnap
typedef std::list<COLMObject*> OLM_OWNERS_ORDER_LIST;

/**	オーナーコンテナ */
class COLMOwners : public CObject,public SDOMSerializable {
private:
	typedef TOLMAllocator<COLMOwner> ALLOCATOR;
	typedef std::list<COLMOwner,ALLOCATOR> OWNERS;
	typedef OWNERS::iterator Itr;

	friend class COLMKeepers;
	friend class COLMDelExKeepers;
public:
	typedef OWNERS::const_iterator ConstItr;

public:
	OWNERS m_owners;

public:
	COLMOwners();
	virtual ~COLMOwners();
	virtual void Serialize(SDOMArchive*) = 0;
	virtual void AssertValid() const;

	void Move(int nL,int nR);
	ConstItr Begin() const;
	ConstItr End() const;
	int GetCount() const;
	const COLMObject* GetObject(int nPos) const;
	COLMObject* GetObject(int nPos);
	void ReorderFront(const OLM_OWNERS_ORDER_LIST& panes);
	void ReorderBack(const OLM_OWNERS_ORDER_LIST& panes);
	void ReorderUp(const OLM_OWNERS_ORDER_LIST& panes);
	void ReorderDown(const OLM_OWNERS_ORDER_LIST& panes);
	void LoadOrderSnap(const COLMOwnersOrderSnap&);
	void SaveOrderSnap(COLMOwnersOrderSnap&) const;
protected:
	void Serialize(SDOMArchive*,BOOL writeid,SDOMArchive::CLASS_FACTORY,SDOMArchive::CLASS_NAMER);
	void Save(SDOMArchive*,BOOL writeid,SDOMArchive::CLASS_FACTORY,SDOMArchive::CLASS_NAMER);
	void Load(SDOMArchive*,BOOL writeid,SDOMArchive::CLASS_FACTORY,SDOMArchive::CLASS_NAMER);
private:
	int FindPos(const COLMObject*) const;
	Itr GetItr(int nPos);
}; // class COLMOwners

/**	タイプセーフなOLMオブジェクトオーナーコンテナ */
template <
	class _VT,BOOL _WRITEID>
class TOLMOwners : public COLMOwners
{
public:
	_VT const * GetObject(int nPos) const {
		return static_cast<_VT const *>(COLMOwners::GetObject(nPos));
	}

	_VT* GetObject(int nPos) {
		return static_cast<_VT*>(COLMOwners::GetObject(nPos));
	}
	
	static const _VT* I2P(const ConstItr& i) {
		const COLMOwner& o = *i;
		const COLMObject* po = o.GetObject();
		return static_cast<const _VT*>(po);
	}

	virtual void Serialize(SDOMArchive* ar) {
		ASSERT(AfxIsValidAddress(this,sizeof COLMOwners));
		ASSERT_VALID(this);
		COLMOwners::Serialize(ar,_WRITEID,_VT::factory,_VT::namer);
	}

}; // class TOLMOwners

/** キーパーコンテナ */
class COLMKeepers
{
public:
	typedef COLMOwners::OWNERS OWNERS;
	typedef TOLMAllocator<COLMKeeper> ALLOCATOR;
	typedef std::list<COLMKeeper,ALLOCATOR> KEEPERS;
	KEEPERS m_keepers;
	
public:
	virtual ~COLMKeepers();
	virtual void OwnNew(COLMObject*);
	virtual void InheritedFrom(COLMOwners&,int,int);
	virtual void TransferTo(COLMOwners&,int);
}; // class COLMKeepers

typedef std::list<COLMObject*> OLM_OWNER_DEL_EX_LIST;
class COLMDelExKeepers
{
private:
	typedef std::pair<int,COLMKeepers> DEL_EX_KEEPER;
	typedef std::list<DEL_EX_KEEPER> DEL_EX_KEEPERS;
	DEL_EX_KEEPERS m_keepers;
	OLM_OWNER_DEL_EX_LIST m_dellist;

public:
	void InheritedFrom(COLMOwners&,const OLM_OWNER_DEL_EX_LIST&);
	void TransferTo(COLMOwners&);
}; // COLMDelExKeepers
