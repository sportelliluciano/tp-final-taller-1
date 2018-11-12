#pragma once

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