#ifndef __RT_ISICG_IMPLICIT_SURFACE__
#define __RT_ISICG_IMPLICIT_SURFACE__

#include "base_object.hpp"

namespace RT_ISICG
{
	class ImplicitSurface : public BaseObject
	{
	  public:
		ImplicitSurface()		   = delete;
		virtual ~ImplicitSurface() = default;

		ImplicitSurface( const std::string & p_name ) : BaseObject( p_name ) {}

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;

		// Check for any intersection between p_tMin and p_tMax.
		virtual bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	  private:
		// Signed Distance Function
		virtual float _sdf( const Vec3f & p_point ) const = 0;

		// Evaluate normal by computing gradient at 'p_point'
		virtual Vec3f _evaluateNormal( const Vec3f & p_point ) const
		{
			const float eps = 0.000001; // or some other value
			const Vec3f hx	= Vec3f( eps, 0.f, 0.f );
			const Vec3f hy	= Vec3f( 0.f, eps, 0.f );
			const Vec3f hz	= Vec3f( 0.f, 0.f, eps );

			Vec3f normal = glm::normalize( Vec3f( _sdf( p_point + hx ) - _sdf( p_point - hx ),
												 _sdf( p_point + hy ) - _sdf( p_point - hy ),
												 _sdf( p_point + hz ) - _sdf( p_point - hz ) ) );
			//std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;

			return normal;
		}

	  private:
		const float _minDistance = 1e-5f;
		const int	_maxIter	 = 1000;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SURFACE__
