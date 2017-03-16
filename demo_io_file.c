//
//  demo_io_file.c
//  demo_io_file
//
//  Created by KaiWu on Aug/10/16.
//  Copyright © 2016 KaiWu. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "io_file.h"

extern int type_property[5];

int main()
{
    char *fname = "/Users/BlacKay/Documents/Projects/Data/MVS+VH/box/dense_recon.nvm.cmvs/00/models/box_VH.ply";
    
    double *vertices = NULL, *normals = NULL, *colors = NULL;
    double *faces = NULL, *grids = NULL;
    
    int num_vertices, num_faces, num_grids;
    
    ply_header_read(fname, &num_vertices, &num_faces, NULL);
    
    vertices = (double *)malloc(DIM * num_vertices * sizeof(double));
    if(type_property[1])
        normals = (double *)malloc(DIM * num_vertices * sizeof(double));
    if(type_property[2])
        colors = (double *)malloc(DIM * num_vertices * sizeof(double));
    if(type_property[3])
        faces = (double *)malloc(DIM * num_faces * sizeof(double));
    if(type_property[4])
        grids = (double *)malloc(2 * num_grids * sizeof(double));
    
    ply_read_1(fname, vertices, normals, colors, faces, grids);
    
    for (int i = 0; i < 100; ++i)
    {
        printf("%f, %f, %f, %f, %f, %f, %d, %d, %d\n", vertices[DIM * i + 0], vertices[DIM * i + 1], vertices[DIM * i + 2], normals[DIM * i + 0], normals[DIM * i + 1], normals[DIM * i + 2], (int)faces[DIM * i + 0], (int)faces[DIM * i + 1], (int)faces[DIM * i + 2]);
    }
    
//    ply_write_1("/Users/BlacKay/Documents/Projects/Open Source/mesh_opt_pos_normal/samples/panel/panel-small-new.ply", vertices, normals, colors, faces, grids, num_vertices, num_faces, num_grids);
//    ply_write_1("/Users/BlacKay/Documents/Projects/Open Source/mesh_opt_pos_normal/samples/shell/shell-new.ply", vertices, normals, colors, NULL, grids, num_vertices, -1, 768, 1024);
}
