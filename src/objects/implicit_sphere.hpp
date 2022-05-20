#ifndef __RT_ISICG_IMPLICIT_SPHERE__
#define __RT_ISICG_IMPLICIT_SPHERE__

#include "implicit_surface.hpp"


namespace RT_ISICG
{
	class ImplicitSphere : public ImplicitSurface
	{
	  public:
		ImplicitSphere()		   = delete;
		virtual ~ImplicitSphere() = default;

		ImplicitSphere( const std::string & p_name, Vec3f p_center, float p_rayon ) : ImplicitSurface( p_name ), _center(p_center), _rayon(p_rayon) {}

		

	  private:
		// Signed Distance Function
		float _sdf( const Vec3f & p_point ) const
		{
			return ( glm::length( p_point - _center ) - _rayon );
		}


	  private:
		float _rayon ;
		Vec3f _center = VEC3F_ZERO;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SPHERE__
