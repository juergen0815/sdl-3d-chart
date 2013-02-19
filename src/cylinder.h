#ifndef CYLINDER_H
#define CYLINDER_H

#include "err.h"
#include "entity.h"
#include "vector.h"

#include <vector>

class Cylinder : public Entity
{
public:
    enum {
        VERTEX_BUFFER = 0,
        INDEX_BUFFER,

        MAX_BUFFERS
    };

    class RenderState : public ::RenderState
    {
        float       m_Radius;
        float       m_Length;
        Vector      m_ColorFrom,
                    m_ColorTo;

        friend class Cylinder;

    } *m_RenderStateProxy;

private:
    int m_Buffers[ MAX_BUFFERS ];

    typedef std::vector<Vector> VertexArray;
    typedef std::vector<Vector> ColorArray;
    typedef std::vector<unsigned int> IndexArray;

    int         m_Stride;
    VertexArray m_VertexBuffer; // linear buffer
    VertexArray m_NormalBuffer; // linear buffer
    ColorArray  m_ColorBuffer;  // color buffer overlays Vertex Array
    IndexArray  m_IndexArray;   // standard array to map vertices to tris

public:
    Cylinder( float length = 1.0f, float radius = 0.5f, const Vector& col0 = Vector({ 1.0f, 1.0f, 0.0f, 1.0f }), const Vector& col1 = Vector({ 0.0f, 1.0f, 1.0f, 1.0f }) );

    virtual ~Cylinder();

    void SetColors( const Vector& colorFrom, const Vector& colorTo );

protected:
    void MakeCylinder( float meridians, float parallels, Cylinder::RenderState* renderState );

protected:
    virtual bool DoInitialize( Renderer* renderer ) throw(std::exception);

    virtual void DoRender( int pass ) throw(std::exception);
};


#endif /* CYLINDER_H */
