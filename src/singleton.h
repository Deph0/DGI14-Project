#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template <class T>
class Singleton {
public:
	static T* getInstance() {
        if (_t == NULL)
            _t = new T();
		return _t;
	}

private:
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);

    static T* _t;

protected:
    Singleton() { }
};

template <class T>
T* Singleton<T>::_t = NULL;

#endif // _SINGLETON_H_
