#ifndef __RT_ISICG_MICRO_FACE_MATERIAL__
#define __RT_ISICG_MICRO_FACE_MATERIAL__
#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/phong_brdf.hpp"
#include "brdfs/micro_face_brdf.hpp"
namespace RT_ISICG
{
	class MicroFaceMaterial : public BaseMaterial
	{
	  public:
		MicroFaceMaterial( const std::string & p_name, const Vec3f & p_diffuse, float p_s )
			: BaseMaterial( p_name ), _brdfLambert( p_diffuse ), _brdfPhong( p_diffuse, p_s )
		{
		}
		virtual ~MicroFaceMaterial() = default;
		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return _brdfLambert.evaluate() * 0.7f + _brdfPhong.evaluate( p_ray, p_hitRecord ) * 0.3f;
		}
		inline const Vec3f & getFlatColor() const override
		{
			return _brdfLambert.getKd() * 0.7f + _brdfPhong.getKd() * 0.3f;
		}

	  protected:
		LambertBRDF _brdfLambert;
		PhongBRDF	_brdfPhong;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_MICRO_FACE_MATERIAL__