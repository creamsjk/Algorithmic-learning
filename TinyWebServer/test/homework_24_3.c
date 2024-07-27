#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include<sys/stat.h>
#include<dirent.h>

#define PORT 8080
#define BUFFER_SIZE 4096

// Function to send HTTP response
void send_response(int client_fd, const char *header, const char *content_type, const char *body, size_t body_len) {
    char response_header[BUFFER_SIZE];
    int header_len = sprintf(response_header, 
                             "HTTP/1.1 %s\r\n"
                             "Server: MyHttpServer1.0\r\n"
                             "Connection: close\r\n"
                             "Content-Type: %s\r\n"
                             "Content-Length: %ld\r\n\r\n",
                             header, content_type, body_len);
    send(client_fd, response_header, header_len, 0);
    if (body != NULL && body_len > 0) {
        send(client_fd, body, body_len, 0);
    }
}

// Function to send image file
void send_image(int client_fd, const char *file_path) {
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1) {
        perror("open image error");
        const char *error_body = "<html><body>404 Not Found</body></html>";
        send_response(client_fd, "404 Not Found", "text/html", error_body, strlen(error_body));
        return;
    }

    char buffer[BUFFER_SIZE];
    int n;
    struct stat file_stat;
    fstat(file_fd, &file_stat);
    size_t file_size = file_stat.st_size;

    send_response(client_fd, "200 OK", "image/jpg", NULL, file_size);

    while ((n = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
        send(client_fd, buffer, n, 0);
    }
    close(file_fd);
}

void send_list_file(int client_fd, const char* file_path){

    DIR *d;
    struct dirent *dir;
    char buffer[BUFFER_SIZE];

    d = opendir(file_path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
             struct stat st;
            stat(dir->d_name, &st);
            if (S_ISREG(st.st_mode)||S_ISDIR(st.st_mode)||1) {
                snprintf(buffer, sizeof(buffer), "<a href=\"%s/%s\">%s</a><br>",file_path, dir->d_name, dir->d_name);
               send(client_fd, buffer, strlen(buffer), 0);
            }
        }

           // send(client_fd, buffer, strlen(buffer), 0);
        closedir(d);
    }
}

int main(void) {
    // 1. Create socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. Bind address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind error");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Listen
    if (listen(listen_fd, 10) == -1) {
        perror("listen error");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    int client_fd;

    while (1) {
        client_fd = accept(listen_fd, NULL, NULL);
        if (client_fd == -1) {
            perror("accept error");
            continue; // Continue to accept the next connection
        }

        char buffer[BUFFER_SIZE] = {0};

        // Receive request
        int received = recv(client_fd, buffer, sizeof(buffer), 0);
        if (received == -1) {
            perror("recv error");
            close(client_fd);
            continue;
        }

        printf("Received request:\n%s\n", buffer);

        // Parse the request
        char method[16], path[256];
        sscanf(buffer, "%s %s", method, path);


         const char *ext = strrchr(path, '.');

        // Send response based on the request path
        // 第一个if是默认列表吧算是,后边都是服务器拥有的资源 可以加载进来然后显示图片
        if(strcmp(path,"/")==0){
                   
            const char* startLine = "HTTP/1.1 200 OK\r\n";
            const char* headers = "Server: MYHttpServer1.0\r\n"
                "Connection: keep-alive\r\n"
                "Content-Type: text/html;charset=utf-8\r\n"
                "Content-Location：/home/sunrise"
                "Content-Length:";
            const char* emptyLine = "\r\n"; 
            const char * body_head="<html><head>This is file list </head><body><br>";
            const char *body_end="</body></html>";
            char buff[BUFFER_SIZE]={0};
            sprintf(buff,"%s%s%ld\r\n%s%s<a href=\"/home/sunrise\">/home/sunrise</a><br>%s",startLine,headers,sizeof(buff),emptyLine,body_head,body_end);
            send(client_fd,buff,strlen(buff),0);
            // "<a href=\"/gg\">gg</a>;
            //   send(client_fd,body_head,sizeof(body_head),0);
            
        }
        else if(strcmp(path,"/home/sunrise")==0){

            const char* startLine = "HTTP/1.1 200 OK\r\n";
            const char* headers = "Server: MYHttpServer1.0\r\n"
                "Connection: keep-alive\r\n"
                "Content-Type: text/html;charset=utf-8\r\n"
                "Content-Location：/home/sunrise\r\n"
                "Content-Length:";
            const char* emptyLine = "\r\n"; 
            const char * body_head="<html><head>This is file list </head><body><br>";
            char buff[BUFFER_SIZE]={0};
            sprintf(buff,"%s%s%ld\r\n%s%s",startLine,headers,sizeof(buff),emptyLine,body_head);
           //  printf("\n%s\n",buff);
            send(client_fd,buff,strlen(buff),0);
            send_list_file(client_fd,"/home/sunrise");
            const char *body_end="</body></html>";
            send(client_fd,body_end,strlen(body_end),0);

        }
        else if (strcmp(path, "/image") == 0) {
            const char *html_body = "<html><head>This is a test</head><body><img src=\"/home/sunrise/3.jpg\" alt=\"1\"><img src=\"/home/sunrise/2.jpg\"></body></html>";
            send_response(client_fd, "200 OK", "text/html", html_body, strlen(html_body));
        }/* else if (strcmp(path, "/home/sunrise/3.jpg") == 0) {
            // Send image response
            send_image(client_fd, "/home/sunrise/3.jpg");
        }else if (strcmp(path,"/home/sunrise/2.jpg") == 0){

            send_image(client_fd, "/home/sunrise/2.jpg");
        }else if(strcmp(path,"/home/sunrise/4.jpg") == 0){

            send_image(client_fd, "/home/sunrise/4.jpg");
        }else if(strcmp(path,"/home/sunrise/5.jpg") == 0){

            send_image(client_fd, "/home/sunrise/5.jpg");
        
        }*/else if(ext && (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".png") == 0)){

            send_image(client_fd, path);
        

        } else {
            const char *error_body = "<html><body>404 Not Found</body></html>";
            send_response(client_fd, "404 Not Found", "text/html", error_body, strlen(error_body));
        }

        close(client_fd); // Close the client connection
    }

    close(listen_fd); // Close the listening socket

    return 0;
}

