#pragma once

#include <map>

/** �񋓒l�Ɩ����}�b�s���O����N���X
	(���p���@)
	enum {
		PICT_TYPE_JPG,
		PICT_TYPE_PNG,
		PICT_TYPE_GIF
	};
	ENUM_TYPE(PictType);
	ENUM_TYPE_ITEM(PICT_TYPE_JPG,"jpg")
	ENUM_TYPE_ITEM(PICT_TYPE_PNG,"png")
	ENUM_TYPE_ITEM(PICT_TYPE_PNG,"gif") */
class SDOMEnumType {
private:
	std::map<int,CString> m_i2s;
	std::map<CString,int> m_s2i;
public:
	/** �w�肵���񋓎q�l�����񋓎q�����邩�ǂ��� */
	BOOL Exists(int value) const;
	/** �w�肵���񋓎q�������񋓎q�����邩�ǂ��� */
	BOOL Exists(LPCTSTR name) const;
	/** �񋓎q����񋓎q�l�ɕύX */
	int Name2Value(LPCTSTR name) const;
	/** �񋓎q�l��񋓎q���ɕύX */
	LPCTSTR Value2Name(int value) const;
private:
	friend class SDOMEnumTypeItem;
	/** �񋓎q���`���� */
	void Define(int value,LPCTSTR name);
};

/** SDOMEnumType�̊e���ڂ��`����ׂ̃w���p�[�N���X */
class SDOMEnumTypeItem {
public:
	/** @param type  �񋓌^��(C++�̌^�E�ϐ��������K���ɂ��������Ă��邱��)
	    @param value �񋓎q�l
		@param name  �񋓎q�� */
	SDOMEnumTypeItem(const SDOMEnumType& type,int value,LPCTSTR name);
};

#define __ENUM_TYPE_CONCATENATE_DIRECT( s1, s2 ) s1##s2
#define __ENUM_TYPE_CONCATENATE( s1, s2 ) __ENUM_TYPE_CONCATENATE_DIRECT( s1, s2 )

#define ENUM_TYPE(type) static const SDOMEnumType type
#define ENUM_TYPE_ITEM(type,value,name)  static const SDOMEnumTypeItem __ENUM_TYPE_CONCATENATE(type,__LINE__)(type,value,name)
