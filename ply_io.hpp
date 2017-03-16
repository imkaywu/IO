//
//  ply_io.hpp
//  3D_Reconstruction_Delaunay_Graph_Cut
//
//  Created by KaiWu on Feb/29/16.
//  Copyright © 2016 KaiWu. All rights reserved.
//

#ifndef ply_io_hpp
#define ply_io_hpp

#include <iostream>
#include <vector>

#include "RPly/rply.h"
#include "RPly/rplyfile.h"

using std::vector;


#ifndef POINT_3D_AND_FACE
#define POINT_3D_AND_FACE

struct Point_3D
{
    int u, v;
    float x, y, z;
    vector<int> ind_vis_views;
};

struct Face
{
    long inds[3];
};

#endif /* POINT_3D_AND_FACE */


class PLY_IO
{
private:
    // static variables
    static Point_3D point_3d;
    static vector<Point_3D> points_3d;
    static Face face;
    static vector<Face> faces;
    static int ind_property;
    
    // callback functions
    static int vertex_cb(p_ply_argument argument);
    static int face_cb(p_ply_argument argument);
    
public:
    PLY_IO() { }
    PLY_IO(vector<Point_3D> &points_3d, vector<Face> &faces);
    ~PLY_IO() { };
    
    int ply_read(const char* name);
    int ply_write(const char* name, vector<Point_3D> &points_3d, vector<Face> &faces);
    void ply_get(vector<Point_3D> &points_3d, vector<Face> &faces);
};

inline PLY_IO::PLY_IO(vector<Point_3D> &points_3d, vector<Face> &faces)
{
    this->points_3d = points_3d;
    this->faces = faces;
}

inline void PLY_IO::ply_get(vector<Point_3D> &points_3d, vector<Face> &faces)
{
    points_3d = this->points_3d;
    faces = this->faces;
}

#endif /* ply_io_hpp */