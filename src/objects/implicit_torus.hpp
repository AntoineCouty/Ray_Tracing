#ifndef __RT_ISICG_IMPLICIT_TORUS__
#define __RT_ISICG_IMPLICIT_TORUS__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitTorus : public ImplicitSurface
	{
	  public:
		ImplicitTorus()			  = delete;
		virtual ~ImplicitTorus() = default;

		ImplicitTorus( const std::string & p_name, Vec3f p_center, float p_rayonSup, float p_rayonInf )
			: ImplicitSurface( p_name ), _center( p_center ), _rayonSup( p_rayonSup ), _rayonInf( p_rayonInf )
		{
		}

	  private:
		// Signed Distance Function
		float _sdf( const Vec3f & p_point ) const
		{
			Vec2f p = Vec2f( p_point.x - _center.x, p_point.z - _center.z );
			Vec2f q = Vec2f( glm::length( p ) - _rayonSup, p_point.y - _center.y);
			return length( q ) - _rayonInf;
			
		}

	  private:
		float _rayonSup;
		float _rayonInf;
		Vec3f _center = VEC3F_ZERO;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_TORUS__
