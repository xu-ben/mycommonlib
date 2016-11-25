/*
 * linux.cc
 *
 *  Created on: 2016年11月4日
 *      Author: ben
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/time.h> // todo加上条件编译命令判断是否支持
#include <windows.h>
#include <unistd.h>//sleep函数, gnu特有头文件
#ifdef _WINDOWS_H
#define sleep(a) Sleep(a*1000)
#endif

/*
 * 获取当前时间距1970-1-1 0:0:0.000的毫秒数
 * 返回0表示函数执行过程中出现错误
 */
inline uint64_t GetCurrentTimeMillis() {
#ifdef __USE_ISOC11
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	return ((uint64_t) ts.tv_sec) * 1000 + ts.tv_nsec / 1000000;
//TODO 下面这一行的宏定义, 可能存在兼容性问题, 但目前在多个平台测试还没有发现问题,发现再改
#elif defined _STRUCT_TIMEVAL || defined _TIMEVAL_DEFINED
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((uint64_t) tv.tv_sec) * 1000 + tv.tv_usec / 1000;
#else
	return 0;
#endif
}

void testclock() {
	clock_t t = clock();
	clock_t aa = CLOCKS_PER_SEC;
	printf("CLOCKS_PER_SEC = %lld\n", CLOCKS_PER_SEC);
	printf("%lld\n", t);
	sleep(3);
	t = clock();
	printf("%lld\n", t);
	for (int i = 0; i < 10000000; i++) {
		int a = 1;
		int b = a + 1;
		a = b - a;
	}
	t = clock();
	printf("%lld\n", t);
}

void testmillis() {
	uint64_t t = GetCurrentTimeMillis();
	printf("%llu\n", t);
//	sleep(3);
	t = GetCurrentTimeMillis();
	printf("%llu\n", t);
	int a = 1;
	for (int i = 0; i < 10000000; i++) {
		int b = a + 1;
		a = b - a + 5;
	}
	t = GetCurrentTimeMillis();
	printf("%llu\n", t);
}

int main() {
	puts("testclock()");
	testclock();
	puts("testmillis()");
	testmillis();
	return 0;
}

