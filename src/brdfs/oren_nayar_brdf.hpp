#ifndef __RT_ISICG_BRDF_MATTE__
#define __RT_ISICG_BRDF_MATTE__
#include "defines.hpp"
#include <ray.hpp>
#include <lights/light_sample.hpp>
#include <hit_record.hpp>
namespace RT_ISICG
{
	class OrenNayarBRDF
	{
	  public:
		OrenNayarBRDF( const Vec3f & p_kd, float p_sigma ) : _kd( p_kd ), _sigma( p_sigma ) {};
		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( Vec3f wo, HitRecord p_hitrecord, Vec3f wi ) const
		{
			Vec3f normal = p_hitrecord._normal;
			Vec3f dir_I = wi;
			Vec3f dir_O	 = -wo;

			
			float wI_theta = glm::acos( glm::dot( dir_I , normal) );
			float wO_theta = glm::acos( glm::dot( dir_O, normal ) );

			Vec3f normal_wO_phi = glm::normalize( normal * glm::cos( wO_theta ) );
			Vec3f normal_wI_phi = glm::normalize( normal * glm::cos( wI_theta ) );
			


			float sigma2 = _sigma * _sigma;

			float A = 1 - ( 0.5 * sigma2 ) / ( sigma2 + 0.33 );
			float B = ( 0.45 * sigma2 ) / ( sigma2 + 0.09 );

			float alpha, beta;
			if ( wI_theta >= wO_theta ) { 
				alpha = wI_theta;
				beta  = wO_theta;
			}
			else
			{
				alpha = wO_theta;
				beta  = wI_theta;
			}
			return _kd * INV_PIf * ( A + ( B * glm::max( 0.f, glm::dot( normal_wI_phi, normal_wO_phi ) ) * glm::sin( alpha ) * glm::tan( beta ) ) );
		}
		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;
		float _sigma;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_BRDF_MATTE__