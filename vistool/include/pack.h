#pragma once

namespace ND {
    typedef struct {
        int id_; // obstacle id
        int class_id;
        float x;
        float y;
        float z;
        
        float w;
        float l;
        float h;

        float theta;
        
    } bev_obstacle_t;

    // more bev types;
}
