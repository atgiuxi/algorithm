#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <string.h>

int main() {
    // 1.获取监听套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY/*绑定所有IP*/; 
    socklen_t len = sizeof(addr);

    // 2.绑定IP地址和端口
    int ret = bind(fd, (struct sockaddr *)&addr, len);

    // 3.监听套接字
    ret = listen(fd, 1024);

	// 4.这个地方本来是accept获取新连接。但是不能直接accept
    // 4.1.
    int epoll_fd = epoll_create(1);

    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN; // 监听读事件

    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
    struct epoll_event events[1024]; // 保存就绪的事件

    while (1) 
	{
        int ret = epoll_wait(epoll_fd, events, 1024, -1);

        for (int i = 0; i < ret; i++) 
		{
            int fd_from_events = events[i].data.fd;
            uint32_t event_from_events = events[i].events;

            if (event_from_events & EPOLLIN) 
			{
                if (fd_from_events == fd) 
				{
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
                    if (client_fd < 0) 
					{
                        perror("accept failed");
                        continue;
                    }

                    struct epoll_event client_event;
                    client_event.data.fd = client_fd;
                    client_event.events = EPOLLIN;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event) < 0) 
					{
                        perror("epoll_ctl add client failed");
                        close(client_fd);
                        continue;
                    }
                    printf("new connection accepted : %d \n",client_fd);
                } else 
				{
                    // 处理客户端数据
                    char buffer[1024] = {0};
                    int n = read(fd_from_events, buffer, sizeof(buffer) - 1);
                    if (n <= 0) 
					{
                        // 客户端关闭或读取错误，关闭文件描述符
                        if (n == 0) {
                            printf("Client disconnected\n");
                        } else {
                            perror("read failed");
                        }
                        close(fd_from_events);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd_from_events, NULL);
                    } else 
					{

                        buffer[n] = '\0';
                        printf("Received: %s : %d\n", buffer,fd_from_events);
                    }
                }
            }
			else{/*其他事件*/}
        }
    }

    // 5.关闭监听套接字和epoll
    close(fd);
    close(epoll_fd);

    return 0;
}
