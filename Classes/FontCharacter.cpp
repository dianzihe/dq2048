#include "FontCharacter.h"

CFontCharacter::CFontCharacter(void)
{
}

CFontCharacter::~CFontCharacter(void)
{
}

bool CFontCharacter::isEmoji(std::string &content)
{
	if( content.size() < 3 ) return false; //���Ȳ������Ǳ����

	for (size_t i = 0; i < content.size(); i++)
	{

		unsigned char oneChar = content[i];

		if( i+ 1 > content.size()) return false;

		unsigned char twoChar = ((unsigned char)content[i+1]);

		if( i+2 < content.size())
		{
			//������ڴ�����Ǻ���������ͬ����ǰ׺��ͬ���ַ�
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
		case 0xE2://IOS�밲׿���б���ͷ
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
		case 0xF0://IOS����ͷ
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
		case 0xED://��׿����ͷ
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

		// ����Ǹ��ֽ��� 0XXX XXXX ��ʽ��˵������һ��Ӣ�����֣�ռ1�ֽ�
		if ((chr >> 7) == 0)
		{
			if (nInfoNumber != 0)
			{
				return false;
			}
			nInfoNumber = 0;
		}
		// ������ֽ��� 1111 110X ��ʽ��˵������һ�����ֵ�ͷ���Ҹ�����ռ6�ֽ�
		else if ((chr >> 1) == 126)
		{
			if (nInfoNumber != 0)
			{
				return false;
			}
			nInfoNumber = 5;
		}
		// ������ֽ��� 1111 10XX ��ʽ��˵������һ�����ֵ�ͷ���Ҹ�����ռ5�ֽ�
		else if ((chr >> 2) == 62)
		{
			if (nInfoNumber != 0)
			{
				return false;
			}
			nInfoNumber = 4;
		}
		// ������ֽ��� 1111 0XXX ��ʽ��˵������һ�����ֵ�ͷ���Ҹ�����ռ4�ֽ�
		else if ((chr >> 3) == 30)
		{
			if (nInfoNumber != 0)
			{
				return false;
			}
			nInfoNumber = 3;
		}
		// ������ֽ��� 1110 XXXX ��ʽ��˵������һ�����ֵ�ͷ���Ҹ�����ռ3�ֽ�
		else if ((chr >> 4) == 14)
		{
			if (nInfoNumber != 0)
			{
				return false;
			}
			nInfoNumber = 2;
		}
		// ������ֽ��� 110X XXXX ��ʽ��˵������һ�����ֵ�ͷ���Ҹ�����ռ2�ֽ�
		else if ((chr >> 5) == 6)
		{
			if (nInfoNumber != 0)
			{
				return false;
			}
			nInfoNumber = 1;
		}
		// ���ֽ��� 10XX XXXX ��ʽ�����п�����һ��utf8���ֵĺ����ֽ�
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
		// �ڰ�׿�ϣ��ᵥ�δ��������ַ��������жϣ��ʲ������ַ�ʽ [13-04-29 Joeson]
		bool isDigitAndEdh = ((chr >= 48 && chr <= 57) || (chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122) || chr == 95);
		if (!isDigitAndEdh)
		{
			return false;
		}
	}
	return true;
}

//0000 �C 007F 0xxxxxxx
//0080 �C 07FF 110xxxxx 10xxxxxx(���ı��뷶Χ)
//0800 �C FFFF 1110xxxx 10xxxxxx 10xxxxxx
bool CFontCharacter::IsUTF8Chinese( const std::string&str )
{
	unsigned int nInfoNumber = 0;
	unsigned char ch;
	//�жϱ���
	unsigned char temp;
	if (str.size() >= 2)
	{
		while(nInfoNumber < str.size())
		{
			ch = (unsigned char) str.at(nInfoNumber);
			//˵����Ӣ��
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


//�������ġ���λ�ֽڡ��ķ�Χ��0xB0-0xF7������λ�ֽڡ��ķ�Χ��0xA1-0xFE��ռ�õ���λ�� 72*94=6768��������5����λ��D7FA-D7FE��
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
