#pragma once
#include <iostream>

enum FontCodeTpye
{
	FontCodeTpye_GB2312Code,
	FontCodeTpye_GBK,
	FontCodeTpye_UTF8,
	FontCodeTpye_Count,
};
class CFontCharacter
{
private:
	CFontCharacter(void);
public:
	~CFontCharacter(void);
	static bool isEmoji(std::string &content);

	static bool IsGB2312Code(const std::string& str);
	static bool IsGBKCode(const std::string& str);
	static bool IsBIG5(const std::string& str);
	static bool IsUTF8Code(const std::string& str);
	static bool IsDigit(const std::string& str);
	static bool IsDigitAndEdh(const std::string& str);

	static bool IsUTF8Chinese( const std::string&str );
	static bool IsGBKChinese( const std::string&str );
	static bool IsGB2312Chinese(const std::string& str);
};
	