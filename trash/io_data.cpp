//
//  io_data.cpp
//  LibICP
//
//  Created by KaiWu on Jul/18/16.
//  Copyright © 2016 KaiWu. All rights reserved.
//

#include "io_data.hpp"

// must redeclare the static member variables in .cpp file
io::Vertices io::verts;
int io::property_ind;

int io::read(Vertices& verts)
{
    switch(f)
    {
        case PLY:
            return read_PLY(verts);
            break;
        case OBJ:
            return read_OBJ(verts);
            break;
        case TXT:
            return read_TXT(verts);
            break;
        default:
            cout << "Invalid file format. Please use .PLY, .OBJ, or .TXT files" << endl;
            return -1;
    }
    return 0;
}

int io::write(Vertices& verts)
{
    switch(f)
    {
        case PLY:
            return write_PLY(verts);
            break;
        case OBJ:
            return write_OBJ(verts);
            break;
        case TXT:
            return write_TXT(verts);
            break;
        default:
            cout << "Invalid file format. Please use .PLY, .OBJ, or .TXT files" << endl;
            return -1;
    }
    return 0;
}

int io::read_OBJ(Vertices& verts)
{
    char   s[200];
    float  x, y, z;
    
    // open file (in ASCII mode)
    FILE* in = fopen(input_file.c_str(), "r");
    if (!in)
    {
        cout << "Open file failed" << endl;
        return -1;
    }
    
    // clear line once
    memset(&s, 0, 200);
    
    //--- First pass, counts vertices
    int n_vertices = 0;
    while (in && !feof(in) && fgets(s, 200, in))
    {
        // comment
        if (s[0] == '#' || isspace(s[0]))
            continue;
        // vertex
        else if (strncmp(s, "v ", 2) == 0)
            n_vertices++;
    }
    fseek(in, 0, 0); ///< rewind
    // std::cout << "read #vertices: " << n_vertices << std::endl;
    verts.resize(Eigen::NoChange,n_vertices);
    
    //--- Second pass, fills in
    int curr_vertex=0;
    while (in && !feof(in) && fgets(s, 200, in))
    {
        // comment
        if (s[0] == '#' || isspace(s[0]))
            continue;
        // vertex
        else if (strncmp(s, "v ", 2) == 0)
        {
            if (sscanf(s, "v %f %f %f", &x, &y, &z))
            {
                verts(0,curr_vertex) = x;
                verts(1,curr_vertex) = y;
                verts(2,curr_vertex) = z;
                curr_vertex++;
            }
        }
        
        // normal
        else if (strncmp(s, "vn ", 3) == 0)
        {
            continue;
        }
        
        // face
        else if (strncmp(s, "f ", 2) == 0)
        {
            continue;
        }
        
        // clear line
        memset(&s, 0, 200);
    }
    
    fclose(in);
    return 0;
}

int io::write_OBJ(Vertices& verts)
{
    char s[200];
    long size_verts = verts.cols();
    
    FILE* out = fopen(output_file.c_str(), "w");
    if (!out)
    {
        cout << "Open file failed" << endl;
        return -1;
    }
    
    // clear line once
    memset(&s, 0, 200);
    
    //--- Second pass, fills in
    int curr_vertex=0;
    while (curr_vertex < size_verts)
    {
        // vertex
        fprintf(out, "v %f %f %f\n", verts(0, curr_vertex), verts(1, curr_vertex), verts(2, curr_vertex));
        curr_vertex++;
        
        // clear line
        memset(&s, 0, 200);
    }
    
    fclose(out);
    return 0;
}

int io::read_PLY(Vertices& verts)
{
    p_ply ply = ply_open(input_file.c_str(), NULL, 0, NULL);
    
    if(!ply)
        return -1;
    
    if(!ply_read_header(ply))
        return -1;
    
    long instance_num = get_ply_instance_num(ply, 0);
    this->verts.resize(Eigen::NoChange, instance_num);
    verts.resize(Eigen::NoChange, instance_num);
    
    ply_set_read_cb(ply, "vertex", "x", read_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "y", read_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "z", read_cb, NULL, 0);
    
//    ply_set_read_cb(ply, "face", "ind_vertex", face_cb, NULL, 0);
    
    if (!::ply_read(ply))
        return -1;
    
    verts = this->verts;
    
    if(!ply_close(ply))
        return -1;
    
    return 0;
}

