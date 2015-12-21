#include "stdafx.h"
namespace Tools
{
	vector<unsigned short> Process::pressed_buttons=vector<unsigned short>();
	unsigned Process::GetProcessBase()
	{
		return base_address;
	}
	
	void Process::SetLanguage(Language lang)
	{
		//Language tmp=GetCurrentLanguage();
		while(GetCurrentLanguage()!=lang)
		{
			//tmp=GetCurrentLanguage();
			//PushKeyboardButton(KeyboardButton::ALT);
			PressKeyboardButton(KeyboardButton::LSHIFT,50,KeyboardButton::ALT);
		}
	}
	bool Process::IsButtonPressed(unsigned short button)
	{
		for (auto t_button:pressed_buttons)
		{
			if (button==t_button)
			{
				return true;
			}
		}
		return false;
	}
	void Process::MultipleKeyboardButtonPush(unsigned short button,unsigned quantity,unsigned long interval)
	{
		for (unsigned i=0;i<quantity;i++)
		{
			PressKeyboardButton(button,interval);
		}
	}
	void Process::TypeByKeyboard(char * string)
	{
		unsigned additional_key=0;
		unsigned short key;
		while(*string)
		{
			additional_key=0;
			if (*string>=0x41 && *string<=0x5A)
			{
				additional_key=KeyboardButton::LSHIFT;
				key=*string;
			}
			else if (*string>=0x61 && *string<=0x7A)
			{
				key=*string-0x20;
			}
			else if (*string>=0x30 && *string <=0x39)
			{
				key=*string;
			}
			else
			{
				switch(*string)
				{
				case '@':
					key=0x32;
					additional_key=KeyboardButton::LSHIFT;
					break;
				case '.':
					key=KeyboardButton::PERIOD;
					break;
				}	
			}
			PressKeyboardButton(key,50,additional_key);
			string++;
			
		}
	}
}