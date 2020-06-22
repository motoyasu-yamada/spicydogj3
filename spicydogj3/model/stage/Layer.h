#pragma once

#include "KeyFrameOwners.h"
#include "Label.h"
#include "model/Selectable.h"
#include "sdom/olm.h"
#include "utility/Bezier.h"

class Char;
class KeyFrame;
class AffineMatrix;
class Coloring;

/** タイムラインレイヤー */
class LayerStatus {
public:
	CString m_strName;
	BOOL m_bVisible;
	BOOL m_bLocked;

	LayerStatus();
};

/** タイムラインレイヤー */
class Layer
:	public COLMObject,
	public Selectable
{
public:
	enum {FRAME_NULL,FRAME_EMPTYKEY,FRAME_KEY,FRAME_EMPTY_TWEEN,FRAME_BEGIN_TWEEN,FRAME_END_TWEEN,FRAME_TWEEN};

private:
	TOLMReferer<Char> m_char;
	KeyFrameOwners    m_keyFrames;
	LayerStatus       m_status;

public:
	Layer();
	virtual ~Layer();
	virtual Layer* Clone() const;
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	CString GetName() const;
	BOOL GetLock() const;
	BOOL GetVisible() const;
	void SetName(const CString&);
	void SetLock(BOOL);
	void SetVisible(BOOL);
	int GetFrameCount() const;
	int GetFrameType(int) const;
	KeyFrameOwners& GetKeyFrameOwners();
	KeyFrame* GetKeyFrame(int);
	const KeyFrame* GetKeyFrame(int) const;
	const KeyFrame* GetBeginKeyFrame(int nFrame) const;
	const KeyFrame* GetEndKeyFrame(int nFrame) const;
	void AttachChar(Char*);
	const Char* GetAttachedChar() const;
	Char* GetAttachedChar();
	BOOL GetFrameInfo(int pos,AffineMatrix*,Coloring*,BOOL onlyKey) const;
	void GetLabelList(const Stage*,LABEL_NAME_LIST&) const;
	KeyFrame* GenerateKeyFrame(int pos,BOOL empty) const;
	void Draw   (int nFrame,CDC*) const;
	void GetBezier(int,CBEZIER&) const;
	static BOOL IsKeyFrameType(int nType);
	static SDOMSerializable* factory(LPCWSTR name);
	static LPCWSTR namer(SDOMSerializable* s);
};
