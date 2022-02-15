#include "point_light.hpp"

namespace RT_ISICG
{
	LightSample PointLight::sample( const Vec3f & p_point ) const { 
		
		float distance = glm::distance( p_point, _position );
		float att		= glm::pow( 1.f / distance, 2 );
		Vec3f direction = glm::normalize(p_point - _position);
		Vec3f radiance	= _color * _power * att;
		return LightSample(direction, distance, radiance, 1.f);
	
	}

} // namespace RT_ISICG