//
// Created by Cuent on 2/23/2025.
//

#ifndef OBSERVERPATTERN_H
#define OBSERVERPATTERN_H
#include <memory>
#include <vector>

#include "Packet.h"

class Observer {
public:
    virtual void Update(PacketHeader packet_header) = 0;
    virtual ~Observer() {}
};

class Subject {
private:
    std::vector<std::shared_ptr<Observer>> observers;
    PacketHeader packet_header;

public:
    void Attach(std::shared_ptr<Observer> observer) {
        observers.push_back(observer);

    }

    // void Detach(std::shared_ptr<Observer> observer) {
    //     observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    // }

    void Notify() {

        for (const auto& observer : observers) {
            observer->Update(packet_header);
        }
    }

    void SetPacket(PacketHeader newPacket) {
        packet_header = newPacket;
        Notify();
    }
};
#endif //OBSERVERPATTERN_H
