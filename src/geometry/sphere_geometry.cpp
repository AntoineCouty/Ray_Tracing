#include "sphere_geometry.hpp"

namespace RT_ISICG
{
	bool SphereGeometry::intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const
	{
		/// TODO !
		Vec3f dir = p_ray.getDirection();
		Vec3f c_ray = p_ray.getOrigin();
		float a	= dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
		float b	= 2 * ( dir.x * c_ray.x - dir.x * _center.x + dir.y * c_ray.y - dir.y * _center.y + dir.z * c_ray.z - dir.z * _center.z );
		float c	= c_ray.x * c_ray.x + c_ray.y * c_ray.y + c_ray.z * c_ray.z + _center.x * _center.x + _center.y * _center.y + _center.z * _center.z - 2 * (c_ray.x * _center.x + c_ray.y * _center.y + c_ray.z * _center.z) - _radius * _radius;
		float delta = b * b - 4 * a * c;

		p_t1		= -b + glm::sqrt( delta ) / 2 * a;
		p_t2		= -b - glm::sqrt( delta ) / 2 * a;

		if ( p_t1 > 0 || p_t2 > 0 )
		{
			if ( p_t2 < 0 || (p_t2 > p_t1 && p_t1 > 0)) { 
				return true;
			}
			if ( p_t2 < p_t1 || p_t1 < 0 )
			{ 
				float tmp = p_t2;
				p_t2	  = p_t1;
				p_t1	  = tmp;
				return true;
			}
		}

		p_t1 = -1.f;
		p_t2 = -1.f;

		return false;
	}

} // namespace RT_ISICG
