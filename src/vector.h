/*
 * vector.h
 *
 *  Created on: 2013-01-17
 *      Author: jurgens
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include "err.h"

#include <cmath>
#include <cstring>

class Vector
{
public:
    enum Coord {
        X, Y, Z, W
    };
    enum Color {
        R, G, B, A
    };
    // re-use vector as pure storage for s,t coords - will not work with cross/dot product!
    enum TexCoord {
        U, V, S, T
    };
private:
    // this will hardly pad, but we must ensure we can tightly pack a float[4] into a linear array!
#pragma pack( push, 1 )
    float vec[4];
#pragma pack( pop )
    // rest of class is inlined which degrades the whole vector into 4 floats and a "namespace" - never create a vtable for this class!!!
    // A vertex will only need 3 components, but all load store are 4 anyway...at the cost of an additional float...
public:
    Vector() : vec({0,0,0,1})
    {
    }

    Vector( const Vector& v )
    {
        std::memcpy(vec,v.vec, 4*sizeof(float));
    }

    Vector( const float v[4] )
    {
        std::memcpy(vec,v, 4*sizeof(float));
    }

    Vector( float x, float y, float z )
    {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
        vec[3] = 1;
    }

    Vector( float r, float g, float b, float a )
    {
        vec[0] = r;
        vec[1] = g;
        vec[2] = b;
        vec[3] = a;
    }

    operator float* () { return vec; }

    operator const float* () const { return vec; }

    float operator[]( Vector::Coord coord ) const {
        return vec[ (int)coord ];
    }

    float operator[]( Vector::Color color ) const {
        return vec[ (int)color ];
    }

    float& operator[]( Vector::Coord coord ) {
        return vec[ (int)coord ];
    }

    float& operator[]( Vector::Color color ) {
        return vec[ (int)color ];
    }

    void operator+=( float v ) {
        vec[0] += v;
        vec[1] += v;
        vec[2] += v;
        vec[3] += v;
    }

    void operator-=( float v ) {
        vec[0] -= v;
        vec[1] -= v;
        vec[2] -= v;
        vec[3] -= v;
    }

    void operator*=( float v ) {
        vec[0] *= v;
        vec[1] *= v;
        vec[2] *= v;
        vec[3] *= v;
    }

    void operator+=( const Vector& v )
    {
        vec[0] += v.vec[0];
        vec[1] += v.vec[1];
        vec[2] += v.vec[2];
        vec[3] += v.vec[3];
    }

    void operator-=( const Vector& v )
    {
        vec[0] -= v.vec[0];
        vec[1] -= v.vec[1];
        vec[2] -= v.vec[2];
        vec[3] -= v.vec[3];
    }

    void operator*=( const Vector& v )
    {
        vec[0] *= v.vec[0];
        vec[1] *= v.vec[1];
        vec[2] *= v.vec[2];
        vec[3] *= v.vec[3];
    }

    Vector& Add( const Vector& v )
    {
        *this += v;
        return *this;
    }

    Vector operator+( const float v[4] ) const
    {
        return Vector(v).Add(*this);
    }

    Vector operator+( const Vector& v ) const
    {
        return Vector(*this).Add(v);
    }

    Vector& Sub( const Vector& v )
    {
        *this -= v;
        return *this;
    }

    Vector operator-( const float v[4] ) const
    {
        // avoid copying vector twice
        return Vector(v).Mul(-1).Add(*this);
    }

    Vector operator-( const Vector& v ) const
    {
        return Vector(*this).Sub( v );
    }

    Vector& Mul( const Vector& v )
    {
        *this *= v;
        return *this;
    }

    Vector operator*( const Vector& v ) const
    {
        return Vector(*this).Mul( v );
    }

    Vector& Add( float v )
    {
        *this += v;
        return *this;
    }

    Vector operator+( float v ) const
    {
        return Vector(*this).Add( v );
    }

    Vector& Sub( float v )
    {
        *this -= v;
        return *this;
    }

    Vector operator-( float v ) const
    {
        return Vector(*this).Sub( v );
    }

    Vector& Mul( float v )
    {
        *this *= v;
        return *this;
    }

    Vector operator*( float v ) const
    {
        return Vector(*this).Mul( v );
    }

    float Magnitude() const
    {
        // only x,y,z values - ignore w. Always positive. There is no neg sqrt
        return std::sqrt( vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2] );
    }

    float Dot( const Vector& v ) const
    {
        return vec[0]*v.vec[0] + vec[1]*v.vec[1] + vec[2]*v.vec[2];
    }

    Vector& Cross( const Vector& v )
    {
        float x = vec[Y] * v.vec[Z] - vec[Z] * v.vec[Y];
        float y = vec[Z] * v.vec[X] - vec[X] * v.vec[Z];
        float z = vec[X] * v.vec[Y] - vec[Y] * v.vec[X];

        vec[X] = x;
        vec[Y] = y;
        vec[Z] = z;
        vec[W] = 0;
        return *this;
    }

    Vector& Normalize()
    {
        // Magnitude is always positive!
        float length = Magnitude();
        if ( length > 0 ) {
            Mul( 1/length );
        }
        return *this;
    }

    Vector Normalized() const
    {
        return Vector( *this ).Normalize();
    }

    Vector& ClampTop()
    {
        if ( vec[ Vector::R ] > 1.0f) vec[ Vector::R ] = 1.0f;
        if ( vec[ Vector::G ] > 1.0f) vec[ Vector::G ] = 1.0f;
        if ( vec[ Vector::B ] > 1.0f) vec[ Vector::B ] = 1.0f;
        if ( vec[ Vector::A ] > 1.0f) vec[ Vector::A ] = 1.0f;
        return *this;
    }

    Vector& ClampBottom()
    {
        if ( vec[ Vector::R ] < 0.0f) vec[ Vector::R ] = 0.0f;
        if ( vec[ Vector::G ] < 0.0f) vec[ Vector::G ] = 0.0f;
        if ( vec[ Vector::B ] < 0.0f) vec[ Vector::B ] = 0.0f;
        if ( vec[ Vector::A ] < 0.0f) vec[ Vector::A ] = 0.0f;
        return *this;
    }
};

inline Vector& CLAMP_TOP( Vector& v )
{
    return v.ClampTop();
}

inline Vector& CLAMP_BOTTOM( Vector& v )
{
    return v.ClampBottom();
}


#endif /* VECTOR_H_ */
