#include "bvh.hpp"
#include "geometry/triangle_mesh_geometry.hpp"
#include "objects/triangle_mesh.hpp"
#include "utils/chrono.hpp"
#include <algorithm>

namespace RT_ISICG
{
	void BVH::build( std::vector<TriangleMeshGeometry> * p_triangles )
	{
		std::cout << "Building BVH..." << std::endl;
		if ( p_triangles == nullptr || p_triangles->empty() )
		{
			throw std::exception( "BVH::build() error: no triangle provided" );
		}
		_triangles = p_triangles;
		Chrono chr;
		chr.start();
		_root = new BVHNode();
		_buildRec( _root, 0, _triangles->size(), 0 );

		chr.stop();

		std::cout << "[BVH DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		p_hitRecord._distance = p_tMax + 1;
		return _intersectRec(_root, p_ray, p_tMin, p_tMax, p_hitRecord);
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		return _intersectAnyRec( _root, p_ray, p_tMin, p_tMax );
	}

	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{
		
		for ( int i = p_firstTriangleId; i < p_lastTriangleId; i++ )
		{
			p_node->_aabb.extend( ( *_triangles )[ i ].getAABB() );
			
		}
		
		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId	 = p_lastTriangleId;
		
		if ( p_depth < _maxDepth && ( p_lastTriangleId - p_firstTriangleId ) > _maxTrianglesPerLeaf )
		{

			size_t axe = p_node->_aabb.largestAxis();
			
			const unsigned int partition = (p_firstTriangleId + p_lastTriangleId)*0.5;
			
			std::partial_sort( _triangles->begin() + p_firstTriangleId,
							   _triangles->begin() + p_lastTriangleId,
							   _triangles->begin() + p_lastTriangleId,
							   [ axe ](  TriangleMeshGeometry & a, TriangleMeshGeometry & b)->bool{ return a.getAABB().centroid()[ axe ] < b.getAABB().centroid()[ axe ];});


			p_node->_left = new BVHNode();
			p_node->_right = new BVHNode();
			_buildRec( p_node->_left, p_firstTriangleId, partition, p_depth + 1 );
			_buildRec( p_node->_right, partition, p_lastTriangleId , p_depth + 1 );
		}
		

	}


	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		if ( p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) ) { 
			if ( p_node->isLeaf() ) {
				
				float  tClosest = p_tMax;			 // Hit distance.
				size_t hitTri	= -1; // Hit triangle id.
				Vec2f  p_uv;
				for ( size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
				{
					float t;

					if ( ( *_triangles )[ i ].intersect( p_ray, t, p_uv ) )
					{
						if ( t >= p_tMin && t <= tClosest )
						{
							tClosest = t;
							hitTri	 = i;
						}
					}
				}
				if ( hitTri != -1 && tClosest < p_hitRecord._distance ) // Intersection found.
				{
					const MeshTriangle * mesh = ( *_triangles )[ hitTri ].getMesh();
					p_hitRecord._point	  = p_ray.pointAtT( tClosest );
					p_hitRecord._normal = ( *_triangles )[ hitTri ].getSmoothNormal(p_uv);
					p_hitRecord.faceNormal( p_ray.getDirection() );
					p_hitRecord._distance = tClosest;
					p_hitRecord._object	  = mesh;
					
					return true;
				}
				return false;
			}
			
			bool left  = _intersectRec( p_node->_left, p_ray, p_tMin, p_tMax, p_hitRecord );
			bool right = _intersectRec( p_node->_right, p_ray, p_tMin, p_tMax, p_hitRecord );
			return ( left || right );
			
		}
		return false;
	}
	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		if ( p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
		{
			if ( p_node->isLeaf() ) {
				for ( size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
				{
					float t;
					Vec2f p_uv;

					if ( ( *_triangles )[ i ].intersect( p_ray, t, p_uv ) )
					{ 
						return true;
					}
				}
				return false;
			}

			bool left  = _intersectAnyRec( p_node->_left, p_ray, p_tMin, p_tMax );
			bool right = _intersectAnyRec( p_node->_right, p_ray, p_tMin, p_tMax );
			return ( left || right );
			
		}
		return false;
	}
} // namespace RT_ISICG
