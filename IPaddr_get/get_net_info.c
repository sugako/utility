#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <sys/time.h>
#include <netdb.h>

#define MAX_IFR 3
unsigned char	hbuf[1024], sbuf[1024];
struct addrinfo	hints, *res;

#if 0
int main(void)
{
        int i, fd; 
        struct ifreq ifr[MAX_IFR];
        struct ifconf ifc;
	    unsigned int	lnum;

        fd = socket(AF_INET, SOCK_DGRAM, 0); 

        ifc.ifc_len = sizeof(ifr);
        ifc.ifc_ifcu.ifcu_buf = (void *)ifr;

        ioctl(fd, SIOCGIFCONF, &ifc);

        lnum = ifc.ifc_len / sizeof(struct ifreq);

        for (i = 0; i < lnum; i++) {
                ioctl(fd, SIOCGIFADDR, &ifr[i]);
                printf("addr: %s\n", inet_ntoa(((struct sockaddr_in *)&(ifr[i].ifr_addr))->sin_addr));
                ioctl(fd, SIOCGIFNETMASK, &ifr[i]);
                printf("netmask: %s\n", inet_ntoa(((struct sockaddr_in *)&(ifr[i].ifr_addr))->sin_addr));
        }   

        close(fd);

        return;
}
#endif

#define	IPV6_ADDRESS_STR_LENGTH 32

int main(void)
{
	unsigned char ipv6[16];
	int i;
	
	get_my_ipv6("eth0", ipv6);
	printf("IP=");
	for(i=0; i<16 ; i++){
		printf("%02x ", ipv6[i]);
	}
	printf("\n\r");

}
int get_my_ipv6(char *if_name, unsigned char *ipv6_p)
{
	FILE*			fd = NULL;									// /proc/net/route�t�@�C���ւ̃|�C���^
	char			temp_buff[256];								// 1�s���̃��[�g���ꎞ�o�b�t�@
	unsigned char	temp_address[IPV6_ADDRESS_STR_LENGTH+1];
	char			cnv_buff[3];
	char*			p_ret;
	int				rc;
	int				i, j;

	struct
	{
		//struct in6_addr		ipv6_addr;
		char				ipv6_addr[IPV6_ADDRESS_STR_LENGTH];
		unsigned int		info1;
		unsigned int		info2;
		unsigned int		info3;
		unsigned int		info4;
		char				iface[64];
	} inet6_info;
	
	//------------------------------------------
	// inet6�����I�[�v��
	fd = fopen("/proc/net/if_inet6", "r");
	if(fd == NULL)
	{
		printf("[get_linux_gwaddr] /proc/net/if_inet6 fopen error\n\r");
		return 0;
	}
#ifdef DEBUG
	printf("/proc/net/if_inet6 open OK\n\r");
#endif
	while(1){
		// if_inet6�t�@�C������s�P�ʂœǂݍ���
		memset((char *)temp_buff, 0, sizeof(temp_buff));
		p_ret = fgets(temp_buff, sizeof(temp_buff), fd);
		if(p_ret == NULL) break;

        // ipv6 address����W�J�擾
		memset((char *)&inet6_info, 0, sizeof(inet6_info));
		rc = sscanf(temp_buff, "%s%u%u%u%u%s",
					inet6_info.ipv6_addr,
					&inet6_info.info1,
					&inet6_info.info2,
					&inet6_info.info3,
					&inet6_info.info4,
					inet6_info.iface);
		if(rc == 0 || rc == EOF) break;

		// �C���^�t�F�[�X������v����ꍇ
		if(strstr(inet6_info.iface, if_name) != NULL){
			// �����N���[�J���A�h���X����񂪈�v����ꍇ
			if(strstr(inet6_info.ipv6_addr, "fe80") != NULL){
				// IPv6�����N���[�J���A�h���X���擾
				memset(cnv_buff, 0, sizeof(cnv_buff));
				for(i=0,j=0 ; i<16 ; i++){
					cnv_buff[0] = inet6_info.ipv6_addr[j++];
					cnv_buff[1] = inet6_info.ipv6_addr[j++];
					sscanf(cnv_buff, "%x", ipv6_p);
					ipv6_p++;
				}
				break;
			}
		}
	}
	fclose(fd);
	printf("IP=%s\n\r", inet6_info.ipv6_addr);
	return 0;
	
}

