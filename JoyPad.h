//joypad

#pragma once


#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#include<vector>

template<typename T>
void SafeRelease(T*& p)
{

	if (p)
	{
		p->Release();
		p = NULL;
	}

}


class JoyPad
{
public:
	JoyPad();
	~JoyPad();

	bool Initialize(HWND window);

	void Update();

private:

	//女医パッド一個分の情報
	struct JoyPadSet {

		JoyPadSet()
		{
			inputDevice = NULL;
		}

		//デバイスをしまっておく
		LPDIRECTINPUTDEVICE8 inputDevice;

		//入寮ク情報
		DIJOYSTATE2 joypad;

		//前回の入力情報
		DIJOYSTATE2 joypadOld;

		
		
	};

	//女医パッド配列
	std::vector<JoyPadSet> m_joyPadSet;

	//DirectInputインスタンス
	LPDIRECTINPUT8 m_pDInput;

};

