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
		p_hitRecord._distance = p_tMax + 1 ;
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
			
			
			
			std::partial_sort( _triangles->begin() + p_firstTriangleId,
							   _triangles->begin() + p_lastTriangleId,
							   _triangles->begin() + p_lastTriangleId,
							   [ axe ](  TriangleMeshGeometry & a, TriangleMeshGeometry & b)->bool{ return a.getAABB().centroid()[ axe ] < b.getAABB().centroid()[ axe ];});

			Vec2i partition = _findPartition( p_node );
			//std::cout <<p_firstTriangleId << " "  << partition[ 0 ] << " " << partition[ 1 ] << " " << p_lastTriangleId << std::endl;
			p_node->_left = new BVHNode();
			p_node->_right = new BVHNode();
			_buildRec( p_node->_left, p_firstTriangleId, partition[0], p_depth + 1 );
			_buildRec( p_node->_right, partition[1], p_lastTriangleId , p_depth + 1 );
		}
		

	}

	Vec2i BVH::_findPartition( BVHNode * p_node ) const { 

		SAH * sah = new SAH();
		float size		= ( p_node->_lastTriangleId - p_node->_firstTriangleId );
		//std::cout << p_node->_firstTriangleId << " " << p_node->_lastTriangleId << " " << size * 0.5 << std::endl;
		float sa_parent = _computeTotalArea(p_node->_aabb);
		float sa_right;
		float sa_left;
		float note;
		for ( int i = p_node->_firstTriangleId + size*0.4 ; i < p_node->_lastTriangleId - size*0.4 ; i++ )
		{
			sa_right = _computeTotalArea( i, p_node->_lastTriangleId );

			for ( int j = i; j < i + _maxTrianglesPerLeaf; j++ )
			{
				sa_left = _computeTotalArea( p_node->_firstTriangleId, j );
				note = ( sa_left * ( j - p_node->_firstTriangleId ) + sa_right * ( p_node->_lastTriangleId - i ) ) / sa_parent;
				//std::cout << note << std::endl;
				if ( note < sah->_note ) { 
					//std::cout << j << " " << i << " " << note << std::endl;
					sah->_note = note;
					sah->_lastIdLeft = j;
					sah->_firstIdRight = i;
				}
			}
		}
		Vec2i lastId = { sah->_lastIdLeft, sah->_firstIdRight };
		return lastId;
	}

	float BVH::_computeTotalArea(int p_first, int p_last) const {

		AABB aabb;
		for ( int i = p_first; i < p_last; i++ )
		{
			aabb.extend( ( *_triangles )[ i ].getAABB() );
		}

		Vec3f diag = aabb.diagonal();

		return 2.f * ( diag.x * diag.y + diag.x * diag.z + diag.y * diag.z );
	}

	float BVH::_computeTotalArea( AABB aabb ) const
	{
		
		Vec3f diag = aabb.diagonal();

		return 2.f * ( diag.x * diag.y + diag.x * diag.z + diag.y * diag.z );
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
				size_t hitTri	= -1;				// Hit triangle id.
				Vec2f  p_uv;
				Vec2f  uv;
				for ( size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
				{
					float t;

					if ( ( *_triangles )[ i ].intersect( p_ray, t, p_uv ) )
					{
						if ( t >= p_tMin && t <= tClosest )
						{
							tClosest = t;
							hitTri	 = i;
							uv		 = p_uv;
						}
					}
				}
				if ( hitTri != -1 && tClosest < p_hitRecord._distance ) // Intersection found.
				{
					const MeshTriangle * mesh = ( *_triangles )[ hitTri ].getMesh();
					p_hitRecord._point	  = p_ray.pointAtT( tClosest );
					p_hitRecord._normal = ( *_triangles )[ hitTri ].getSmoothNormal(uv);
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
						if ( t >= p_tMin && t <= p_tMax )
						{ 
							return true;
						}
					}
				}
				return false;
			}

			
			return ( _intersectAnyRec( p_node->_left, p_ray, p_tMin, p_tMax ) || _intersectAnyRec( p_node->_right, p_ray, p_tMin, p_tMax ) );
			
		}
		return false;
	}
} // namespace RT_ISICG
