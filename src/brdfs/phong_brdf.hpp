#ifndef __RT_ISICG_BRDF_PHONG__
#define __RT_ISICG_BRDF_PHONG__
#include "defines.hpp"
#include <ray.hpp>
#include <hit_record.hpp>
#include "lights/light_sample.hpp"

namespace RT_ISICG
{
	class PhongBRDF
	{
	  public:
		PhongBRDF(  const Vec3f & p_ks, float p_s ) : _ks(p_ks), _s( p_s ) {};
		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( Ray ray, HitRecord hitrecord, LightSample ls ) const
		{
			Vec3f wO	 = -ray.getDirection();
			Vec3f normal = hitrecord._normal;

			float cos_theta = 1.f;

			for ( int i = 0; i < _s; i++ ) {
				cos_theta *= glm::dot( wO,  glm::normalize(glm::reflect( wO, normal )) );
			}
			
			return _ks * cos_theta;
		}
		inline const Vec3f & getKs() const { return _ks; }

	  private:
		Vec3f _ks = WHITE;
		float _s;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_BRDF_PHONG__