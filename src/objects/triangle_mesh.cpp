#include "triangle_mesh.hpp"

namespace RT_ISICG
{
	bool MeshTriangle::intersect( const Ray & p_ray,
								  const float p_tMin,
								  const float p_tMax,
								  HitRecord & p_hitRecord ) const
	{
		
		return _bvh.intersect( p_ray, p_tMin, p_tMax, p_hitRecord );
	}

	bool MeshTriangle::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		return _bvh.intersectAny( p_ray, p_tMin, p_tMax );
	}
} // namespace RT_ISICG
