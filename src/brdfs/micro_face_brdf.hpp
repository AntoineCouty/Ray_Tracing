#ifndef __RT_ISICG_BRDF_MICRO_FACE__
#define __RT_ISICG_BRDF_MICRO_FACE__
#include "defines.hpp"
#include <hit_record.hpp>
#include <ray.hpp>
#include <lights/light_sample.hpp>
namespace RT_ISICG
{
	class MicroFaceBRDF
	{
	  public:
		MicroFaceBRDF( const Vec3f & p_kd, float p_sigma, const Vec3f & p_F0 ): _kd( p_kd ), _sigma( p_sigma ), _alpha( p_sigma * p_sigma ), _F0( p_F0 ) {};
		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( Ray p_ray, HitRecord p_hitrecord, LightSample p_ls ) const
		{
			Vec3f wO = glm::normalize( p_ray.getDirection() );
			Vec3f wI = glm::normalize( p_ls._direction );
			float alpha2 = _alpha * _alpha;
			Vec3f h = glm::normalize( wO + wI );

			float D = alpha2 * INV_PIf * glm::pow( glm::pow( glm::dot( p_hitrecord._normal, h ), 2 ) * ( alpha2 - 1.f ) + 1.f, 2 );

			float G1wO = glm::dot( p_hitrecord._normal, wO );
			float G1wI = glm::dot( p_hitrecord._normal, wI );
			float k	   = ( ( _sigma + 1 ) * ( _sigma + 1 ) ) / 8.f;

			float G = gFunc( G1wO, k ) * gFunc( G1wI, k );

			Vec3f F = _F0 + ( 1.f - _F0 ) * glm::pow( 1.f - glm::dot( h, wI ), 5.f );
			return ( D * F * G ) / ( 4.f * G1wO * G1wI );
		}
		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;
		float _sigma;
		float _alpha;
		Vec3f _F0;

		inline float gFunc( float x, float k ) const { return x / ( x * ( 1.f - k ) + k ); }
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_BRDF_MICRO_FACE__