#ifndef __RT_ISICG_BLINN_PHONG_MATERIAL__
#define __RT_ISICG_BLINN_PHONG_MATERIAL__
#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/blinn_phong_brdf.hpp"
namespace RT_ISICG
{
	class BlinnPhongMaterial : public BaseMaterial
	{
	  public:
		BlinnPhongMaterial( const std::string & p_name, const Vec3f & p_diffuse, float p_s )
			: BaseMaterial( p_name ), _brdfLambert( p_diffuse ), _brdfBlinnPhong( p_diffuse, p_s )
		{
		}
		virtual ~BlinnPhongMaterial() = default;
		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return _brdfLambert.evaluate() * 0.7f
				   + _brdfBlinnPhong.evaluate( p_ray, p_hitRecord, p_lightSample ) * 0.3f;
		}
		inline const Vec3f & getFlatColor() const override
		{
			return _brdfLambert.getKd() * 0.7f + _brdfBlinnPhong.getKd() * 0.3f;
		}

	  protected:
		LambertBRDF _brdfLambert;
		BlinnPhongBRDF _brdfBlinnPhong;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_BLINN_PHONG_MATERIAL__