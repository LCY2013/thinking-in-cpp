//
// Created by fufeng on 2024/2/18.
//
export module ips.network:request;

import <string>;
import <map>;

namespace ips::network {
    export class Request {
    public:
        Request() {}

        void setPath(const std::string &path) {
            _path = path;
        }

        const std::string &getPath() const {
            return _path;
        }

        void setHeader(const std::string &key, const std::string &value) {
            _headers[key] = value;
        }

        std::string getHeader(const std::string &key) {
            std::string value = _headers[key];
            return value;
        }

        void setQuery(std::map <std::string, std::string> &query) {
            _query = query;
        }

        const std::map <std::string, std::string> &getQuery() const {
            return _query;
        }

        std::string &&getBody() {
            return "";
        }

    private:
        std::string _path;
        std::map <std::string, std::string> _headers;
        std::map <std::string, std::string> _query;
    };
}

