#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>

const uint64_t BUFFER_DEFAULT_SIZE = 1024;
class Buffer{
private:
    std::vector<char> _buffer; //使用vector 进行内存空间管理
    uint64_t _reader_idx; //读偏移
    uint64_t _writer_idx; //写偏移

public:
    Buffer() :_reader_idx(0), _writer_idx(0), _buffer(BUFFER_DEFAULT_SIZE){}
    //对buffer 的起始地址做封装
    char* Begin() {return &*_buffer.begin();}
    //获取当前写入起始地址
    //_buffer 空间起始地址 + 写偏移量
    char* WritePosition(){ return Begin() + _writer_idx;}
    //获取当前读取起始地址
    char* ReadPosition(){ return Begin() + _reader_idx;}

    // _buffer : [ ... _reader_idx .. _writer_idx ...]
    //获取前沿空闲空间大小（读偏移之前
    uint64_t HeadIdleSize() {return _reader_idx;}
    //获取后续空闲空间大小（写偏移之后
    uint64_t TailIdleSize() { return _buffer.size() - _writer_idx;}
    //获取可读数据大小
    uint64_t ReadAbleSize() { return _writer_idx - _reader_idx;}
    //将读偏移向后移动
    void MoveReadOffset(uint64_t len) {
        //向后移动的大小必须小于可读空间大小
        assert(len <= ReadAbleSize());
        _reader_idx += len;
    }
    //将写偏移向后移动
    void MoveWriteOffset(uint64_t len){
        //向后移动的大小必须小于当前后边的空闲大小。
        assert(len <= TailIdleSize());
        _writer_idx += len;
    }
    //确保可写空间足够（整体空闲空间足够：移动，否则：扩容）
    void EnsureWriteSpaceEnough(uint64_t len){
        //如果末尾空闲空间足够，直接返回；
        if(TailIdleSize() >= len) return;
        //如果不够，看加上前面的空闲空间大小后是否足够,够了就将数据移动到起始位置。
        if(len <= TailIdleSize() + HeadIdleSize()){
            //将数据移动到起始位置
            uint64_t rsz = ReadAbleSize(); //把数据大小先保存起来。
            //把可读数据拷贝到起始位置
            std::copy(ReadPosition(), ReadPosition() + rsz, Begin()); 
            _reader_idx = 0; //读偏移归0
            _writer_idx = rsz; //写偏移置为rsz
        }else{
            //总体空间不够，需要扩容，不移动数据，直接给写偏移之后扩容足够的空间即可。
            _buffer.resize(_writer_idx + len);
        }
    }
    //写入数据
    void Write(const void* data, uint64_t len){
        //确保足够空间
        //拷贝数据进缓冲区
        EnsureWriteSpaceEnough(len);
        const char* d = (const char*) data;
        std::copy(d, d+len, WritePosition());
    }
    void WriteString(const std::string& data){
        return Write(&data[0], data.size());
    }
    //读取数据
    void WriteBuffer(Buffer& data){
        return Write(data.ReadPosition(),data.ReadAbleSize());
    }
    void Read(void* buf, uint64_t len){
        //要求获取数据大小必须小于可读数据大小
        assert(len <= ReadAbleSize());
        //ReadPosition() 是void*，+len是没有办法确定步长，需要强转
        //我们在这里直接修改ReadPosition 及其相关接口的返回类型为char* 方便偏移
        //copy 要求第三个参数是char*类型
        std::copy(ReadPosition(), ReadPosition() + len, (char*)buf);
    }
    std::string ReadAsString(uint64_t len){
        assert(len <= ReadAbleSize());
        std::string str;
        str.resize(len);
        //不可以用str.c_str()，因为这个的返回值类型是const char* 无法修改
        Read(&str[0], len);
        return str;
    }

    //读取和弹出同时进行
    void ReadAndPop(void* buf, uint64_t len){
        Read(buf, len);
        MoveReadOffset(len);
    }
    std::string ReadAsStringAndPop(uint64_t len){
        assert(len <= ReadAbleSize());
        std::string str = ReadAsString(len);
        MoveReadOffset(len);
        return str;
    }
    void WriteAndPush(void* data, uint64_t len){
        Write(data, len);
        MoveWriteOffset(len);
    }
    void WriteStringAndPush(std::string& data){
        WriteString(data);
        MoveWriteOffset(data.size());
    }
    void WriteBufferAndPush(Buffer& data){
        WriteBuffer(data);
        MoveWriteOffset(data.ReadAbleSize());
    }
    //获取一行
    char* FindCRLF(){
        //字符查找
        void* res = memchr(ReadPosition(), '\n', ReadAbleSize());
        return (char*)res;
        //找到了返回地址，找不到返回空
    }
    /*通常获取一行数据，这种情况一般针对ASCII码，当作字符串返回没有问题*/
    std::string GetLine(){
        char* pos = FindCRLF();
        if(pos == nullptr) return "";
        //pos 是找到的换行符，减去可读的起始位置，+1是为了把换行符也读取出来
        return ReadAsString(pos - ReadPosition() + 1);
    }
    std::string GetLineAndPop(){
        std::string str = GetLine();
        MoveReadOffset(str.size());
        return str;
    }
    //清理功能:偏移量归0
    void Clear(){ _reader_idx = 0; _writer_idx = 0;}
};