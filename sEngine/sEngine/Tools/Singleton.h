#ifndef __SINGLETON_H__
#define __SINGLETON_H__
#include "CommonTools.h"

namespace Common{

	//-------------------------------------------------------------
	template <class T> class Singleton
	{
	public:
		static T* get();
		static void kill();
		static void Singleton<T>::set(T* _i);

	protected:
		static T* m_i;

	private:
		T& operator=(const T&){}
	};

	template <class T> T* Singleton<T>::m_i=0;

	//-------------------------------------------------------------
	template <class T>  T* Singleton<T>::get()
	{
		if(m_i==0){
			m_i=new T();
		}
		return m_i;
	}

	//-------------------------------------------------------------
	template <class T> void Singleton<T>::kill()
	{
		SafeDelete(m_i);
	}

	//-------------------------------------------------------------
	template <class T> void Singleton<T>::set(T* _i)
	{
		if(m_i==0 && _i){
			m_i=_i;
		}  
	}
}

#endif