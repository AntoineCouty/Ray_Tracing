#include "bvh.hpp"
#include "geometry/triangle_mesh_geometry.hpp"
#include "utils/chrono.hpp"

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

		_buildRec( _root, 0, _triangles->size(), 0 );

		/// TODO

		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		return _intersectRec(_root, p_ray, p_tMin, p_tMax, p_hitRecord);
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		/// TODO
		return false;
	}

	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{
		if ( p_depth > _maxDepth && ( p_lastTriangleId - p_firstTriangleId ) > _maxTrianglesPerLeaf )
		{
			p_node->_firstTriangleId = p_firstTriangleId;
			p_node->_lastTriangleId = p_lastTriangleId;
			
			for ( int i = p_firstTriangleId; i < p_lastTriangleId; i++ )
			{
				p_node->_aabb.extend( (*_triangles )[ i ].getAABB() );
			}

			size_t axe = p_node->_aabb.largestAxis();
			_sort( axe, p_firstTriangleId, p_lastTriangleId );
			Vec3f centerXYZ = p_node->_aabb.centroid();
			float  center;
			
			const unsigned int partition = (p_firstTriangleId + p_lastTriangleId)*0.5;
			
			_buildRec( p_node->_left, p_firstTriangleId, partition, p_depth + 1 );
			_buildRec( p_node->_right, partition, p_lastTriangleId, p_depth + 1 );
		}

	}

	void  BVH::_sort( int axe, int p_firstTriangleId, int p_lastTriangleId ) {
		bool isSort = false;
		switch ( axe )
		{
			case 0:
				while ( !isSort )
				{
					isSort = true;
					for ( int i = p_firstTriangleId; i < p_lastTriangleId - 1; i++ )
					{
						if ( ( *_triangles )[ i ].getAABB().centroid().x > ( *_triangles )[ i + 1 ].getAABB().centroid().x )
						{
							isSort = false;
							std::swap( ( *_triangles )[ i ], ( *_triangles )[ i + 1 ] );
						}
					}
				}
				break;
			case 1:
				while ( !isSort )
				{
					isSort = true;
					for ( int i = p_firstTriangleId; i < p_lastTriangleId - 1; i++ )
					{
						if ( ( *_triangles )[ i ].getAABB().centroid().y > ( *_triangles )[ i + 1 ].getAABB().centroid().y )
						{
							isSort = false;
							std::swap( ( *_triangles )[ i ], ( *_triangles )[ i + 1 ] );
						}
					}
				}
				break;
			case 2:
				while ( !isSort )
				{
					isSort = true;
					for ( int i = p_firstTriangleId; i < p_lastTriangleId - 1; i++ )
					{
						if ( ( *_triangles )[ i ].getAABB().centroid().z > ( *_triangles )[ i + 1 ].getAABB().centroid().z )
						{
							isSort = false;
							std::swap( ( *_triangles )[ i ], ( *_triangles )[ i + 1 ] );
						}
					}
				}
				break;
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
				
			}
			else
			{
				bool left  = _intersectRec( p_node->_left, p_ray, p_tMin, p_tMax, p_hitRecord );
				bool right = _intersectRec( p_node->_right, p_ray, p_tMin, p_tMax, p_hitRecord );
				return ( left || right );
			}
		}
		return false;
	}
	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		if ( p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) ) { 
			return true; }
		else if (! p_node->isLeaf() )
		{
			bool left = _intersectAnyRec( p_node->_left, p_ray, p_tMin, p_tMax );
			bool right = _intersectAnyRec( p_node->_right, p_ray, p_tMin, p_tMax );
			return ( left || right );
		}
		return false;
	}
} // namespace RT_ISICG
