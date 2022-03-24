#include "aabb.hpp"
#include <objects/triangle_mesh.hpp>

namespace RT_ISICG
{

	bool AABB::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		Vec3f origin = p_ray.getOrigin();
		Vec3f dir	 = p_ray.getDirection();

		float t_min_x = ( _min.x - origin.x ) / dir.x;
		float t_max_x = ( _max.x - origin.x ) / dir.x;

		if ( t_min_x > t_max_x )
		{
			float tmp = t_min_x;
			t_min_x	  = t_max_x;
			t_max_x	  = tmp;
		}

		float t_min_y = ( _min.y - origin.y ) / dir.y;
		float t_max_y = ( _max.y - origin.y ) / dir.y;

		if ( t_min_y > t_max_y )
		{
			float tmp = t_min_y;
			t_min_y	  = t_max_y;
			t_max_y	  = tmp;
		}

		if ( t_min_x > t_max_y || t_min_y > t_max_x ) return false;

		float t_min = glm::max( t_min_x, t_min_y );
		float t_max = glm::min( t_max_x, t_max_y );

		float t_min_z = ( _min.z - origin.z ) / dir.z;
		float t_max_z = ( _max.z - origin.z ) / dir.z;

		if ( t_min_z > t_max_z )
		{
			float tmp = t_min_z;
			t_min_z	  = t_max_z;
			t_max_z	  = tmp;
		}
		

		if ( t_min > t_max_z || t_min_z > t_max ) return false;

		t_min = glm::max( t_min, t_min_z );
		t_max = glm::min( t_max, t_max_z );

		return ( t_min >= p_tMin && t_max <= p_tMax );
	
	}


} // namespace RT_ISICG
