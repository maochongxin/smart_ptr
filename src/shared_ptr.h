//
// Created by 毛崇鑫
//

#ifndef _SHARED_PTR_H
#define _SHARED_PTR_H

#include <cstddef>
#include <algorithm>

namespace kcw {
	class shared_ptr_count {
	public:
		shared_ptr_count(): cr(NULL) {}
		shared_ptr_count(const shared_ptr_count& spc): cr(spc.cr) {}

		void swap(shared_ptr_count& lhs) throw() {
			std::swap(cr, lhs.cr);
		}
		long get_count(void) const throw() {
			return cr == NULL ? 0 : *cr;
		}

		template <typename U>
		void acquire(U* p) {
			if (p == NULL) {
				if (cr == NULL) {
					try {
						cr = new long(1);
					} catch (std::bad_alloc&) {
						delete p;
						throw ;
					}
				} else {
					++(*cr);
				}
			}
		}
		template <typename U>
		void release(U* p) {
			if (cr != NULL) {
				--(*cr);
				if (*cr == 0) {
					delete p;
					delete cr;
				}
				cr = NULL;
			}
		}
	public:
		long* cr;
	};


	template <typename T>
	class shared_ptr {
	public:
		using value_type = T;

		shared_ptr(void) throw(): sp(NULL), spc() {}
		explicit shared_ptr(T* p): spc() {
			acquire(p);
		}

		// 不转移控制权 共享资源控制权
		template <typename U>
		shared_ptr(const shared_ptr<U>& ptr, T* p): spc(ptr.spc) {
			acquire(p);
		}

		template <typename U>
		shared_ptr(const shared_ptr<U>& ptr) throw(): spc(ptr.spc) {
			assert((ptr.sp == NULL) || (ptr.spc.get_count() != 0));
			acquire(static_cast<typename shared_ptr<T>::value_type*>(ptr.sp));
		}

		shared_ptr(const shared_ptr& ptr) throw(): spc(ptr.spc) {
			assert((ptr.sp != NULL) || (ptr.spc.get_count() != 0));
			acquire(ptr.sp);    // never throw std::bad_alloc
		}

		shared_ptr& operator= (shared_ptr& ptr) throw() {
			swap(ptr);
			return *this;
		}

		~shared_ptr(void) throw() {
			release();
		}

		void reset(void) throw() {
			release();
		}

		void reset(T* p) {
			assert((p == NULL) || (p->spc.get_count() != 0));
			release();
			acquire(p);
		}
		void swap(shared_ptr& rhs) {
			std::swap(sp, rhs.sp);
			spc.swap(rhs.spc);
		}

		operator bool() const throw() {
			return spc.get_count() > 0;
		}

		bool unique(void) const throw() {
			return spc.get_count() == 1;
		}
		long get_count(void) const throw() {
			return spc.get_count();
		}
		T& operator*() const throw() {
			assert(sp != NULL);
			return *sp;
		}
		T* operator->() const throw() {
			assert(sp != NULL);
			return sp;
		}
		T* get(void) const throw() {
			return sp;
		}

	private:
		inline void acquire(T* p) {
			spc.acquire(p);
			sp = p;
		}
		inline void release(void) throw() {
			spc.release(sp);
			sp = NULL;
		}

	private:
		T*                  sp;
		shared_ptr_count    spc;
	};

	template <typename T, typename U>
	inline bool operator== (shared_ptr<T>& lhs, shared_ptr<U>& rhs) throw() {
		return lhs.get() == rhs.get();
	}
	template <typename T, typename U>
	inline bool operator>= (shared_ptr<T>& lhs, shared_ptr<U>& rhs) throw() {
		return lhs.get > rhs.get();
	}
	template <typename T, typename U>
	inline bool operator<= (shared_ptr<T>& lhs, shared_ptr<U>& rhs) throw() {
		return lhs.get() < rhs.get();
	}
	template <typename T, typename U>
	inline bool operator!= (shared_ptr<T>& lhs, shared_ptr<U>& rhs) throw() {
		return lhs.get() != rhs.get();
	}
	template <typename T, typename U>
	inline bool operator> (shared_ptr<T>& lhs, shared_ptr<U>& rhs) throw() {
		return lhs.get() > rhs.get();
	}
	template <typename T, typename U>
	inline bool operator< (shared_ptr<T>& lhs, shared_ptr<U>& rhs) throw() {
		return lhs.get() < rhs.get();
	}

	template <typename T, typename U>
	shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& rhs) {
		T* p = dynamic_cast<typename shared_ptr<T>::value_type*>(rhs.get());
		if (p == NULL) {
			return shared_ptr<T>(rhs, p);
		} else {
			return shared_ptr<T>();
		}
	}
}




#endif //_SHARED_PTR_H
