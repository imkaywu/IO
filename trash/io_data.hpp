//
//  io_data.hpp
//  LibICP
//
//  Created by KaiWu on Jul/18/16.
//  Copyright © 2016 KaiWu. All rights reserved.
//
// rewrite this
// 1. no class
// 2. pointer/array for storage

#ifndef io_data_hpp
#define io_data_hpp

#include <iostream>
#include "Eigen/Dense"
#include "RPly/rply.h"
#include "RPly/rplyfile.h"

using std::cout;
using std::endl;
using std::string;
using Eigen::MatrixBase;

enum FORMAT {PLY = 0, OBJ, TXT};

class io
{
    typedef Eigen::Matrix3Xf Vertices;
    
    string input_file;
    string output_file;
    FORMAT f = PLY;
    
    static Vertices verts;
    static int property_ind;
    
private:
    int read_PLY(Vertices& verts);
    int read_OBJ(Vertices& verts);
    int read_TXT(Vertices& verts){ return 0; };
    int write_PLY(Vertices& verts);
    int write_OBJ(Vertices& verts);
    int write_TXT(Vertices& verts){ return 0; };
    
    // callback functions
    static int read_cb(p_ply_argument argument);
    static int write_source_cb(p_ply_argument argument);
    static int write_vertex_cb(p_ply_argument argument);
    static int setup_callbacks(p_ply iply, p_ply oply);
    
public:
    io(string& input_file, string& output_file, FORMAT& f)
    {
        this->input_file = input_file;
        this->output_file = output_file;
        this->f = f;
        property_ind = 0;
    };
    ~io(){ };
    
    int read(Vertices& verts);
    int write(Vertices& verts);
};


#endif /* io_data_hpp */
