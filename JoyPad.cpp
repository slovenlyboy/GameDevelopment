
#include "JoyPad.h"


//検出した女医パッドを格納するための構造体
struct SearchJoyPadSet
{
	//directInput
	LPDIRECTINPUT8 dinput;

	//各コントローラごとのデバイスをしまう
	std::vector<LPDIRECTINPUTDEVICE8> joyPadDev;

	//コンストラクタ
	SearchJoyPadSet()
	{
		dinput = NULL;
	
	}

};

//検出した入力オブジェクト毎に呼ばれるコールバック
BOOL CALLBACK EnumObjectCalBack(LPCDIDEVICEOBJECTINSTANCE pInstance, void* pvRef)
{
	//次を検索
	return DIENUM_CONTINUE;
}


//検出した女医パッドごとに呼ばれるコールバック
BOOL CALLBACK EnumJoyPadCallBack(LPCDIDEVICEINSTANCE lpddi, void* pvRef)
{

	HRESULT ret;

	SearchJoyPadSet*  param = (SearchJoyPadSet*)pvRef;

	LPDIRECTINPUTDEVICE8 dev;

	//コントローラごとにデバイス生成
	ret = param->dinput->CreateDevice(lpddi->guidInstance, &dev, NULL);

	if (ret != DI_OK)//失敗したら結果を返す
	{
		goto NEXT;
		
	}

	//データ形式を設定（拡張機能付きジョイスティック）
	ret =dev->SetDataFormat(&c_dfDIJoystick2);

	if (ret != DI_OK)//失敗したら結果を返す
	{
		goto NEXT;

	}

	//配列に追加
	param->joyPadDev.push_back(dev);

NEXT:

	//次を検索
	return DIENUM_CONTINUE;
}


JoyPad::JoyPad()
{

	//配列にあるデバイスを開放
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

	//結果
	HRESULT ret;


	//DirectInputの作成
	ret = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pDInput,NULL);


	if (ret != DI_OK)//失敗したら結果を返す
	{
		return false;
	}


	SearchJoyPadSet param;

	param.dinput = m_pDInput;

	//利用可能デバイスを列挙
	ret = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyPadCallBack, &param, DIEDFL_ATTACHEDONLY);




	if (ret != DI_OK)//失敗したら結果を返す
	{
		return false;
	}
	
	
	//デバイス配列を設定
	std::vector<LPDIRECTINPUTDEVICE8>::iterator it;
	for (it = param.joyPadDev.begin(); it != param.joyPadDev.end(); it++)
	{
		LPDIRECTINPUTDEVICE8 dev = *it;

		//アプリがアクティブ時デバイスを排他アクセスに設定
		ret = dev->SetCooperativeLevel(window, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		if (ret != DI_OK)//失敗したら結果を返す
		{
			continue;
		}

		//アクセス権要求
		ret = dev->Acquire();

		if (ret != DI_OK)//失敗したら結果を返す
		{
			continue;
		}


		JoyPadSet initPad;

		initPad.inputDevice = dev;

		//配列に追加する
		


	}

	return true;
}

void JoyPad::Update()
{
	//全てのデバイスに対して処理する
	std::vector<JoyPadSet>::iterator it;
	for (it = m_joyPadSet.begin(); it != m_joyPadSet.end(); it++)
	{
		HRESULT ret;
		//情報を更新する
		ret = it->inputDevice->Poll();
		if (ret != DI_OK)
		{
			//アクセス権要求
			ret = it->inputDevice->Acquire();
			while (ret == DIERR_INPUTLOST)
			{
				ret = it->inputDevice->Acquire();
			}

			//前フレームの入力情報をバックアップ
			it->joypadOld = it->joypad;
			//入力情報を取得
			ret = it->inputDevice->GetDeviceState(sizeof(DIJOYSTATE2), &it->joypad);

			


			if (ret != DI_OK)//失敗したらやり直す
			{
				continue;
			}




			if ((BYTE)(it->joypad.rgbButtons[0] & 0x80 == 0))
			{
				//ボタンが押されている

			}

		}

	}


}
