#include "triangle_mesh.hpp"

namespace RT_ISICG
{
	bool MeshTriangle::intersect( const Ray & p_ray,
								  const float p_tMin,
								  const float p_tMax,
								  HitRecord & p_hitRecord ) const
	{
		float  tClosest = p_tMax;			 // Hit distance.
		size_t hitTri	= _triangles.size(); // Hit triangle id.
		Vec2f uv;
		float  u = 0.f;
		float  v = 0.f;
		Vec3f  normal_barycenter = Vec3f(0.f);
		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			float t;
			
			if (!_aabb.intersect(p_ray, p_tMin, p_tMax)) { 
				return false;
			}

			if ( _triangles[ i ].intersect( p_ray, t, uv ) )
			{
				if ( t >= p_tMin && t <= tClosest )
				{
					u = uv[ 0 ];
					v = uv[ 1 ];
					
					tClosest = t;
					hitTri	 = i;
				}
			}
		}
		if ( hitTri != _triangles.size() ) // Intersection found.
		{
			const unsigned int * indices = _triangles[ hitTri ].getIndices();
			normal_barycenter	= glm::normalize( ( 1.f - u - v ) * _normals[ indices[ 0 ] ]+ u * _normals[ indices[ 1 ] ] + v * _normals[ indices[ 2 ] ] );
			p_hitRecord._point	= p_ray.pointAtT( tClosest );			p_hitRecord._normal = normal_barycenter;
			p_hitRecord.faceNormal( p_ray.getDirection() );
			p_hitRecord._distance = tClosest;
			p_hitRecord._object	  = this;

			return true;
		}
		return false;
	}

	bool MeshTriangle::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		
		if ( !_aabb.intersect( p_ray, p_tMin, p_tMax ) ) { return false; }
		Vec2f uv;
		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			float t;
			if ( _triangles[ i ].intersect( p_ray, t, uv ) )
			{
				if ( t >= p_tMin && t <= p_tMax ) return true; // No need to search for the nearest.
			}
		}
		return false;
	}
} // namespace RT_ISICG
