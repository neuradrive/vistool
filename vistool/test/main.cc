#include "pack.h"
#include "vistool.h"
#include <memory>

#ifdef YAML_CONF
#include "yaml-cpp/yaml.h"
#endif

#include <thread>
#include <unistd.h>
#include <string>

using namespace ND;

void viz_test(int thread_id) {
    std::shared_ptr<Vistool> viz = std::make_shared<Vistool>();

    std::string title = "ND-BEV-PLAYER-";
    title += std::to_string(thread_id);

#ifdef YAML_CONF
    YAML::Node config = YAML::LoadFile("../test/test.yaml");
#else
    viz->Init(10, title);
#endif

    bev_obstacle_t obj_test;

    // test obj
    obj_test.id_ = 0;
    obj_test.x = 20;
    obj_test.y = 30;
    obj_test.theta = 1.2;
    obj_test.w = 5;
    obj_test.l = 1.5;
    int frame = 1;
    
    while (1)
    {
        viz->obstacles->clear();
        viz->obstacles->push_back(obj_test);
        viz->cvDisplay(frame);
        frame++;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    int THREAD_NUM = 2;

    std::thread t[THREAD_NUM];

    for (int i = 0; i < THREAD_NUM; i++) {
        t[i] = std::thread(viz_test, i);
    }

    for (auto& ti: t) {
        ti.join();
    }
    
    return 0;
}
