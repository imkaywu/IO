//
//  mex_ply_read.c
//  
//
//  Created by KaiWu on Aug/10/16.
//
//

#include <stdio.h>
#include "mex.h"
#include "io_file.h"

extern int type_property[5];

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
//    if(nrhs != 7)
//        mexErrMsgTxt("Seven input argument required.");
    
    if(nlhs != 0)
        mexErrMsgTxt("Too many ouput arguments");
    
    double *points = NULL, *normals = NULL, *rgbs = NULL;
    double *faces = NULL, *grids = NULL;
    
    char *fname = mxArrayToString(prhs[0]);
    mexPrintf("%s\n", fname);
    points = (double *)mxGetPr(prhs[1]);
//    for (int i = 0; i < 10; i++) {
//        mexPrintf("%d: %f %f %f\n", i + 1, points[3 * i + 0], points[3 * i + 1], points[3 * i + 2]);
//    }
//    normals = (double *)mxGetPr(prhs[2]);
    rgbs = (double *)mxGetPr(prhs[2]);
    // no face input
    grids = (double *)mxGetPr(prhs[3]);
    int num_vertices = (int)mxGetScalar(prhs[4]);
    mexPrintf("vertices number: %d\n", num_vertices);
    // no face number
    int row = (int)mxGetScalar(prhs[5]);
    int col = (int)mxGetScalar(prhs[6]);
    mexPrintf("row: %d\n", row);
    mexPrintf("col: %d\n", col);
    
    int ret_val = ply_write_1(fname, points, normals, rgbs, faces, grids, num_vertices, -1, row, col);
}
