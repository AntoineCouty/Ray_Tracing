#ifndef __RT_ISICG_PHONG_MATERIAL__
#define __RT_ISICG_PHONG_MATERIAL__
#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/phong_brdf.hpp"
namespace RT_ISICG
{
	class PlasticMaterial : public BaseMaterial
	{
	  public:
		PlasticMaterial( const std::string & p_name, const Vec3f & p_diffuse, const Vec3f & p_speculaire, float p_s )
			: BaseMaterial( p_name ), _brdfLambert( p_diffuse ), _brdfPhong( p_speculaire, p_s )
		{
		}
		virtual ~PlasticMaterial() = default;
		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return _brdfLambert.evaluate() * 0.7f + _brdfPhong.evaluate( p_ray, p_hitRecord, p_lightSample ) * 0.3f;
		}
		inline const Vec3f & getFlatColor() const override { return _brdfLambert.getKd() * 0.7f + _brdfPhong.getKs() * 0.3f; }

	  protected:
		LambertBRDF _brdfLambert;
		PhongBRDF _brdfPhong;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_PHONG_MATERIAL__