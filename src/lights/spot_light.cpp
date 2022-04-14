#include "spot_light.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	LightSample SpotLight::sample( const Vec3f & p_point ) const
	{
		float rho	= _rayon;
		float theta		= randomFloat() * TWO_PIf;
		Vec3f p_pos = Vec3f( _position.x + rho * glm::cos( theta ), _position.y + rho * glm::sin( theta ), _position.z );

		float distance	= glm::distance( p_point, p_pos );
		Vec3f rayDirection = glm::normalize( p_pos - p_point );
		Vec3f radiance	= Vec3f( 0.f );
		float pdf = 1.f;
		

		if ( glm::degrees( glm::acos( glm::dot( -rayDirection, _direction ) ) ) < _angle )
		{
			float fac_geo = glm::pow( distance, 2 ) / glm::dot( glm::normalize( _direction ), -rayDirection );
			pdf = fac_geo / _area;
			radiance = _color * _power / pdf;
		}

		//std::cout << radiance.x << std::endl;

		return LightSample( rayDirection, distance, radiance, pdf );
	}

} // namespace RT_ISICG
