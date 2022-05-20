#include "lights/point_light.hpp"
#include "path_integrator.hpp"
namespace RT_ISICG
{
	Vec3f PathIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;
		
		return _liRecursif( p_scene, p_ray, p_tMin, p_tMax, 0, 0, false );
	}

	Vec3f PathIntegrator::_liRecursif( const Scene & p_scene,
										  const Ray &	p_ray,
										  const float	p_tMin,
										  const float	p_tMax,
										  int			p_nbBounces,
										  int			p_nbBouncesIndirect,
										  bool			p_inside ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			if ( p_nbBounces == _nbBounces ) { return _backgroundColor; }
			if ( p_nbBouncesIndirect == _nbIndirect ) { return _backgroundColor; }

			if (glm::length(hitRecord._object->getMaterial()->getEmission()) > EPSILON) {
				return hitRecord._object->getMaterial()->getEmission();
			}
				


			if ( hitRecord._object->getMaterial()->isMirror() )
			{
				
				Ray ray_reflect = Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				ray_reflect.offset( hitRecord._normal );
				return _liRecursif( p_scene, ray_reflect, p_tMin, p_tMax, p_nbBounces+1, p_nbBouncesIndirect, false );
			}
			else if ( hitRecord._object->getMaterial()->isTransparent() )
			{
				float n1, n2 = 1.f;
				float ior	 = hitRecord._object->getMaterial()->getIOR();
				Vec3f normal = hitRecord._normal;
				Vec3f wI_dir = p_ray.getDirection();
				

				if ( p_inside )
				{
					n1 = ior;
					n2 = 1.f;
				}
				else
				{
					n1 = 1.f;
					n2 = ior;
				}

				Vec3f refract_dir = glm::refract( wI_dir, normal, n1 / n2 );
				Ray	  ray_reflect = Ray( hitRecord._point, glm::reflect( wI_dir, normal ) );
				Ray	  ray_refract = Ray( hitRecord._point, refract_dir );
				float ret		  = _getFresnelCoefficient( n1, n2, wI_dir, refract_dir, normal );
				ray_reflect.offset( normal );
				Vec3f reflect
					= _liRecursif( p_scene, ray_reflect, p_tMin, p_tMax, p_nbBounces + 1, p_nbBouncesIndirect, p_inside ) * ret;
				ray_refract.offset( -normal );
				return reflect + _liRecursif( p_scene, ray_refract, p_tMin, p_tMax, p_nbBounces + 1, p_nbBouncesIndirect , !p_inside ) * ( 1 - ret );
			}
			
			
			//Vec3f direct =  _directLightingMain( p_ray, p_scene, hitRecord );
			Vec3f indirect = VEC3F_ZERO;
				
				

			for ( int i = 0; i < _nbPath; i++ )
			{
				Vec3f dir;

				do
				{
					float x	= ( randomFloat() - 0.5 ) * 2.0;
					float y	= ( randomFloat() - 0.5 ) * 2.0;
					float z	= ( randomFloat() - 0.5 ) * 2.0;
					dir = glm::normalize(Vec3f( x, y, z ));
				} while ( glm::dot(dir, dir) > 1.0 );

				dir		  = glm::dot( dir, hitRecord._normal ) < 0 ? -dir : dir;
				Ray wi = Ray( hitRecord._point, dir );
				wi.offset( hitRecord._normal );
					
				indirect += _liRecursif( p_scene, wi, p_tMin, p_tMax, p_nbBounces, p_nbBouncesIndirect+1, p_inside )
							* _indirectLighting( p_ray.getDirection(), dir, hitRecord ) * PI_2f;
			}
			indirect /= _nbPath;
			return indirect;
			
		}
		else
		{
			return _backgroundColor;
		}
	}

	Vec3f PathIntegrator::_directLightingMain( Ray p_ray, const Scene & p_scene, HitRecord p_hitRecord ) const
	{
		Vec3f lum;
		Vec3f lum_list = Vec3f( 0.f );
		float nbLightSample;
		for ( BaseLight * light : p_scene.getLights() )
		{
			nbLightSample = 1;
			lum			  = Vec3f( 0.f );
			if ( light->getSurface() ) { nbLightSample = _nbLightSamples; }
			for ( int i = 0; i < nbLightSample; i++ )
			{
				LightSample ls	  = light->sample( p_hitRecord._point );
				Ray			o_ray = Ray( p_hitRecord._point, ls._direction );
				o_ray.offset( p_hitRecord._normal );

				if ( !p_scene.intersectAny( o_ray, SHADOW_EPSILON, ls._distance ) )
				{
					lum += _directLighting( p_ray, ls, p_hitRecord );
				}
			}
			lum /= Vec3f( float( nbLightSample ) );

			lum_list += lum;
		}
		return lum_list;
	}

	/*Vec3f PathIntegrator::_indirectLightingMain( Ray p_ray, const Scene & p_scene, HitRecord p_hitRecord ) const
	{
		Vec3f lum;
		Vec3f lum_list = Vec3f( 0.f );
		float nbLightSample;
		for ( BaseLight * light : p_scene.getLights() )
		{
			nbLightSample = 1;
			lum			  = Vec3f( 0.f );
			if ( light->getSurface() ) { nbLightSample = _nbLightSamples; }

			for ( int i = 0; i < nbLightSample; i++ )
			{

				lum += _indirectLighting( p_ray, p_hitRecord );
			}
			lum /= Vec3f( float( nbLightSample ) );

			lum_list += lum;
		}
		return lum_list;
	}*/




	Vec3f PathIntegrator::_directLighting( Ray ray, LightSample ls, HitRecord hitRecord ) const
	{
		float angle = glm::max( 0.f, glm::dot( glm::normalize( hitRecord._normal ), glm::normalize( ls._direction ) ) );
		return hitRecord._object->getMaterial()->shade( ray.getDirection(), hitRecord, ls._direction ) * ls._radiance * angle;
	}

	Vec3f PathIntegrator::_indirectLighting( Vec3f wo, Vec3f wi, HitRecord hitRecord ) const
	{
		float angle = glm::max( 0.f, glm::dot( glm::normalize( hitRecord._normal ), wi) );
		return hitRecord._object->getMaterial()->shade( wo, hitRecord, wi ) * angle;
	}


	float PathIntegrator::_getFresnelCoefficient( float n1,
													 float n2,
													 Vec3f wI_dir,
													 Vec3f refract_dir,
													 Vec3f p_normal,
													 bool  use_schlick ) const
	{
		float cos_theta_i = glm::dot( -wI_dir, p_normal );

		if ( use_schlick )
		{
			float R0 = ( ( n1 - n2 ) / ( n1 + n2 ) ) * ( ( n1 - n2 ) / ( n1 + n2 ) );
			return R0 + ( 1 - R0 ) * glm::pow( 1 - cos_theta_i, 5 );
		}

		float cos_theta_t = glm::dot( refract_dir, -p_normal );

		// Reflectance total
		if ( n1 > n2 )
		{
			float critical_angle = glm::asin( n2 / n1 );
			if ( glm::acos( cos_theta_i ) > critical_angle ) { return 1.f; }
		}

		if ( cos_theta_i == 1.f ) { return ( ( n1 - n2 ) / ( n1 + n2 ) ) * ( ( n1 - n2 ) / ( n1 + n2 ) ); }

		float n1CosThetaI = n1 * cos_theta_i;
		float n1CosThetaT = n1 * cos_theta_t;

		float n2CosThetaI = n2 * cos_theta_i;
		float n2CosThetaT = n2 * cos_theta_t;

		float Rs = ( n1CosThetaI - n2CosThetaT ) / ( n1CosThetaI + n2CosThetaT );

		float Rp = ( n1CosThetaT - n2CosThetaI ) / ( n1CosThetaT + n2CosThetaI );

		return ( Rs * Rs + Rp * Rp ) * 0.5;
	}
} // namespace RT_ISICG