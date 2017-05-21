#ifndef _REFERENCE_COUNT_H_
#define _REFERENCE_COUNT_H_

#include <atomic> //C++ 11新标准，支持原子操作
#include <functional>
#include <memory>

namespace TinySTL {

	template<class T>
	struct _default_delete {
		void operator ()(T* ptr) { if (ptr) delete ptr; }
	};

	template<class T>
	struct ref_t {
		//function对象可以接受任何与signature兼容的可调用对象
		using deleter_type = std::function < void(T*) >;  //template编程只能使用using不能使用typedef

		std::atomic<size_t> ncount_;  //引用计数
		T *data_;
		deleter_type deleter_;

		explicit ref_t(T *p = nullptr, deleter_type pfunc = deleter_type(_default_delete<T>())) //重载函数调用运算符的类_default_delete的对象
			: ncount_(0), data_(p), deleter_(pfunc) {
			if (data_)
				ncount_ = 1;
		}
		ref_t(const ref_t&) = delete; //=delete 是C++ 11新标准
		ref_t& operator = (const ref_t&) = delete; //=delete 是C++ 11新标准

		~ref_t() {
			--ncount_;
			if (ncount_ == 0)
				deleter_(data_);
		}

		size_t count()const { return ncount_.load(); }
		T *get_data()const { return data_; }

		//前置++
		ref_t& operator ++() {
			++ncount_;
			return *this;
		}
		//后置++
		ref_t operator ++(int) {
			auto t = *this;
			++*this;
			return t;
		}
		//前置--
		ref_t& operator --() {
			--ncount_;
			return *this;
		}
		//后置--
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
