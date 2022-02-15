#include "plane_geometry.hpp"

namespace RT_ISICG
{
	bool PlaneGeometry::intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const
	{ 
		p_t2 = -1.f;
		p_t1 = -1.f;
		if ( glm::dot( p_ray.getDirection(), _normal ) == 0.f ) { 
			return false;
		}
		
		p_t1 = glm::dot( _point - p_ray.getOrigin(), _normal )/glm::dot(p_ray.getDirection(), _normal);
		
		return true;
	}

} // namespace RT_ISICG
