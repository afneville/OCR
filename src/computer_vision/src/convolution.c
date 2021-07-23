#include "../includes/convolution.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

float * create_kernel(kernel_configuration type, int kernel_dimensions) {

    float * kernel = (float *) malloc(sizeof(float) * kernel_dimensions * kernel_dimensions);
    /* float kernel[kernel_dimensions * kernel_dimensions]; */

    if (type == Mean) {
        for (int i = 0; i < kernel_dimensions * kernel_dimensions; i++) {
            kernel[i] = 1;
        }

    } else if (type == Gaussian) {

        if (kernel_dimensions == 3) {
            float weights[] = {1.0,2.0,1.0,
                               2.0,4.0,2.0,
                               1.0,2.0,1.0};

            memcpy(kernel, weights, sizeof(float) * 9);

        } else if (kernel_dimensions == 5) {
            float weights[] = {1.0,  4.0,  6.0,  4.0,  1.0,
                               4.0,  16.0, 24.0, 16.0, 4.0, 
                               6.0,  24.0, 36.0, 24.0, 6.0,
                               4.0,  16.0, 24.0, 16.0, 4.0,
                               1.0,  4.0,  6.0,  4.0,  1.0};

            memcpy(kernel, weights, sizeof(float) * 25);
        }
    }
    return kernel;
}

float * apply_convolution(image_data * image, float * kernel, int kernel_dimensions) {

    // create a tree to store the data
    /* node * root = create_node(); */
    float * blurred_image = (float *) malloc(sizeof(float) * image->height * image->width);
    int kernel_center = ((kernel_dimensions + 1) / 2) -1;

    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) { // for every pixel

            float sum_used_weights = 0;
            float weighted_sum = 0;

            for (int j = 0; j < kernel_dimensions * kernel_dimensions; j++) {
                /* printf("%f\n", kernel[j]); */
                // get x and y displacement from center (within the kernel)
                int x_pos = j % kernel_dimensions;
                int y_pos = j / kernel_dimensions;
                x_pos = x_pos - kernel_center;
                y_pos = y_pos - kernel_center;

                // if parts of the kernel fall outside the image boundaries
                // those parts are ignored when calculating the convolved value
                if( y+y_pos >= 0 &&
                    y+y_pos < image->height &&
                    x+x_pos >= 0 &&
                    x+x_pos < image->width ) {

                    sum_used_weights += kernel[j];
                    weighted_sum += (image->pixels[((y+y_pos) * image->width) + (x+x_pos)] * kernel[j]);

                }
            }
            /* printf("%f\n", sum_used_weights); */
            float intensity = weighted_sum / sum_used_weights;
            /* printf("%d %d %f\n", x, y, intensity); */
            blurred_image[y*image->width + x] = intensity;
            /* node_data current_pixel = {(y*image->width + x), intensity}; */
            /* insert_data(root, &current_pixel); */

        }
    }
    return blurred_image;
    /* return root; */
};

float * mean_pool_image(image_data * image, int dimensions, int * height, int * width) {

    * height = image->height / dimensions;
    * width = image->width / dimensions;

    float * pixelated_image = (float *) malloc(sizeof(float) * (* height) * (* width));

    for (int y = 0; y < image->height; y+=dimensions) {
        for (int x = 0; x < image->width; x+=dimensions) { // for every pixel
            float kernel_sum  = 0.0;
            if (x + dimensions < image->width && y + dimensions < image->height) {
                for (int j = 0; j < dimensions; j++ ) {
                    for (int i = 0; i < dimensions; i++) {
                        kernel_sum += image->pixels[(y + j) * image->width + (x + i)];
                    }
                }
            pixelated_image[(y/dimensions) * (* width) + (x/dimensions)] = (kernel_sum / (dimensions * dimensions));
            }
        }
    }
    return pixelated_image;
};

float * max_pool_image(image_data * image, int dimensions, int * height, int * width) {

    // set the dimensions of the new image
    * height = image->height / dimensions;
    * width = image->width / dimensions;

    float * pixelated_image = (float *) malloc(sizeof(float) * (* height) * (* width));

    for (int y = 0; y < image->height; y+=dimensions) {
        for (int x = 0; x < image->width; x+=dimensions) { // for every pixel
            float kernel_maximum  = 255.0;
            if (x + dimensions < image->width && y + dimensions < image->height) {
                // find the darkest pixel in the image
                for (int j = 0; j < dimensions; j++ ) {
                    for (int i = 0; i < dimensions; i++) {
                        if (image->pixels[(y + j) * image->width + (x + i)] < kernel_maximum) {
                            kernel_maximum = image->pixels[(y + j) * image->width + (x + i)];
                        }
                    }
                }
            pixelated_image[(y/dimensions) * (* width) + (x/dimensions)] = kernel_maximum;
            }
        }
    }
    return pixelated_image;
};
