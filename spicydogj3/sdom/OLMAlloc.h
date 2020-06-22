#pragma once

#include <xmemory>

template<class _Ty> 
inline _Ty *_OLMAllocate(ptrdiff_t _N, _Ty *) {
	if (_N < 0) {
		_N = 0;
	}
	return ((_Ty *)operator new((size_t)_N * sizeof (_Ty))); 
}

template<class _T1, class _T2> 
void _OLMConstruct(_T1 *_P, const _T2& _V) {
	new ((void *)_P) _T1(_V); 
}

template<class _Ty> 
void _OLMDestroy(_Ty *_P) {
	_DESTRUCTOR(_Ty, _P); 
}

template<class _Ty>
	class TOLMAllocator {
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef _Ty *pointer;
	typedef const _Ty *const_pointer;
	typedef _Ty& reference;
	typedef const _Ty& const_reference;
	typedef _Ty value_type;
	pointer address(reference _X) const {
		return (&_X); 
	}
	const_pointer address(const_reference _X) const {
		return (&_X); 
	}
	pointer allocate(size_type _N, const void *) {
		return (_OLMAllocate((difference_type)_N, (pointer)0)); 
	}
	char *_Charalloc(size_type _N) {
		return (_OLMAllocate((difference_type)_N,(char *)0)); 
	}
	void deallocate(void *_P, size_type) {
		operator delete(_P); 
	}
	void construct(pointer _P, const _Ty& _V) {
		_OLMConstruct(_P, _V);
	}
	void destroy(pointer _P) {
		_OLMDestroy(_P); 
	}
	size_t max_size() const {
		size_t _N = (size_t)(-1) / sizeof (_Ty);
		return (0 < _N ? _N : 1); 
	}
};

template<class _Ty, class _U> inline
BOOL operator==(const TOLMAllocator<_Ty>&, const TOLMAllocator<_U>&) {
	return (true); 
}

template<class _Ty, class _U> inline
BOOL operator!=(const TOLMAllocator<_Ty>&, const TOLMAllocator<_U>&) {
	return (false); 
}
