#ifndef LOSS_H
#define LOSS_H

#include "../clover.h"

// current image
color I[1024][1024];
int I_height, I_width;

// ground truth image
color GT[1024][1024];
int GT_height, GT_width;

double loss() {
    std::string tmp;

    // read ground truth image
    freopen("GT.ppm", "r", stdin);
    std::cin >> tmp; // read "P5"
    std::cin >> GT_height >> GT_width;
    std::cin >> tmp; // read "255"
    for(int j=GT_height-1; j>=0; j--) {
        for(int i=0; i<GT_width; i++) {
            std::cin >> GT[j][i];
        }
    }

    // read current image
    freopen("I.ppm", "r", stdin);
    std::cin >> tmp; // read "P5"
    std::cin >> I_height >> I_width;
    std::cin >> tmp; // read "255"
    for(int j=I_height-1; j>=0; j--) {
        for(int i=0; i<I_width; i++) {
            std::cin >> I[j][i];
        }
    }

    // compute l2 loss
    double res = 0;
    for(int j=I_height; j>=0; j--) {
        for(int i=0; i<I_width; i++) {
            res += (I[j][i] - GT[j][i]).length_squared() / I_height / I_width;
        }
    }

    return res;
}

#endif