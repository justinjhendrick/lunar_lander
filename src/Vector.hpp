#ifndef VECTOR_H_
#define VECTOR_H_

// 2 dimensional float vectors used for collision detection
class Vector {
    public:
        float x;
        float y;
        Vector(float _x, float _y);

        // value of 3rd coordinate of cross product
        // the 3rd coordinate is the only nonzero component
        // because a and b must be coplanar.
        static float cross(const Vector& a, const Vector& b);
        static Vector minus(const Vector& a, const Vector& b);
        static Vector plus(const Vector& a, const Vector& b);
        // dot product
        static float dot(const Vector& a, const Vector& b);

        // Does p + r intersect q + s?
        // http://stackoverflow.com/questions/563198/
        // how-do-you-detect-where-two-line-segments-intersect
        static bool segments_intersect(const Vector& p,
                                       const Vector& r,
                                       const Vector& q,
                                       const Vector& s);
};

#endif
