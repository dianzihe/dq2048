#include "FontCharacter.h"

CFontCharacter::CFontCharacter(void)
{
}

CFontCharacter::~CFontCharacter(void)
{
}

bool CFontCharacter::isEmoji(std::string &content)
{
	if( content.size() < 3 ) return false; //长度不够则不是表情符

	for (size_t i = 0; i < content.size(); i++)
	{

		unsigned char oneChar = content[i];

		if( i+ 1 > content.size()) return false;

		unsigned char twoChar = ((unsigned char)content[i+1]);

		if( i+2 < content.size())
		{
			//如果现在处理的是后两个是相同，则前缀不同的字符
			//
			unsigned char threeChar =((unsigned char)content[i+2]);

			if( twoChar == 0xE2 && threeChar == 0x83 )
			{
				switch(oneChar)
				{
				case 0x23:
				case 0x30:
				case 0x31:
				case 0x32:
				case 0x33:
				case 0x34:
				case 0x35:
				case 0x36:
				case 0x37:
				case 0x38:
				case 0x39:
					return true;
					break;
				default:
					break;
				}
			}
		}

		switch (oneChar)
		{
		case 0xE0:
			{
				switch(twoChar)
				{
				case 0xB8:
					return true;
					break;
				default:
					break;
				}
			}
			break;
		case 0xE3:
			{
				switch(twoChar)
				{
					//case 0x80:
					//case 0x81:
				case 0x82:
				case 0x83:
				case 0x84:
				case 0x85:
				case 0x86:
				case 0x87:
				case 0x88:
				case 0x89:
				case 0x8A:
					return true;
					break;
				default:
					break;
				}
			}
			break;
		case 0xC2:
			{
				switch(twoChar)
				{
				case 0xA9:
				case 0xAE:
					return true;
					break;
				default:
					break;
				}
			}
			break;
		case 0xE2://IOS与安卓都有表情头
			{
				switch (twoChar)
				{
				case 0x96:
				case 0x97:
				case 0x9E:
				case 0xAC:
				case 0x8A:
				case 0x8B:
				case 0x8C:
				case 0x8D:
				case 0x8E:
				case 0x8F:
				case 0x90:
				case 0x91:
				case 0x92:
				case 0x93:
				case 0x95:
				case 0x98:
				case 0x99:
				case 0x9A:
				case 0x9B:
				case 0x9C:
				case 0x9D:
				case 0xAD:
				case 0x94:
					//case 0x80:
					//case 0x81:
				case 0x82:
				case 0x83:
				case 0x84:
				case 0x85:
				case 0x86:
				case 0x87:
				case 0x88:
				case 0x89:
					return true;
					break;

				default:
					break;
				}
			}
			break;
		case 0xF0://IOS表情头
			//if (i+1 <content.size())
			{
				switch (twoChar)
				{
				case 0x9F:
					return true;
					break;
				default:
					break;
				}
			}
			break;
		case 0xED://安卓表情头
			{
				switch(twoChar)
				{
				case 0xBC:
				case 0xBD:
				case 0xA0:
				case 0xB1:
				case 0xB2:
				case 0xB3:
				case 0xB4:
				case 0xB5:
				case 0xB6:
				case 0xB7:
				case 0xB8:
				case 0xB9:
				case 0xBA:
				case 0xBB:
				case 0xBE:
				case 0xBF:
					return true;
					break;
				}
			}
			break;
		default:
			break;
		}
	}
	return false;
}

bool CFontCharacter::IsGB2312Code(const std::string& str)
{
	unsigned char ch1;
	unsigned char ch2;
	if (str.size() >= 2)
	{
		ch1 = (unsigned char)str.at(0);
		ch2 = (unsigned char)str.at(1);
		return ((ch1 >= 0xA1 && ch1 <= 0xF7 && ch2 >= 0xA1 && ch2 <= 0xFE)?true:false);
	}
	else 
	{
		return false;
	}
}

bool CFontCharacter::IsGBKCode(const std::string& str)
{
	unsigned char ch1;
	unsigned char ch2;
	if (str.size() >= 2)
	{
		ch1 = (unsigned char)str.at(0);	
		ch2 = (unsigned char)str.at(1);
	    
		return (ch1 >= 0x81 && ch1 <= 0xFE && ((ch2 >= 0x40 && ch2 <= 0xFE)||(ch2>=0x80 && ch2<=0xfe))?true :false);
	
	}
	else 
	{
		return false;
	}
}

bool CFontCharacter::IsBIG5( const std::string& str )
{
	unsigned char ch = 0;
	unsigned char ch1 = 0;

	if (str.size() < 2)
	{
		return false;
	}

	ch =str.at(0);
	ch1 =str.at(1);

	return ((ch >= 0xa1 && ch<=0xf9 &&   
		(ch1>=0x40 && ch1<=0x7e ||   
		ch1>=0xa1 && ch1<=0xfe)) ? true : false);  
}



