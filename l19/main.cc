#include"tcpServer.hpp"

#include<memory>

void NetCal(Connection* conn, std::string &request){
    // logMessage(DEBUG, "NetCal has been called, get a request : %s", request.c_str());
    //反序列化
    request_t req;
    if(!req.DeserializeRequest(request)) return;
    //业务处理
    response_t resp = calculator(req);
    //构建应答，序列话
    std::string sendstr = resp.SerializeResponse();
    sendstr = Encode(sendstr);
    //交给服务器，让服务器发送
    conn->_outbuffer += sendstr;
    //让底层的tcpserver 发送出去
    conn->_tsvr->EnableReadWrite(conn, true, true);

}

int main(){
    std::unique_ptr<TcpServer> svr(new TcpServer());
    svr->Dispatcher(NetCal);
    return 0;
}