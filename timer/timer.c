#include <time.h>
#include <stdio.h>
#include <error.h>

//=================================================
// get_time
// 現在の時間情報を得る
// Linuxシステムから現時刻の情報を引数の構造体 timevalへ読み出
// struct timeval *tv: 現時間の情報
// rc:                 処理結果　0=正常、　0以外=エラー
//=================================================
int get_time (struct timespec *tv)
{
	int rc=0;
	
	rc = clock_gettime(CLOCK_MONOTONIC_RAW, tv);
	if(rc)
	{
	    perror("[get_time] gettimeofday error");
	    rc = -1;
	}
	return(rc);
}

//=================================================
// diff_time
// 指定時間からの経過時間を得る
// 数値の単位はmsec(誤差範囲は最大500μsec)
// 算出可能な経過時間は49日まで
// struct timeval *tv: 現時間の情報
// time:               0以上=経過時間msec、0未満=エラー
//=================================================
int diff_time (struct timespec *start_time)
{
	struct timespec	end_time, result;
	int			rc;
	unsigned long	diff_time;
	unsigned long	adjust;
	
	rc = get_time (&end_time);
	if(rc) return(-1);

	result.tv_sec = end_time.tv_sec - start_time->tv_sec;
    	result.tv_nsec = end_time.tv_nsec - start_time->tv_nsec;
	if(result.tv_nsec < 0)
 	{
    	    result.tv_nsec += 1000000000;
    	    result.tv_sec -= 1;
 	}

	//printf("diff.tv_sec=%d diff.tv_usec=%d\n\r", result.tv_sec, result.tv_nsec);
	adjust = result.tv_nsec % 1000000;
	diff_time = (result.tv_sec*1000 + result.tv_nsec/1000000);  // convert msec value
	
	// 端数の四捨五入
	if(adjust > 500)
	{
	    diff_time += 1;
	}
	
	return(diff_time);
}
