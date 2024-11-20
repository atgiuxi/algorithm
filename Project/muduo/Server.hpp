#include <iostream>
#include <vector>


#define BUFFER_DEFALT_SIZE 1024
class Buffer
{
private:
	std::vector<char> _buffer;	 	// 按照字节(char很妙)存储,用户缓冲区
	uint64_t _writer_idx;			// 写偏移
	uint64_t _read_idx;				// 读偏移
public:
	Buffer():_writer_idx(0),_read_idx(0),_buffer(BUFFER_DEFALT_SIZE){}

public:	
	
};