#include "quad_light.hpp"
#include "utils/random.hpp"


namespace RT_ISICG
{
	LightSample QuadLight::sample( const Vec3f & p_point ) const
	{
		Vec3f p_pos		= _position + ( _u * randomFloat() ) + ( _v * randomFloat() );

		float distance	= glm::distance( p_point, p_pos );
		Vec3f direction = glm::normalize( p_pos - p_point );

		float cos_theta = glm::abs(( glm::dot( _normal, -direction )));
		float pdf	  = 1.f / _area *( distance * distance ) / cos_theta;
		
		Vec3f radiance	= _color * _power / pdf;
		/*std::cout << pdf << std::endl;
		std::cout << radiance.x << " " << radiance.y << radiance.z << std::endl;*/
		return LightSample( direction, distance, radiance, pdf );
	}

} // namespace RT_ISICG }
