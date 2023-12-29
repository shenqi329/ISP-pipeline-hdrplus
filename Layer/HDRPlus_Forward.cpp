#include "HDRPlus_Forward.h"
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

class PrintTime {
public:
	PrintTime(string name) {
		_start = steady_clock::now();
		_name = name;
	}
	void print() {
		steady_clock::time_point end = steady_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - _start);

		// 输出时间间隔
		printf("Elapsed time :%s: %lld ms\n", _name.data(), duration.count());
	}
private:
	steady_clock::time_point _start;
	string _name;
};

void CHDRPlus_Forward::Forward(MultiUshortImage *InRawImage,MultiUcharImage *OutRGBImage8, TGlobalControl *pControl)
{
	const int Framenum = pControl->nFrameNum;
	int nFrameID[12];
	for (int k = 0; k < Framenum; k++)
	{
		nFrameID[k] = k;
	}
	MultiUshortImage OutDpcRaw;
	MultiUshortImage OutRGBImage16;
	if (m_nBlockMatchFusionEnable)
	{
		PrintTime printTime("m_HDRPlus_BlockMatchFusion");
		m_HDRPlus_BlockMatchFusion.Forward(InRawImage, nFrameID, Framenum, pControl);
		printTime.print();
		// if (m_HDRPlus_BlockMatchFusion.m_bDumpFileEnable)
		{
			InRawImage[0].SaveSingleChannelToBitmapFile("outbmp/BlockMatchFusion.bmp", 0, InRawImage[0].GetMaxVal(), 256, 0);
		}
	}
	if (m_nDPCorrectionEnable)
	{
		PrintTime printTime("m_HDRPlus_DPCorrection");
		m_HDRPlus_DPCorrection.Forward(&InRawImage[0], &OutDpcRaw, pControl);
		printTime.print();
		if (m_HDRPlus_DPCorrection.m_bDumpFileEnable)
		{
			OutDpcRaw.SaveSingleChannelToBitmapFile("outbmp/DPCorrection.bmp", 0, OutDpcRaw.GetMaxVal(), 256, 0);
		}
	}
	else
	{
		OutDpcRaw.Clone(&InRawImage[0]);
	}
	if (m_nBlackWhiteLevelEnable)
	{
		PrintTime printTime("m_HDRPlus_BlackWhiteLevel");
		m_HDRPlus_BlackWhiteLevel.Forward(&OutDpcRaw, pControl);
		printTime.print();
		if (m_HDRPlus_BlackWhiteLevel.m_bDumpFileEnable)
		{
			OutDpcRaw.SaveSingleChannelToBitmapFile("outbmp/BlackWhiteLevel.bmp", 0, OutDpcRaw.GetMaxVal(), 256, 0);
		}
	}
	if (m_nWhiteBalanceEnable)
	{
		PrintTime printTime("m_HDRPlus_WhiteBalance");
		m_HDRPlus_WhiteBalance.Forward(&OutDpcRaw, pControl);
		printTime.print();
		if (m_HDRPlus_WhiteBalance.m_bDumpFileEnable)
		{
			OutDpcRaw.SaveSingleChannelToBitmapFile("outbmp/WhiteBalance.bmp", 0, OutDpcRaw.GetMaxVal(), 256, 0);
		}
	}
	if (m_nDemosaicingEnable)
	{
		/*m_HDRPlus_Demosaicing.Forward(&OutDpcRaw, &OutRGBImage16, pControl);
		if (m_HDRPlus_Demosaicing.m_bDumpFileEnable)
		{
			MultiUcharImage tmpOutRGB;
			m_HDRPlus_Normalize.Forward(&OutRGBImage16, &tmpOutRGB);
			tmpOutRGB.SaveRGBToBitmapFile("outbmp/Demosaicing.bmp");
		}*/
		//////Improved version////////////
		PrintTime printTime("m_HDRPlus_Demosaicing");
		m_HDRPlus_Demosaicing.Forward2(&OutDpcRaw, &OutRGBImage16, pControl);
		printTime.print();
		if (m_HDRPlus_Demosaicing.m_bDumpFileEnable)
		{
			MultiUcharImage tmpOutRGB;
			m_HDRPlus_Normalize.Forward(&OutRGBImage16, &tmpOutRGB);
			tmpOutRGB.SaveRGBToBitmapFile("outbmp/Demosaicing.bmp");
		}
	}
	if (m_nColorCorectEnable)
	{
		PrintTime printTime("m_HDRPlus_ColorCorect");
		m_HDRPlus_ColorCorect.Forward(&OutRGBImage16, pControl);
		printTime.print();
		if (m_HDRPlus_ColorCorect.m_bDumpFileEnable)
		{
			MultiUcharImage tmpOutRGB;
			m_HDRPlus_Normalize.Forward(&OutRGBImage16, &tmpOutRGB);
			tmpOutRGB.SaveRGBToBitmapFile("outbmp/ColorCorect.bmp");
		}
	}
	if (m_nTonemappingEnable)
	{
		PrintTime printTime("m_HDRPlus_Tonemapping");
		m_HDRPlus_Tonemapping.Forward(&OutRGBImage16, pControl);
		printTime.print();
		if (m_HDRPlus_Tonemapping.m_bDumpFileEnable)
		{
			MultiUcharImage tmpOutRGB;
			m_HDRPlus_Normalize.Forward(&OutRGBImage16, &tmpOutRGB);
			tmpOutRGB.SaveRGBToBitmapFile("outbmp/Tonemapping.bmp");
		}
	}
	if (m_nGammaCorrectEnable)
	{
		PrintTime printTime("m_HDRPlus_GammaCorrect");
		m_HDRPlus_GammaCorrect.Forward(&OutRGBImage16, pControl);
		printTime.print();
		if (m_HDRPlus_GammaCorrect.m_bDumpFileEnable)
		{
			MultiUcharImage tmpOutRGB;
			m_HDRPlus_Normalize.Forward(&OutRGBImage16, &tmpOutRGB);
			tmpOutRGB.SaveRGBToBitmapFile("outbmp/GammaCorrect.bmp");
		}
	}
	if (m_nChromaDenoiseEnable)
	{
		PrintTime printTime("m_HDRPlus_ChromaDenoise");
		m_HDRPlus_ChromaDenoise.Forward(&OutRGBImage16, pControl);
		printTime.print();
		if (m_HDRPlus_ChromaDenoise.m_bDumpFileEnable)
		{
			MultiUcharImage tmpOutRGB;
			m_HDRPlus_Normalize.Forward(&OutRGBImage16, &tmpOutRGB);
			tmpOutRGB.SaveRGBToBitmapFile("outbmp/ChromaDenoise.bmp");
		}
	}
	if (m_nContrastEnable)
	{
		PrintTime printTime("m_HDRPlus_Contrast");
		m_HDRPlus_Contrast.Forward(&OutRGBImage16, pControl);
		printTime.print();
		if (m_HDRPlus_Contrast.m_bDumpFileEnable)
		{
			MultiUcharImage tmpOutRGB;
			m_HDRPlus_Normalize.Forward(&OutRGBImage16, &tmpOutRGB);
			tmpOutRGB.SaveRGBToBitmapFile("outbmp/Contrast.bmp");
		}
	}
	if (m_nSharpenEnable)
	{
		PrintTime printTime("m_HDRPlus_Sharpen");
		m_HDRPlus_Sharpen.Forward(&OutRGBImage16);
		printTime.print();
		if (m_HDRPlus_Sharpen.m_bDumpFileEnable)
		{
			MultiUcharImage tmpOutRGB;
			m_HDRPlus_Normalize.Forward(&OutRGBImage16, &tmpOutRGB);
			tmpOutRGB.SaveRGBToBitmapFile("outbmp/Sharpen.bmp");
		}
	}
	PrintTime printTime("m_HDRPlus_Normalize");
	m_HDRPlus_Normalize.Forward(&OutRGBImage16, OutRGBImage8);
	printTime.print();
	if (m_bDumpFileEnable)
	{
		OutRGBImage8->SaveRGBToBitmapFile("outbmp/Normalize.bmp");
	}
}
