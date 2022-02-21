#include "Clip.hpp"

#include <array>

#include <Hinae/Vector3.hpp>

#define BASE_COMPARE(X, Y, Z, W, OP) X OP W && Y OP W && Z OP W

bool clip_spacae_culling(const Point4f& a, const Point4f& b, const Point4f& c, f32 z_near)
{
    // near <= w <= far
    if(BASE_COMPARE(a.w, b.w, c.w, z_near, <))
        return false;
    // depth test will clip
	// if(BASE_COMPARE(a.w, b.w, c.w, z_far, >))
    //     return false;

    // -w <= x, y, z <= w
    if(a.x >  a.w && b.x >  b.w && c.x >  c.w)
        return false;
    if(a.x < -a.w && b.x < -b.w && c.x < -c.w)
        return false;
    if(a.y >  a.w && b.y >  b.w && c.y >  c.w)
        return false;
    if(a.y < -a.w && b.y < -b.w && c.y < -c.w)
        return false;
    if(a.z >  a.w && b.z >  b.w && c.z >  c.w)
        return false;
    if(a.z < -a.w && b.z < -b.w && c.z < -c.w)
    {
        return false;
    }
    
	return true;
}

bool back_face_culling(const Vector3f& a, const Vector3f& b, const Vector3f& c)
{
    // NDC space, negative z
    const Vector2f edge1 = cast<Vector2>(b) - cast<Vector2>(a);
    const Vector2f edge2 = cast<Vector2>(c) - cast<Vector2>(a);
    return cross(edge1, edge2) < 0;
}
