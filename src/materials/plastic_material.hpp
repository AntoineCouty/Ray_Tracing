#ifndef __RT_ISICG_PHONG_MATERIAL__
#define __RT_ISICG_PHONG_MATERIAL__
#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/phong_brdf.hpp"
#include "brdfs/blinn_phong_brdf.hpp"
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
		Vec3f shade( const Vec3f & p_ray,
					 const HitRecord &	 p_hitRecord,
					 const Vec3f & p_lightSample ) const override
		{
			return _brdfLambert.evaluate() + _brdfPhong.evaluate( p_ray, p_hitRecord, p_lightSample );
		}
		inline const Vec3f & getFlatColor() const override { return _brdfLambert.getKd() + _brdfPhong.getKs(); }

		const Vec3f getEmission()  override { return Vec3f(80.f, 80.f, 80.f); }

		

	  protected:
		LambertBRDF _brdfLambert;
		PhongBRDF _brdfPhong;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_PHONG_MATERIAL__