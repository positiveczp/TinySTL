#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <cstdint>
#include <iostream>
#include <stdexcept>
//#include <string>

#include "Allocator.h"
#include "string.h"
#include "UninitializedFunctions.h"

namespace TinySTL {

	//bitmap�ὫN�ϵ���8�ı���
	//���bitmapʵ���ϻ������bit���Ǵ��ڵ���N��
	template<size_t N>
	class bitmap {
	public:
		typedef allocator<uint8_t> dataAllocator;
	private:
		uint8_t *start_;
		uint8_t *finish_;
		const size_t size_;//��¼����bit
		const size_t sizeOfUINT8_;//��¼����uint8_t
		enum EAlign { ALIGN = 8 };
	public:
		bitmap();

		//Returns the number of bits in the bitset that are set (i.e., that have a value of one)
		size_t count() const;
		size_t size() const { return size_; }
		//Returns whether the bit at position pos is set (i.e., whether it is one).
		bool test(size_t pos) const;
		//Returns whether any of the bits is set (i.e., whether at least one bit in the bitset is set to one).
		bool any() const;
		//Returns whether none of the bits is set (i.e., whether all bits in the bitset have a value of zero).
		bool none() const;
		//Returns whether all of the bits in the bitset are set (to one).
		bool all() const;

		bitmap& set();
		bitmap& set(size_t pos, bool val = true);
		bitmap& reset();
		bitmap& reset(size_t pos);
		bitmap& flip();
		bitmap& flip(size_t pos);

		//std::string to_string() const;
		string to_string() const;

		template<size_t N>
		friend std::ostream& operator <<(std::ostream& os, const bitmap<N>& bm);
	private:
		size_t roundUp8(size_t bytes);
		//��i�ĵ�nthΪ��ΪnewVal
		void setNthInInt8(uint8_t& i, size_t nth, bool newVal);
		//ȡ��i�ĵ�nthλ
		uint8_t getMask(uint8_t i, size_t nth)const { return (i & (1 << nth)); }
		//����n��λ��ת��Ϊ���ڵڼ���uint8_t��
		size_t getNth(size_t n)const { return (n / 8); }
		//����n��λ��ת��Ϊ���ڵ�N��uint8_t�еĵڼ���bit��
		size_t getMth(size_t n)const { return (n % EAlign::ALIGN); }
		void allocateAndFillN(size_t n, uint8_t val);
		void THROW(size_t n)const;
	};// end of bitmap

}
#endif
