
#include "JoyPad.h"


//���o��������p�b�h���i�[���邽�߂̍\����
struct SearchJoyPadSet
{
	//directInput
	LPDIRECTINPUT8 dinput;

	//�e�R���g���[�����Ƃ̃f�o�C�X�����܂�
	std::vector<LPDIRECTINPUTDEVICE8> joyPadDev;

	//�R���X�g���N�^
	SearchJoyPadSet()
	{
		dinput = NULL;
	
	}

};

//���o�������̓I�u�W�F�N�g���ɌĂ΂��R�[���o�b�N
BOOL CALLBACK EnumObjectCalBack(LPCDIDEVICEOBJECTINSTANCE pInstance, void* pvRef)
{
	//��������
	return DIENUM_CONTINUE;
}


//���o��������p�b�h���ƂɌĂ΂��R�[���o�b�N
BOOL CALLBACK EnumJoyPadCallBack(LPCDIDEVICEINSTANCE lpddi, void* pvRef)
{

	HRESULT ret;

	SearchJoyPadSet*  param = (SearchJoyPadSet*)pvRef;

	LPDIRECTINPUTDEVICE8 dev;

	//�R���g���[�����ƂɃf�o�C�X����
	ret = param->dinput->CreateDevice(lpddi->guidInstance, &dev, NULL);

	if (ret != DI_OK)//���s�����猋�ʂ�Ԃ�
	{
		goto NEXT;
		
	}

	//�f�[�^�`����ݒ�i�g���@�\�t���W���C�X�e�B�b�N�j
	ret =dev->SetDataFormat(&c_dfDIJoystick2);

	if (ret != DI_OK)//���s�����猋�ʂ�Ԃ�
	{
		goto NEXT;

	}

	//�z��ɒǉ�
	param->joyPadDev.push_back(dev);

NEXT:

	//��������
	return DIENUM_CONTINUE;
}


JoyPad::JoyPad()
{

	//�z��ɂ���f�o�C�X���J��
	std::vector<JoyPadSet>::iterator it;
	for (it = m_joyPadSet.begin(); it != m_joyPadSet.end(); it++)
	{
		SafeRelease(it->inputDevice);
	}
}


JoyPad::~JoyPad()
{
}

bool JoyPad::Initialize(HWND window)
{

	//����
	HRESULT ret;


	//DirectInput�̍쐬
	ret = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pDInput,NULL);


	if (ret != DI_OK)//���s�����猋�ʂ�Ԃ�
	{
		return false;
	}


	SearchJoyPadSet param;

	param.dinput = m_pDInput;

	//���p�\�f�o�C�X���
	ret = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyPadCallBack, &param, DIEDFL_ATTACHEDONLY);




	if (ret != DI_OK)//���s�����猋�ʂ�Ԃ�
	{
		return false;
	}
	
	
	//�f�o�C�X�z���ݒ�
	std::vector<LPDIRECTINPUTDEVICE8>::iterator it;
	for (it = param.joyPadDev.begin(); it != param.joyPadDev.end(); it++)
	{
		LPDIRECTINPUTDEVICE8 dev = *it;

		//�A�v�����A�N�e�B�u���f�o�C�X��r���A�N�Z�X�ɐݒ�
		ret = dev->SetCooperativeLevel(window, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		if (ret != DI_OK)//���s�����猋�ʂ�Ԃ�
		{
			continue;
		}

		//�A�N�Z�X���v��
		ret = dev->Acquire();

		if (ret != DI_OK)//���s�����猋�ʂ�Ԃ�
		{
			continue;
		}


		JoyPadSet initPad;

		initPad.inputDevice = dev;

		//�z��ɒǉ�����
		


	}

	return true;
}

void JoyPad::Update()
{
	//�S�Ẵf�o�C�X�ɑ΂��ď�������
	std::vector<JoyPadSet>::iterator it;
	for (it = m_joyPadSet.begin(); it != m_joyPadSet.end(); it++)
	{
		HRESULT ret;
		//�����X�V����
		ret = it->inputDevice->Poll();
		if (ret != DI_OK)
		{
			//�A�N�Z�X���v��
			ret = it->inputDevice->Acquire();
			while (ret == DIERR_INPUTLOST)
			{
				ret = it->inputDevice->Acquire();
			}

			//�O�t���[���̓��͏����o�b�N�A�b�v
			it->joypadOld = it->joypad;
			//���͏����擾
			ret = it->inputDevice->GetDeviceState(sizeof(DIJOYSTATE2), &it->joypad);

			


			if (ret != DI_OK)//���s�������蒼��
			{
				continue;
			}




			if ((BYTE)(it->joypad.rgbButtons[0] & 0x80 == 0))
			{
				//�{�^����������Ă���

			}

		}

	}


}
