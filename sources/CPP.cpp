// Copyright 2020 Andrey

#include <thread>
#include <iostream>
#include <mutex>
#include <string>
#include <time.h>
#include <picosha2.h>
#include <chrono>
#include <vector>
#include <signal.h>
#include <memory>
#include <boost/log/trivial.hpp>
#include "nlohmann/json.hpp"

bool working;
nlohmann::json document;

void OnExitEvent(int cod) {
    if (cod) {
    working = false;
    }
}
std::string FormatJSON(nlohmann::json doc) {
    std::string out = "[\n";
    for (uint64_t i = 0; i < doc.size(); i++) {
    uint64_t ts = static_cast<uint64_t>(doc[i]["timestamp"]);
        out += "    {\n        \"timestamp\" = " + std::to_string(ts) + ",\n";

        std::string hash = static_cast<std::string>(doc[i]["hash"]);
        out += "        \"hash\" = \"" + hash + "\",\n";
        std::string data = static_cast<std::string>(doc[i]["data"]);
        out += "        \"data\" = \"" + data + "\"\n    },\n";
    }
    out += "]";
    return out;
}

void OutputJSON(uint64_t timestamp, std::string hash, std::string data){
    nlohmann::json temp;
    temp["hash"] = hash;
    temp["timestamp"] = timestamp;
    temp["data"] = data;
    document.push_back(temp);
    }
void TF(std::shared_ptr<std::mutex> mutex, int i, uint64_t startingPoint){
    const std::string hashEnd = "0000";
    srand(i + 99999 + time(NULL));
    while (working)
    {
        mutex->lock();
        std::string randomstr = std::to_string(std::rand());
        mutex->unlock();
        std::string hexString = picosha2::hash256_hex_string(randomstr);
        //int strlen = hexString.length();

      if (hexString.substr(hexString.length() - hashEnd.length()) == hashEnd){
      mutex->lock();
      std::chrono::time_point now = std::chrono::high_resolution_clock::now();
      uint64_t timespan =
          now.time_since_epoch().count() - startingPoint;
      BOOST_LOG_TRIVIAL(info) << i << " "
          << timespan << " " << randomstr << ": " << hexString << std::endl;
      OutputJSON(timespan, hexString, randomstr);
      mutex->unlock();
        }else{
mutex->lock();
std::chrono::time_point now = std::chrono::high_resolution_clock::now();
unsigned int timespan = now.time_since_epoch().count() - startingPoint;
BOOST_LOG_TRIVIAL(trace) << i << " "
    << randomstr << ": " << hexString << std::endl;
OutputJSON(timespan, hexString, randomstr);
mutex->unlock();
        }
    }
}

int main(int argc, char* argv[])
{
    signal(SIGINT, OnExitEvent);
    working = true;
    std::chrono::time_point start = std::chrono::high_resolution_clock::now();
    std::shared_ptr<std::mutex> mutex = std::make_shared<std::mutex>();
    std::string jsonName;
    unsigned int maxThreads;

    if (argc == 2) {
        jsonName = argv[1];
        maxThreads = std::thread::hardware_concurrency();
        std::cout << "threads:" << maxThreads << std::endl;
    }else if (argc >= 3){
        jsonName = argv[1];
        maxThreads = std::stoi(argv[2]);
    }else{
        exit(1);
    }
    std::cout << "threads:" << maxThreads << std::endl;
    std::thread** threads = new std::thread * [maxThreads];

    for (unsigned int i = 0; i < maxThreads; i++) {
        threads[i] =
            new std::thread(TF, mutex, i, start.time_since_epoch().count());
    }
    for (unsigned int i = 0; i < maxThreads; i++) {
        threads[i]->join();
    }
    std::ofstream jsonFile;
    jsonFile.open(jsonName);
    std::string out = FormatJSON(document);
    jsonFile.write(out.c_str(), out.size());
    jsonFile.close();
}
