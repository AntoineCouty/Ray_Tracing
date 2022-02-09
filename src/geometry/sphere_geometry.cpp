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
		float c = 
		p_t1 = -1.f;
		p_t2 = -1.f;

		return false;
	}

} // namespace RT_ISICG
