#ifndef __RT_ISICG_PATH_INTEGRATOR__
#define __RT_ISICG_PATH_INTEGRATOR__

#include "base_integrator.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	class PathIntegrator : public BaseIntegrator
	{
	  public:
		PathIntegrator() : BaseIntegrator() {}
		virtual ~PathIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::PATH; }

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	  private:
		Vec3f _liRecursif( const Scene & p_scene,
						   const Ray &	 p_ray,
						   const float	 p_tMin,
						   const float	 p_tMax,
						   int			 p_nbBounces,
						   int			 p_nbBouncesIndirect,
						   bool			 p_inside ) const;
		Vec3f _directLighting( Ray ray, LightSample ls, HitRecord hitrecord ) const;
		Vec3f _indirectLighting( Vec3f wo, Vec3f wi, HitRecord hitRecord ) const;
		Vec3f _directLightingMain( Ray p_ray, const Scene & p_scene, HitRecord hitrecord ) const;

		float _getFresnelCoefficient( float n1,
									  float n2,
									  Vec3f wI_dir,
									  Vec3f refract_dir,
									  Vec3f p_normal,
									  bool	use_schlick = false ) const;
		int	  _nbLightSamples = 16;
		int	  _nbBounces	  = 5;
		int	  _nbIndirect	  = 3;
		int	  _nbPath		  = 1;
		float _p;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PATH_INTEGRATOR__