int io::read_cb(p_ply_argument argument)
{
    double val = ply_get_argument_value(argument);
    int instance_ind = static_cast<int>(get_argument_instance_index(argument));
    
    switch(property_ind++)
    {
        case 0:
            verts(0, instance_ind) = (float) val;
            break;
        case 1:
            verts(1, instance_ind) = (float) val;
            break;
        case 2:
            verts(2, instance_ind) = (float) val;
            property_ind = 0;
            break;
    }
    
    return 1; // different from others
}

int io::write_vertex_cb(p_ply_argument argument)
{
    void *pdata;
    ply_get_argument_user_data(argument, &pdata, NULL);
    
    int instance_ind = static_cast<int>(get_argument_instance_index(argument));
    
    p_ply_property property;
    ply_get_argument_property(argument, &property, NULL, NULL);
    const char *name;
    ply_get_property_info(property, &name, NULL, NULL, NULL);
    
    
    if(strcmp(name, "x") == 0)
        ply_write((p_ply) pdata, verts(0, instance_ind));
    else if(strcmp(name, "y") == 0)
        ply_write((p_ply) pdata, verts(1, instance_ind));
    else if(strcmp(name, "z") == 0)
        ply_write((p_ply) pdata, verts(2, instance_ind));
    
    /* just pass the value from the input file to the output file */
//    ply_write((p_ply) pdata, ply_get_argument_value(argument));
    return 1;
}

int io::write_source_cb(p_ply_argument argument)
{
    void *pdata;
    /* just pass the value from the input file to the output file */
    ply_get_argument_user_data(argument, &pdata, NULL);
    ply_write((p_ply) pdata, ply_get_argument_value(argument));
    return 1;
}

int io::setup_callbacks(p_ply iply, p_ply oply) {
    p_ply_element element = NULL;
    /* iterate over all elements in input file */
    while ((element = ply_get_next_element(iply, element))) {
        p_ply_property property = NULL;
        long ninstances = 0;
        const char *element_name;
        ply_get_element_info(element, &element_name, &ninstances);
        /* add this element to output file */
        if (!ply_add_element(oply, element_name, ninstances)) return 0;
        /* iterate over all properties of current element */
        while ((property = ply_get_next_property(element, property))) {
            const char *property_name;
            e_ply_type type, length_type, value_type;
            ply_get_property_info(property, &property_name, &type,
                                  &length_type, &value_type);
            /* setup input callback for this property */
            if(strcmp(element_name, "vertex") == 0)
                ply_set_read_cb(iply, element_name, property_name, write_vertex_cb, oply, 0);
            else
                ply_set_read_cb(iply, element_name, property_name, write_source_cb, oply, 0);
            /* add this property to output file */
            if (!ply_add_property(oply, property_name, type, length_type,
                                  value_type)) return 0;
        }
    }
    return 1;
}

int io::write_PLY(Vertices& verts)
{
    const char* value;
    
    this->verts = verts;
    
    p_ply ply_in = ply_open(input_file.c_str(), NULL, 0, NULL);
    if (!ply_in)
        return -1;
    
    if(!ply_read_header(ply_in))
        return -1;
    
    p_ply ply_out = ply_create(output_file.c_str(), PLY_ASCII, NULL, 0, NULL);
    if(!ply_out)
        return -1;
    
    if(!setup_callbacks(ply_in, ply_out))
        return -1;
    
    value = NULL;
    while ((value = ply_get_next_comment(ply_in, value)))
    {
        if(!ply_add_comment(ply_out, value))
            return -1;
    }
    
    value = NULL;
    while ((value = ply_get_next_obj_info(ply_in, value)))
    {
        if(!ply_add_obj_info(ply_out, value))
            return -1;
    }
    
    if(!ply_write_header(ply_out))
        return -1;
    
    if(!ply_read(ply_in))
        return -1;
    
    if(!ply_close(ply_in))
        return -1;
    
    if(!ply_close(ply_out))
        return -1;
    
    return 1;
}


