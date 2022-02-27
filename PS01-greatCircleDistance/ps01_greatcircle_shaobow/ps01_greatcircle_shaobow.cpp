// ps01_greatcircle_shaobow.cpp : This script calculates the distance along arc between any pair of selected points in the data set
// FYI, func "scanf" is used
// code written by Shaobo Wang from F21-24780B
// final version on 09/01/2021

#include <stdio.h>
#include <math.h> 

#define PT_NUM 2 // number of points
#define ARRAY_DIM 2*PT_NUM // dimension of data array

double havDistance(double ptr[ARRAY_DIM], int num1, int num2, double R); // func for calculating distance with Haversine Formula

int main(void) {

    double pt[ARRAY_DIM]; // point info arrary for all points in format:{latitude,longitude,...}
    double dis = 0; // distance along arc

    const double pi = 3.1415926; // def PI
    const double r = 6371; // radius of earth

    printf("24-780B Engineering Computation Prob Set 1 - Distance Calculator\n");

    for (int n = 1; n <= PT_NUM; n++) {
        printf("   Enter latitude  of point %d > ", n);
        scanf("%lf", &pt[2*n-2]);
        pt[2*n-2] *= (pi / 180);

        printf("   Enter longitude of point %d > ", n);
        scanf("%lf", &pt[2*n-1]);
        pt[2*n-1] *= (pi / 180);
    } // input point info

    dis = havDistance(pt, 1, 2, r); // calculate distance between two selected points

    printf("\nDistance is >>> %.3fkm\n", dis);

    return 0;
}

double havDistance(double ptr[ARRAY_DIM], int num1, int num2, double R) {

    double pt1_lat = ptr[2 * num1 - 2];
    double pt1_long = ptr[2 * num1 - 1];
    double pt2_lat = ptr[2 * num2 - 2];
    double pt2_long = ptr[2 * num2 - 1];

    double a = pow(sin((pt1_lat - pt2_lat) / 2), 2) + cos(pt1_lat) * cos(pt2_lat) * pow(sin((pt1_long - pt2_long) / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double dis = R * c;

    return dis;
}