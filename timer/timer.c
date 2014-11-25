#include <time.h>
#include <stdio.h>
#include <error.h>

//=================================================
// get_time
// ���݂̎��ԏ��𓾂�
// Linux�V�X�e�����猻�����̏��������̍\���� timeval�֓ǂݏo
// struct timeval *tv: �����Ԃ̏��
// rc:                 �������ʁ@0=����A�@0�ȊO=�G���[
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
// �w�莞�Ԃ���̌o�ߎ��Ԃ𓾂�
// ���l�̒P�ʂ�msec(�덷�͈͍͂ő�500��sec)
// �Z�o�\�Ȍo�ߎ��Ԃ�49���܂�
// struct timeval *tv: �����Ԃ̏��
// time:               0�ȏ�=�o�ߎ���msec�A0����=�G���[
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
	
	// �[���̎l�̌ܓ�
	if(adjust > 500)
	{
	    diff_time += 1;
	}
	
	return(diff_time);
}
