#pragma once

#define  Declare_Singleton(classname) \
			 public: \
			 static classname& Instance() \
{ \
	static classname _instance; \
	return _instance; \
} \
protected: \
    classname() = default; \
    ~classname() = default; \
                            \
    classname(const classname& rInstance); \
    classname& operator = (const classname& rInstance) \

