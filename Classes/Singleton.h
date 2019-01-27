#ifndef __TSingleton__H__
#define __TSingleton__H__


#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
/**************************************************/
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif
/**************************************************/
#endif

#include <assert.h>

class Tag_Singleton_Manual;
class Tag_Singleton_Auto;

/************************************************************************/
/*                 Singleton<Tag_Singleton_Manual>                      */
/************************************************************************/
template<typename T,typename TypeTag=Tag_Singleton_Manual>class Singleton;

template<typename T>
class Singleton<T,Tag_Singleton_Manual>
{
public:
	static T& instance() {
		if ( (void*)instance_ == (void*)0x0badbeef )
			instance_ = new T;
		assert(instance_);
		return *instance_;
	};
	static void destroy() {
		if ( instance_ != 0 && (void*)instance_ != (void*)0x0badbeef ) {
			delete instance_;
			instance_ = 0;
		}
	}
	static bool valid()
	{
		return instance_ != NULL && ( (void*)instance_ != (void*)0x0badbeef );
	}
protected:
	Singleton(void){};
	~Singleton(void){};
private:
	static T* instance_;
	Singleton( const Singleton& );
	const Singleton& operator=( const Singleton& );
};

template<typename T>
T* Singleton<T,Tag_Singleton_Manual>::instance_ = (T*)((void*)0x0badbeef);


/************************************************************************/
/*                   Singleton<Tag_Singleton_Auto>                      */
/************************************************************************/
template<typename T>
class Singleton<T,Tag_Singleton_Auto>
{
public:
	static T& instance() {
		static T obj;
		return obj;
	};
	static bool valid() { return true; }
protected:
	Singleton(void){};
	~Singleton(void){};
private:
	Singleton( const Singleton& );
	const Singleton& operator=( const Singleton& );
};

#endif
