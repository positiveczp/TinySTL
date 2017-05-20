#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <cstdlib>

namespace TinySTL {

	/*
	**�ռ������������ֽ���Ϊ��λ����
	**�ڲ�ʹ��
	**��Ϊ�����ڴ棬��δ����������Զ���static��Ա
	*/
	class alloc {
	
	private:
		enum EAlign { ALIGN = 8 }; //С��������ϵ��߽�
		enum EMaxBytes { MAXBYTES = 128 }; //С����������ޣ�������������malloc����
		enum ENFreeLists { NFREELISTS = (EMaxBytes::MAXBYTES / EAlign::ALIGN) }; //free-lists�ĸ���
		enum ENObjs { NOBJS = 20 }; //ÿ�����ӵĽڵ���
		
	private:
		//free-lists�Ľڵ㹹��
		union obj {
			union obj *next;
			char client[1];
		};

		static obj *free_list[ENFreeLists::NFREELISTS];
	
	private:
		static char *start_free; //�ڴ����ʼλ��
		static char *end_free; //�ڴ�ؽ���λ��
		static size_t heap_size; 

	private:
		//��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes + EAlign::ALIGN - 1) & ~(EAlign::ALIGN - 1));
		}
		//���������С������ʹ�õ�n��free-list��n��0��ʼ����
		static size_t FREELIST_INDEX(size_t bytes) {
			return (((bytes)+EAlign::ALIGN - 1) / EAlign::ALIGN - 1);
		}
		//����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free-list
		static void *refill(size_t n);
		//����һ���ռ䣬������nobjs����СΪsize������
		//�������nobjs�������������㣬nobjs���ܻή��
		static char *chunk_alloc(size_t size, size_t& nobjs);

	public:
		static void *allocate(size_t bytes);
		static void deallocate(void *ptr, size_t bytes);
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
	};

}

#endif
