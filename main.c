#include "http.h"
 static void usage(const char* proc)
{
	printf("usage :%s [local_ip] [local port]\n", proc);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		usage(argv[0]);
		return 1;
	}

	int listen_sock = startup(argv[1], atoi(argv[2]));
	if (listen_sock < 0)
	{
		return 2;
	}

	int epfd = epoll_create(MAX_FD);
    if (epfd < 0)
	{
		perror("create error");
		return 5;
	}

    struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;
	epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&ev);
	int nums = -1;
	struct epoll_event revs[EPOLL_REVS_SIZE];
	int timeout = -1;
	while (1)
	{
		switch((nums = epoll_wait(epfd, revs, EPOLL_REVS_SIZE, timeout)))
		{
			case 0:
					printf("timeout...\n");
					break;
			case -1:
                   perror("wait error");
				   break;
			default:
				   {
					   int i = 0;
                      for (;i<nums;++i)
					  {
						  int sock = revs[i].data.fd;
						  if (sock == listen_sock && (revs[i].events & EPOLLIN) )
						  {
							struct sockaddr_in client;
							socklen_t len = sizeof(client);
							int new_sock = accept(listen_sock, (struct sockaddr*)&client,\
							&len);
							if (new_sock < 0)
							{
								perror("accept  error");
								continue;
							}

							set_noblock(new_sock);
	                       	
                      		printf("get a client [%s:%d]\n", inet_ntoa(client.sin_addr),\
			            	ntohs(client.sin_port));

							ev.events = EPOLLIN;
							ev.data.fd = new_sock;
							epoll_ctl(epfd, EPOLL_CTL_ADD, new_sock, &ev);
						  }
						  else if(sock != listen_sock)
						  {
							  if(revs[i].events & EPOLLIN)
							  {		  
                     	    	pthread_t id;
	                        	pthread_create(&id , NULL, handler_request, (void*)sock);
	                        	pthread_detach(id);
								ev.events = EPOLLOUT;
								ev.data.fd =sock;
								epoll_ctl(epfd,EPOLL_CTL_MOD, sock ,&ev);
							  }
		/*				     else if(revs[i].events & EPOLLOUT)
						     {
                                const char *msg = "HTTP1.0 OK 200 \r\n\r\n";
								write(sock,msg,strlen(msg));
								close(sock);
								epoll_ctl(epfd,EPOLL_CTL_DEL, sock, NULL);
						     }*/
							  else
							  {}
					  }
				   }

		}
				break;
	}
	}
	close(listen_sock);
	return 0;

}
