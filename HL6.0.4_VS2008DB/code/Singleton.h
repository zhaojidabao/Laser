
#pragma once

#include<windows.h>

// #define SINGLETON_DECLARE(Singleton) \
// public:\
// 	static Singleton* GetInstance()	\
// 	{if (m_instance == 0) m_instance = new Singleton();\
// 	return m_instance;}\
// 	\
// 	void ReleaseInstance()\
// 	{if (m_instance) delete m_instance; m_instance = 0;}\
// 	virtual ~Singleton(void);\
// private:\
// 	Singleton& operator=(const Singleton&);\
// 	Singleton(const Singleton&);\
// 	Singleton(void);\
// static Singleton* m_instance;\
// 
// #define SINGLETON_IMPLEMENT(Singleton) \
// Singleton* Singleton::m_instance = 0;

template<typename T>
class CSingletonCleaner  
{  
public:  
	CSingletonCleaner(T* m_Instance)
		: m_Instance(m_Instance)
	{
		
	}  
	
	virtual~CSingletonCleaner()  
	{  
		if (m_Instance != NULL)
		{//都要删除了，还要之何用？可能还要创建呢！留有用之身，尚待机会。程序退出它才失效的。
			m_Instance->m_bSingletonFlag = FALSE;
			delete m_Instance;
			m_Instance = NULL;
		}
	}  
	
private:
	T* m_Instance;
};


#define SINGLETON_DECLARE(ClassName) \
public:\
	static ClassName* ClassName::GetInstance();\
	static BOOL m_bSingletonFlag;\
private: \
	friend class CSingletonCleaner<ClassName>; \
	static ClassName* m_Instance; \
private: \
	ClassName(const ClassName &rhs){}\
	ClassName &operator=(const ClassName &rhs){}\
	ClassName(void); \
	~ClassName(void); \

//哈哈，API形式下的临界区，是否发现API更具魅力呢！
  
#define SINGLETON_IMPLEMENT(ClassName) \
	ClassName* ClassName::m_Instance = NULL; \
	BOOL ClassName::m_bSingletonFlag = FALSE;\
	ClassName* ClassName::GetInstance() \
{ \
	if (NULL == m_Instance) \
	{ \
		if (NULL == m_Instance) \
		{\
			CRITICAL_SECTION cs;\
			InitializeCriticalSection(&cs);\
			EnterCriticalSection(&cs);\
			m_Instance = new ClassName(); \
			m_bSingletonFlag = TRUE;\
			static CSingletonCleaner<ClassName> Cleaner(m_Instance); \
			LeaveCriticalSection(&cs);\
			DeleteCriticalSection(&cs);\
		}\
	}\
	return m_Instance; \
}\
	
//#define SINGLETON_DECLARE(ClassName) \
//public: \
//	static ClassName* GetInstance(); \
//private: \
//	friend class CSingletonCleaner; \
//	static ClassName* m_Instance; \
//	static CCriticalSection m_csSingleton; \
//private: \
//	ClassName(const ClassName &rhs); \
//	ClassName &operator=(const ClassName &rhs); \
//	ClassName(void); \
//	~ClassName(void); \
//
//
//#define SINGLETON_IMPLEMENT(ClassName) \
//	ClassName* ClassName::m_Instance = NULL; \
//	CCriticalSection ClassName::m_csSingleton; \
//	ClassName* ClassName::GetInstance() \
//{ \
//	if (NULL == m_Instance) \
//{ \
//	CSingleLock lock(&m_csSingleton, TRUE); \
//	if (NULL == m_Instance) \
//{ \
//	m_Instance = new ClassName(); \
//	static CSingletonCleaner<ClassName> cl(m_Instance); \
//} \
//}\
//	return m_Instance; \
//}\