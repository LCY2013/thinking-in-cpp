//
// Created by fufeng on 2024/2/18.
//
export module ips.network:connection;

import <functional>;
import <memory>;
import <vector>;

import :request;
import :response;

namespace ips::network {
    export using RequestPtr = std::shared_ptr<Request>;
    export using ResponsePtr = std::shared_ptr<Response>;

    export using OnRequestHandler = std::function<void(RequestPtr, ResponsePtr)>;

    export class Connection {
    public:
        Connection() {}

        void onRequest(OnRequestHandler handler) {
            _requestHandlers.push_back(handler);
        }

    private:
        std::vector <OnRequestHandler> _requestHandlers;
    };
}
