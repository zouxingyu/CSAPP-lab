/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "zxy", /* Team name */

    "Harry Q. Bovik",    /* First member full name */
    "bovik@nowhere.edu", /* First member email address */

    "", /* Second member full name (leave blank if none) */
    ""  /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) {
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) {
    int i, j;
    int k = dim - 1;
    for (i = 0; i < dim; i += 4) {
        for (j = 0; j < dim; j += 4) {
            int a0 = i * dim + j, a1 = (i + 1) * dim + j, a2 = (i + 2) * dim + j, a3 = (i + 3) * dim + j;
            int b0 = (k - j) * dim + i, b1 = (k - 1 - j) * dim + i, b2 = (k - 2 - j) * dim + i, b3 = (k - 3 - j) * dim + i;
            dst[b0] = src[a0];
            dst[b1] = src[a0 + 1];
            dst[b2] = src[a0 + 2];
            dst[b3] = src[a0 + 3];
            dst[b0 + 1] = src[a1];
            dst[b1 + 1] = src[a1 + 1];
            dst[b2 + 1] = src[a1 + 2];
            dst[b3 + 1] = src[a1 + 3];
            dst[b0 + 2] = src[a2];
            dst[b1 + 2] = src[a2 + 1];
            dst[b2 + 2] = src[a2 + 2];
            dst[b3 + 2] = src[a2 + 3];
            dst[b0 + 3] = src[a3];
            dst[b1 + 3] = src[a3 + 1];
            dst[b2 + 3] = src[a3 + 2];
            dst[b3 + 3] = src[a3 + 3];
        }
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() {
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&rotate, rotate_descr);
    /* ... Register additional test functions here */
}

/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) {
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) {
    sum->red += (int)p.red;
    sum->green += (int)p.green;
    sum->blue += (int)p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) {
    current_pixel->red = (unsigned short)(sum.red / sum.num);
    current_pixel->green = (unsigned short)(sum.green / sum.num);
    current_pixel->blue = (unsigned short)(sum.blue / sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) {
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for (ii = max(i - 1, 0); ii <= min(i + 1, dim - 1); ii++)
        for (jj = max(j - 1, 0); jj <= min(j + 1, dim - 1); jj++)
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) {
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) {
    int i, j;
    for (i = 0; i < dim; i += 8) {
        for (j = 0; j < dim; j += 8) {
            int red, green, blue, num;
            red = green = blue = num = 0;

            for (int ii = 0; ii < 8; ii++) {
                for (int jj = 0; jj < 8; jj++) {
                    int cl = j + jj - 1, cr = j + jj + 1, cu = i + ii - 1, cd = i + ii + 1;
                    if (cl < 0 || cr >= dim || cu < 0 || cd >= dim) {
                        for (int iii = max(cu, 0); iii <= min(cd, dim - 1); iii++) {
                            int base = iii * dim;
                            for (int jjj = max(cl, 0); jjj <= min(cr, dim - 1); jjj++) {
                                red += (int)src[base + jjj].red;
                                green += (int)src[base + jjj].green;
                                blue += (int)src[base + jjj].blue;
                                num++;
                            }
                        }
                        int dst_i = i + ii, dst_j = j + jj, dst_base = dst_i * dim;
                        dst[dst_base + dst_j].red = (unsigned short)(red / num);
                        dst[dst_base + dst_j].green = (unsigned short)(green / num);
                        dst[dst_base + dst_j].blue = (unsigned short)(blue / num);
                        red = green = blue = num = 0;
                    } else {
                        for (int iii = cu; iii <= cd; iii++) {
                            int base = iii * dim;
                            for (int jjj = cl; jjj <= cr; jjj++) {
                                red += (int)src[base + jjj].red;
                                green += (int)src[base + jjj].green;
                                blue += (int)src[base + jjj].blue;
                            }
                        }
                        int dst_i = i + ii, dst_j = j + jj, dst_base = dst_i * dim;
                        dst[dst_base + dst_j].red = (unsigned short)(red / 9);
                        dst[dst_base + dst_j].green = (unsigned short)(green / 9);
                        dst[dst_base + dst_j].blue = (unsigned short)(blue / 9);
                        red = green = blue = 0;
                    }
                }
            }
        }
    }
}

/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}
