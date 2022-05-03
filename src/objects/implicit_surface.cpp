#include "implicit_surface.hpp"

namespace RT_ISICG
{
	bool ImplicitSurface::intersect( const Ray & p_ray,
									 const float p_tMin,
									 const float p_tMax,
									 HitRecord & p_hitRecord ) const
	{
		float dist = _sdf( p_ray.getOrigin() );
		Vec3f p_next;
		float distAtPoint = dist;
		int	  nbIter	  = 0;

		
		while ( distAtPoint > _minDistance && dist < p_tMax && nbIter < _maxIter)
		{
			p_next		= p_ray.pointAtT( dist );
			distAtPoint	   = _sdf( p_next );
			dist += distAtPoint;
			nbIter++;
		}

		if ( distAtPoint < _minDistance )
		{
			p_hitRecord._point	= p_next;
			p_hitRecord._normal = _evaluateNormal( p_next );
			p_hitRecord.faceNormal( p_ray.getDirection() );
			p_hitRecord._distance = dist;
			p_hitRecord._object	  = this;
			return true;
		}
		
		return false;
	}

	bool ImplicitSurface::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float dist = _sdf( p_ray.getOrigin() );
		Vec3f p_next;
		float distAtPoint = dist;
		int	  nbIter	  = 0;

		while ( distAtPoint > _minDistance && dist < p_tMax && nbIter < _maxIter )
		{
			p_next		= p_ray.pointAtT( dist );
			distAtPoint = _sdf( p_next );
			dist += distAtPoint;
			nbIter++;
		}

		if ( distAtPoint < _minDistance )
		{
			return true;
		}

		return false;
	}
} // namespace RT_ISICG
