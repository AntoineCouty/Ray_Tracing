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
			Vec3f p =  p_point - _center ;
			Vec2f q = Vec2f( glm::length( Vec2f(p.x, p.y )) - _rayonSup,p.z);
			return length( q ) - _rayonInf;
			
		}

	  private:
		float _rayonSup;
		float _rayonInf;
		Vec3f _center = VEC3F_ZERO;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_TORUS__
