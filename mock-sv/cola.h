#pragma once

#define COLOR_ROJO      "\x1b[31m"
#define COLOR_VERDE     "\x1b[32m"
#define COLOR_AMARILLO  "\x1b[33m"
#define COLOR_RESET     "\x1b[0m"

#include "../src/libs/json.hpp"

class Cola {
public:
    Cola() { }

    void push(const nlohmann::json& data) {
        mx.lock();
        real_cola.push_back(data);
        mx.unlock();
    }

    nlohmann::json pull() {
        mx.lock();
        nlohmann::json data = real_cola.back();
        real_cola.pop_back();
        mx.unlock();
        return data;
    }

    bool esta_vacia() {
        return real_cola.empty();
    }

private:
    std::mutex mx;
    std::list<nlohmann::json> real_cola;
};