#ifndef _REFERENCE_COUNT_H_
#define _REFERENCE_COUNT_H_

#include <atomic> //C++ 11�±�׼��֧��ԭ�Ӳ���
#include <functional>
#include <memory>

namespace TinySTL {

	template<class T>
	struct _default_delete {
		void operator ()(T* ptr) { if (ptr) delete ptr; }
	};

	template<class T>
	struct ref_t {
		//function������Խ����κ���signature���ݵĿɵ��ö���
		using deleter_type = std::function < void(T*) >;  //template���ֻ��ʹ��using����ʹ��typedef

		std::atomic<size_t> ncount_;  //���ü���
		T *data_;
		deleter_type deleter_;

		explicit ref_t(T *p = nullptr, deleter_type pfunc = deleter_type(_default_delete<T>())) //���غ����������������_default_delete�Ķ���
			: ncount_(0), data_(p), deleter_(pfunc) {
			if (data_)
				ncount_ = 1;
		}
		ref_t(const ref_t&) = delete; //=delete ��C++ 11�±�׼
		ref_t& operator = (const ref_t&) = delete; //=delete ��C++ 11�±�׼

		~ref_t() {
			--ncount_;
			if (ncount_ == 0)
				deleter_(data_);
		}

		size_t count()const { return ncount_.load(); }
		T *get_data()const { return data_; }

		//ǰ��++
		ref_t& operator ++() {
			++ncount_;
			return *this;
		}
		//����++
		ref_t operator ++(int) {
			auto t = *this;
			++*this;
			return t;
		}
		//ǰ��--
		ref_t& operator --() {
			--ncount_;
			return *this;
		}
		//����--
		ref_t operator --(int) {
			auto t = *this;
			--*this;
			return t;
		}
	};

	template<class T>
	bool operator ==(const ref_t<T>& lhs, const ref_t<T>& rhs) {
		return lhs.get_data() == rhs.get_data();
	}
	template<class T>
	bool operator !=(const ref_t<T>& lhs, const ref_t<T>& rhs) {
		return !(lhs == rhs);
	}
}
#endif
