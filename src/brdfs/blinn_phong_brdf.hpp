#ifndef __RT_ISICG_BRDF_BLINN_PHONG__
#define __RT_ISICG_BRDF_BLINN_PHONG__
#include "defines.hpp"
#include "lights/light_sample.hpp"
#include <hit_record.hpp>
#include <ray.hpp>

namespace RT_ISICG
{
	class BlinnPhongBRDF
	{
	  public:
		BlinnPhongBRDF( const Vec3f & p_ks, float p_s ) : _ks( p_ks ), _s( p_s ) {};
		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( Ray ray, HitRecord hitrecord, LightSample ls ) const
		{
			Vec3f wO	 = -ray.getDirection();
			Vec3f wI	 = ls._direction;
			Vec3f normal = hitrecord._normal;
			Vec3f h		 = glm::normalize( wO + wI );
			return _ks *  glm::pow( glm::max( 0.f, glm ::dot( normal, h ) ), _s );
		}
		inline const Vec3f & getKs() const { return _ks; }

	  private:
		Vec3f _ks = WHITE;
		float _s;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_BRDF_BLINN_PHONG__