bool CFontCharacter::IsUTF8Code(const std::string& str)
{
	unsigned char chr = 0;
	unsigned int nInfoNumber = 0;
	for (int i = 0; i < (int)str.size(); ++i)
	{
		chr = (unsigned char)str.at(i);

		// 如果是该字节是 0XXX XXXX 样式，说明其是一个英文文字，占1字节
		if ((chr >> 7) == 0)
		{
			if (nInfoNumber != 0)
			{
				return false;
			}
			nInfoNumber = 0;
		}
		// 如果该字节是 1111 110X 样式，说明其是一个文字的头，且该文字占6字节
		else if ((chr >> 1) == 126)
		{
			if (nInfoNumber != 0)
			{
				return false;
			}
			nInfoNumber = 5;
		}
		// 如果该字节是 1111 10XX 样式，说明其是一个文字的头，且该文字占5字节
		else if ((chr >> 2) == 62)
		{
			if (nInfoNumber != 0)
			{
				return false;
			}
			nInfoNumber = 4;
		}
		// 如果该字节是 1111 0XXX 样式，说明其是一个文字的头，且该文字占4字节
		else if ((chr >> 3) == 30)
		{
			if (nInfoNumber != 0)
			{
				return false;
			}
			nInfoNumber = 3;
		}
		// 如果该字节是 1110 XXXX 样式，说明其是一个文字的头，且该文字占3字节
		else if ((chr >> 4) == 14)
		{
			if (nInfoNumber != 0)
			{
				return false;
			}
			nInfoNumber = 2;
		}
		// 如果该字节是 110X XXXX 样式，说明其是一个文字的头，且该文字占2字节
		else if ((chr >> 5) == 6)
		{
			if (nInfoNumber != 0)
			{
				return false;
			}
			nInfoNumber = 1;
		}
		// 该字节是 10XX XXXX 样式，它有可能是一个utf8汉字的后续字节
		else if ((chr >> 6) == 2)
		{
			if (nInfoNumber != 0)
			{
				--nInfoNumber;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool CFontCharacter::IsDigit(const std::string& str)
{
	unsigned char chr;
	for (int i = 0; i < (int)str.size(); ++i)
	{
		chr = str.at(i);
		if (chr < 48 || chr > 57)
		{
			return false;
		}
	}
	return true;
}
bool CFontCharacter::IsDigitAndEdh(const std::string& str)
{
	unsigned char chr;
	for (int i = 0; i < (int)str.size(); ++i)
	{
		chr = str.at(i);
		// 在安卓上，会单次传入整个字符串进行判断，故采用这种方式 [13-04-29 Joeson]
		bool isDigitAndEdh = ((chr >= 48 && chr <= 57) || (chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122) || chr == 95);
		if (!isDigitAndEdh)
		{
			return false;
		}
	}
	return true;
}

//0000 – 007F 0xxxxxxx
//0080 – 07FF 110xxxxx 10xxxxxx(中文编码范围)
//0800 – FFFF 1110xxxx 10xxxxxx 10xxxxxx
bool CFontCharacter::IsUTF8Chinese( const std::string&str )
{
	unsigned int nInfoNumber = 0;
	unsigned char ch;
	//判断变量
	unsigned char temp;
	if (str.size() >= 2)
	{
		while(nInfoNumber < str.size())
		{
			ch = (unsigned char) str.at(nInfoNumber);
			//说明是英文
			if (ch >> 7 == 0)
			{
				return false;
			}
			else if (ch >>6 == 2)
			{
				return false;
			}
			else
			{
				if (ch >>5 == 7)
				{
					temp = (ch << 3 >>3);
					if (temp <0x80 && temp >0x1f)
					{
						return false;
					}

					++nInfoNumber;
					ch = str.at(nInfoNumber);
					if (ch >> 6 != 2)
					{
						return false;
					}


					++nInfoNumber;
					ch = str.at(nInfoNumber);
					if (ch >> 6 != 2)
					{
						return false;
					}
					temp = (ch << 3 >> 3);
					if (temp <0x80 && temp >0x1f)
					{
						return false;
					}
				}
				else
				{
					temp = (ch << 3 >>3);
					if (temp <=0 && temp >0x1f)
					{
						return false;
					}

					++nInfoNumber;
					ch = str.at(nInfoNumber);
					if (ch >> 6 != 2)
					{
						return false;
					}

				}
			}
			++nInfoNumber;
		}
	}


	return true;

}

bool CFontCharacter::IsGBKChinese( const std::string&str )
{
	unsigned char ch = 0;
	unsigned int nInfoNumber = 0;

	if (str.size() < 2)
		return false;

	if ((str.size()%2))
	{
		return false;
	}

	while(nInfoNumber < str.size())
	{
		ch = (unsigned char) str.at(nInfoNumber);
		if (ch < 0x81 && ch > 0xFE)
		{
			return false;
		}
		++nInfoNumber;
		
		ch = (unsigned char) str.at(nInfoNumber);
		if (ch < 0x40 && ch > 0xFE)
		{
			return false;
		}
		++nInfoNumber;
	}
	return true;
}


//汉字区的“高位字节”的范围是0xB0-0xF7，“低位字节”的范围是0xA1-0xFE，占用的码位是 72*94=6768。其中有5个空位是D7FA-D7FE。
bool CFontCharacter::IsGB2312Chinese( const std::string& str )
{
	unsigned char ch = 0;
	unsigned int nInfoNumber = 0;

	if (str.size() < 2)
		return false;

	if ((str.size()%2))
	{
		return false;
	}

	while(nInfoNumber < str.size())
	{
		ch = (unsigned char) str.at(nInfoNumber);

		if (ch < 0xB0 && ch > 0xF7)
		{
			return false;
		}

		if (ch == 0xD7)
		{
			int nTemp =nInfoNumber+1;
            ch = (unsigned char) str.at(nTemp);
			if (ch>=0xFA && ch<=0xFE)
			{
				return false;
			}
		}
		++nInfoNumber;

		ch = (unsigned char) str.at(nInfoNumber);
		if (ch < 0xA1 && ch > 0xFE)
		{
			return false;
		}
		++nInfoNumber;
	}
	return true;
}
