#ifndef __RT_ISICG_BRDF_OREN__NAYAR__
#define __RT_ISICG_BRDF_OREN__NAYAR__
#include "defines.hpp"
#include <ray.hpp>
#include <lights/light_sample.hpp>
namespace RT_ISICG
{
	class OrenNayarBRDF
	{
	  public:
		OrenNayarBRDF( const Vec3f & p_kd, float p_sigma ) : _kd( p_kd ), _sigma( p_sigma ) {};
		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( Ray p_ray, LightSample p_ls ) const { 
			Vec3f dir_I = -p_ls._direction;
			Vec3f dir_O	 = p_ray.getDirection();

			float wI_p	 = glm::sqrt( dir_I.x * dir_I.x + dir_I.y * dir_I.y + dir_I.z * dir_I.z );
			float wI_phi = glm::acos(dir_I.z / wI_p);
			float wI_theta = glm::asin( dir_I.y / (wI_p * glm::sin( wI_phi ) ) );

			float wO_p	 = glm::sqrt( dir_O.x * dir_O.x + dir_O.y * dir_O.y + dir_O.z * dir_O.z );
			float wO_phi   = glm::acos( dir_O.z / wO_p );
			float wO_theta = glm::asin( dir_O.y / ( wO_p * glm::sin( wO_phi ) ) );

			float sigma2 = _sigma * _sigma;

			float A = 1 - ( 0.5 * sigma2 ) / ( sigma2 + 0.33 );
			float B = ( 0.45 * sigma2 ) / ( sigma2 + 0.09 );

			float alpha, beta;
			if ( wI_theta > wO_theta ) { 
				alpha = wI_theta;
				beta  = wO_theta;
			}
			else
			{
				alpha = wO_theta;
				beta  = wI_theta;
			}
			return _kd * INV_PIf * ( A + ( B * glm::max( 0.f, glm::cos( wI_phi - wO_phi ) ) * glm::sin( alpha ) * glm::tan( beta ) ) );
		}
		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;
		float _sigma;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_BRDF_OREN__NAYAR__