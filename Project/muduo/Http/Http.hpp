#include "../Server.hpp"
#include <fstream>
#include <unordered_map>
#include <sys/stat.h>

std::unordered_map<int, std::string> _statu_msg = {
    {100,  "Continue"},
    {101,  "Switching Protocol"},
    {102,  "Processing"},
    {103,  "Early Hints"},
    {200,  "OK"},
    {201,  "Created"},
    {202,  "Accepted"},
    {203,  "Non-Authoritative Information"},
    {204,  "No Content"},
    {205,  "Reset Content"},
    {206,  "Partial Content"},
    {207,  "Multi-Status"},
    {208,  "Already Reported"},
    {226,  "IM Used"},
    {300,  "Multiple Choice"},
    {301,  "Moved Permanently"},
    {302,  "Found"},
    {303,  "See Other"},
    {304,  "Not Modified"},
    {305,  "Use Proxy"},
    {306,  "unused"},
    {307,  "Temporary Redirect"},
    {308,  "Permanent Redirect"},
    {400,  "Bad Request"},
    {401,  "Unauthorized"},
    {402,  "Payment Required"},
    {403,  "Forbidden"},
    {404,  "Not Found"},
    {405,  "Method Not Allowed"},
    {406,  "Not Acceptable"},
    {407,  "Proxy Authentication Required"},
    {408,  "Request Timeout"},
    {409,  "Conflict"},
    {410,  "Gone"},
    {411,  "Length Required"},
    {412,  "Precondition Failed"},
    {413,  "Payload Too Large"},
    {414,  "URI Too Long"},
    {415,  "Unsupported Media Type"},
    {416,  "Range Not Satisfiable"},
    {417,  "Expectation Failed"},
    {418,  "I'm a teapot"},
    {421,  "Misdirected Request"},
    {422,  "Unprocessable Entity"},
    {423,  "Locked"},
    {424,  "Failed Dependency"},
    {425,  "Too Early"},
    {426,  "Upgrade Required"},
    {428,  "Precondition Required"},
    {429,  "Too Many Requests"},
    {431,  "Request Header Fields Too Large"},
    {451,  "Unavailable For Legal Reasons"},
    {501,  "Not Implemented"},
    {502,  "Bad Gateway"},
    {503,  "Service Unavailable"},
    {504,  "Gateway Timeout"},
    {505,  "HTTP Version Not Supported"},
    {506,  "Variant Also Negotiates"},
    {507,  "Insufficient Storage"},
    {508,  "Loop Detected"},
    {510,  "Not Extended"},
    {511,  "Network Authentication Required"}
};

