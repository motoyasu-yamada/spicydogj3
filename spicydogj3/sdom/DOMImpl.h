#pragma once

#include <xercesc/dom/DOM.hpp>
namespace xercesc_2_2 { class DOMImplementation; };

namespace DOMImpl {
	inline DOMImplementation* getImpl() {
		return DOMImplementationRegistry::getDOMImplementation(L"Core");
	}
};
