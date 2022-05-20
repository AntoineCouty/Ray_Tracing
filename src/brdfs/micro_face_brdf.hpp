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
		MicroFaceBRDF( const Vec3f & p_kd, float p_sigma ): _kd( p_kd ), _sigma( p_sigma ), _alpha( p_sigma * p_sigma ), _F0( p_kd ) {};


		inline Vec3f evaluate( Vec3f wo, HitRecord p_hitrecord, Vec3f wi ) const
		{
			Vec3f wO =  -wo;
			Vec3f wI = wi ;
			Vec3f normal = p_hitrecord._normal;
			Vec3f h = glm::normalize(wO + wI) ;
			
			float G1wO = glm::dot( normal, wO );
			float G1wI = glm::dot( normal, wI );

			float D = dFunc( normal, h );
			
			float G = gFunc( G1wO ) * gFunc( G1wI );
			
			Vec3f F = fFunc( wO, h );
	
			return ( D * F * G ) / ( 4.f * G1wO * G1wI );
		}
		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;
		float _sigma;
		float _alpha;
		Vec3f _F0;

		inline float dFunc( const Vec3f & normal, const Vec3f & h ) const
		{
			float alpha2 = _alpha * _alpha;
			return alpha2 / ( PIf * glm::pow( glm::pow( glm::dot( normal, h ), 2 ) * ( alpha2 - 1.f ) + 1.f, 2 ) );
		};

		inline float gFunc( float x ) const { 
			float k = ( ( _sigma + 1 ) * ( _sigma + 1 ) ) / 8.f;
			return x / ( x * ( 1.f - k ) + k ); 
		}

		inline Vec3f fFunc( const Vec3f & wO, const Vec3f & h) const {
			return _F0 + ( 1.f - _F0 ) * glm::pow( 1.f - glm::dot( h, wO ), 5.f );
		};
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_BRDF_MICRO_FACE__