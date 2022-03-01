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
		PhongBRDF( const Vec3f & p_kd, float p_s ) : _kd( p_kd ), _s(p_s) {};
		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( Ray ray, HitRecord hitrecord, LightSample ls ) const
		{
			return _kd * glm::pow(glm::dot( ray.getDirection(), glm::reflect(ls._direction, hitrecord._normal ) ), _s );
		}
		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;
		float _s;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_BRDF_PHONG__