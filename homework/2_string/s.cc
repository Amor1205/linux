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

    void push_back(char c){
	if(_size == _capacity)
	{
		reserve(_capacity == 0 ? 4 : _capacity * 2);

	}
	_str[_size] = c;
	++_size;
	_str[_size] = '\0';
	
    }

    string& operator+=(char c){
	    push_back(c);
	    return *this;
    }
    void append(const char* str){
	    size_t len = strlen(str);
	    if(_size+ len >= _capacity)
	    {
		    reserve(_size + len);

	    }
	    strcpy(_str+ _size, str);
	    _size += len;

    }
    string& operator+=(const char* str){
	    append(str);
	    return *this;
    }
    void clear(){
	    _str[0] = '\0';
	    _capacity = _size = 0;
    }


    const char* c_str()const {
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

    void resize(size_t n, char c = '\0'){
	    if(n < _size)
	    {
		    _size = n;
		    _str[_size] = '\0';
	    }
	    else{
		    if(n > _capacity)
		    {
			    reserve(n);
		    }
		    memset(_str + _size, c , n - _size);
		    _size = n;
		    _str[_size] = '\0';
	    }
    }
    void reserve(size_t n){
	    if(n > _capacity)
	    {
		    char* tmp = new char[n+1];
		    strcpy(tmp,_str);
		    delete[] _str;

		    _str = tmp;
		    _capacity = n;
	    }
    }

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

    bool operator<(const string& s){
	    return !(*this > s);
    }
    bool operator<=(const string& s){
	    return !(*this > s);
    }
    bool operator>(const string& s){
	if(strcmp(_str, s._str))
		return true;
	else{
		return false;
	}
    }
    bool operator>=(const string& s){
	    return (*this > s || *this == s);
    }
    bool operator==(const string& s){
	    if(strcmp(_str,s._str)==0)
		    return true;
	    return false;
    }
    bool operator!=(const string& s){
	    return !(*this == s);
    }


    // 返回c在string中第一次出现的位置

    size_t find (char c, size_t pos = 0) const{
	    assert(pos <= _size && pos >= 0);
	    while(pos<=_size)
	    {
		    if(_str[pos] == c)
			    return pos;
		    else{
			    ++pos;
		    }
	    }
	    return std::string::npos;
    }

    // 返回子串s在string中第一次出现的位置

    size_t find (const char* s, size_t pos = 0) const{
	    assert(pos <= _size && pos >= 0);
	    const char* ptr =  strstr(_str + pos, s);
	    if( ptr == nullptr)
	    {
		    return std::string::npos;
	    }
	    else{
		    return ptr - _str;
	    }
	    
    }
    // 在pos位置上插入字符c/字符串str，并返回该字符的位置

    string& insert(size_t pos, char c){
	assert(pos <= _size && pos >= 0);
	if(_size == _capacity)
	{
		reserve(_capacity == 0 ? 4 : 2 * _capacity);
	}	
	if(pos == _size)
	{
		push_back(c);
	}
	else{
		size_t end = _size + 1;
		while( end > pos )
		{
			_str[end] = _str[end - 1];
			-- end;
		}
		_str[pos] = c;
		++_size;
	}
	return *this;
    }
    string& insert(size_t pos, const char* str){
	assert(pos <= _size && pos >= 0);
	size_t len = strlen(str);
	if(_size + len > _capacity)
	{
		reserve(_size + len);
	}
	if(pos == _size)
	{
		append(str);
	}
	else{
		size_t end = _size + len;
		while(pos < end)
		{
			_str[end] = _str[end - len];
			-- end;
		}
		memcpy(_str + pos, str, len);
		_size += len;
		return *this;
	}

    }     

    // 删除pos位置上的元素，并返回该元素的下一个位置

    string& erase(size_t pos = 0, size_t len = std::string::npos){
	if(len == std::string::npos || len + pos >= _size)
	{
		_str[pos] = '\0';
		_size = pos;
	}
	else{
		strcpy(_str + pos, _str + pos + len);
		_size -= len;
	}
    }
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
	s.insert(0,"hello??????");
	s.erase(5,6);
	for(auto ch : s)
	{
		std::cout << ch;
	}
	std::cout << " ";
	std::cout << s.find('o');	
	//std::cout << s ;
/*	amor::string::iterator it = s.begin();
	while(it != s.end())
	{
		std::cout << *it;
		++it;
	}
	std::cout << " ";
	//std::cout << std::endl;
*/
	return 0;
}