std::unordered_map<std::string, std::string> _mime_msg = {
    {".aac",        "audio/aac"},
    {".abw",        "application/x-abiword"},
    {".arc",        "application/x-freearc"},
    {".avi",        "video/x-msvideo"},
    {".azw",        "application/vnd.amazon.ebook"},
    {".bin",        "application/octet-stream"},
    {".bmp",        "image/bmp"},
    {".bz",         "application/x-bzip"},
    {".bz2",        "application/x-bzip2"},
    {".csh",        "application/x-csh"},
    {".css",        "text/css"},
    {".csv",        "text/csv"},
    {".doc",        "application/msword"},
    {".docx",       "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    {".eot",        "application/vnd.ms-fontobject"},
    {".epub",       "application/epub+zip"},
    {".gif",        "image/gif"},
    {".htm",        "text/html"},
    {".html",       "text/html"},
    {".ico",        "image/vnd.microsoft.icon"},
    {".ics",        "text/calendar"},
    {".jar",        "application/java-archive"},
    {".jpeg",       "image/jpeg"},
    {".jpg",        "image/jpeg"},
    {".js",         "text/javascript"},
    {".json",       "application/json"},
    {".jsonld",     "application/ld+json"},
    {".mid",        "audio/midi"},
    {".midi",       "audio/x-midi"},
    {".mjs",        "text/javascript"},
    {".mp3",        "audio/mpeg"},
    {".mpeg",       "video/mpeg"},
    {".mpkg",       "application/vnd.apple.installer+xml"},
    {".odp",        "application/vnd.oasis.opendocument.presentation"},
    {".ods",        "application/vnd.oasis.opendocument.spreadsheet"},
    {".odt",        "application/vnd.oasis.opendocument.text"},
    {".oga",        "audio/ogg"},
    {".ogv",        "video/ogg"},
    {".ogx",        "application/ogg"},
    {".otf",        "font/otf"},
    {".png",        "image/png"},
    {".pdf",        "application/pdf"},
    {".ppt",        "application/vnd.ms-powerpoint"},
    {".pptx",       "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
    {".rar",        "application/x-rar-compressed"},
    {".rtf",        "application/rtf"},
    {".sh",         "application/x-sh"},
    {".svg",        "image/svg+xml"},
    {".swf",        "application/x-shockwave-flash"},
    {".tar",        "application/x-tar"},
    {".tif",        "image/tiff"},
    {".tiff",       "image/tiff"},
    {".ttf",        "font/ttf"},
    {".txt",        "text/plain"},
    {".vsd",        "application/vnd.visio"},
    {".wav",        "audio/wav"},
    {".weba",       "audio/webm"},
    {".webm",       "video/webm"},
    {".webp",       "image/webp"},
    {".woff",       "font/woff"},
    {".woff2",      "font/woff2"},
    {".xhtml",      "application/xhtml+xml"},
    {".xls",        "application/vnd.ms-excel"},
    {".xlsx",       "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
    {".xml",        "application/xml"},
    {".xul",        "application/vnd.mozilla.xul+xml"},
    {".zip",        "application/zip"},
    {".3gp",        "video/3gpp"},
    {".3g2",        "video/3gpp2"},
    {".7z",         "application/x-7z-compressed"}
};

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

	static std::string UrlEncode(const std::string url,bool covert_space_to_plus)
	{
		std::string res;

		for(auto& ch : url)
		{
			if(ch =='.' || ch == '-' || ch == '_' || ch == '~' || std::isalnum(ch)){
				res += ch;
				continue;
			}

			if(ch == ' ' && covert_space_to_plus == true)
			{
				res += '+';
				continue;
			}

			char tmp[4] = {0};
			// %%(表示一个%)%02x的含义是将变量ch的值以十六进制格式(x)输出，并且前面加上一个百分号（%）
			// 测试 %02x 和 %%%02x
			snprintf(tmp,4,"%%%02x",ch);
			res += tmp;
		}
		return res;
	}

	static char Hextoi(char c)
	{
		if(c >= '0' && c <= '9') {return c - '0';}
		else if(c >= 'a' && c <= 'z') {return c - 'a' + 10;}
		else if(c >= 'A' && c <= 'Z') {return c - 'A' + 10;}
		else return -1;
	}

	static std::string UrlDecode(const std::string url, bool convert_plus_to_space)
	{
		std::string res;

		for(int i = 0;i < url.size();i++)
		{
			if(url[i] == '+' && convert_plus_to_space == true)
			{
				res += ' ';
				continue;
			}

			if(url[i] == '%' && i + 2 < url.size())
			{
				char ch1 = Hextoi(url[i + 1]);
				char ch2 = Hextoi(url[i + 2]);

				res += ch1 * 16 + ch2;
				i += 2;
				continue;
			}
			res += url[i];
		}
		return res;
	}

	// 获取状态码信息
	static std::string StatuDesc(int statu)
	{
		auto it = _statu_msg.find(statu);
		if(it != _statu_msg.end())
		{
			return it->second;
		}
		return "Unknow";
	}

	// mime : 是一种标准，用来表示文档、文件的格式.
	static std::string ExtMime(const std::string &filename)
	{
		// 从尾部找文件的后缀名
		ssize_t pos = filename.find_last_of(".");
		if(pos == std::string::npos)
		{
			return "application/octet-stream";
		}

		// 截取字符串 
		std::string exs = filename.substr(pos);
		auto it = _mime_msg.find(exs);
		if(it == _mime_msg.end())
		{
			return "application/octet-stream";
		}
		return it->second;
	}
	
	// 判断文件的属性！
	// 判断一个文件是否是目录
	static bool IsDirectory(const std::string& filename)
	{
		struct stat st;
		int ret = stat(filename.c_str(),&st);
		if(ret < 0)
		{
			return false;
		}

		return S_ISDIR(st.st_mode);
	}

	// 判断一个文件是否是普通文件
	static bool IsRegular(const std::string& filename)
	{
		struct stat st;
		int ret = stat(filename.c_str(),&st);
		if(ret < 0)
		{
			return false;
		}
		return S_ISREG(st.st_mode);
	}

	// http请求的资源路径有效性判断 /../login.html(无效).判断是否有效,但是找不找得到就不一定了
	static bool ValidPath(const std::string &path)
	{
		// 思想：按照 / 进行路径分割。深度不能小于0
		std::vector<std::string> subdir;
		Split(path,"/",&subdir);
		int level = 0; 
		for(auto & dir : subdir)
		{
			if(dir == "..")
			{
				level--;
				if(level < 0) return false;
				continue;
			}
			level++;
		}
		return true;
	}

};