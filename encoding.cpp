/*
 * encoding.cpp
 *
 *  Created on: 2012-1-2
 *      Author: ben
 */
#include "stdafx.h"
#include "encoding.h"

#include <cstring>
using namespace std;

//#include <windows.h>

/**
 * ��UTF-8������ı�תΪUTF-16���ı�
 * ����ֵΪת����utf-16�ı��ĳ���
 * ԭ������
	UCS-2���� 					UTF-8 �ֽ���(������)
	U-00000000 - U-0000007F: 	0xxxxxxx
	U-00000080 - U-000007FF: 	110xxxxx 10xxxxxx
	U-00000800 - U-0000FFFF: 	1110xxxx 10xxxxxx 10xxxxxx
	U-00010000 - U-001FFFFF: 	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	U-00200000 - U-03FFFFFF: 	111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	U-04000000 - U-7FFFFFFF: 	1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	UTF-8�����ַ������Ͽ�����ൽ6���ֽڳ�,Ȼ��16λBMP��Basic Multilingual Plane���ַ����ֻ�õ�3�ֽڳ���
 */
int Utf8ToUnicode(const char *utf8, wchar_t* unicode) {
	int len8 = strlen(utf8);
	int len16 = 0;
	unsigned short int t, r;
	if(unicode != NULL) {
		for (int i = 0; i < len8;) {
			t = utf8[i] & 0xff;
			if(t < 0x80) {
				r = t;
				i++;
			}else if(t < 0xe0) {
				r = t & 0x1f;
				r <<= 6;
				t = utf8[i + 1];
				r += t & 0x3f;
				i += 2;
			}else if(t < 0xf0){
				r = t & 0x0f;
				r <<= 6;
				t = utf8[i + 1];
				r += t & 0x3f;
				r <<= 6;
				t = utf8[i + 2];
				r += t & 0x3f;
				i += 3;
			}else {//����������
				r = 0;
				i++;
			}
			unicode[len16++] = r;
		}
		unicode[len16] = 0;
	}else {
		for (int i = 0; i < len8;) {
			t = utf8[i] & 0xff;
			if(t < 0x80) {
				i++;
			}else if(t < 0xe0) {
				i += 2;
			}else if(t < 0xf0){
				i += 3;
			}else {
				i++;
			}
			len16++;
		}
	}
	return len16;
}

//��UTF-8������ı�תΪUnicode���ı�
//int Utf8ToUnicode_old(const char *utf8, wchar_t* unicode) {
//	int len;
//	DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
//	len = (int)dwNum;
//	if(unicode != NULL) {
//		MultiByteToWideChar(CP_UTF8, 0, utf8, -1, unicode, len + 1);
//	}
//	return len;
//}
