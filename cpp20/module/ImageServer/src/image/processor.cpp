//
// Created by fufeng on 2024/2/18.
//
export module ips.image:processor;

import <string>;
import <cstdint>;

namespace ips::image {
    export class Processor {
    public:
        void setWidth(int32_t width) {
            _width = width;
        }
        void setHeight(int32_t height) {
            _height = height;
        }
        void setQuality(int32_t quality) {
            _quality = quality;
        }
        void setMode(const std::string &mode) {
            _mode = mode;
        }

        int32_t getWidth() const {
            return _width;
        }
        int32_t getHeight() const {
            return _height;
        }
        int32_t getQuality() const {
            return _quality;
        }
        const std::string& getMode() const {
            return _mode;
        }
        std::string&& processImage(const std::string &imageFile) {
            return "";
        }

    private:
        int32_t _width;
        int32_t _height;
        int32_t _quality;
        std::string _mode;
    };
}
