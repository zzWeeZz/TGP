#pragma once
#include <string>

class String
{
public:
	String();
	String(String& str);
	String(const char* cptr);
	String(const std::string& str);
	
	void operator=(const std::string& str);
	void operator=(const char* cptr);

	bool operator==(const std::string& str);
	bool operator==(const char* str);
	bool operator==(const String& str);

	bool operator!=(const std::string& str);
	bool operator!=(const char* str);
	bool operator!=(const String& str);

	void Resize();
	const char* c_str();
	char* Data();
	size_t GetSize();

	~String();

private:
	char* m_Data;
	size_t m_Size;
};

inline String::String()
{
	m_Data = new char[16];
	m_Size = 16;
	strset(m_Data, 0);
}

inline String::String(String& str)
{
	m_Data = new char[str.GetSize()];
	m_Size = str.GetSize();
	strcpy(m_Data, str.m_Data);
}

inline String::String(const char* cptr)
{
	int size = strlen(cptr);
	size--;
	size |= size >> 1;
	size |= size >> 2;
	size |= size >> 4;
	size |= size >> 8;
	size |= size >> 16;
	size++;
	m_Data = new char[size];
	m_Size = size;
	strcpy(m_Data, cptr);
}

inline String::String(const std::string& str)
{
	m_Data = new char[str.size()];
	m_Size = str.size();
	strcpy(m_Data, str.c_str());
}

inline void String::operator=(const std::string& str)
{
	strset(m_Data, 0);
	while (str.size() >= m_Size)
	{
		Resize();
	}
	strcpy(m_Data, str.c_str());
}

inline void String::operator=(const char* cptr)
{
	while (strlen(cptr) >= m_Size)
	{
		Resize();
	}
	strcpy(m_Data, cptr);
}

inline bool String::operator==(const std::string& str)
{
	return 0 == strcmp(m_Data, str.c_str());
}

inline bool String::operator==(const char* str)
{
	return 0 == strcmp(m_Data, str);
}

inline bool String::operator==(const String& str)
{
	return 0 == strcmp(m_Data, str.m_Data);
}

inline bool String::operator!=(const std::string& str)
{
	return !operator==(str);
}

inline bool String::operator!=(const char* str)
{
	return !operator==(str);
}

inline bool String::operator!=(const String& str)
{
	return !operator==(str);
}

inline void String::Resize()
{
	if (m_Size == 0)
	{
		m_Size = 2;
		m_Data = new char[m_Size];
	}
	char* temp = new char[m_Size];
	m_Size *= 2;
	strcpy(temp, m_Data);
	m_Data = new char[m_Size];
	strcpy(m_Data, temp);
}

inline const char* String::c_str()
{
	return m_Data;
}

inline char* String::Data()
{
	return m_Data;
}

inline size_t String::GetSize()
{
	return m_Size;
}

inline String::~String()
{
	/*delete[] m_Data;*/
}