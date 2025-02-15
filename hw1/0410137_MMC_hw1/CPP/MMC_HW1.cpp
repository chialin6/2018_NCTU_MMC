// MMC_HW1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"  
#include <iostream>  
#include <fstream>  
#include <string>  
#include <windows.h>  
#include <gdiplus.h> 
#include <vector>
#pragma comment(lib, "gdiplus.lib")  

using namespace std;
using namespace Gdiplus;

void BitmapToJpg(Bitmap* p_bmp, int width, int height);
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

wstring utf8toUtf16(const string & str);

int main(int argc, char *argv[])
{
	GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;

	GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);
	cout << "Please type the filename(with .bmp) " << endl;
	
	string bmpname;
	cin >> bmpname;
	wstring infilename = utf8toUtf16(bmpname);

	Bitmap* bmp = new Bitmap(infilename.c_str());
	UINT height = bmp->GetHeight();
	UINT width = bmp->GetWidth();
	cout << "width " << width << ", height " << height << endl;
	
	BitmapToJpg(bmp, width, height);

	delete bmp;
	GdiplusShutdown(gdiplustoken);
	return 0;
}

void BitmapToJpg(Bitmap* p_bmp, int width, int height)
{
	CLSID pngClsid;
	int result = GetEncoderClsid(L"image/jpeg", &pngClsid);
	if (result != -1)
		std::cout << "Encoder succeeded" << std::endl;
	else
		std::cout << "Encoder failed" << std::endl;
	p_bmp->Save(L"output.jpg", &pngClsid, NULL);
	//delete p_bmp;
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0;          // number of image encoders
	UINT size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
	{
		return -1;  // Failure
	}

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
	{
		return -1;  // Failure
	}

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

wstring utf8toUtf16(const string & str)
{
	if (str.empty())
		return wstring();

	size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
		str.data(), (int)str.size(), NULL, 0);
	if (charsNeeded == 0)
		throw runtime_error("Failed converting UTF-8 string to UTF-16");

	vector<wchar_t> buffer(charsNeeded);
	int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
		str.data(), (int)str.size(), &buffer[0], buffer.size());
	if (charsConverted == 0)
		throw runtime_error("Failed converting UTF-8 string to UTF-16");

	return wstring(&buffer[0], charsConverted);
}