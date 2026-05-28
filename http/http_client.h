#ifndef _H_HTTP_CLIENT_H_
#define _H_HTTP_CLIENT_H_

#include <string>

class HttpClient {
public:
    std::string fetch(const std::string& url);
};

#endif // _H_HTTP_CLIENT_H_