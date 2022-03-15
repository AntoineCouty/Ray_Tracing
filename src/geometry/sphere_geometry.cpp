#include "sphere_geometry.hpp"

namespace RT_ISICG
{
	bool SphereGeometry::intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const
	{
		/// TODO !
		Vec3f dir = p_ray.getDirection();
		Vec3f c_ray = p_ray.getOrigin();
		float a	= glm::dot( dir, dir );
		float b	= glm::dot(Vec3f(2.f)*(c_ray -_center), dir);
		float c	= glm::dot(c_ray -_center, c_ray -_center) - _radius * _radius;
		float delta = b * b - 4 * a * c;

		if ( delta < 0 ) {
			p_t1 = -1.f;
			p_t2 = -1.f;
			return false;
		}

		if ( delta == 0 )
		{
			p_t1 = -b / 2.0 * a;
			p_t2 = -1.f;
			return true;
		}

		p_t1 = (-b + glm::sqrt( delta )) / ( 2.0 * a ) < 0 ? -1.f : (-b + glm::sqrt( delta )) / ( 2.0 * a );
		p_t2 = (-b - glm::sqrt( delta )) / ( 2.0 * a ) < 0 ? -1.f : (-b - glm::sqrt( delta )) / ( 2.0 * a );

		if ( (p_t2 < p_t1 && p_t2 != -1.f) || p_t1 == -1.f)
		{ 
			float tmp = p_t2;
			p_t2	  = p_t1;
			p_t1	  = tmp;
		}
		return true;
	}

} // namespace RT_ISICG
