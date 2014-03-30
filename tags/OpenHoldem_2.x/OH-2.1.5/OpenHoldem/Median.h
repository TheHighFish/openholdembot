/*
 * Algorithm from N. Wirth's book, implementation by N. Devillard.
 * This code in public domain.
 */

#ifndef _INC_MEDIAN_H

#define _INC_MEDIAN_H


double kth_smallest(double a[], int n, int k);

#define median(a,n) kth_smallest(a,n,(((n)&1)?((n)/2):(((n)/2)-1)))


#endif // _INC_MEDIAN_H