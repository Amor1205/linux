#include "protocol.hpp"
#include "sock.hpp"

void Usage(string proc)
{
    cout << "How to use : calClient ?" << endl;
    cout << "Usage: " << proc << " ip"
         << " port" << endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
        Usage(argv[0]);
    int sock = Sock::Socket();
    Sock::Connect(sock, argv[1], atoi(argv[2]));
    // 业务逻辑
    // 短链接
    request_t req;
    memset(&req, 0, sizeof(req));
    cout << "Please Enter Data1 # ";
    cin >> req.x;
    cout << "Please Enter Data2 # ";
    cin >> req.y;
    cout << "Please Enter the operator # ";
    cin >> req.op;
    std::string json_string = SerializeRequest(req);
    ssize_t s = write(sock, json_string.c_str(), json_string.size());
    char buffer[1024];
    s = read(sock, buffer, sizeof(buffer));
    if (s > 0)
    {
        response_t resp;
        buffer[s] = 0;
        std::string str = buffer;
        DeserializeResponse(str, resp);

        cout << "code : " << resp.code << endl;
        if (resp.code == 0)
        {
            cout << "result : " << resp.result << endl;
        }
        else
        {
            cout << "Wrong!" << endl;
        }
    }

    // ssize_t w = write(sock, &req, sizeof(req));
    // if(w <= 0) exit(6);

    // response_t resp;
    // ssize_t s = read(sock, &resp, sizeof(resp));
    // if(s == sizeof(resp)){
    //     //读到完整报文
    //     cout << "code : " << resp.code << endl;
    //     if(resp.code == 0){
    //         cout << "result : " << resp.result << endl;
    //     }else{
    //         cout << "Wrong!" << endl;
    //     }
    // }
    return 0;

}