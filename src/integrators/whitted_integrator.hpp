#ifndef __RT_ISICG_WHITTED_INTEGRATOR__
#define __RT_ISICG_WHITTED_INTEGRATOR__

#include "base_integrator.hpp"

namespace RT_ISICG
{
	class WhittedIntegrator : public BaseIntegrator
	{
	  public:
		WhittedIntegrator() : BaseIntegrator() {}
		virtual ~WhittedIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::WHITTED; }

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;
		

	  private:
		Vec3f _liRecursif( const Scene & p_scene,
							const Ray &	  p_ray,
							const float	  p_tMin,
							const float	  p_tMax,
							int			  p_nbBounces ) const;
		Vec3f _directLighting( Ray ray, LightSample ls, HitRecord hitrecord ) const;
		Vec3f _directLightingMain( const Scene & p_scene, const Ray & p_ray, HitRecord hitrecord ) const;
		int	  _nbLightSamples = 16;
		int	  _nbBounces	  = 5;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_WHITTED_INTEGRATOR__
