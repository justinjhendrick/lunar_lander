#ifndef VECTOR_H_
#define VECTOR_H_

// 2 dimensional float vectors used for collision detection
class Vector {
    public:
        float x;
        float y;
        Vector(float _x, float _y);
        static float cross_mag(const Vector& a, const Vector& b);
        static Vector minus(const Vector& a, const Vector& b);
        static Vector plus(const Vector& a, const Vector& b);
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
