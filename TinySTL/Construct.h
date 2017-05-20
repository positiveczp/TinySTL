#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include<new> //placement new
#include "TypeTraits.h"

namespace TinySTL {

	template<class T1, class T2>
	inline void construct(T1 *ptr1, const T2& value) {
		new(ptr1) T1(value);
	}

	//һ�����������
	template<class T>
	inline void destroy(T *ptr) {
		ptr->~T();
	}

	//���������������ж��Ƿ�ΪPOD���ͣ����ǵĻ���һ����destroy����������������
	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD_type;
		_destroy(first, last, is_POD_type());
	}

	template<class ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, _true_type) {}

	template<class ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, _false_type) {
		for (; first != last; ++first) {
			destroy(&*first);
		}
	}


}


#endif

