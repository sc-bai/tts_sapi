
#include <iostream>
#include <sapi.h>
#include <sphelper.h>
#include <tchar.h>

#pragma comment(lib, "sapi.lib")

int main1()
{
	std::wstring strSpeak = L"北京，晴，20到30度，偏东风一到二级,降水概率10%";
    ISpVoice* pVoice = nullptr;
	if (FAILED(::CoInitialize(NULL)))
		MessageBox(NULL, (LPCWSTR)L"COM接口初始化失败！", (LPCWSTR)L"提示", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);

	//获取SpVoice接口

	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);


	if (SUCCEEDED(hr))
	{
		pVoice->SetVolume((USHORT)100); //设置音量，范围是 0 -100
		pVoice->SetRate(2); //设置速度，范围是 -10 - 10
		hr = pVoice->Speak(strSpeak.c_str(), 0, NULL);

		pVoice->Release();

		pVoice = NULL;
	}

	//释放com资源
	::CoUninitialize();
    std::cout << "Hello World!\n";
	return 0;
}


int main12() {
	int nChannel =  2;	//声道设置
	int nSamplesPreSec = 55100;			//采样率设置
	int nBitsPerSample = 16;			//采样比特设置
	int nBlockAlign = (nChannel * nBitsPerSample) / 8;

	//此处很重要，录音文件的参数在此配置
	WAVEFORMATEX waveFormate;
	waveFormate.wFormatTag = WAVE_FORMAT_PCM;	//编码,WAVE_FORMAT_ALAW、WAVE_FORMAT_MULAW、WAVE_FORMAT_PCM...	
	waveFormate.nChannels = nChannel;	//采样声道数，对于单声道音频设置为1，立体声设置为2
	waveFormate.nSamplesPerSec = nSamplesPreSec; //采样率,8.0 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz
	waveFormate.nAvgBytesPerSec = nSamplesPreSec * nBlockAlign;	//每秒的数据率，就是每秒能采集多少字节的数据,nSamplesPerSec × nBlockAlign
	waveFormate.nBlockAlign = nBlockAlign; //一个块的大小，采样bit的字节数乘以声道数, (nChannels × wBitsPerSample) / 8
	waveFormate.wBitsPerSample = nBitsPerSample; //8 or 16,采样比特  8bits/次
	waveFormate.cbSize = 0;


	std::wstring strSpeak = L"北京，晴，20到30度，偏东风一到二级,降水概率10%";
	ISpVoice* pVoice = nullptr;
	if (FAILED(::CoInitialize(NULL)))
		MessageBox(NULL, (LPCWSTR)L"COM接口初始化失败！", (LPCWSTR)L"提示", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);

	CComPtr<ISpStream> cpISpStream;
	CComPtr<ISpStreamFormat> cpISpStreamFormat;
	CSpStreamFormat spStreamFormat;
	pVoice->GetOutputStream(&cpISpStreamFormat);
	spStreamFormat.AssignFormat(cpISpStreamFormat);

	HRESULT hResult = SPBindToFile(L"D:\\test.wav",
		SPFM_CREATE_ALWAYS,
		&cpISpStream,
		&spStreamFormat.FormatId(),
		&waveFormate);

	if (SUCCEEDED(hResult))
	{
		//语音包、音量、语速设置
		pVoice->SetVolume((USHORT)100); //设置音量，范围是 0 -100
		pVoice->SetRate(0); //设置速度，范围是 -10 - 10

		pVoice->SetOutput(cpISpStream, TRUE);
		pVoice->Speak(strSpeak.c_str(), SPF_DEFAULT, NULL);
		//MessageBox(_T("保存 WAV 文件成功!"), _T("提示"), MB_OK);

		//ShellExecute(NULL, _T("open"), m_strFileDir, NULL, NULL, SW_SHOWNORMAL);
	}
	else
	{
		//MessageBox(_T("保存 WAV 文件失败!"), _T("提示"), MB_OK | MB_ICONWARNING);
	}

	return 0;
}

#define _USING_V110_SDK71_
#include<sapi.h>
#include<stdio.h>
#include<sphelper.h>
int main() {

	::CoInitialize(NULL);
	ISpVoice* pSpVoice = NULL;
	IEnumSpObjectTokens* pSpEnumTokens = NULL;
	if (FAILED(CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void**)&pSpVoice))) {
		printf("Failed to create instance of ISpVoice!\n");
		return -1;
	}
	if (SUCCEEDED(SpEnumTokens(SPCAT_VOICES, NULL, NULL, &pSpEnumTokens))) {
		ISpObjectToken* pSpToken = NULL;
		while (SUCCEEDED(pSpEnumTokens->Next(1, &pSpToken, NULL)) && pSpToken != NULL)
		{
			printf("ISpObjectToken\n");
			CSpDynamicString dstrDesc;
			SpGetDescription(pSpToken, &dstrDesc);

			pSpVoice->SetVoice(pSpToken);

			//pSpVoice->Speak(L"Hello,Windows NT", SPF_DEFAULT, NULL);
			pSpVoice->Speak(L"北京，晴，20到30度，偏东风一到二级,降水概率10%", SPF_DEFAULT, NULL);
			pSpToken->Release();
		}
		pSpEnumTokens->Release();
	}
	pSpVoice->Release();
	::CoUninitialize();
	printf("over\n");
	return 0;
}