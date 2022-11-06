#include <iostream>
#include <assert.h>
//模拟实现string类，并完成测试
#include <cstring>
namespace amor
{

  class string

  {

    friend std::ostream& operator<<(std::ostream& os, const amor::string& s);
    friend std::istream& operator>>(std::istream& is,  amor::string& s);

  public:

    typedef char* iterator;
    typedef const char* const_iterator;
  public:
    string()
    	:_str(new char[1]),_capacity(0),_size(0)
    {
	_str[0] ='\0';
    }
    string(const char* str = "")
	    :_capacity(strlen(str)),_size(_capacity)
	    {
		   _str = new char[_capacity + 1];
		   std::strcpy(_str,str);
	    }
   	
    void swap(string& s)
    {
	    std::swap(_str,s._str);
	    std::swap(_capacity,s._capacity);
	    std::swap(_size,s._size);
    }

    string(const string& s)
	    :_str(nullptr),_capacity(0),_size(0)
    {
	string tmp(s._str);
	swap(tmp);
    }
    string& operator=(const string &s){
	    if(*this != s)
	    {
		string tmp(s._str);
		swap(tmp);
	    }
	    return *this;
    }
    ~string(){
	    delete[] _str;
	    _str = nullptr;
	    _capacity = 0;
	    _size = 0;
    }


    //////////////////////////////////////////////////////////////

    // iterator

    iterator begin(){
	    return _str;
    }
    iterator end(){
	    return _str + _size;
    }
    iterator rbegin(){
	    return _str + _size - 1;
    }
    iterator rend(){
	    return _str - 1;
    }
    const_iterator begin() const {
	    return _str;
    }
    const_iterator end() const {
	    return _str + _size;
    }
    const_iterator rbegin() const {
	    return _str + _size - 1;
    }
    const_iterator rend() const {
	    return _str - 1;
    }
    /////////////////////////////////////////////////////////////

    // modify

    void push_back(char c);

    string& operator+=(char c);

    void append(const char* str);

    string& operator+=(const char* str);

    void clear();


    const char* c_str()const{
	    return _str;
    }

    /////////////////////////////////////////////////////////////

    // capacity

    size_t size()const{
	    return _size;
    }

    size_t capacity()const{
	    return _capacity;
    }

    bool empty()const{
	    return _size==0;
    }

    void resize(size_t n, char c = '\0');

    void reserve(size_t n);


    /////////////////////////////////////////////////////////////

    // access

    char& operator[](size_t index){
	    assert(index < _size && index >= 0);
	    return _str[index];
    }
    const char& operator[](size_t index)const{
	    assert(index < _size && index >= 0);
	    return _str[index];
    }

    /////////////////////////////////////////////////////////////

    //relational operators

    bool operator<(const string& s);

    bool operator<=(const string& s);

    bool operator>(const string& s);

    bool operator>=(const string& s);

    bool operator==(const string& s);

    bool operator!=(const string& s);


    // 返回c在string中第一次出现的位置

    size_t find (char c, size_t pos = 0) const;

    // 返回子串s在string中第一次出现的位置

    size_t find (const char* s, size_t pos = 0) const;

    // 在pos位置上插入字符c/字符串str，并返回该字符的位置

    string& insert(size_t pos, char c);

    string& insert(size_t pos, const char* str);

     

    // 删除pos位置上的元素，并返回该元素的下一个位置

    string& erase(size_t pos, size_t len);

  private:

    char* _str;

    size_t _capacity;

    size_t _size;

  };

};//amor
 
std::ostream& operator<<(std::ostream& os, const amor::string& s)
{
	for(auto ch : s)
	{
		os << ch;
	}
	return os;	
}
std::istream& operator>>(std::istream is, amor::string& s)
{
	char ch = is.get();
	while(ch != ' ' && ch != '\n')
	{
	//	s += ch;
		ch = is.get();
	}
	return is;
}
int main()
{
	amor::string s("hello world");
	/*for(auto ch : s)
	{
		std::cout << ch << " ";
	}
	std::cout << std::endl;
	*/
	//std::cout << s ;
	amor::string::iterator it = s.begin();
	while(it != s.end())
	{
		std::cout << *it << " ";
		++it;
	}
	std::cout << std::endl;

	return 0;
}
