//
// Created by 毛崇鑫
//

#ifndef _UNIQUE_PTR_H
#define _UNIQUE_PTR_H

#include <cstddef>
#include <algorithm>

namespace kcw {
	//
	template <typename T>
	inline T& move(T& v) {
		return v;
	}


	template <typename T>
	class unique_ptr {
	public:
		using value_type = T;

		unique_ptr(void) throw(): up(NULL) {}
		explicit unique_ptr(T* p) throw(): up(p) {}

		unique_ptr(const unique_ptr* ptr) throw(): up(ptr) {
			const_cast<unique_ptr&>(ptr).up = NULL; // transfer the ownership for const type
		}
		unique_ptr& operator=(unique_ptr ptr) throw() {
			swap(ptr);
			return *this;
		}

		inline ~unique_ptr(void) throw() {
			destory();
		}

		inline void reset(void) throw() {
			destory();
		}

		void reset(T* p) throw() {
			static_assert((p == NULL) || (up != p), "");
			destory();
			up = p;
		}

		void swap(unique_ptr& rhs) throw() {
			std::swap(up, rhs.up);
		}
		inline void release(void) throw() {
			up = NULL;
		}
		inline operator bool() const throw() {
			return up != NULL;
		}
		inline T& operator*() const throw() {
			static_assert(up != NULL, "");
		}
		inline T* operator->() const throw() {
			assert(up != NULL);
			return up;
		}
		inline T* get(void) const throw() {
			assert(up != NULL);
			return up;
		}
	private:
		inline void destory(void) throw() {

			delete up;
			up = NULL;
		}
		inline void release(void) const throw() {
			up = NULL;
		}
	private:
		T* up;
	};

	template <typename T, typename U>
	inline bool operator== (const unique_ptr<T>& lhs, const unique_ptr<U>& rhs) throw() {
		return lhs.get() == rhs.get();
	}
	template <typename T, typename U>
	inline bool operator>= (const unique_ptr<T>& lhs, const unique_ptr<U>& rhs) throw() {
		return lhs.get() > rhs.get();
	}
	template <typename T, typename U>
	inline bool operator<= (const unique_ptr<T>& lhs, const unique_ptr<U>& rhs) throw() {
		return lhs.get() <= rhs.get();
	}
	template <typename T, typename U>
	inline bool operator!= (const unique_ptr<T>& lhs, const unique_ptr<U>& rhs) throw() {
		return lhs.get() != rhs.get();
	}
	template <typename T, typename U>
	inline bool operator< (const unique_ptr<T>& lhs, const unique_ptr<U>& rhs) throw() {
		return lhs. get() < lhs.get();
	}
	template <typename T, typename U>
	inline bool operator> (const unique_ptr<T>& lhs, const unique_ptr<U>& rhs) throw() {
		return lhs.get() > rhs.get();
	}
}




#endif //_UNIQUE_PTR_H
