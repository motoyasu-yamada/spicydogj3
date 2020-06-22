#pragma once

#include <typeinfo>

class SDOMArchive;
/** SDOM�փV���A���C�Y�\�ȃI�u�W�F�N�g */
class SDOMSerializable {
public:
	virtual void Serialize(SDOMArchive*) = 0;

	inline CString getTypeName() const {
		const type_info& type = typeid(*this);
		CString name = type.name();
		return name.Mid(6);
	}

};
