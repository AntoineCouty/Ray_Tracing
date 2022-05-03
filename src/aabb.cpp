#include "aabb.hpp"
#include <objects/triangle_mesh.hpp>

namespace RT_ISICG
{

	bool AABB::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		Vec3f origin = p_ray.getOrigin();
		Vec3f dir	 = p_ray.getDirection();

		Vec3f vecMin = ( _min - origin ) / dir;
		Vec3f vecMax = ( _max - origin ) / dir;
		

		if ( vecMin.x > vecMax.x )
		{
			float tmp = vecMin.x;
			vecMin.x  = vecMax.x;
			vecMax.x  = tmp;
		}


		if ( vecMin.y > vecMax.y )
		{
			float tmp = vecMin.y;
			vecMin.y  = vecMax.y;
			vecMax.y  = tmp;
		}

		if ( vecMin.x > vecMax.y || vecMin.y > vecMax.x ) return false;

		float t_min = glm::max( vecMin.x, vecMin.y );
		float t_max = glm::min( vecMax.x, vecMax.y );

		if ( vecMin.z > vecMax.z )
		{
			float tmp = vecMin.z;
			vecMin.z  = vecMax.z;
			vecMax.z  = tmp;
		}
		

		if ( t_min > vecMax.z || vecMin.z > t_max ) return false;

		t_min = glm::max( t_min, vecMin.z );
		t_max = glm::min( t_max, vecMax.z );
		
		return ( t_min < p_tMax && t_max > p_tMin );
	
	}


} // namespace RT_ISICG
