#ifndef SINGLETON_HPP
#define SINGLETON_HPP

namespace xxd
{
template<class T>
class Singleton
{
public:
	static T& Get();
	virtual ~Singleton() { };
private:
	class SingletonAutoRelease final
	{
	public:
		~SingletonAutoRelease()
		{
			delete SingletonInstance;
			SingletonInstance = nullptr;
		};	
	};

	static T* SingletonInstance;
};
}

template<class T>
T* xxd::Singleton<T>::SingletonInstance = nullptr;

template<class T>
inline T& xxd::Singleton<T>::Get()
{
	if(!SingletonInstance)
	{
		static SingletonAutoRelease GC;
		SingletonInstance = new T();
	}
	return *SingletonInstance;
}

#endif
