#pragma once

template<typename _T> class DOMNodeP {
private:
	mutable BOOL _owns;
	mutable _T* _ptr;

public:
	explicit DOMNodeP(_T* p = 0)
	:	_owns(p != 0),
		_ptr (p) {
	}

	DOMNodeP(const DOMNodeP& r) 
	:	_owns(r._owns), 
		_ptr (r.release()) {
		ASSERT(_owns && _ptr || !_owns && !_ptr);
	}

	DOMNodeP& operator=(const DOMNodeP& r) {
		ASSERT(_owns && _ptr || !_owns && !_ptr);
		if (this == &r) {
			return *this;
		}
		ASSERT((_ptr != r.get()) || (_ptr == NULL && r.get() == NULL));

		if (_owns) {
			_ptr->release();
		}

		if (r._owns) {
			_ptr  = r.release(); 
			_owns = true;
		}

		ASSERT(_owns && _ptr || !_owns && !_ptr);
		return *this; 
	}

	~DOMNodeP() {
		if (_owns) {
			_ptr->release();
		}
	}

	_T* operator->() const {
		return get();
	}
	
	_T* get() const {
		return _ptr;
	}

	_T* release() const {
		if (_owns) {
			_owns = false;
			_T* p = _ptr;
			_ptr  = NULL;
			ASSERT(p != NULL);
			return p; 
		} else {
			ASSERT(_ptr == NULL);
			return NULL;
		}
	}
};


