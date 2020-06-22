#pragma once

#include <typeinfo>

class SDOMArchive;
/** SDOMへシリアライズ可能なオブジェクト */
class SDOMSerializable {
public:
	virtual void Serialize(SDOMArchive*) = 0;

	inline CString getTypeName() const {
		const type_info& type = typeid(*this);
		CString name = type.name();
		return name.Mid(6);
	}

};
