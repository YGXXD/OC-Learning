//
//  Singleton.hpp
//  ios-render
//
//  Created by chenqiudu on 2023/7/18.
//

#ifndef Singleton_hpp
#define Singleton_hpp

namespace xxd
{

template<class T>
class Singleton
{
public:
    static T& Get();

private:
    class SingletonAutoRelease final
    {
    public:
        ~SingletonAutoRelease()
        {
            delete singletonInstance;
            singletonInstance = nullptr;
        };
    };

    static T* singletonInstance;
};

template<class T>
T* Singleton<T>::singletonInstance = nullptr;

template<class T>
inline T& Singleton<T>::Get()
{
    if(!singletonInstance)
    {
        static SingletonAutoRelease GC;
        singletonInstance = new T();
    }
    return *singletonInstance;
}

};


#endif /* Singleton_hpp */
