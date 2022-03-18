#ifndef __RT_ISICG_MICRO_FACE_MA*ERIAL__
#define __RT_ISICG_MICRO_FACE_MATERIAL__
#include "base_material.hpp"
#include "brdfs/oren_nayar_brdf.hpp"
#include "brdfs/phong_brdf.hpp"
#include "brdfs/micro_face_brdf.hpp"
namespace RT_ISICG
{
	class MicroFaceMaterial : public BaseMaterial
	{
	  public:
		MicroFaceMaterial( const std::string & p_name, const Vec3f & p_diffuse, float p_sigma, float p_metalness)
			: BaseMaterial( p_name ), _brdfMicroFace( p_diffuse, p_sigma ), _brdfOrenNayar( p_diffuse, p_sigma ), _metalness( p_metalness )
		{
		}
		virtual ~MicroFaceMaterial() = default;
		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return ( 1.f - _metalness ) * _brdfOrenNayar.evaluate( p_ray, p_hitRecord, p_lightSample ) + _metalness * _brdfMicroFace.evaluate( p_ray, p_hitRecord, p_lightSample );
		}
		inline const Vec3f & getFlatColor() const override
		{
			return ( 1.f - _metalness ) * _brdfOrenNayar.getKd() + _metalness * _brdfMicroFace.getKd();
		}

	  protected:
		MicroFaceBRDF _brdfMicroFace;
		OrenNayarBRDF	  _brdfOrenNayar;
		float		  _metalness;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_MICRO_FACE_MATERIAL__