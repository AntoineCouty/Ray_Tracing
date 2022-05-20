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
		BlinnPhongMaterial( const std::string & p_name, const Vec3f & p_diffuse, const Vec3f & p_specular, float p_s )
			: BaseMaterial( p_name ), _brdfLambert( p_diffuse ), _brdfBlinnPhong( p_specular, p_s )
		{
		}
		virtual ~BlinnPhongMaterial() = default;
		Vec3f shade( const Vec3f & p_ray,
					 const HitRecord &	 p_hitRecord,
					 const Vec3f & p_lightSample ) const override
		{
			return _brdfLambert.evaluate() + _brdfBlinnPhong.evaluate( p_ray, p_hitRecord, p_lightSample ) ;
		}
		inline const Vec3f & getFlatColor() const override
		{
			return _brdfLambert.getKd()  + _brdfBlinnPhong.getKs();
		}

	  protected:
		LambertBRDF _brdfLambert;
		BlinnPhongBRDF _brdfBlinnPhong;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_BLINN_PHONG_MATERIAL__