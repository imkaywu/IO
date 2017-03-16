//
//  ply_io.cpp
//  3D_Reconstruction_Delaunay_Graph_Cut
//
//  Created by KaiWu on Feb/29/16.
//  Copyright © 2016 KaiWu. All rights reserved.
//

#include "ply_io.hpp"

// must redeclare the static member variables in .cpp file
Point_3D PLY_IO::point_3d;
vector<Point_3D> PLY_IO::points_3d;
Face PLY_IO::face;
vector<Face> PLY_IO::faces;
int PLY_IO::ind_property;


int PLY_IO::vertex_cb(p_ply_argument argument)
{
    double val = ply_get_argument_value(argument);
    
    switch(ind_property++)
    {
        case 0:
            point_3d.u = (int) val;
            break;
        case 1:
            point_3d.v = (int) val;
            break;
        case 2:
            point_3d.x = (float) val;
            break;
        case 3:
            point_3d.y = (float) val;
            break;
        case 4:
            point_3d.z = (float) val;
            break;
        default:
        {
            long length, value_index;
            ply_get_argument_property(argument, NULL, &length, &value_index);
            
            if(value_index < 0)
                point_3d.ind_vis_views.resize(length);
            else
                point_3d.ind_vis_views[value_index] = val;
            
            if(value_index == length - 1)
            {
                ind_property = 0;
                points_3d.push_back(point_3d);
            }
            break;
        }
    }
    
    return 1;
}

int PLY_IO::face_cb(p_ply_argument argument)
{
    long length, value_index;
    ply_get_argument_property(argument, NULL, &length, &value_index);
    
    double val = ply_get_argument_value(argument);
    
    if(value_index >= 0)
        face.inds[value_index] = val;
    
    if(value_index == 2)
        faces.push_back(face);
    
    return 1;
}

int PLY_IO::ply_read(const char* name)
{
    p_ply ply = ply_open(name, NULL, 0, NULL);
    
    if(!ply)
        return 0;
    
    if(!ply_read_header(ply))
        return 0;
    
    ply_set_read_cb(ply, "vertex", "u", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "v", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "x", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "y", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "z", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "ind_vis_views", vertex_cb, NULL, 1);
    
    ply_set_read_cb(ply, "face", "ind_vertex", face_cb, NULL, 0);
    
    if (!::ply_read(ply))
        return 0;
    
    if(!ply_close(ply))
        return 0;
    
    return 1;
}

int PLY_IO::ply_write(const char* name, vector<Point_3D> &points_3d, vector<Face> &faces)
{
    long num_vertices = static_cast<long>(points_3d.size());
    long num_faces = static_cast<long>(faces.size());
    
    p_ply ply = ply_create(name, PLY_ASCII, NULL, 0, NULL);
    
    if (!ply)
        return 0;
    
    ply_add_element(ply, "vertex", num_vertices);
    ply_add_scalar_property(ply, "u", PLY_INT);
    ply_add_scalar_property(ply, "v", PLY_INT);
    ply_add_scalar_property(ply, "x", PLY_FLOAT);
    ply_add_scalar_property(ply, "y", PLY_FLOAT);
    ply_add_scalar_property(ply, "z", PLY_FLOAT);
    ply_add_list_property(ply, "ind_vis_views", PLY_INT, PLY_INT);
    
    ply_add_element(ply, "face", num_faces);
    ply_add_list_property(ply, "ind_vertex", PLY_UCHAR, PLY_INT);
    
    if (!ply_write_header(ply))
        return 0;
    
    // write vertices
    std::cout << "#### Writing vertices..." << std::endl;
    for (int i = 0; i < num_vertices; ++i)
    {
        ::ply_write(ply, points_3d[i].u);
        ::ply_write(ply, points_3d[i].v);
        ::ply_write(ply, points_3d[i].x);
        ::ply_write(ply, points_3d[i].y);
        ::ply_write(ply, points_3d[i].z);
        
        int num_vis_views = static_cast<int>(points_3d[i].ind_vis_views.size());
        ::ply_write(ply, num_vis_views);
        for (int j = 0; j < num_vis_views; ++j)
            ::ply_write(ply, points_3d[i].ind_vis_views[j]);
    }
    
    // write polygons
    std::cout << "#### Writing polygons..." << std::endl;
    for (int i = 0; i < num_faces; ++i)
    {
        int num_faces = 3;
        ::ply_write(ply, num_faces);
        
        for(int j = 0; j < num_faces; ++j)
        {
            ::ply_write(ply, faces[i].inds[j]);
        }
    }
    
    if (!ply_close(ply))
        return 0;
    
    return 1;
}
