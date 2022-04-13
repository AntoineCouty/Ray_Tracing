#include "triangle_mesh_geometry.hpp"
#include "objects/triangle_mesh.hpp"

namespace RT_ISICG
{
	TriangleMeshGeometry::TriangleMeshGeometry( const unsigned int p_v0,
												const unsigned int p_v1,
												const unsigned int p_v2,
												MeshTriangle *	   p_refMesh )
		: _v0( p_v0 ), _v1( p_v1 ), _v2( p_v2 ), _refMesh( p_refMesh )
	{
		_faceNormal = glm::normalize( glm::cross( _refMesh->_vertices[ p_v1 ] - _refMesh->_vertices[ p_v0 ],
												  _refMesh->_vertices[ p_v2 ] - _refMesh->_vertices[ p_v0 ] ) );

		_aabb.extend( _refMesh->_vertices[ _v0 ] );
		_aabb.extend( _refMesh->_vertices[ _v1 ] );
		_aabb.extend( _refMesh->_vertices[ _v2 ] );


		_v[ 0 ]		= p_v0;
		_v[ 1 ]		= p_v1;
		_v[ 2 ]		= p_v2;
	}

	bool TriangleMeshGeometry::intersect( const Ray & p_ray, float & p_t, Vec2f & p_uv ) const
	{
		const Vec3f & o	 = p_ray.getOrigin();
		const Vec3f & d	 = p_ray.getDirection();
		const Vec3f & v0 = _refMesh->_vertices[ _v0 ];
		const Vec3f & v1 = _refMesh->_vertices[ _v1 ];
		const Vec3f & v2 = _refMesh->_vertices[ _v2 ];
		
		

		Vec3f v0v1 = v1 - v0;
		Vec3f v0v2 = v2 - v0;
		Vec3f h	   = glm::cross( d, v0v2 );
		float a	   = glm::dot( v0v1, h );

		if ( a == 0.f )
			return false;
		
		float f = 1.f / a;
		Vec3f s = o - v0;
		float u = f * glm::dot( s, h );

		if ( u < 0.f || u > 1.f ) 
			return false;

		Vec3f q = glm::cross( s, v0v1 );
		float v = f * glm::dot( q, d );
		if ( v < 0.f || u + v > 1.f ) 
			return false;

		p_t = f * glm::dot( v0v2, q );
		
		p_uv = Vec2f( u, v );
		
		return true;

	}


	Vec3f TriangleMeshGeometry::getSmoothNormal( Vec2f p_uv ) const
	{
		Vec3f n0 = _refMesh->_normals[ _v0 ];
		Vec3f n1 = _refMesh->_normals[ _v1 ];
		Vec3f n2 = _refMesh->_normals[ _v2 ];
		float u	 = p_uv[ 0 ];
		float v  = p_uv[ 1 ];
		return glm::normalize( ( 1.f - u - v ) * n0 + u * n1 + v * n2 );
	}

} // namespace RT_ISICG
