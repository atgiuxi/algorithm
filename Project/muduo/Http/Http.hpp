#include "../Server.hpp"
#include <fstream>

class Util
{
public:
	// 按照sep字符分割 : 存放到array中!
	static ssize_t Split(const std::string& src,const std::string& sep,std::vector<std::string>* array)
	{
		int start = 0;

		while(start < src.size())
		{
			// find就是找到对应的下标
			ssize_t pos = src.find(sep,start); // 注意找的位置要改变了
			if(pos == std::string::npos) // 找到结尾
			{
				// 这里不能是pos.pos截取到尾部那就再去访问src.substr就崩溃了
				array->push_back(src.substr(start));
				break;
			}
			if(start == pos) // 去除sep,如sep是多个空格去除.
			{
				start = pos + 1;
				continue;
			}
			// substr(起始的位置,截取的长度)
			array->push_back(src.substr(start,pos - start));
			// 起始的位置跳过sep
			start = pos + 1;
		}
		return array->size();
	}

	// 读取文件中所有内容	
	static bool ReadFile(const std::string& filename,std::string * buf)
	{
		std::ifstream ifs(filename,std::ios::binary);
		if(ifs.is_open() == false)
		{
			printf("OPEN %s FILE FAILED!",filename.c_str());
			return false;
		}

		// 打开成功
		// 计算文件的大小
		size_t fsize = 0;
		ifs.seekg(0,ifs.end);	// 跳转文件末尾 
		fsize = ifs.tellg();	// 获取当前位置相对于起始位置的偏移量.相当于文件的大小
		ifs.seekg(0,ifs.beg);	// 跳转到文件的起始位置

		buf->resize(fsize);// 提前开辟空间
		ifs.read(&(*buf)[0]/*c_str()是const的*/,fsize);

		if(ifs.good() == false)
		{
			printf("READ %s FILE FAILED!",filename.c_str());
			ifs.close();
			return false;
		}
		ifs.close();
		return true;
	}

	// 将数据读入文件中
	static bool WriteFile(const std::string filename,const std::string& buf)
	{
		std::ofstream ofs(filename,std::ios::binary | std::ios::trunc/*文件存在清空*/);
		if(ofs.is_open() == false)
		{
			printf("OPEN %s FILE FAILED!",filename.c_str());
			return false;
		}

		ofs.write(buf.c_str(),buf.size());
		if(ofs.good() == false)
		{
			printf("WRITE %s FILE FAILED!",filename.c_str());
			ofs.close();
			return false;
		}
		ofs.close();
		return true;
	}

	

};