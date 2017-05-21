#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <utility>
#include "reference_count.h"

namespace TinySTL {

	template<class T>
	struct default_delete {
		void operator ()(T* ptr) { if (ptr) delete ptr; }
	};
	//��������
	template<class T>
	struct default_delete < T[] > {
		void operator ()(T* ptr) { if (ptr) delete[] ptr; }
	};

	//**************************************unique_ptr*********************************************
	//deleter��unique_ptr���͵�һ���֣���shared_ptr��һ��
	template<class T, class D = default_delete<T>>
	class unique_ptr {
	public:
		typedef T		element_type;
		typedef D		deleter_type;
		typedef element_type *pointer;
	public:
		explicit unique_ptr(T *data = nullptr) :data_(data) {}
		unique_ptr(T *data, deleter_type del) :data_(data), deleter(del) {}

		//IO������unique_ptr�������Կ����͸�ֵ�����ǿ����ƶ�
		unique_ptr(unique_ptr&& up) :data_(nullptr) {
			TinySTL::swap(data_, up.data_); //ȷ���ƺ�Դ������
		}
		unique_ptr& operator = (unique_ptr&& up) {
			if (&up != this) {
				clean();
				TinySTL::swap(*this, up);
			}
			return *this;
		}

		//unique_ptr�����Կ����͸�ֵ��=delete��c++ 11�ı�׼
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator = (const unique_ptr&) = delete;

		~unique_ptr() { clean(); }

		//get()��ȡ�������͵�ָ��
		const pointer get()const { return data_; }
		pointer get() { return data_; }
		deleter_type& get_deleter() { return deleter; }
		const deleter_type& get_deleter()const { return deleter; }
		
		//����ת�������
		operator bool()const { return get() != nullptr; }

		pointer release() {
			T *p = nullptr;
			TinySTL::swap(p, data_);
			return p;
		}
		void reset(pointer p = pointer()) {
			clean();
			data_ = p;
		}
		void swap(unique_ptr& up) { TinySTL::swap(data_, up.data_); }

		const element_type& operator *()const { return *data_; }
		const pointer operator ->()const { return data_; }
		element_type& operator *() { return *data_; }
		pointer operator ->() { return data_; }
	
	private:
		inline void clean() {
			deleter(data_);
			data_ = nullptr;
		}
	
	//����ָ���ɾ����
	private:
		element_type *data_;
		deleter_type deleter;
	};

	template <class T, class D>
	void swap(unique_ptr<T, D>& x, unique_ptr<T, D>& y) {
		x.swap(y);
	}

	template <class T1, class D1, class T2, class D2>
	bool operator == (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs) {
		return lhs.get() == rhs.get();
	}
	template <class T, class D>
	bool operator == (const unique_ptr<T, D>& up, nullptr_t p) {
		return up.get() == p;
	}
	template <class T, class D>
	bool operator == (nullptr_t p, const unique_ptr<T, D>& up) {
		return up.get() == p;
	}
	template <class T1, class D1, class T2, class D2>
	bool operator != (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs) {
		return !(lhs == rhs);
	}
	template <class T, class D>
	bool operator != (const unique_ptr<T, D>& up, nullptr_t p) {
		return up.get() != p;
	}
	template <class T, class D>
	bool operator != (nullptr_t p, const unique_ptr<T, D>& up) {
		return up.get() != p;
	}

	//�ɱ��ģ��
	template <class T, class... Args>
	unique_ptr<T> make_unique(Args&&... args) {
		return unique_ptr<T>(new T(std::forward<Args>(args)...));
	};

	//**************************************shared_ptr*********************************************
	template<class T>
	class shared_ptr {
	public:
		typedef T element_type;
	private:
		template<class Type>
		using ref_t = ref_t < Type >; //template���ֻ��ʹ��using����ʹ��typedef
	public:
		explicit shared_ptr(T *p = nullptr) :ref_(new ref_t<T>(p)) {} //���ü�������heap��
		template<class D>
		shared_ptr(T *p, D del) : ref_(new ref_t<T>(p, del)) {} //���Զ����Լ���deleter

		shared_ptr(const shared_ptr& sp) {
			copy_ref(sp.ref_);
		}

		shared_ptr& operator = (const shared_ptr& sp) {
			if (this != &sp) {
				decrease_ref();
				copy_ref(sp.ref_);
			}
			return *this;
		}

		~shared_ptr() { decrease_ref(); }

		const element_type& operator *()const { return *(get()); }
		const element_type *operator ->()const { return get(); }
		element_type& operator *() { return *(get()); }
		element_type *operator ->() { return get(); }

		const element_type* get() const { return ref_->get_data(); }
		element_type* get() { return ref_->get_data(); }
		size_t use_count() const { return ref_->count(); }

		//����ת�������
		operator bool() const { return get() != nullptr; }
	
	private:
		void decrease_ref() {
			if (ref_->get_data()) {
				--(*ref_);
				if (use_count() == 0) //����Ƿ���Ҫdelete heap������ڴ�
					delete ref_;
			}
		}
		void copy_ref(ref_t<T> *r) {
			ref_ = r;
			++(*ref_);
		}

	//���ü���
	private:
		ref_t<T> *ref_;
	};

	template<class T1, class T2>
	bool operator == (const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs) {
		return lhs.get() == rhs.get();
	}
	template<class T>
	bool operator == (const shared_ptr<T>& sp, nullptr_t p) {
		return sp.get() == p;
	}
	template<class T>
	bool operator == (nullptr_t p, const shared_ptr<T>& sp) {
		return sp == p;
	}
	template<class T1, class T2>
	bool operator != (const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs) {
		return !(lhs == rhs);
	}
	template<class T>
	bool operator != (const shared_ptr<T>& sp, nullptr_t p) {
		return !(sp == p);
	}
	template<class T>
	bool operator != (nullptr_t p, const shared_ptr<T>& sp) {
		return !(sp == p);
	}

	//�ɱ��ģ��
	template<class T, class...Args>
	shared_ptr<T> make_shared(Args... args) {
		return shared_ptr<T>(new T(std::forward<Args>(args)...));
	}


























}

#endif