//
//  mex_ply_read.c
//  
//
//  Created by KaiWu on Aug/10/16.
//
//

#include <stdio.h>
#include "mex.h"
#include "matrix.h"
#include "io_file.h"

extern int type_property[5];

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if(nrhs != 1)
        mexErrMsgTxt("One input argument required.");
    
    if(nlhs > 3)
        mexErrMsgTxt("Too many ouput arguments");
    
    char *fname = mxArrayToString(prhs[0]);
    
    int num_vertices, num_faces, num_grids;
    ply_header_read(fname, &num_vertices, &num_faces, &num_grids);
//     mexPrintf("vertices num: %d\n faces num: %d\n grids num: %d\n", num_vertices, num_faces, num_grids);
    
    double *points = NULL, *normals = NULL, *rgbs = NULL;
    double *faces = NULL, *grids = NULL;
    
    // points is one of the outputs
    if(type_property[0])
    {
        plhs[0] = mxCreateDoubleMatrix(DIM, num_vertices, mxREAL);
        points = mxGetPr(plhs[0]);
    }
    // normals is one of the outputs
    if(type_property[1])
    {
        plhs[1] = mxCreateDoubleMatrix(DIM, num_vertices, mxREAL);
        normals = mxGetPr(plhs[1]);
    }
    // rgbs is one of the outputs
//    if(type_property[2])
//    {
//        plhs[2] = mxCreateDoubleMatrix(DIM, num_vertices, mxREAL);
//        rgbs = mxGetPr(plhs[2]);
//    }
    // faces is one of the outputs
    if(type_property[3]) // create return array for face
    {
        plhs[2] = mxCreateDoubleMatrix(DIM, num_faces, mxREAL);
        faces = mxGetPr(plhs[2]);
    }
    // grids is one of the outputs
//    if(type_property[4])
//    {
//        plhs[1] = mxCreateDoubleMatrix(1, num_grids, mxREAL);
//        grids = mxGetPr(plhs[1]);
//    }
    
    int ret_val = ply_read_1(fname, points, normals, rgbs, faces, grids);
	
	
}
