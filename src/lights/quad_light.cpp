#include "quad_light.hpp"
#include "utils/random.hpp"


namespace RT_ISICG
{
	LightSample QuadLight::sample( const Vec3f & p_point ) const
	{
		Vec3f p_pos		= _position + ( _u * randomFloat() ) + ( _v * randomFloat() );

		float distance	= glm::distance( p_point, p_pos );
		Vec3f direction = glm::normalize( p_point - p_pos );

		float fac_geo  = glm::pow( distance, 2 ) / glm::dot( glm::normalize(_normal), glm::normalize(direction) );
		float pdf	   = fac_geo / _area;

		Vec3f radiance	= _color * _power / pdf;

		
		return LightSample( direction, distance, radiance, pdf );
	}

} // namespace RT_ISICG }
