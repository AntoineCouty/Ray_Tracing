#ifndef __RT_ISICG_TRANSPARENT_MATERIAL__
#define __RT_ISICG_TRANSPARENT_MATERIAL__
#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/phong_brdf.hpp"
namespace RT_ISICG
{
	class TransparentMaterial : public BaseMaterial
	{
	  public:
		TransparentMaterial( const std::string & p_name, bool p_useShlick ) : BaseMaterial( p_name ), _useShlick(p_useShlick) {}
		virtual ~TransparentMaterial() = default;
		Vec3f shade( const Vec3f & p_ray,
					 const HitRecord &	 p_hitRecord,
					 const Vec3f & p_lightSample ) const override
		{
			return BLACK;
		}
		inline const Vec3f & getFlatColor() const override { return BLACK; }

		const float getIOR() override { return _ior; }

		const bool isTransparent() override { return true; }

		const bool isUseShlick() override { return _useShlick; }



	   private:
		float _ior = 1.3f;
		 bool  _useShlick;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_TRANSPARENT_MATERIAL__