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

	//����p�b�h����̏��
	struct JoyPadSet {

		JoyPadSet()
		{
			inputDevice = NULL;
		}

		//�f�o�C�X�����܂��Ă���
		LPDIRECTINPUTDEVICE8 inputDevice;

		//�����N���
		DIJOYSTATE2 joypad;

		//�O��̓��͏��
		DIJOYSTATE2 joypadOld;

		
		
	};

	//����p�b�h�z��
	std::vector<JoyPadSet> m_joyPadSet;

	//DirectInput�C���X�^���X
	LPDIRECTINPUT8 m_pDInput;

};

