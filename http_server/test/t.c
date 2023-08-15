#include <string.h>
#include <stdio.h>

int main(){
	const char* s = "POST /login HTTP/1.1
Accept-Language: zh-SG,zh-CN;q=0.9,zh-Hans;q=0.8
Upgrade-Insecure-Requests: 1
Host: 101.43.172.175:8080
Content-Length: 41
Origin: http://101.43.172.175:8080
Accept-Encoding: gzip, deflate
Content-Type: application/x-www-form-urlencoded
Connection: keep-alive
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Referer: http://101.43.172.175:8080/
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/16.1 Safari/605.1.15

username=1234&password=1234&submit=SUBMIT";
	printf("%d", sizeof(s));
	return 0;
}

