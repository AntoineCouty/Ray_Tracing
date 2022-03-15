#include "whitted_integrator.hpp"
#include "lights/point_light.hpp"
namespace RT_ISICG
{
	Vec3f WhittedIntegrator::Li( const Scene & p_scene,
										const Ray &	  p_ray,
										const float	  p_tMin,
										const float	  p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{ 
			return _liRecursif( p_scene, p_ray, p_tMin, p_tMax, 0 );
		}
		else
		{
			return _backgroundColor;
		}
	}
	Vec3f WhittedIntegrator::_liRecursif( const Scene & p_scene,
										  const Ray &	p_ray,
										  const float	p_tMin,
										  const float	p_tMax,
										  int			p_nbBounces ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			if ( p_nbBounces == _nbBounces ) { return BLACK; }
			if ( hitRecord._object->getMaterial()->isMirror() )
			{
				Vec3f ray_reflect = glm::reflect( p_ray.getDirection(), hitRecord._normal );
				_liRecursif( p_scene, Ray(hitRecord._point ,ray_reflect), p_tMin, p_tMax, p_nbBounces++ );
			}
			else
			{
				return _directLightingMain( p_scene, p_ray, hitRecord );
			}
		}
		else
		{
			return _backgroundColor;
		}
		
	}

	Vec3f WhittedIntegrator::_directLightingMain( const Scene & p_scene, const Ray & p_ray, HitRecord p_hitRecord ) const {
		Vec3f lum;
		Vec3f lum_list = Vec3f( 0.f );
		for ( BaseLight * light : p_scene.getLights() )
		{
			lum = Vec3f( 0.f );

			if ( light->getSurface() )
			{
				for ( int i = 0; i < _nbLightSamples; i++ )
				{
					LightSample ls	  = light->sample( p_hitRecord._point );
					Ray			o_ray = Ray( p_hitRecord._point, -ls._direction );
					o_ray.offset( p_hitRecord._normal );

					if ( !p_scene.intersectAny( o_ray, 0.f, ls._distance ) )
					{
						lum += _directLighting( o_ray, ls, p_hitRecord );
					}
				}
				lum /= Vec3f( float( _nbLightSamples ) );
			}
			else
			{
				LightSample ls	  = light->sample( p_hitRecord._point );
				Ray			o_ray = Ray( p_hitRecord._point, -ls._direction );
				o_ray.offset( p_hitRecord._normal );

				if ( !p_scene.intersectAny( o_ray, 0.f, ls._distance ) )
				{
					lum = _directLighting( o_ray, ls, p_hitRecord );
				}
			}
			lum_list += lum;
		}
		return lum_list;
	}

	Vec3f WhittedIntegrator::_directLighting( Ray ray, LightSample ls, HitRecord hitRecord ) const
	{
		float angle = glm::max( 0.f, glm::dot( glm::normalize( hitRecord._normal ), glm::normalize( -ls._direction ) ) );
		return hitRecord._object->getMaterial()->shade( ray, hitRecord, ls ) * ls._radiance * angle;
	}
} // namespace RT_ISICG