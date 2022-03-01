#include "direct_lighting_integrator.hpp"
#include "lights/point_light.hpp"
namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			Vec3f lum = Vec3f( 0.f );

			for ( BaseLight * light : p_scene.getLights() ) {

				if (light->getSurface()) { 
					//#pragma omp parallel for
					for ( int i = 0; i < _nbLightSamples; i++ ) {
						LightSample ls	  = light->sample( hitRecord._point );
						Ray o_ray = Ray( hitRecord._point, -ls._direction );
						o_ray.offset( hitRecord._normal );
						int count = 1;
						if( !p_scene.intersectAny( o_ray, 0.f, 15.f ) ) { 
							lum += _directLighting(o_ray, ls, hitRecord ); 
							count++;
						}
						lum /= Vec3f( float(count) );
					}

				}
				else
				{
					LightSample ls	  = light->sample( hitRecord._point );
					Ray o_ray = Ray( hitRecord._point, -ls._direction );
					o_ray.offset( hitRecord._normal );
					int count = 0;
					if ( !p_scene.intersectAny( o_ray, 0.f, 15.f ) ) { lum += _directLighting(o_ray, ls, hitRecord ); }
					
				}
					
			}
			return lum;
		}
		else
		{
			return _backgroundColor;
		}
	}

	Vec3f DirectLightingIntegrator::_directLighting( Ray ray, LightSample ls, HitRecord hitRecord ) const { 
		float angle = glm::max( 0.f, -glm::dot( glm::normalize( hitRecord._normal ), glm::normalize( ls._direction ) ) );
		return hitRecord._object->getMaterial()->shade( ray, hitRecord, ls ) * ls._radiance * angle; 
	}
} // namespace RT_ISICG