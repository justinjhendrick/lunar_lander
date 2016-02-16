#ifndef VECTOR_H_
#define VECTOR_H_

// 2 dimensional double vectors used for collision detection
class Vector {
    public:
        double x;
        double y;
        Vector(double _x, double _y);

        // value of 3rd coordinate of cross product
        // the 3rd coordinate is the only nonzero component of the cross product
        // because a and b are coplanar.
        static double cross(const Vector& a, const Vector& b);
        static Vector minus(const Vector& a, const Vector& b);
        static Vector plus(const Vector& a, const Vector& b);
        // dot product
        static double dot(const Vector& a, const Vector& b);

        // Does p + r intersect q + s?
        // http://stackoverflow.com/questions/563198/
        // how-do-you-detect-where-two-line-segments-intersect
        static bool segments_intersect(const Vector& p,
                                       const Vector& r,
                                       const Vector& q,
                                       const Vector& s);
};

#endif
